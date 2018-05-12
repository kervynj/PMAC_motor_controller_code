/*
 * PMAC_init.h
 *
 *  Created on: May 3, 2018
 *      Author: mitch
 */

#ifndef PMAC_INIT_H_
#define PMAC_INIT_H_
#define SAMPLE_F ((float) 10000)
#define rotor_f ((float) 500)

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"


// Function Prototypes

void timer_pinConfig(char port, int pin, int tim);
void sample_init();
void sample_interrupt_init();
TIM_OCInitTypeDef threePhase_timerConfig(uint16_t period);
void updateDuty(TIM_TypeDef* TIMx, uint32_t* dutyCount);



#endif /* PMAC_INIT_H_ */
