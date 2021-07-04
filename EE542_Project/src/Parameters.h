/*
 * Parameters.h
 *
 *  Created on: Jan 12, 2021
 *      Author: root
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_


#define numV  20 //for simplicity choose numV as a multiple of numApp
#define numT  256
#define numS  4
#define numApp 5

#define wT	  0.33
#define wP	  0.33
#define wC	  0.33

int TotalFronts;

struct Sol{
	int s[2*numS];
	int n;
	int np;
	int FrontIndx;
};

struct Front{
	int SolIndx[2*numS];
	int size;
};

struct VirtMachine {
   int  sp; // processing speed
   int  theta; //energy consumption rate processing
   int  alpha; // sending energy consumption
   int  beta; // receiving energy consumption
   int  sendband; // sending bandwidth
   int  recband; // receiving bandwidth
   int  pc; //computing price
}VirtMachine;

struct Task{
   int  ni; // number of instructions
   int  ds; // data size in bytes
}Task;


struct encode{
	float sol[numT*numV]; //flattened matrix
}encode;

struct FitnessFun{
	int latency;
	int power;
	int cost;
};



const int delay = 2;
const int TransmissionPrice = 2;

struct Task workFlow[numT*numApp];
struct VirtMachine v[numV];
struct encode solutions[numS];
struct encode solutions_sjf[numApp];
struct encode solutions_1[numS];
/*struct FrontObj	Front_Sol[2*numS];
struct FrontObj	Front_Sol_sort[2*numS];*/
struct FitnessFun fit[2*numS];
struct Front fronts[2*numS+1];
struct Sol Solutions[2*numS];


#endif /* PARAMETERS_H_ */
