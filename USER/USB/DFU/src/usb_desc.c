/**
  ******************************************************************************
  * @file    usb_desc.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Descriptors for Msotptool Mouse Demo
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


/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/* USB��׼�豸������*/
const uint8_t DFU_DeviceDescriptor[DFU_SIZ_DEVICE_DESC] =
{
    0x12,                       /*bLength�����ȣ��豸�������ĳ���Ϊ18�ֽ�*/
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType�����ͣ��豸�������ı����0x01*/
    0x00,                       /*bcdUSB����ʹ�õ�USB�汾Ϊ2.0*/
    0x02,
    0x00,                       /*bDeviceClass���豸��ʹ�õ������*/
    0x00,                       /*bDeviceSubClass���豸��ʹ�õ��������*/
    0x00,                       /*bDeviceProtocol���豸��ʹ�õ�Э��*/
    0x40,                       /*bMaxPacketSize����������Ϊ64�ֽ�*/
    
    0x83,                       /*idVendor������IDΪ0x0483*/
    0x04,
    
    0x11,                       /*idProduct����ƷIDΪ0xDF11*/
    0xDF,
    
    MS_LINK_VERSION_L,          /*bcdDevice���豸�İ汾��ΪMS_LINK_VERSION_H.MS_LINK_VERSION_L*/
    MS_LINK_VERSION_H,
    1,                          /*iManufacturer:�����ַ���������*/
    2,                          /*iProduct����Ʒ�ַ���������*/
    3,                          /*iSerialNumber���豸�����к��ַ�������*/
    0x01                        /*bNumConfiguration���豸��1������*/
}; /* DFU�豸������ */

/* USB��������������(���á��ӿڡ��˵㡢�ࡢ����)(Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t DFU_ConfigDescriptor[DFU_SIZ_CONFIG_DESC] =
{
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    DFU_SIZ_CONFIG_DESC,
   
    0x00,				  /* wTotalLength: �ֽڷ��� */
		
    0x01,         /*bNumInterfaces��������֧�ֵĽӿ�����1��*/
    0x01,         /*bConfigurationValue�������õ�ֵ*/
    0x00,         /*iConfiguration�������õ��ַ���������ֵ����ֵΪ0��ʾû���ַ���*/ 
    0x80,         /* bmAttributes:�豸��һЩ���ԣ�0x80��ʾ���Թ��磬��֧��Զ�̻���	D7:��������Ϊ1��D6:�Ƿ��Թ��磬D5���Ƿ�֧��Զ�̻��ѣ�D4~D0����������Ϊ0*/
    0x32,         /*�������ϻ�õ�������Ϊ100mA */

    /****** Descriptor of DFU interface 0 Alternate setting 0***********/
    0x09,         /*bLength�����ȣ��ӿ��������ĳ���Ϊ9�ֽ� */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType���ӿ�������������Ϊ0x4 */
    0x00,         /*bInterfaceNumber���ýӿڵı��*/
    0x00,         /*bAlternateSetting���ýӿڵı��ñ�� */
    0x00,         /*bNumEndpoints���ýӿ���ʹ�õĶ˵���*/
    0xFE,         /*bInterfaceClass�ýӿ���ʹ�õ���Ϊ DFU*/
    0x01,         /*bInterfaceSubClass���ýӿ����õ����� 1=BOOT, 0=no boot */
    0x02,         /*nInterfaceProtocol :DFUģʽ*/
    0x04,	 	  /* iInterface:�ӿ��ַ��������������� */
    
    /******************** DFU����������********************/
    0x09,   /*blength:DFU�������ĳ���Ϊ9�ֽ�*/
    DFU_DESCRIPTOR_TYPE,   /*����������������Ϊ0x21*/
    0x0B,   /*bmAttribute
                                             bitCanDnload             = 1      (bit 0)	��������
                                             bitCanUpload             = 1      (bit 1)	�ϴ�����
                                             bitManifestationTolerant = 0      (bit 2)	�豸�����½׶κ��Ƿ��ͨ��USBͨѶ
                                             bitWillDetach            = 1      (bit 3)	���յ�DFU_DETACH����ʱ���������detach-attac
                                             Reserved                          (bit4-6)	����
                                             bitAcceleratedST         = 0      (bit 7)  ��Ϊ��1,�豸�����ϴ��ٶ����ӵ�4096�ֽ�/����*/
    0xFF,   /*DetachTimeOut:��ʱʱ�� 255 ms*/
    0x00,
    wTransferSizeB0,
    wTransferSizeB1,          /* TransferSize:����ĳ���Ϊ1024 Byte*/
    0x1A,                     /* bcdDFUVersion:DFUЭ��汾*/
    0x01   
}; /* MOUSE_ConfigDescriptor */
	
const uint8_t Msotptool_ReportDescriptor[Msotptool_SIZ_REPORT_DESC] =
{
    0x06, 0x00, 0xFF,                   // usage page(�۩w�q)
    0x09, 0x01,                         // usage(0) undefined
    0xA1, 0x01,                         // Collection(Application)
 //   0x85, 0x01,                         // REPORT_ID (1)
    0x15, 0x00,                         // Logical Minimum (0)
    0x26, 0xFF, 0x00,                   // Logical Maximum (255)
    0x19, 0x01,                         // Usage Minimum(0x01)
    0x29, 0x02,                         // Usage Maximum(0x05)
    0x75, 0x08,                         // REPORT SIZE (8)
    0x95, 0x40,                         // REPORT COUNT(64)
    0xB1, 0x02,                         // FEATURE(DATA,VARIABLE,ABSOLUTE)
    0xC0 ,                              // END COLLECTION
}; /* Msotptool_ReportDescriptor */

/* USB String Descriptors (optional) */
const uint8_t DFU_StringLangId[DFU_SIZ_STRING_LANGID] =
{
    DFU_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04
}; /* LangID = 0x0409: U.S. English */

//ʹ��UNICODE����
const u8 DFU_StringVendor[DFU_SIZ_STRING_VENDOR] =	    
{
	DFU_SIZ_STRING_VENDOR,    /* Size of manufaturer string */
	USB_STRING_DESCRIPTOR_TYPE,     /* bDescriptorType = String descriptor */
	/* Manufacturer: "MACROSILICON" */ 
	'H',0x00, 'e',0x00, 'f',0x00, 'e',0x00, 'i',0x00, ' ',0x00,
	'M',0x00, 'a',0x00, 'c',0x00, 'r',0x00, 'o',0x00,
    'S',0x00, 'i',0x00, 'l',0x00, 'i',0x00, 'c',0x00, 'o',0x00, 'n',0x00, ' ',0x00,
};

const u8 DFU_StringProduct[DFU_SIZ_STRING_PRODUCT] =
{
	DFU_SIZ_STRING_PRODUCT,          /* bLength */
	USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'M',0x00, 'S',0x00, '8',0x00, '0',0x00, '0',0x00, '5',0x00, ' ',0x00,
    'M',0x00, 'S',0x00, 'L',0x00, 'i',0x00, 'n',0x00, 'k',0x00, ' ',0x00, 
//    'd',0x00, 'o',0x00, 'n',0x00, 'g',0x00, 'l',0x00, 'e',0x00, ' ',0x00, 
//    'V',0x00, '2',0x00, '.',0x00, '0',0x00, '1',0x00, ' ',0x00,
};

uint8_t DFU_StringSerial[DFU_SIZ_STRING_SERIAL] =
  {
    DFU_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0
  };
  
uint8_t DFU_StringInterface0[DFU_SIZ_STRING_INTERFACE0] =
  {
    DFU_SIZ_STRING_INTERFACE0,
    0x03,
    // Interface 0: "@Internal Flash   /0x08000000/12*001Ka,500*001Kg"
    '@', 0, 'I', 0, 'n', 0, 't', 0, 'e', 0, 'r', 0, 'n', 0, 'a', 0, 'l', 0,  /* 18 */
    ' ', 0, 'F', 0, 'l', 0, 'a', 0, 's', 0, 'h', 0, ' ', 0, ' ', 0, /* 16 */

    '/', 0, '0', 0, 'x', 0, '0', 0, '8', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, '0', 0, /* 22 */

    '/', 0, '1', 0, '2', 0, '*', 0, '0', 0, '0', 0, '1', 0, 'K', 0, 'a', 0, /* 18 */
    ',', 0, '5', 0, '0', 0, '0', 0, '*', 0, '0', 0, '0', 0, '1', 0, 'K', 0, 'g', 0, /* 20 */
  };
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

