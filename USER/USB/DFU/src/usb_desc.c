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

/* USB标准设备描述符*/
const uint8_t DFU_DeviceDescriptor[DFU_SIZ_DEVICE_DESC] =
{
    0x12,                       /*bLength：长度，设备描述符的长度为18字节*/
    USB_DEVICE_DESCRIPTOR_TYPE, /*bDescriptorType：类型，设备描述符的编号是0x01*/
    0x00,                       /*bcdUSB：所使用的USB版本为2.0*/
    0x02,
    0x00,                       /*bDeviceClass：设备所使用的类代码*/
    0x00,                       /*bDeviceSubClass：设备所使用的子类代码*/
    0x00,                       /*bDeviceProtocol：设备所使用的协议*/
    0x40,                       /*bMaxPacketSize：最大包长度为64字节*/
    
    0x83,                       /*idVendor：厂商ID为0x0483*/
    0x04,
    
    0x11,                       /*idProduct：产品ID为0xDF11*/
    0xDF,
    
    MS_LINK_VERSION_L,          /*bcdDevice：设备的版本号为MS_LINK_VERSION_H.MS_LINK_VERSION_L*/
    MS_LINK_VERSION_H,
    1,                          /*iManufacturer:厂商字符串的索引*/
    2,                          /*iProduct：产品字符串的索引*/
    3,                          /*iSerialNumber：设备的序列号字符串索引*/
    0x01                        /*bNumConfiguration：设备有1种配置*/
}; /* DFU设备描述符 */

/* USB配置描述符集合(配置、接口、端点、类、厂商)(Configuration, Interface, Endpoint, Class, Vendor */
const uint8_t DFU_ConfigDescriptor[DFU_SIZ_CONFIG_DESC] =
{
    0x09, /* bLength: Configuration Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    DFU_SIZ_CONFIG_DESC,
   
    0x00,				  /* wTotalLength: 字节返回 */
		
    0x01,         /*bNumInterfaces：配置所支持的接口数量1个*/
    0x01,         /*bConfigurationValue：该配置的值*/
    0x00,         /*iConfiguration：该配置的字符串的索引值，该值为0表示没有字符串*/ 
    0x80,         /* bmAttributes:设备的一些特性，0x80表示非自供电，不支持远程唤醒	D7:保留必须为1，D6:是否自供电，D5：是否支持远程唤醒，D4~D0：保留设置为0*/
    0x32,         /*从总线上获得的最大电流为100mA */

    /****** Descriptor of DFU interface 0 Alternate setting 0***********/
    0x09,         /*bLength：长度，接口描述符的长度为9字节 */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType：接口描述符的类型为0x4 */
    0x00,         /*bInterfaceNumber：该接口的编号*/
    0x00,         /*bAlternateSetting：该接口的备用编号 */
    0x00,         /*bNumEndpoints：该接口所使用的端点数*/
    0xFE,         /*bInterfaceClass该接口所使用的类为 DFU*/
    0x01,         /*bInterfaceSubClass：该接口所用的子类 1=BOOT, 0=no boot */
    0x02,         /*nInterfaceProtocol :DFU模式*/
    0x04,	 	  /* iInterface:接口字符串描述符的索引 */
    
    /******************** DFU功能描述符********************/
    0x09,   /*blength:DFU描述符的长度为9字节*/
    DFU_DESCRIPTOR_TYPE,   /*功能描述符的类型为0x21*/
    0x0B,   /*bmAttribute
                                             bitCanDnload             = 1      (bit 0)	下载性能
                                             bitCanUpload             = 1      (bit 1)	上传性能
                                             bitManifestationTolerant = 0      (bit 2)	设备在心事阶段后是否可通过USB通讯
                                             bitWillDetach            = 1      (bit 3)	接收到DFU_DETACH命令时会进行总线detach-attac
                                             Reserved                          (bit4-6)	保留
                                             bitAcceleratedST         = 0      (bit 7)  该为置1,设备将会上传速度增加到4096字节/命令*/
    0xFF,   /*DetachTimeOut:超时时间 255 ms*/
    0x00,
    wTransferSizeB0,
    wTransferSizeB1,          /* TransferSize:传输的长度为1024 Byte*/
    0x1A,                     /* bcdDFUVersion:DFU协议版本*/
    0x01   
}; /* MOUSE_ConfigDescriptor */
	
const uint8_t Msotptool_ReportDescriptor[Msotptool_SIZ_REPORT_DESC] =
{
    0x06, 0x00, 0xFF,                   // usage page(﹚竡)
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

//使用UNICODE编码
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

