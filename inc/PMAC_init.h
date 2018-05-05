/*
 * PMAC_init.h
 *
 *  Created on: May 3, 2018
 *      Author: mitch
 */

#ifndef PMAC_INIT_H_
#define PMAC_INIT_H_

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_gpio.h"
#include "stm32f0xx_tim.h"

// Function Prototypes

void timer_pinConfig(char port, int pin);
TIM_OCInitTypeDef threePhase_timerConfig(uint16_t period);



#endif /* PMAC_INIT_H_ */
