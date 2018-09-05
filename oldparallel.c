#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <malloc.h>

#define N 100
#define RANGE 10
#define CALC_TIME (((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec))*1000)

int *integers;

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

    	// compute the work distribution
    	int remainder = N % size;
    	int local_counts[size], offsets[size];
    	int sum = 0;
    	
	for (long i = 0; i < size; i++) {
        	local_counts[i] = N / size;
        	if (remainder > 0) {
            	local_counts[i] += 1;
            	remainder--;
        	}
        	offsets[i] = sum;
        	sum += local_counts[i];
    	}

    	int localArray[local_counts[rank]];
	int sumq;
	int sumT[size];

	if (rank == ROOT) {
		srand((unsigned) time(&t));

        	for(int i = 0 ; i < N ; i++ )
                	integers[i] = rand() % RANGE;
        
        	gettimeofday(&tv1, NULL);
        	seq_adder();
        	gettimeofday(&tv2, NULL);
       
        	printf ("Sequential Adder Time: %f milliseconds\n", CALC_TIME);
	}

    	MPI_Scatterv(integers, local_counts, offsets, MPI_INT, localArray, local_counts[rank], MPI_INT, ROOT, MPI_COMM_WORLD);

	for (int i = 0; i < local_counts[rank]; i++)
		sumq += localArray[i];

	MPI_Gatherv(localArray, local_counts[rank], MPI_INT, integers, local_counts, offsets, MPI_INT, ROOT, MPI_COMM_WORLD);
	MPI_Finalize();

	if(rank == ROOT) {
		printf("T Sum: %i\n",sumq);
	}
	return 0;
}

void seq_adder() {
	unsigned long i, sum;

        for( i = 0, sum = 0 ; i < N ; i++ )	
		sum += (long)integers[i];

        printf("Sequential Adder Sum: %lu\n",sum);
}
