/*
 * spin_mp_dj.h
 *
 *  Created on: Oct 25, 2011
 *      Author: deny
 */

#ifndef SPIN_MP_DJ_H_
#define SPIN_MP_DJ_H_
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SENSE 0
#define UNSENSE 1

int cri_num;
int sense;
omp_lock_t senselock;
omp_lock_t numlock;

void my_barrier_ini(int th_num)
{
	sense=SENSE;
	cri_num=th_num;
	omp_init_lock(&senselock);
	omp_init_lock(&numlock);
}

void my_barrier(int thd_id,int th_num)
{
	int tmp;
	omp_set_lock(&senselock);
	tmp=sense;
	omp_unset_lock(&senselock);

	omp_set_lock(&numlock);
	if((cri_num=cri_num-1)!=0)
	{
		omp_unset_lock(&numlock);
		while(1)
		{
			omp_set_lock(&senselock);
			if(sense!=tmp)
			{
				omp_unset_lock(&senselock);
				break;
			}
			omp_unset_lock(&senselock);
		}
	}
	else
	{
		omp_unset_lock(&numlock);
		omp_set_lock(&senselock);
		sense=(sense==SENSE)?UNSENSE:SENSE;
		omp_set_lock(&numlock);
		cri_num=th_num;
		omp_unset_lock(&numlock);
		omp_unset_lock(&senselock);
	}
}


#endif /* SPIN_MP_DJ_H_ */
