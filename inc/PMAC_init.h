/*
 * PMAC_init.h
 *
 *  Created on: May 3, 2018
 *      Author: mitch
 */

#ifndef PMAC_INIT_H_
#define PMAC_INIT_H_
#define sample_f ((uint16_t) 1000)

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"
#include "stm32f0xx_misc.h"


// Function Prototypes

void timer_pinConfig(char port, int pin, int tim);
void sample_init();
void sample_interrupt_init();
void sample_time_handler();
TIM_OCInitTypeDef threePhase_timerConfig(uint16_t period);



#endif /* PMAC_INIT_H_ */
