/**
  ******************************************************************************
  * @file    led.h
  * @author  Dan wu
  * @version V1.0.0
  * @date    23-Aug-2017
  * @brief   This file is functions for MSLink mini DFU Bootloader led.
  * @Hardware£º---------------------
  *          |   PA3  - LED_R       |
  *          |   PA4  - LED_G       |
  *           ----------------------
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#ifndef __LED_H__
#define	__LED_H__
#include "stm32f10x.h"

#define LED_R_GPIO_PORT             GPIOA
#define LED_R_GPIO_CLK              RCC_APB2Periph_GPIOA
#define LED_R_GPIO_PIN              GPIO_Pin_3

#define LED_G_GPIO_PORT             GPIOA
#define LED_G_GPIO_CLK              RCC_APB2Periph_GPIOA
#define LED_G_GPIO_PIN              GPIO_Pin_4

/* the macro definition to trigger the Signal enable or disable */
#define ON  0
#define OFF 1
                        
#define LED_R(a)	    if (a)	\
                        GPIO_SetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN);\
                        else		\
                        GPIO_ResetBits(LED_R_GPIO_PORT, LED_R_GPIO_PIN)
                        
#define LED_G(a)	    if (a)	\
                        GPIO_SetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN);\
                        else		\
                        GPIO_ResetBits(LED_G_GPIO_PORT, LED_G_GPIO_PIN)

#define DigitalToggle(p,i)		{p->ODR ^=i;}			//Êä³ö·´×ª×´Ì¬

#define LED_R_TOGGLE    DigitalToggle(LED_R_GPIO_PORT, LED_R_GPIO_PIN)
#define LED_G_TOGGLE    DigitalToggle(LED_G_GPIO_PORT, LED_G_GPIO_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */
