/*
 * PMAC_init.c
 *
 *  Created on: May 3, 2018
 *      Author: Mitchell Johnston
 */

#include "PMAC_init.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"

// global variables
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_BDTRInitTypeDef		 TIM_BDTRStructure;

void timer_pinConfig(char port, int pin)
{

	uint32_t rcc;
	GPIO_TypeDef *port_num;
	uint32_t pin_num;

	GPIO_InitTypeDef GPIO_InitStructure;

	switch(port)
	{
		case 'A':
			port_num = GPIOA;
			rcc = RCC_AHBPeriph_GPIOA;
			break;
		case 'B':
			port_num = GPIOB;
			rcc = RCC_AHBPeriph_GPIOB;
	}

	pin_num = 0x01 << pin; // left shift to generate corresponding pin address for data bit

	/* GPIOA Clocks enable */
	RCC_AHBPeriphClockCmd(rcc, ENABLE);

	/* GPIOA Configuration: Channel as alternate function 2 (timer1) push-pull */
	GPIO_InitStructure.GPIO_Pin = pin_num;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //set MODER register for GPIO to 0x10 for alternate peripheral
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
	GPIO_Init(port_num, &GPIO_InitStructure);

	GPIO_PinAFConfig(port_num, pin, GPIO_AF_2); // arg(2) - PinSource takes on pin Number values, therefore no bit shifting required
}


TIM_OCInitTypeDef threePhase_timerConfig(uint16_t period)
{

	/* initialize GPIO pin structures for timer outputs
	 *
	 * ___________________________
	 * 	TIM1_CH1	|	PA8
	 * 	TIM1_CH1N   |   PA7
	 * 	TIM1_CH2	|   PA9
	 * 	TIM1_CH2N   |   PB0
	 * 	TIM1_CH3    |   PA10
	 * 	TIM1_CH3N   |   PB1
	 *
	 */

	uint16_t periodCount=0; // TIM1 base timer AAR register value

	/* Compute the value to be set in ARR register to generate signal frequency at 10Khz */
	periodCount = (SystemCoreClock / period ) - 1;

	TIM_OCInitTypeDef  TIM_OCInitStructure;

	// configure GPIO pins for timer 1 as 50MHz push-pull
	timer_pinConfig('A', 8);
	timer_pinConfig('A', 7);
	timer_pinConfig('A', 9);
	timer_pinConfig('B', 0);
	timer_pinConfig('A', 10);
	timer_pinConfig('B', 1);

	/* TIM1 clock enable */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);

	/* Time Base configuration */
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_Period = periodCount;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	/* Channel Configuration in PWM mode */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // set on compare catch
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;


	TIM_BDTRStructInit(&TIM_BDTRStructure); // initialize break & Dead time Register with default values

	TIM_BDTRStructure.TIM_OSSRState = TIM_OSSRState_Enable;
	TIM_BDTRStructure.TIM_DeadTime = (uint16_t)  0x0032; // DT [ns] = val*1e9/(48e6)
	TIM_BDTRStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable; //automatically reset MOE bit on update event flag

	TIM_BDTRConfig(TIM1, &TIM_BDTRStructure);

	TIM_OCInitStructure.TIM_Pulse = 0; // load timer with 0% duty to begin

	/* Configure Channels 1,2,3 for TIM1 with structure parameters  */
	TIM_OC1Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);
	TIM_OC2Init(TIM1, &TIM_OCInitStructure);

	/* TIM1 counter enable */
	TIM_Cmd(TIM1, ENABLE);

	/* TIM1 Main Output Enable */
	TIM_CtrlPWMOutputs(TIM1, ENABLE);

	return TIM_OCInitStructure; // return timer output compare structure to main for functionality
}
