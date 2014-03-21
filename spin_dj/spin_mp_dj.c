/*
 ============================================================================
 Name        : omp_barrier_dj.c
 Author      : Jie Dong
 Version     :
 Copyright   : Georgia Tech
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "spin_mp_dj.h"

#define ITERATION 1000

int main(int argc,char* argv[])
{
	int th_num=argv[1][0]-'0';
	FILE* myfile;

	omp_set_num_threads(th_num);
	omp_init_lock(&senselock);
	omp_init_lock(&numlock);
	my_barrier_ini(th_num);
	myfile=fopen("mp_result.txt","a");

	#pragma omp parallel
	{
		struct timeval time1,time2;
		int thd_id=omp_get_thread_num();
		int n;
		double avg_time=0;

		for(n=0;n<ITERATION;)
		{
			n++;
			gettimeofday(&time1,NULL);
			my_barrier(thd_id,th_num);
			gettimeofday(&time2,NULL);
			avg_time+=(double)((time2.tv_sec-time1.tv_sec)*1000000+(time2.tv_usec-time1.tv_usec))/ITERATION;
		}
		if(thd_id==0)
		{
			printf("work done\n");
			fprintf(myfile,"np: %d avg_time:%f\n",th_num,avg_time);
		}
	}
	return EXIT_SUCCESS;
}
