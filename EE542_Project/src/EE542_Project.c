/*
 ============================================================================
 Name        : Test_sched_cfs.c
 Author      : Mahmoud Alasmar
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#define __USE_GNU
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdint.h>
#include <signal.h>
#include <math.h>
#include <pthread.h>
#include <sys/time.h>
#include <time.h>

#include <math.h>
#include "Parameters.h"
#include "functions.h"

#include "Benchmark.h"


long int gTime(void)
{
     struct timeval rtclk;
     gettimeofday(&rtclk, NULL);
	 return rtclk.tv_usec;

}


int main(void) {
	srand(time(0));

	long int t0,t1;

	int i=0;
	int k;
	int j;
	while(i < numT*numApp)
	{
		workFlow[i] = GenerateTask();
		i++;
	}

	i=0;
	while(i<numV)
	{
		v[i] = GenerateVirt();
		i++;
	}


	int total_latency = 0;
	int total_power = 0;
	int total_cost = 0;

	printf("/*************************/\n");
	printf("Start Proposed Method Test\n");
	printf("/*************************/\n");
	int appindx = 0;

	t0 = gTime();
	for(appindx = 0;appindx<numApp;appindx++)
	{
		i=0;
		while(i<numS)
		{
			solutions[i] = GenerateSol();
			i++;
		}

		i=0;
	/*	while(i<numS)
		{
			printf("solution_%d = ",i);
			for(j=0;j<numT;j++)
			{
				for(k=0;k<numV;k++)
				{
					printf("%f\t ",solutions[i].sol[k+numV*j]);
				}
				printf("\n");
			}
			printf("\n");
			i++;
		}*/

		int solutions_2[numS];

		nsga(solutions,solutions_1,solutions_2,appindx);


		//printf("total number of fronts = %d\n",TotalFronts);

/*		i=0;
		while(i<2*numS)
		{
			printf("Solution = %d,\tFront=%d,\tlatency = %d,\tpower = %d,\tcost = %d\n ",i,Solutions[i].FrontIndx,fit[i].latency,fit[i].power,fit[i].cost);
			i++;
		}*/

/*		i=0;
		while(i<numS)
		{
			printf("chosen Solution = %d,\tFront=%d,\tlatency = %d,\tpower = %d,\tcost = %d\n ",solutions_2[i],Solutions[solutions_2[i]].FrontIndx,fit[solutions_2[i]].latency,fit[solutions_2[i]].power,fit[solutions_2[i]].cost);
			i++;
		}*/

		int bestsol;
		bestsol = electre(fit,solutions_2);

		total_latency = total_latency + fit[bestsol].latency;
		total_power = total_power + fit[bestsol].power;
		total_cost = total_cost + fit[bestsol].cost;
/*
		printf("best solution is %d\n",bestsol);

		for(j=0;j<numT;j++)
		{
			for(k=0;k<numV;k++)
			{
				printf("%f\t ",solutions_1[bestsol].sol[k+numV*j]);
			}
			printf("\n");
		}
		printf("\n");

		printf("app charac\n");
		for(i=0;i<numApp;i++)
			for(j=0;j<numT;j++)
				printf("ni = %d\t ds = %d\n",workFlow[j+i*numApp].ni,workFlow[j+i*numApp].ds);*/



	}
	t1 = gTime();

	printf("prop total execution time = %ld\n",(t1-t0));

	printf("Total latency = %f\tTotal Power=%f\tTotal Cost=%f\n",(float)total_latency/10000,(float)total_power/10000,(float)total_cost/10000);


	printf("/*************************/\n");
	printf("Start Shortest Job First Test\n");
	printf("/*************************/\n");

	t0 = gTime();
	ShortestJobFirst();
	t1 = gTime();

	printf("sjf total execution time = %ld\n",(t1-t0));

	total_latency = 0;
	total_power = 0;
	total_cost = 0;

	for(k=0;k<numApp;k++)
		for(i=0;i<numT;i++)
			for(j=0;j<numV;j++)
			{
				total_latency = total_latency+(int) fun_compL(solutions_sjf[k].sol[j+i*numV],workFlow[i+k*numT].ni,v[j].sp) + (int) fun_commL(solutions_sjf[k].sol[j+i*numV],workFlow[i+k*numT].ds,delay);
				total_power = total_power + (int) fun_compE(solutions_sjf[k].sol[j+i*numV],workFlow[i+k*numT].ni,v[j].sp,v[j].theta) + (int) fun_commE(solutions_sjf[k].sol[j+i*numV],workFlow[i+k*numT].ds,v[j].alpha,v[j].beta,v[j].sendband,v[j].recband);
				total_cost = total_cost + (int) fun_compOC(solutions_sjf[k].sol[j+i*numV],workFlow[i+k*numT].ni,v[j].pc) + (int) fun_commOC(solutions_sjf[k].sol[j+i*numV],workFlow[i+k*numT].ds,TransmissionPrice) ;
			}

	printf("Total latency = %f\tTotal Power=%f\tTotal Cost=%f\n",(float)total_latency/10000,(float)total_power/10000,(float)total_cost/10000);




	return 0;
}
