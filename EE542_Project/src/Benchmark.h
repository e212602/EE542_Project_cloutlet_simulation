/*
 * Benchmark.h
 *
 *  Created on: Jan 24, 2021
 *      Author: root
 */

#ifndef BENCHMARK_H_
#define BENCHMARK_H_


/*
 * Pick Ready task with shortest computing latency
 */


void ShortestJobFirst(void)
{
	int nT = numT;
	int i,j,k,z;
	float minT = 10000;
	float comptime;
	int numAll;

	float AllocT[numApp] = {100};
	int   AllTid[numApp];

	float tmpV,tmpV1;
	int   tmpID,tmpID1;


	numAll = 0;
	i = 0;
	while(numAll != nT)
	{
		i = 0;
			for(k=0;k<numApp;k++)
			{
				AllocT[k] = 100;
				AllTid[k] = 0;
			}

			for(k=0;k<numApp;k++)
			{
				comptime = fun_compL(1,workFlow[numAll+k*numT].ni,v[i].pc);
				for(j=0;j<numApp;j++)
				{
					if(comptime < AllocT[j])
					{
						tmpV = AllocT[j];
						tmpID = AllTid[j];

						AllocT[j] = comptime;
						AllTid[j] = k;
						for(z=j+1;z<numApp;z++)
						{
							tmpV1 = AllocT[z];
							tmpID1 = AllTid[z];

							AllocT[z] = tmpV;
							AllTid[z] = tmpID;

							tmpV = tmpV1;
							tmpID = tmpID1;
						}
						break;
					}
					if(j==numApp-1)
					{
						AllocT[j] = comptime;
						AllTid[j] = k;
					}
				}
			}
			for(k=0;k<numApp;k++)
			{
				solutions_sjf[AllTid[k]].sol[numAll*numV+k+i] = 1;
			}
			i = i + numApp;
			if(i==numV)
				i=0;
			numAll++;

	}

/*	for(i=0;i<numApp;i++)
	{
		for(j=0;j<numT;j++)
			printf("%d\t",workFlow[j+numT*i].ni);
		printf("\n");
	}

	for(i=0;i<numApp;i++)
	{
		printf("app = %d\n",i);
		for(j=0;j<numT;j++)
		{
			for(k=0;k<numV;k++)
			{
				printf("%f\t",solutions_sjf[i].sol[k+numV*j]);
			}
			printf("\n");
		}
	}*/
}




#endif /* BENCHMARK_H_ */
