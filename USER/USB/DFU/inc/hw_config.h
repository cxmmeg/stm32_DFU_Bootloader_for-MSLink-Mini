/**
  ******************************************************************************
  * @file    hw_config.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Hardware Configuration & Setup
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include "platform_config.h"
#include "usb_type.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Definition of MS Link Version */
#define MS_LINK_VERSION_L               0x00  //�ΰ汾��
#define MS_LINK_VERSION_H               0x02  //���汾��

#define ApplicationAddress              0x08003800
/* Exported macro ------------------------------------------------------------*/
#define DFU_ENTER                       GPIOB
#define RCC_APB2Periph_GPIO_DFU         RCC_APB2Periph_GPIOB
#define DFU_ENTER_PIN                   GPIO_Pin_14

/* Exported define -----------------------------------------------------------*/
#define CURSOR_STEP     20

#define DOWN            2
#define LEFT            3
#define RIGHT           4
#define UP              5

/* Exported functions ------------------------------------------------------- */
void DFU_Button_Config(void);
uint8_t DFU_Button_Read (void);
void Set_System(void);
void Set_USBClock(void);
void GPIO_AINConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void Msotptool_Send(u8 buf0,u8 buf1,u8 buf2,u8 buf3);
void Get_SerialNum(void);

#endif  /*__HW_CONFIG_H*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
