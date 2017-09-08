/**
  ******************************************************************************
  * @file    led.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    23-Aug-2017
  * @brief   This file is MSLink mini DFU Bootloader led source code.
  * 
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
  ******************************************************************************/
#include    "led.h"

/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 * ����  ����
 * ���  ����
 */
void LED_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(LED_R_GPIO_CLK | LED_G_GPIO_CLK, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = LED_R_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_R_GPIO_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = LED_G_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_G_GPIO_PORT, &GPIO_InitStructure);
    
    LED_R(ON);
    LED_G(OFF);
}

/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
