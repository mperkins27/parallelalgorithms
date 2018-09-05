#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 100
#define RANGE 10

#define CALC_TIME (((double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec))*1000)

void seq_adder(int *integers);

int main(int argc, char** argv) {
	struct timeval  tv1, tv2;
	unsigned long i,sum;
        time_t t;
	int integers[SIZE];

        srand((unsigned) time(&t));


	gettimeofday(&tv1, NULL);
	seq_adder(integers);
	gettimeofday(&tv2, NULL);
	printf ("Sequential Adder Time: %f milliseconds\n", CALC_TIME);
	
	
	return 0;
}

void seq_adder(int *integers) {
	unsigned long i, sum;

        for( i = 0, sum = 0 ; i < SIZE ; i++ )	
		sum += (long)integers[i];

        printf("Sequential Adder Sum: %lu\n",sum);
}
