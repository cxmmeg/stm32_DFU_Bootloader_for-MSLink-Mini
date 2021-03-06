/**
  ******************************************************************************
  * @file    usb_prop.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processing related to Msotptool Mouse demo
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
#ifndef __USB_PROP_H
#define __USB_PROP_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;

typedef struct HID_otprw
{
    uint8_t usb_hid_tx_buffer[64];
    uint8_t usb_hid_rx_buffer[64];
    uint8_t set_report_flag;
    uint16_t otp_read_addr ;
    uint16_t otp_write_addr;
    uint16_t otp_read_length ;
    uint8_t otp_write_length ;
    uint8_t icp_status ;
}
HID_otprw_TypeDef;

extern HID_otprw_TypeDef ms8005otp;

/* Exported constants --------------------------------------------------------*/

/* Definition of MS HID command */
#define MS_HID_CMD_GET_VERSION          0x30    //get mslink firmware version
#define MS_HID_CMD_GET_CONFIG           0x60    //get mslink config information
#define MS_HID_CMD_SET_CONFIG           0x80    //Set mslink config information
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void set_report_deocder(void);
void DFU_init(void);
void DFU_Reset(void);
void DFU_SetConfiguration(void);
void DFU_SetDeviceAddress (void);
void DFU_Status_In (void);
void DFU_Status_Out (void);
RESULT DFU_Data_Setup(uint8_t);
RESULT DFU_NoData_Setup(uint8_t);
RESULT DFU_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *DFU_GetDeviceDescriptor(uint16_t );
uint8_t *DFU_GetConfigDescriptor(uint16_t);
uint8_t *DFU_GetStringDescriptor(uint16_t);
uint8_t *UPLOAD(uint16_t Length);
uint8_t *DNLOAD(uint16_t Length);
uint8_t *GETSTATE(uint16_t Length);
uint8_t *GETSTATUS(uint16_t Length);
void DFU_write_crc (void);

/* Exported define -----------------------------------------------------------*/
#define Msotptool_GetConfiguration          NOP_Process
#define Msotptool_GetInterface              NOP_Process
#define Msotptool_SetInterface              NOP_Process
#define DFU_GetStatus                       NOP_Process
#define Msotptool_ClearFeature              NOP_Process
#define Msotptool_SetEndPointFeature        NOP_Process
#define Msotptool_SetDeviceFeature          NOP_Process

#define REPORT_DESCRIPTOR                  0x22

/**************************************************/
/* DFU Requests                                   */
/**************************************************/

typedef enum _DFU_REQUESTS {
  DFU_DNLOAD = 1,
  DFU_UPLOAD,
  DFU_GETSTATUS,
  DFU_CLRSTATUS,
  DFU_GETSTATE,
  DFU_ABORT
} DFU_REQUESTS;

/**************************************************/
/* DFU Requests  DFU states                       */
/**************************************************/


#define STATE_appIDLE                 0
#define STATE_appDETACH               1
#define STATE_dfuIDLE                 2
#define STATE_dfuDNLOAD_SYNC          3
#define STATE_dfuDNBUSY               4
#define STATE_dfuDNLOAD_IDLE          5
#define STATE_dfuMANIFEST_SYNC        6
#define STATE_dfuMANIFEST             7
#define STATE_dfuMANIFEST_WAIT_RESET  8
#define STATE_dfuUPLOAD_IDLE          9
#define STATE_dfuERROR                10

/**************************************************/
/* DFU Requests  DFU status                       */
/**************************************************/

#define STATUS_OK                   0x00
#define STATUS_ERRTARGET            0x01
#define STATUS_ERRFILE              0x02
#define STATUS_ERRWRITE             0x03
#define STATUS_ERRERASE             0x04
#define STATUS_ERRCHECK_ERASED      0x05
#define STATUS_ERRPROG              0x06
#define STATUS_ERRVERIFY            0x07
#define STATUS_ERRADDRESS           0x08
#define STATUS_ERRNOTDONE           0x09
#define STATUS_ERRFIRMWARE          0x0A
#define STATUS_ERRVENDOR            0x0B
#define STATUS_ERRUSBR              0x0C
#define STATUS_ERRPOR               0x0D
#define STATUS_ERRUNKNOWN           0x0E
#define STATUS_ERRSTALLEDPKT        0x0F

/**************************************************/
/* DFU Requests  DFU states Manifestation State   */
/**************************************************/

#define Manifest_complete           0x00
#define Manifest_In_Progress        0x01


/**************************************************/
/* Special Commands  with Download Request        */
/**************************************************/

#define CMD_GETCOMMANDS              0x00
#define CMD_SETADDRESSPOINTER        0x21
#define CMD_ERASE                    0x41

#endif /* __USB_PROP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
