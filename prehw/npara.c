#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#define N 1000000
#define RANGE 10
#define CALC_TIME (((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec))*1000)

struct node *rand_nums = NULL;

void seq_adder(void);
void generateNumbers(void);

struct node {
  int data;
  long size;
  struct node* next;
};

void push(struct node** list, int n);
int pop(struct node** list);
void printStack(struct node* list);

int main(int argc, char** argv) {
	struct timeval  tv1, tv2;
	unsigned long i;
        time_t t;

	int size, rank;
    	const int ROOT = 0; 
	int init = 0;

    	MPI_Init(&argc, &argv);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if(rank == ROOT && !init) {
		generateNumbers();
        	gettimeofday(&tv1, NULL);
                seq_adder();
                gettimeofday(&tv2, NULL);

                printf ("Sequential Adder Time: %f milliseconds\n", CALC_TIME);
		init = 1;

        } else {
		// while there are numbers to draw, send to machine
		long rand_nums_size = N;
		unsigned long sum = 0;

		const int CORES = size;
		int cores = size, maxSteps = 0
		const int numToDraw = size * 2;
		int *numbers = malloc((sizeof(int) * numToDraw);

		long currentRound = 0;
		const long MaxRounds = (N / (cores * 2)) + (N % (cores * 2));

		while (cores >= 1) {
                	cores /= 2;
                        maxSteps++;
                }

		// timer start
		int *numbers = malloc(sizeof(int) * cores * 2);
		while(rand_nums_size > 1) {
			// draw numbers
			// calculate the sum

			for(int i = 0; rand_nums->size > 0 && i < numToDraw; i++)
				numbers[i] = pop();
			
			

			// check size
			// calculate sum
			// push
			// repeat			

			// machine start
        		int step, coresToUse;
			unsigned long sum = 0;

       			step = 0;
	        	while(step < maxSteps) {
	                	coreToUse = CORES / (step * 2);
	
		                if(rank == ROOT) {
					// send numbers, but keep last pair for the root

					for(int core = 1; core <= coresToUse; core++) {
						int a[2];

						a[
		       				MPI_Send(&a[500], 500, MPI_INT, core , 0, MPI_COMM_WORLD);
					}	
					sort(a, 500);
					MPI_Recv(b, 500, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);                
	                	}
	                	else if(rank <= coreToUse) {
					MPI_Recv(b, 500, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
					sort(b, 500);
					MPI_Send(b, 500, MPI_INT, 0, 0, MPI_COMM_WORLD);
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

void seq_adder() {
	unsigned long sum;
	struct node* current;

	current = rand_nums;
	while(current != NULL) {
		sum += (long)current->data;
	    	current = current->next;
	}

        printf("Sequential Adder Sum: %lu\n",sum);
}

void generateNumbers() {
	srand((unsigned) time(&t));

        for(unsigned int i = 0 ; i < N ; i++ )
        	push(rand() % RANGE);
}

void push(int n) {
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
    newNode->size = newNode->size + 1;
  }

}

int pop() {
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
  newNode->size = newNode->size - 1;
 
  return n;
}
