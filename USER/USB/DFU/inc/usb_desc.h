/**
  ******************************************************************************
  * @file    usb_desc.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptor Header for Msotptool Mouse Demo
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
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define DFU_DESCRIPTOR_TYPE                     0x21
#define Msotptool_SIZ_HID_DESC                  0x09
#define Msotptool_OFF_HID_DESC                  0x12

#define DFU_SIZ_DEVICE_DESC                     18
#define DFU_SIZ_CONFIG_DESC                     27

#define Msotptool_SIZ_REPORT_DESC  				23
#define DFU_SIZ_STRING_LANGID                   04
#define DFU_SIZ_STRING_VENDOR                   40
#define DFU_SIZ_STRING_PRODUCT                  30
#define DFU_SIZ_STRING_SERIAL                   26
#define DFU_SIZ_STRING_INTERFACE0               96

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

/* Exported functions ------------------------------------------------------- */
extern const uint8_t DFU_DeviceDescriptor[DFU_SIZ_DEVICE_DESC];
extern const uint8_t DFU_ConfigDescriptor[DFU_SIZ_CONFIG_DESC];
extern const uint8_t Msotptool_ReportDescriptor[Msotptool_SIZ_REPORT_DESC];
extern const uint8_t DFU_StringLangId[DFU_SIZ_STRING_LANGID];
extern const uint8_t DFU_StringVendor[DFU_SIZ_STRING_VENDOR];
extern const uint8_t DFU_StringProduct[DFU_SIZ_STRING_PRODUCT];
extern uint8_t DFU_StringSerial[DFU_SIZ_STRING_SERIAL];
extern uint8_t DFU_StringInterface0[DFU_SIZ_STRING_INTERFACE0];

#define bMaxPacketSize0             0x40     /* bMaxPacketSize0 = 64 bytes   */

#define wTransferSize               0x0400   /* wTransferSize   = 1024 bytes */
/* bMaxPacketSize0 <= wTransferSize <= 32kbytes */
#define wTransferSizeB0             0x00
#define wTransferSizeB1             0x04

#endif /* __USB_DESC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
