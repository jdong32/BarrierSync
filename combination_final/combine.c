#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>
#include "combine.h"

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	omp_set_num_threads(NUM_THREADS);
	mybarrierini();//MUST BE CALLED
#pragma omp parallel 
	{
		int j = 0;
		double avg_time=0;
		struct timeval time2, time1;
		for (j = 0; j < ITERATION; ) {
			j++;
			gettimeofday(&time1,NULL);
			Cbarrier();
			gettimeofday(&time2,NULL);
			avg_time+=(double)((time2.tv_sec-time1.tv_sec)*1000000+(time2.tv_usec-time1.tv_usec))/ITERATION;
		}
		printf("avg_time:%f\n",avg_time);	
	}
	MPI_Finalize();
	
	return 0;
}

