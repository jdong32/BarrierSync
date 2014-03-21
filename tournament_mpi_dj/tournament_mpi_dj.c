/*
 ============================================================================
 Name        : DJDJ.c
 Author      : Jie Dong
 Version     :
 Copyright   : Georgia Tech
 Description : Hello World in C, Ansi-style
 ============================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include "tournament_mpi_dj.h"

#define ITERATION 1000

int my_id;
int num_processes;
MPI_Status mpi_result;

//void barrier(int,int,int opponent[]);
//void barrier(int,int);

int main(int argc, char **argv)
{
  //int my_id,num_processes;
	FILE* myfile;
	struct timeval time1,time2;
	long ii=0;
	
	double avg_time=0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &my_id);

	myfile=fopen("result.txt","w");
	for(ii=0;ii<ITERATION;)
	{
		gettimeofday(&time1,NULL);
		ii++;
		barrier(my_id,num_processes);
		gettimeofday(&time2,NULL);
		avg_time+=(double)((time2.tv_sec-time1.tv_sec)*1000000+(time2.tv_usec-time1.tv_usec))/ITERATION;
	}
	MPI_Finalize();
	gettimeofday(&time2,NULL);
	printf("avg_time:%f\n",avg_time);

	return 0;
}


