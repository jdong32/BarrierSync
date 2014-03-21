#ifndef TOURNAMENT_MPI_DJ_H_
#define TOURNAMENT_MPI_DJ_H_
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <unistd.h>
#define empty -3
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
		opponent[i]=-3;

	for(round=0;offset*2<num_processes+2;round++)
	{
		if((int)(my_id/offset)%2==0)//winner
		{
			oppo_id=my_id+offset;
			if(oppo_id<num_processes)
			{
				opponent[round]=oppo_id;
				MPI_Recv(&sense,1,MPI_INT,oppo_id,tag,MPI_COMM_WORLD,/*&mpi_result*/NULL);
				offset*=2;
				continue;
			}
			else//no opponent
			{
				opponent[round]=empty;
				offset*=2;
				continue;
			}
		}
		else
		{
			oppo_id=my_id-offset;
			MPI_Send(&msg,1,MPI_INT,oppo_id,tag,MPI_COMM_WORLD);
			MPI_Recv(&sense,1,MPI_INT,oppo_id,tag,MPI_COMM_WORLD,/*&mpi_result*/NULL);
			break;
		}
	}
	while(round-1>=0)
	{
		if(opponent[round-1]>0)
		{
			MPI_Send(&msg,1,MPI_INT,opponent[round-1],tag,MPI_COMM_WORLD);
		}
		round--;

	}
	free(opponent);
}

#endif
