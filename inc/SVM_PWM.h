/*
 * SVM_PWM.h
 *
 *  Created on: May 11, 2018
 *      Author: mitch
 */

#ifndef SVM_PWM_H_
#define SVM_PWM_H_

#define PI ((float) 3.14152)
#define VDC ((int) 12)
#define VREF ((float) VDC/1.73)

//function prototypes
float vref_calc(float* t, int* count);
int section(float theta, float* adjTheta);
void durations(float* to, float* t1, float* t2, float theta);
int* vectorA(int sector);
int* vectorB(int sector);
uint32_t* threePhase_SVM(uint16_t periodCount, float to, float t1, float t2, int V1[], int V2[]);


#endif /* SVM_PWM_H_ */
