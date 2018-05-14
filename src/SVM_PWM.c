/*
 * SVM_PWM.c
 *
 *  Created on: May 11, 2018
 *      Author: Mitch
 */

#include "PMAC_init.h"
#include "SVM_PWM.h"
#include <math.h>


float vref_calc(float* t, int* count)
{
	float theta;
	int cnt = *count;

	*t = *t + cnt*(1/SAMPLE_F);

	if (*t > (1/rotor_f))
	{
		*t = *t - (1/rotor_f);
		*count = 1;
	}

	theta = 2*PI*rotor_f*(*t);

	return theta;

}


int section(float theta, float* adjTheta)
{
	int sector = 1;

	// Sector 1
	if (theta > 0 && theta < PI/3 )
	{
		sector = 1;
		*adjTheta = theta;
	}
	// Sector 2
	else if (theta > PI/3 && theta < (2*PI/3))
	{
		sector = 2;
		*adjTheta = theta - PI/3;
	}
	// Sector 3
	else if (theta > (2*PI/3) && theta < (PI))
	{
		sector = 3;
		*adjTheta = theta - 2*PI/3;
	}
	// Sector 4
	else if (theta > (PI) && theta < (4*PI/3))
	{
		sector = 4;
		*adjTheta = theta - PI;
	}
	// Sector 5
	else if (theta > (4*PI/3) && theta < (5*PI/3))
	{
		sector = 5;
		*adjTheta = theta - 4*PI/3;
	}
	// Sector 6
	else
	{
		sector = 6;
		*adjTheta = theta - 5*PI/3;
	}

	return sector;
}

void durations(float* to, float* t1, float* t2, float theta)
{
	*t1 = 1.73*(1/SAMPLE_F)*(VREF)*(sinf(1.047- theta))/VDC; // switching time for V1
	*t2 = 1.73*(1/SAMPLE_F)*(VREF)*(sinf(theta))/VDC;		// switching time for V2
	*to = (1/SAMPLE_F) - (*t1 + *t2);					   // 2x switching times for vector V7 (fully on) & V0 (fully off)
}


int* vectorA(int sector)
{
	static int V[3];

	switch(sector)
	{
		case 1:
			V[0] = 1;
			V[1] = 0;
			V[2] = 0;
			break;
		case 2:
			V[0] = 1;
			V[1] = 1;
			V[2] = 0;
			break;
		case 3:
			V[0] = 0;
			V[1] = 1;
			V[2] = 0;
			break;
		case 4:
			V[0] = 0;
			V[1] = 1;
			V[2] = 1;
			break;
		case 5:
			V[0] = 0;
			V[1] = 0;
			V[2] = 1;
			break;
		case 6:
			V[0] = 1;
			V[1] = 0;
			V[2] = 1;
			break;
	}
	return V;
}


int* vectorB(int sector)
{
	static int V[3];

	switch(sector)
	{
		case 1:
			V[0] = 1;
			V[1] = 1;
			V[2] = 0;
			break;
		case 2:
			V[0] = 0;
			V[1] = 1;
			V[2] = 0;
			break;
		case 3:
			V[0] = 0;
			V[1] = 1;
			V[2] = 1;
			break;
		case 4:
			V[0] = 0;
			V[1] = 0;
			V[2] = 1;
			break;
		case 5:
			V[0] = 1;
			V[1] = 0;
			V[2] = 1;
			break;
		case 6:
			V[0] = 1;
			V[1] = 0;
			V[2] = 0;
			break;
	}
	return V;
}


uint32_t* threePhase_SVM(uint16_t periodCount, float to, float t1, float t2, int V1[], int V2[])
{
	static uint32_t d[3];

	for (int i=0; i <3; i++)
	{
		if ((V1[i] ==0) && (V2[i] ==0)) //  Switching bits 00, positive duty cycle limited to to/2 (V7)
		{
			d[i] = (SystemCoreClock*(to/2 + t1 + t2));
		}
		else if ((V1[i] ==0) && (V2[i] ==1)) // Switching bits are 01, positive duty cycle for t2 + to/2 (V2 & V7)
		{
			d[i] = (SystemCoreClock*(to/2 + t1));
		}
		else if ((V1[i] ==1) && (V2[i]==1)) // Switching bits are 11, positive duty cycle for t1 + t2 + to/2
		{
			d[i] = (SystemCoreClock*(to/2));
		}
		else if ((V1[i] == 1) && (V2[i]==0)) // Switching bits are 10, positive duty cycle for t1 + to/2 (V1 & V7)
		{
			d[i] = (SystemCoreClock*(to/2 + t2)); // switch V1 & V2 in switching sequence to avoid frequency modulation
		}
	}

	return d;

}


