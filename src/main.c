/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    12-May-2018
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "PMAC_init.h"
#include "SVM_PWM.h"

TIM_OCInitTypeDef  TIM_OCInitStructure;

float t = 0;
int count = 0;
int sector = 1;
float theta = 0;
float adjTheta = 0;

int* Va;
int* Vb;
uint32_t* duties;
float t1;
float t2;
float to;


uint16_t pulseCount=0; //TIM1 CCR register value for toggling edge (duty)
uint16_t periodCount = 0;

int main(void)
{

	TIM_OCInitStructure = threePhase_timerConfig(SAMPLE_F);

	periodCount = (SystemCoreClock / ((uint16_t) SAMPLE_F )) - 1;
	pulseCount = (uint16_t) (((uint32_t) 5 * (periodCount - 1)) / 10); // load 50% duty to begin

	uint32_t initial[3] = {pulseCount};

	updateDuty(TIM1, initial);

	/* TIM1 Main Output Enable */
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	sample_init();

	while(1)
	{

		if (TIM_GetITStatus(TIM16, TIM_IT_CC1) != RESET)
		{
			TIM_ClearITPendingBit(TIM16, TIM_IT_CC1); // clear interrupt

			t = t + (1/SAMPLE_F); // iterate to new elapsed time
			theta = vref_calc(&t, &count); // compute theta reference angle
			sector = section(theta, &adjTheta);	   // evaluate sector

			Va = vectorA(sector); // determine switching vector for T1
			Vb = vectorB(sector); // determine switching vector for T2

			durations(&t1, &t2, &to, adjTheta);	// fetch switching durations for t1, t2, to

			duties = threePhase_SVM(periodCount, to, t1, t2, Va, Vb); // compute duty cycles for vector addition Vref = Va + Vb

			updateDuty(TIM1, duties); // modulate duty cycles to synthesize SVM switching waveform
		}
	}

}
