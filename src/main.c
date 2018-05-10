/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f0xx.h"
#include "stm32f0_discovery.h"
#include "PMAC_init.h"

TIM_OCInitTypeDef  TIM_OCInitStructure;

uint16_t period = 10000; // 10kHz
uint16_t pulseCount=0; //TIM1 CCR register value for toggling edge (duty)

int main(void)
{

	 TIM_OCInitStructure = threePhase_timerConfig(period);

	uint16_t periodCount = (SystemCoreClock / period ) - 1;


	/* Compute CCR1 value to generate a duty cycle at 50% for channel 1 */
	pulseCount = (uint16_t) (((uint32_t) 5 * (periodCount - 1)) / 10); // load 50% duty to begin

	TIM_OCInitStructure.TIM_Pulse = (uint16_t) pulseCount; // update CCR register on update event (counter overflow flag)
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC3Init(TIM1, &TIM_OCInitStructure);

	TIM_Cmd(TIM1, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	sample_init();
	//sample_interrupt_init();

	while(1)
	{
		if (TIM_GetITStatus(TIM16, TIM_IT_CC1) != RESET)
		{
			sample_time_handler();
		}
	}

}
