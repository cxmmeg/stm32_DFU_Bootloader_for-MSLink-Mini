/**
  ******************************************************************************
  * @file    main.c
  * @author  Dan wu
  * @version V1.0.0
  * @date    23-Aug-2017
  * @brief   This file is MSLink mini DFU Bootloader source code.
  *
  * Copyright (c) 2017, MacroSilicon Technology Co.,Ltd.
***********************************************************/
#include "stm32f10x.h"
#include "led.h"
#include "SysTick.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "usb_istr.h"
#include "usb_conf.h"
#include "usb_prop.h"
#include "dfu_mal.h"
#include "platform_config.h"


typedef  void (*pFunction)(void);

uint8_t DeviceState;
uint8_t DeviceStatus[6];
pFunction Jump_To_Application;
uint32_t JumpAddress;

void usb_port_set(u8 enable)
{
    RCC->APB2ENR|=1<<2;
    if(enable)_SetCNTR(_GetCNTR()&(~(1<<1)));
    else
    {     
        _SetCNTR(_GetCNTR()|(1<<1));
        GPIOA->CRH&=0XFFF00FFF;
        GPIOA->CRH|=0X00033000;
        GPIOA->ODR &=~(1<<12);
    }
}

int main(void)
{
    SystemInit();
    SysTick_Init();
    DFU_Button_Config();
    LED_GPIO_Config();
    if (DFU_Button_Read() != 0x00)
    { 
        //if (((*(__IO uint32_t*)ApplicationAddress) & 0x2FFE0000 ) == 0x08000000)    //检验跳转地址区域是否正确
        { 		
            JumpAddress = *(__IO uint32_t*) (ApplicationAddress + 4);               //获取跳转程序的RESET中断向量地址
            Jump_To_Application = (pFunction) JumpAddress;                          //强制转换成函数指针
            __set_MSP(*(__IO uint32_t*) ApplicationAddress);                        //将该地址写入R14
            Jump_To_Application();                                                  //调用函数，当函数调用结束时，PC指正指向R14的地址
        }
    }

    FLASH_Unlock();	
	/* Enter DFU mode */
	DeviceState = STATE_dfuERROR;
	DeviceStatus[0] = STATUS_ERRFIRMWARE;
	DeviceStatus[4] = DeviceState;
	//USB_Configuration();
    Delay_us(10);
    usb_port_set(0);
    Delay_us(30);
    usb_port_set(1);
    
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();
    
	while(1)
	{
		 LED_G_TOGGLE;
		 Delay_ms(1000);
	}
    
    return 0;
}

