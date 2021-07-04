/*
 * functions.h
 *
 *  Created on: Jan 14, 2021
 *      Author: root
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

struct Task GenerateTask(void)
{
	struct Task T;
	T.ni = rand() % 1000 + 1;
	T.ds = rand() % 1000 + 1;
	return T;
}

struct VirtMachine GenerateVirt(void)
{
	struct VirtMachine v;
	//v.sp = rand() % 10000 + 1;
/*	v.sp = 1000000;
	v.theta = rand() % 10 + 1;
	v.alpha = rand() % 10 + 1;
	v.beta = rand() % 10 + 1;
	v.sendband = rand() % 1000 + 1;
	v.recband = rand() % 1000 + 1;
	//v.pc = rand() % 10 + 1;
	v.pc = 5;*/

	v.sp = 1000000;
	v.theta = 50;
	v.alpha = 150;
	v.beta = 140;
	v.sendband = 1000;
	v.recband = 1000;
	//v.pc = rand() % 10 + 1;
	v.pc = 1;

	return v;
}

struct encode GenerateSol(void)
{
	struct encode s;
	int acc=0;
	int tmp=0;
	int i;
	int j;
	for(i=0;i<numT;i++)
	{
		acc = 0;
		for(j=0;j<numV;j++)
		{
			tmp = rand()%100;
			acc = acc + tmp;
			s.sol[j+numV*i] = (float)tmp;
		}
		for(j=0;j<numV;j++)
		{
			s.sol[j+numV*i] = s.sol[j+numV*i]/((float) acc);
		}
	}

	for(i=0;i<2*numS;i++)
	{
		Solutions[i].np = 0;
		Solutions[i].n = 0;
		fronts[i].size = 0;
	}
	fronts[i].size = 0;

	return s;
}

float fun_compL(float rij,int ni,int sp)
{
	return rij*((float) ni/sp);
}

float fun_commL(float rij,int ds,int delay)
{
	return rij*((float)ds)*((float)delay);
}


float fun_compE(float rij,float ni,float sp,float theta)
{
	return rij*ni*(1/sp)*theta;
}

float fun_commE(float rij,float ds,float alpha,float beta,float sendband,float recband)
{
	return rij*ds*alpha*(1/sendband)+rij*ds*beta*(1/recband);
}

float fun_compOC(float rij,float ni,float pc)
{
	return rij*ni*pc;
}

float fun_commOC(float rij,float ds,float TransmissionPrice)
{
	return rij*ds*TransmissionPrice;
}

void CrossOver(struct encode* ptr)
{
	int i,j,k;
	struct encode tmp;

	if((numS%2)==0)
	{
		for(i=0;i<numS;i+=2)
			for(j=numT/2;j<numT;j++)
				for(k=0;k<numV;k++)
				{
					tmp.sol[numV*j+k] = ptr[i].sol[numV*j+k];
					ptr[i].sol[numV*j+k] = ptr[i+1].sol[numV*j+k];
					ptr[i+1].sol[numV*j+k] = tmp.sol[numV*j+k];
				}
	}
	else
	{
		for(i=0;i<numS-1;i+=2)
			for(j=numT/2;j<numT;j++)
				for(k=0;k<numV;k++)
				{
					tmp.sol[numV*j+k] = ptr[i].sol[numV*j+k];
					ptr[i].sol[numV*j+k] = ptr[i+1].sol[numV*j+k];
					ptr[i+1].sol[numV*j+k] = tmp.sol[numV*j+k];
				}
	}

}

void Mutation(struct encode* ptr)
{
	int t = rand()%(numT-1);
	int i,j;
	int acc=0;
	int tmp=0;

	for(i=0;i<numS;i++)
	{
		acc = 0;
		for(j=0;j<numV;j++)
		{
			tmp = rand()%100;
			acc = acc + tmp;
			ptr[i].sol[numV*t+j] = (float)tmp;
		}
		for(j=0;j<numV;j++)
		{
			ptr[i].sol[j+numV*t] = ptr[i].sol[j+numV*t]/((float) acc);
		}
	}
}

void Compute_Fitness(struct FitnessFun* fit,struct encode* set_r,int appindx)
{
	int i,j,k;
	for(i=0;i<2*numS;i++)
		{
			fit[i].latency = 0;
			fit[i].power = 0;
			fit[i].cost = 0;

			for(j=0;j<numT;j++)
				for(k=0;k<numV;k++)
				{
					fit[i].latency = fit[i].latency + (int) fun_compL(set_r[i].sol[k+numV*j],workFlow[j+appindx*numT].ni,v[k].sp) + (int) fun_commL(set_r[i].sol[k+numV*j],workFlow[j+appindx*numT].ds,delay);
					fit[i].power = fit[i].power + (int) fun_compE(set_r[i].sol[k+numV*j],workFlow[j+appindx*numT].ni,v[k].sp,v[k].theta) + (int) fun_commE(set_r[i].sol[k+numV*j],workFlow[j+appindx*numT].ds,v[k].alpha,v[k].beta,v[k].sendband,v[k].recband);
					fit[i].cost = fit[i].cost + (int) fun_compOC(set_r[i].sol[k+numV*j],workFlow[j+appindx*numT].ni,v[k].pc) + (int) fun_commOC(set_r[i].sol[k+numV*j],workFlow[j+appindx*numT].ds,TransmissionPrice) ;
				}
		}
}


void NonDominatedSort(struct FitnessFun* ptr)
{
	int i,j,k,z,f;
	z = 0;
	for(i=0;i<2*numS;i++)
	{
		k = 0;
		for(j=0;j<2*numS;j++)
		{
			if(ptr[i].latency < ptr[j].latency && ptr[i].power < ptr[j].power && ptr[i].cost < ptr[j].cost)
			{
				Solutions[i].s[k] = j;
				Solutions[i].n = Solutions[i].n + 1;
				k++;
			}
			else if(ptr[i].latency > ptr[j].latency && ptr[i].power > ptr[j].power && ptr[i].cost > ptr[j].cost)
			{
				Solutions[i].np = Solutions[i].np + 1;
			}
		}

		if(Solutions[i].np == 0)
		{
			Solutions[i].FrontIndx = 0;
			fronts[0].SolIndx[z] = i;
			fronts[0].size = fronts[0].size + 1;
			z++;
		}
	}

	f = 0;

	while(fronts[f].size != 0)
	{
		i = 0;
		z = 0;
		while(i != fronts[f].size)
		{
			j = 0;
			while(j != Solutions[fronts[f].SolIndx[i]].n)
			{
				Solutions[Solutions[fronts[f].SolIndx[i]].s[j]].np = Solutions[Solutions[fronts[f].SolIndx[i]].s[j]].np - 1;
				if(Solutions[Solutions[fronts[f].SolIndx[i]].s[j]].np == 0)
				{
					Solutions[Solutions[fronts[f].SolIndx[i]].s[j]].FrontIndx = f + 1;
					fronts[f+1].SolIndx[z] = Solutions[fronts[f].SolIndx[i]].s[j];
					fronts[f+1].size = fronts[f+1].size + 1;
					z++;
				}
				j++;
			}
			i++;
		}
		f++;
	}

	TotalFronts = f;

}

void nsga(struct encode* ptr,struct encode* out,int* indx,int appindx)
{
	struct encode set_r[2*numS];

	int sz,fnt,chs;

	memcpy(&set_r[0],ptr,sizeof(struct encode)*numS);
	CrossOver(ptr);
	Mutation(ptr);
	memcpy(&set_r[numS],ptr,sizeof(struct encode)*numS);
	//memcpy(out,set_r,sizeof(struct encode)*2*numS);
	Compute_Fitness(fit,set_r,appindx);
	NonDominatedSort(fit);

	//for now use simple selection method, if time permit more complicated selection methods will be designed
	sz = 0;
	fnt = 0;
	chs = 0;
	while(sz != numS)
	{
		indx[sz] = fronts[fnt].SolIndx[chs];
		//memcpy(&out[sz],&set_r[fronts[fnt].SolIndx[chs]],sizeof(struct encode));
		sz++;
		chs++;
		if(sz == fronts[fnt].size)
		{
			fnt++;
			chs = 0;
		}

	}
}

int electre(struct FitnessFun* ptr,int* indx)
{
	float comb_mat[numS*3];
	int i,j;
	long int smsT,smsP,smsC;

	smsT = 0;
	smsP = 0;
	smsC = 0;

	for(i=0;i<numS;i++)
	{
		comb_mat[i] =  ptr[indx[i]].latency;
		smsT = smsT + pow((ptr[indx[i]].latency),2);
	}

	for(i=0;i<numS;i++)
	{
		comb_mat[i+numS] =  ptr[indx[i]].power;
		smsP = smsP + pow((ptr[indx[i]].power),2);
	}

	for(i=0;i<numS;i++)
	{
		comb_mat[i+2*numS] =  ptr[indx[i]].cost;
		smsC = smsC + pow((ptr[indx[i]].cost),2);
	}

	smsT = sqrt(smsT);
	smsP = sqrt(smsP);
	smsC = sqrt(smsC);

	for(i=0;i<numS;i++)
		comb_mat[i] = wT*(comb_mat[i] /((float) smsT));

	for(i=0;i<numS;i++)
		comb_mat[i+numS] = wP*(comb_mat[i+numS] /((float) smsP));

	for(i=0;i<numS;i++)
		comb_mat[i+2*numS] = wC*(comb_mat[i+2*numS] /((float) smsC));


/*	for(i=0;i<3;i++)
	{
		for(j=0;j<numS;j++)
			printf("%f\t",comb_mat[j+i*numS]);
		printf("\n");
	}*/

	float conMat[numS*numS] = {0};

	for(i=0;i<numS;i++)
	{
		for(j=0;j<numS;j++)
			if(comb_mat[i] > comb_mat[j])
				conMat[j+i*numS] = conMat[j+i*numS] + wT;

		for(j=0;j<numS;j++)
			if(comb_mat[i+numS] > comb_mat[j+numS])
				conMat[j+i*numS] = conMat[j+i*numS] + wP;

		for(j=0;j<numS;j++)
			if(comb_mat[i+2*numS] > comb_mat[j+2*numS])
				conMat[j+i*numS] = conMat[j+i*numS] + wC;
	}


	float desMat[numS*numS] = {0};
	//float desMatp[numS*numS] = {0};
	float a,b,c,aa,bb,cc;

	for(i=0;i<numS;i++)
	{
		for(j=0;j<numS;j++)
		{
			if(comb_mat[i] < comb_mat[j])
				a = fabs(comb_mat[i] - comb_mat[j]);
			else
				a = 0;

			aa = fabs(comb_mat[i] - comb_mat[j]);

			if(comb_mat[i+numS] < comb_mat[j+numS])
				b = fabs(comb_mat[i+numS] - comb_mat[j+numS]);
			else
				b = 0;

			bb = fabs(comb_mat[i+numS] - comb_mat[j+numS]);

			if(bb > aa)
				aa = bb;
			if(b>a)
				a = b;

			if(comb_mat[i+2*numS] < comb_mat[j+2*numS])
				c = fabs(comb_mat[i+2*numS] - comb_mat[j+2*numS]);
			else
				c = 0;

			cc = fabs(comb_mat[i+2*numS] - comb_mat[j+2*numS]);

			if(cc>aa)
				aa = cc;
			if(c>a)
				a = c;

			if(i!=j && aa!=0)
				desMat[j+numS*i] = (1-(a / aa));
		}
	}

	float S[numS*numS] = {0};
	for(i=0;i<numS;i++)
		for(j=0;j<numS;j++)
			S[j+numS*i] = conMat[j+numS*i]*desMat[j+numS*i];


	float util[numS]={0};
	float maxU=1000;
	int maxS;
	for(i=0;i<numS;i++)
	{
		for(j=0;j<numS;j++)
		{
			util[i] = util[i] + (S[j+i*numS] - S[i+j*numS]);
		}
		if(util[i] < maxU)
		{
			maxU = util[i];
			maxS = i;
		}
	}

/*	for(i=0;i<numS;i++)
	{
		for(j=0;j<numS;j++)
			printf("%f\t",S[j+numS*i]);
		printf("\n");
	}

	for(i=0;i<numS;i++)
	{
		printf("util %d = %f\n",i,util[i]);
	}*/

	//printf("max util is for sol %d\n",maxS);
	return indx[maxS];
}




#endif /* FUNCTIONS_H_ */
