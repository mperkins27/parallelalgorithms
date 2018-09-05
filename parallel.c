#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#define N 9000000
#define RANGE 10
#define CALC_TIME (((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec))*1000)

int *rand_nums;

void seq_adder(void);

int main(int argc, char** argv) {
	struct timeval  tv1, tv2;
	unsigned long i;
        time_t t;

	int size, rank;
    	const int ROOT = 0; 

    	MPI_Init(&argc, &argv);
    	MPI_Comm_size(MPI_COMM_WORLD, &size);
    	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == ROOT) {
		srand((unsigned) time(&t));

		rand_nums = malloc(sizeof(int) * N);

                for(unsigned int i = 0 ; i < N ; i++ )
                        rand_nums[i] = rand() % RANGE;

        	gettimeofday(&tv1, NULL);
        	seq_adder();
        	gettimeofday(&tv2, NULL);
       
        	printf ("Sequential Adder Time: %f milliseconds\n", CALC_TIME);

		gettimeofday(&tv1, NULL);
	}

	unsigned int elements_per_proc = N / size;
	unsigned int *sub_rand_nums = malloc(sizeof(unsigned int) * elements_per_proc);

        MPI_Scatter(rand_nums, elements_per_proc, MPI_INT, sub_rand_nums, elements_per_proc, MPI_INT, ROOT, MPI_COMM_WORLD);

        unsigned int sub_sum = 0;
        for (unsigned int i = 0; i < elements_per_proc; i++)
                sub_sum += sub_rand_nums[i];

        unsigned int *sumT = NULL;
        if(rank == ROOT)
                sumT = malloc(sizeof(unsigned int) * elements_per_proc);

        MPI_Gather(&sub_sum, 1, MPI_INT, sumT, 1, MPI_INT, ROOT, MPI_COMM_WORLD);
        MPI_Finalize();

        if(rank == ROOT) {
                long total = 0;
                for(unsigned int i = 0; i < elements_per_proc; i++)
                        total += (long)sumT[i];
                printf("Parallel Adder Sum: %lu\n",total);
		gettimeofday(&tv2, NULL);

		printf ("Parallel Adder Time: %f milliseconds\n", CALC_TIME);
        }

	return 0;
}

void seq_adder() {
	unsigned long i, sum;

        for( i = 0, sum = 0 ; i < N ; i++ )	
		sum += (long)rand_nums[i];

        printf("Sequential Adder Sum: %lu\n",sum);
}
