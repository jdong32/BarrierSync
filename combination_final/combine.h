#ifndef COMBINE_H
#define COMBINE_H

#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <string.h>

#define EMPTY -3
#define NUM_THREADS 2
#define ITERATION 1000

typedef struct node_s {
	int k;
	int count;
	int locksense;
	struct node_s* parent;
} node_t;

node_t nodes[NUM_THREADS - 1];
int my_id;
int num_processes;
MPI_Status mpi_result;
MPI_Request mpi_req;


mybarrierini()
{
	int i;
	int tmp=my_id;
	for (i = 0; i < NUM_THREADS; i++) {
		nodes[i].k = 2;
		nodes[i].count = nodes[i].k;
		nodes[i].locksense = 0;
		nodes[i].parent = (i == 0) ? NULL : nodes + (i - 1) / 2;
	}
	my_id=tmp;
	
	
}

void Cbarrier()
{
	int sense = 1;//*
	node_t* mynode = nodes + NUM_THREADS / 2 - 1 + omp_get_thread_num() / 2;//*
	tree_barrier(mynode, &sense);
}

void tree_barrier(node_t* mynode, int * sense)
{
	tree_barrier_aux(mynode, sense);
	*sense = (*sense == 1) ? 0 : 1;
}

void tree_barrier_aux(node_t* mynode, int * sense)
{
	int count_fetched;
#pragma omp critical
	{
		count_fetched = mynode->count--;
	}
	if (count_fetched == 1) {
		if (mynode->parent != NULL) {
			tree_barrier_aux(mynode->parent, sense);
		} else {
			barrier(my_id,num_processes);// TODO: merge with openmpi barrier, send msg to other processes
		}
		mynode->count = mynode->k;
		mynode->locksense = (mynode->locksense == 1) ? 0 : 1;
	}
	while (mynode->locksense != *sense) {}
}

void barrier(int my_id,int num_processes)
{
	int* opponent;
	int sense=-1;
	int msg=-2;
	int tag=1;
	int i;
	long round=0;
	int oppo_id;
	long offset=1;
	opponent=malloc(sizeof(int)*num_processes);
	for(i=0;i<num_processes;i++)
		opponent[i]=EMPTY;

	for(round=0;offset*2<num_processes+2;round++)
	{
		if((int)(my_id/offset)%2==0)//winner
		{
			oppo_id=my_id+offset;
			if(oppo_id<num_processes)
			{
				opponent[round]=oppo_id;
				MPI_Recv(&sense,1,MPI_INT,oppo_id,tag,MPI_COMM_WORLD,&mpi_result);
				offset*=2;
				continue;
			}
			else//no opponent
			{
				opponent[round]=EMPTY;
				offset*=2;
				continue;
			}
		}
		else
		{
			oppo_id=my_id-offset;
			MPI_Send(&msg,1,MPI_INT,oppo_id,tag,MPI_COMM_WORLD);
			MPI_Recv(&sense,1,MPI_INT,oppo_id,tag,MPI_COMM_WORLD,&mpi_result);
			break;
		}
	}

	while(round-1>=0)
	{
		if(opponent[round-1]>0)
			MPI_Send(&msg,1,MPI_INT,opponent[round-1],tag,MPI_COMM_WORLD);
		round--;
	}
	free(opponent);
}


#endif
