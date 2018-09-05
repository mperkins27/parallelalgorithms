#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000000
#define RANGE 10

#define CALC_TIME (((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec))*1000)

void seq_adder(int *integers);

int main(int argc, char** argv) {
	struct timeval  tv1, tv2;
	unsigned long i;
        time_t t;
	int integers[SIZE];

        srand((unsigned) time(&t));

        for( i = 0 ; i < SIZE ; i++ )
                integers[i] = rand() % RANGE;

	gettimeofday(&tv1, NULL);
	seq_adder(integers);
	gettimeofday(&tv2, NULL);

	printf ("Sequential Adder Time: %f milliseconds\n", CALC_TIME);
	
	
	//int world_size;
        //MPI_Comm_size(MPI_COMM_WORLD, &world_size);

        //int world_rank;
        //MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

        //char processor_name[MPI_MAX_PROCESSOR_NAME];
        //int name_len;
       // MPI_Get_processor_name(processor_name, &name_len);

        //printf("Hello from processor %s, rank %d out of %d processors\n",
        //processor_name, world_rank, world_size);

	int myid, numprocs, datastart, dataend, namelen;
	double localsum = 0, totalsum = 0;

	char processorname[MPI_MAX_PROCESSOR_NAME];
	MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);

	MPI_Get_processor_name(processorname, &namelen);


	for (i = 0; i < SIZE; i++)
		localsum += integers[i];

	printf("Process %d on %sData is %lu\n", myid, processorname, localsum);

	MPI_Reduce(&localsum, &totalsum, 1, MPI_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

	if (myid == 0)
		printf("\nTotal sum is %lu\n", &totalsum);

        MPI_Finalize();

	return 0;
}

void seq_adder(int *integers) {
	unsigned long i, sum;

        for( i = 0, sum = 0 ; i < SIZE ; i++ )	
		sum += (long)integers[i];

        printf("Sequential Adder Sum: %lu\n",&sum);
}
