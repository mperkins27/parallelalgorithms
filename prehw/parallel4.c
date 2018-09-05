#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#define N 100
#define RANGE 10

int main(int argc, char** argv) {
	unsigned long i;
        time_t t;

	int size, rank;
    	const int ROOT = 0; 

    	MPI_Init(&argc, &argv);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int *rand_nums;

	if (rank == ROOT) {
		srand((unsigned) time(&t));

		rand_nums = malloc(sizeof(int) * N);

        	for(int i = 0 ; i < N ; i++ ) 
                	rand_nums[i] = rand() % RANGE;
	}

	int elements_per_proc = 25;
	int *sub_rand_nums = malloc(sizeof(int) * elements_per_proc);

    	MPI_Scatter(rand_nums, 25, MPI_INT, sub_rand_nums, elements_per_proc, MPI_INT, ROOT, MPI_COMM_WORLD);

	int sub_sum = 0;
	for (int i = 0; i < elements_per_proc; i++)
		sub_sum += sub_rand_nums[i];

	int *sumT = NULL;
	if(rank == ROOT)
		sumT = malloc(sizeof(int) * elements_per_proc);
	MPI_Gather(&sub_sum, 1, MPI_INT, sumT, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
	MPI_Finalize();

	if(rank == ROOT) {
		int total = 0;
		for(int i = 0; i < elements_per_proc; i++)
			total += sumT[i];
		printf("T Sum: %i\n",total);
	}
	return 0;
}

