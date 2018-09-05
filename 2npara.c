#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#define N 1000000
#define RANGE 10
#define CALC_TIME (((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec))*1000)

struct node {
  int data;
  long size;
  struct node* next;
};


void push(struct node **rand_nums, int n);
int pop(struct node **rand_nums);

void generateNumbers(struct node **rand_nums);
void seq_adder(struct node **rand_nums);

int main(int argc, char** argv) {
	struct timeval  tv1, tv2;
	unsigned long i;
	struct node *rand_nums = NULL;

	int size, rank;
    	const int ROOT = 0; 
	int init = 0;

    	MPI_Init(&argc, &argv);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == ROOT && !init) {
		generateNumbers(&rand_nums);
        	gettimeofday(&tv1, NULL);
                seq_adder(&rand_nums);
                gettimeofday(&tv2, NULL);

                printf ("Sequential Adder Time: %f milliseconds\n", CALC_TIME);
		init = 1;

        } else {
		// while there are numbers to draw, send to machine
		const int CORES = size;
		int maxSteps = 0;

		long currentRound = 0;
		//const long MaxRounds = (N / (cores * 2)) + (N % (cores * 2));

		// draw numbers
		// calculate the sum
		while(rand_nums->size > 0) {
			// machine start
        		int step, coresToUse;
			unsigned long sum = 0;

       			step = 1;
			maxSteps = 2;
			int *status = NULL;
			
			int numToDraw = size * 2;

			int *numbers = malloc(sizeof(int) * CORES * 2);
	       		while(step <= maxSteps) {
				int i;
				for(i = 0; rand_nums->size > 0 && i < numToDraw; i++)
					numbers[i] = pop(&rand_nums);
	               		
				coresToUse = CORES / (step * 2);
				numToDraw = coresToUse * 2;

				// increase step and modify cores to us
				// account for time steps
				/// account for running out of numbers or uneven amount
		                if(rank == ROOT) {
					// send numbers, but keep last pair for the root
					int numIndex, core;
					for(numIndex = 2, core = 1; numIndex < numToDraw, core < coresToUse; numIndex++, core++) {
						int a[2];
						a[core % 2] = numbers[numIndex];
		       				MPI_Send(&a[2], 2, MPI_INT, core , 0, MPI_COMM_WORLD);
					}	
					
					int rootSum = numbers[0] + numbers[1];
					push(&rand_nums,rootSum);
					
					// calculate root sum, receive thread sums
					// push to stack
				
					for(numIndex = 2, core=1; numIndex < numToDraw, core < coresToUse; numIndex++, core++) {
                                                int result;
                                                MPI_Recv((void *)result, 1, MPI_INT, core, 0, MPI_COMM_WORLD, 0);

						push(&rand_nums,result);
                                        }	

	                	}
	                	else if(rank < coresToUse) {
					int b[2];
					MPI_Recv(b, 2, MPI_INT, ROOT, 0, MPI_COMM_WORLD, 0);
					b[0] += b[1];
					MPI_Send(&b[0], 1, MPI_INT, ROOT, 0, MPI_COMM_WORLD);
        		        }
        	        	step++;
			}
	        	// machine end
		}
	}
	
	// MPI finalize??
	MPI_Finalize();
	// free the stack
	return 0;
}

void seq_adder(struct node **rand_nums) {
	unsigned long sum;
	struct node* current;

	current = *rand_nums;
	while(current != NULL) {
		sum += (long)current->data;
	    	current = current->next;
	}

        printf("Sequential Adder Sum: %lu\n",sum);
}

void generateNumbers(struct node **rand_nums) {
        time_t t;
	unsigned int i;
	srand((unsigned) time(&t));

        for(i = 0 ; i < N ; i++ )
        	push(rand_nums, rand() % RANGE);
}

void push(struct node **rand_nums, int n) {
  struct node* newNode = (struct node*)malloc(sizeof(struct node));

  if(newNode == NULL) {
    printf("Malloc failed\n");
    return;
  }
  newNode->data = n;
  if(*rand_nums == NULL) {
    *rand_nums = newNode;
    newNode->next = NULL;
    newNode->size = 1;
  } else {
    newNode->next = *rand_nums;
    *rand_nums = newNode;
    (*rand_nums)->size++;
  }

}

int pop(struct node **rand_nums) {
  struct node* temp;
  int n;
  if(*rand_nums == NULL) {
    printf("No more numbers\n");
    return 0;
  }
  temp = *rand_nums;
  *rand_nums = (*rand_nums)->next;
  n = temp->data;
  free(temp);
  (*rand_nums)->size--;
 
  return n;
}
