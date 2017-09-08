/**
  ******************************************************************************
  * @file    usb_prop.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processing related to Msotptool Mouse Demo
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
#include "usb_conf.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "hw_config.h"
#include "dfu_mal.h"
#include "stm32f10x.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;

HID_otprw_TypeDef ms8005otp;

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    DFU_init,
    DFU_Reset,
    DFU_Status_In,
    DFU_Status_Out,
    DFU_Data_Setup,
    DFU_NoData_Setup,
    DFU_Get_Interface_Setting,
    DFU_GetDeviceDescriptor,
    DFU_GetConfigDescriptor,
    DFU_GetStringDescriptor,
    0,                    /*DFU_EP0Buffer*/
    bMaxPacketSize0       /*Max Packet size*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    Msotptool_GetConfiguration,
    DFU_SetConfiguration,
    Msotptool_GetInterface,
    Msotptool_SetInterface,
    DFU_GetStatus,
    Msotptool_ClearFeature,
    Msotptool_SetEndPointFeature,
    Msotptool_SetDeviceFeature,
    DFU_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)DFU_DeviceDescriptor,
    DFU_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)DFU_ConfigDescriptor,
    DFU_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR Msotptool_Report_Descriptor =
  {
    (uint8_t *)Msotptool_ReportDescriptor,
    Msotptool_SIZ_REPORT_DESC
  };

ONE_DESCRIPTOR Mouse_Hid_Descriptor =
  {
    (uint8_t*)DFU_ConfigDescriptor + Msotptool_OFF_HID_DESC,
    Msotptool_SIZ_HID_DESC
  };

ONE_DESCRIPTOR DFU_String_Descriptor[5] = //注册字符串描述符
  {
    {       (uint8_t*)DFU_StringLangId,          DFU_SIZ_STRING_LANGID       },//语言字符串
    {       (uint8_t*)DFU_StringVendor,          DFU_SIZ_STRING_VENDOR       },//厂商字符串
    {       (uint8_t*)DFU_StringProduct,         DFU_SIZ_STRING_PRODUCT      },//产品字符串
    {       (uint8_t*)DFU_StringSerial,          DFU_SIZ_STRING_SERIAL       },//序列号字符串
    {       (uint8_t*)DFU_StringInterface0,      DFU_SIZ_STRING_INTERFACE0   } //接口描述符
  };

uint32_t wBlockNum = 0, wlength = 0;
uint32_t Manifest_State = Manifest_complete;	
uint32_t Pointer = ApplicationAddress;  		//程序擦写、读写的基地址

/* Extern variables ----------------------------------------------------------*/
extern  uint8_t DeviceState ;
extern  uint8_t DeviceStatus[6];

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : DFU_init.
* Description    : DFU初始化.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_init(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  Get_SerialNum();					//获取序列号

  pInfo->Current_Configuration = 0;	//初始化当前配置值

  PowerOn();						//连接设备

  USB_SIL_Init();					//执行基本的设备初始化操作

  USB_Interrupts_Config();			//使能USB中断

  bDeviceState = UNCONNECTED;
}

/*******************************************************************************
* Function Name  : DFU_Reset.
* Description    : DFU 复位
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_Reset(void)
{
  Device_Info.Current_Configuration = 0;

  /* Current Feature initialization */
  pInformation->Current_Feature = DFU_ConfigDescriptor[7]; //获取当前的特性

  _SetBTABLE(BTABLE_ADDRESS);

  /* Initialize Endpoint 0 */							   
  _SetEPType(ENDP0, EP_CONTROL);						   //设置端点0为控制端点
  _SetEPTxStatus(ENDP0, EP_TX_NAK);						   //设置端点0发送为EP_TX_NAK
  _SetEPRxAddr(ENDP0, ENDP0_RXADDR);					   //设置端点0的接收地址
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);	   //设置端点0的最大接收包长度
  _SetEPTxAddr(ENDP0, ENDP0_TXADDR);					   //设置端点0的发送地址
  SetEPTxCount(ENDP0, Device_Property.MaxPacketSize);	   //设置端点0的最大发送包长度
  Clear_Status_Out(ENDP0);								   //清除端点0状态
  SetEPRxValid(ENDP0);									   //设置端点接收有效

  /* Set this device to response on default address */
  SetDeviceAddress(0);									   //设置设备默认地址

  /* Set the new control state of the device to Attached */
  bDeviceState = ATTACHED;
}

/*******************************************************************************
* Function Name  : DFU_SetConfiguration.
* Description    : 更新设备配置状态.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/*******************************************************************************
* Function Name  : DFU_SetDeviceAddress.
* Description    : 更新设备的编址状态
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}

/*******************************************************************************
* Function Name  : DFU_Status_In.
* Description    : DFU状态输入函数
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_Status_In(void)
{
    
}

/*******************************************************************************
* Function Name  : DFU_Status_Out.
* Description    : DFU状态输出函数
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_Status_Out (void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  uint32_t Addr;

  if (pInfo->USBbRequest == DFU_GETSTATUS)
  {
    if (DeviceState == STATE_dfuDNBUSY)	  	//当前状态为正在状态
    {
      if (wBlockNum == 0)   				//块数为0
      {
        if ((MAL_Buffer[0] ==  CMD_GETCOMMANDS) && (wlength == 1))//获取更多设备性能以及DFU支持哪些设备
        {}
        else if  (( MAL_Buffer[0] ==  CMD_SETADDRESSPOINTER ) && (wlength == 5))//设置地址指针
        {
          Pointer  = MAL_Buffer[1];
          Pointer += MAL_Buffer[2] << 8;
          Pointer += MAL_Buffer[3] << 16;
          Pointer += MAL_Buffer[4] << 24;
        }
        else if (( MAL_Buffer[0] ==  CMD_ERASE ) && (wlength == 5))	//发送擦除带地址扇区命令
        {
          Pointer  = MAL_Buffer[1];					//获取下载的地址
          Pointer += MAL_Buffer[2] << 8;
          Pointer += MAL_Buffer[3] << 16;
          Pointer += MAL_Buffer[4] << 24;
          MAL_Erase(Pointer);						//擦除该地址
        }
      }

      else if (wBlockNum > 1)  						//块数大于1
      {
        Addr = ((wBlockNum - 2) * wTransferSize) + Pointer;//获取写入存储器数据的地址
        MAL_Write(Addr, wlength);					//写入数据
      }												
      wlength = 0;									
      wBlockNum = 0;

      DeviceState =  STATE_dfuDNLOAD_SYNC;			//设置状态为下载同步
      DeviceStatus[4] = DeviceState;
      DeviceStatus[1] = 0;
      DeviceStatus[2] = 0;
      DeviceStatus[3] = 0;
      return;
    }
    else if (DeviceState == STATE_dfuMANIFEST)		//显示进程
    {
      DFU_write_crc();								//写CRC校验
      return;
    }
  }
  return;
}

/*******************************************************************************
* Function Name  : DFU_Data_Setup.
* Description    : 处理带数据的特殊类请求
* Input          : RequestNb.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT DFU_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);
  CopyRoutine = NULL;

  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))//类请求，请求的接收者是接口
  {
    if (RequestNo == DFU_UPLOAD && (DeviceState == STATE_dfuIDLE
                                    || DeviceState == STATE_dfuUPLOAD_IDLE ))//设备空闲，上传空闲
    {
      CopyRoutine = UPLOAD;					  //函数指针指向UPLOAD函数
    }
    else if (RequestNo == DFU_DNLOAD && (DeviceState == STATE_dfuIDLE
                                         || DeviceState == STATE_dfuDNLOAD_IDLE))//设备空闲，下载空闲
    {
      DeviceState = STATE_dfuDNLOAD_SYNC;	  //设置设备状态为下载同步
      CopyRoutine = DNLOAD;					  //函数指针指向DNDOWN函数
    }
    else if (RequestNo == DFU_GETSTATE)		  //DFU_GETSTATE请求
    {
      CopyRoutine = GETSTATE;				  //函数指针指向GETSTATE函数
    }
    else if (RequestNo == DFU_GETSTATUS)	  //DFU_GETSTATUS请求
    {
      CopyRoutine = GETSTATUS;				  //函数指针指向GETSTATUS函数
    }
    else
    {
      return USB_UNSUPPORT;
    }
  }
  else
  {
    return USB_UNSUPPORT;
  }

  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }

  pInformation->Ctrl_Info.CopyData = CopyRoutine;//注册这个函数指针
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);							 //指向概述指针所指向的函数

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : DFU_NoData_Setup.
* Description    : Handle the No data class specific requests.
* Input          : Request Nb.
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT DFU_NoData_Setup(uint8_t RequestNo)
{

  if (Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))//类请求
  {
    /*DFU_NDLOAD*/
    if (RequestNo == DFU_DNLOAD)					//DFU_DNLOAD请求
    {
      /* End of DNLOAD operation*/
      if (DeviceState == STATE_dfuDNLOAD_IDLE || DeviceState == STATE_dfuIDLE )//下载空闲或设备空闲
      {
        Manifest_State = Manifest_In_Progress;		//设置显示状态为显示进程
        DeviceState = STATE_dfuMANIFEST_SYNC;		//设置显示同步
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        DeviceStatus[4] = DeviceState;
        return USB_SUCCESS;
      }
    }
    /*DFU_UPLOAD*/
    else if (RequestNo == DFU_UPLOAD)				//DFU_UPLOAD命令
    {
      DeviceState = STATE_dfuIDLE;					//设置设备空闲
      DeviceStatus[1] = 0;
      DeviceStatus[2] = 0;
      DeviceStatus[3] = 0;
      DeviceStatus[4] = DeviceState;
      return USB_SUCCESS;
    }

    /*DFU_CLRSTATUS*/
    else if (RequestNo == DFU_CLRSTATUS)			//DFU_CLRSTATUS命令
    {

      if (DeviceState == STATE_dfuERROR)			//dfu错误
      {
        DeviceState = STATE_dfuIDLE;				//清除错误状态
        DeviceStatus[0] = STATUS_OK;/*bStatus*/		
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0; /*bwPollTimeout=0ms*/
        DeviceStatus[4] = DeviceState;/*bState*/
        DeviceStatus[5] = 0;/*iString*/
      }
      else
      {   /*State Error*/
        DeviceState = STATE_dfuERROR;				//设置设备状态为升级错误
        DeviceStatus[0] = STATUS_ERRUNKNOWN;/*bStatus*/
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0; /*bwPollTimeout=0ms*/
        DeviceStatus[4] = DeviceState;/*bState*/
        DeviceStatus[5] = 0;/*iString*/
      }
      return USB_SUCCESS;
    }
    /*DFU_ABORT*/
    else if (RequestNo == DFU_ABORT)				//DFU_ABORT终止命令
    {
      if (DeviceState == STATE_dfuIDLE || DeviceState == STATE_dfuDNLOAD_SYNC
          || DeviceState == STATE_dfuDNLOAD_IDLE || DeviceState == STATE_dfuMANIFEST_SYNC
          || DeviceState == STATE_dfuUPLOAD_IDLE )	//设别空闲或上传同步或下载空闲或显示同步或上传空闲
      {
        DeviceState = STATE_dfuIDLE;				//设置设别空闲
        DeviceStatus[0] = STATUS_OK;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0; /*bwPollTimeout=0ms*/
        DeviceStatus[4] = DeviceState;
        DeviceStatus[5] = 0; /*iString*/
        wBlockNum = 0;
        wlength = 0;
      }
      return USB_SUCCESS;
    }
  }


  return USB_UNSUPPORT;

} /* End of DFU_NoData_Setup */

/*******************************************************************************
* Function Name  : DFU_GetDeviceDescriptor.
* Description    : 获取设备描述符
* Input          : Length.
* Output         : None.
* Return         : 返回设备描述符的地址
*******************************************************************************/
uint8_t *DFU_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/*******************************************************************************
* Function Name  : DFU_GetConfigDescriptor.
* Description    : 获取配置描述符
* Input          : Length.
* Output         : None.
* Return         : 返回配置描述符的地址
*******************************************************************************/
uint8_t *DFU_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData (Length, &Config_Descriptor);
}

/*******************************************************************************
* Function Name  : DFU_GetStringDescriptor.
* Description    : 根据索引获取字符串描述
* Input          : Length.
* Output         : None.
* Return         : 返回字符串描述符的地址
*******************************************************************************/
uint8_t *DFU_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;

  if (wValue0 > 8)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &DFU_String_Descriptor[wValue0]);
  }
}

/*******************************************************************************
* Function Name  : DFU_Get_Interface_Setting.
* Description    : 测试是否支持接口和备用接口
* Input          : - Interface : 接口号
*                  - AlternateSetting : 备用接口号
* Output         : None.
* Return         : USB_SUCCESS or USB_UNSUPPORT.
*******************************************************************************/
RESULT DFU_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 3)
  {
    return USB_UNSUPPORT;   /* In this application we don't have more than 3 AlternateSettings */
  }
  else if (Interface > 2)
  {
    return USB_UNSUPPORT; /* In this application we have only 1 interfaces */
  }

  return USB_SUCCESS;
}

/*******************************************************************************
* Function Name  : UPLOAD
* Description    : 上传
* Input          : Length.
* Output         : None.
* Return         : Pointer to data.
*******************************************************************************/
uint8_t *UPLOAD(uint16_t Length)
{
  DEVICE_INFO *pInfo = &Device_Info; 	//获取设备的信息
  uint8_t B1, B0;					 	//16bit的高8位和低8位
  uint16_t offset, returned;		 
  uint8_t *Phy_Addr = NULL;			 
  uint32_t Addr = 0;

  B0 = pInfo->USBwValues.bw.bb0;	 	//获取请求的wValue的低8位数据
  B1 = pInfo->USBwValues.bw.bb1;	 	//获取请求的wValue的高8位数据
  wBlockNum = (uint16_t)B1;				
  wBlockNum = wBlockNum * 0x100;
  wBlockNum += (uint16_t)B0; 			//更新wBlockNum

  B0 = pInfo->USBwLengths.bw.bb0;		//获取请求的wLength的低8位数据
  B1 =  pInfo->USBwLengths.bw.bb1;		//获取请求的wLength的高8位数据
  wlength = (uint16_t)B0;
  wlength = wlength * 0x100;
  wlength += (uint16_t)B1; 				//更新wLength

  offset = pInformation->Ctrl_Info.Usb_wOffset;//获取数据偏移量

  if (wBlockNum == 0)  					//wBlockNum等于0,表示不传输数据
  {
    if (wlength > 3)					
    {
      DeviceState = STATE_dfuIDLE ;
    }
    else
    {
      DeviceState = STATE_dfuUPLOAD_IDLE;
    }

    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;
										   
    MAL_Buffer[0] = CMD_GETCOMMANDS;	   //Get命令
    MAL_Buffer[1] = CMD_SETADDRESSPOINTER; //设置地址指针
    MAL_Buffer[2] = CMD_ERASE;			   //擦除含地址的的扇区

    if (Length == 0)
    {
      pInformation->Ctrl_Info.Usb_wLength = 3 ;
      return NULL;
    }

    return(&MAL_Buffer[0]);
  }
  else if (wBlockNum > 1)				//wBlockNum大于1
  {
    DeviceState = STATE_dfuUPLOAD_IDLE ;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;
    Addr = ((wBlockNum - 2) * wTransferSize) + Pointer;  //计算要上传的数据在存储器中的地址

    Phy_Addr = MAL_Read(Addr, wlength);//读取数据
    returned = wlength - offset;	   //获取上传数据的起始地址

    if (Length == 0)				   //要上传的长度为0
    {
      pInformation->Ctrl_Info.Usb_wLength = returned ;
      return NULL;
    }
    return(Phy_Addr + offset);
  }
  else  							   //即wBlockNum=1，不支持的wBlockNum
  {
    DeviceState = STATUS_ERRSTALLEDPKT;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;

    return NULL;
  }
}

/*******************************************************************************
* Function Name  : DNLOAD
* Description    : 下载
* Input          : Length.
* Output         : None.
* Return         : 返回指向数据的指针
*******************************************************************************/
uint8_t *DNLOAD (uint16_t Length)
{
  DEVICE_INFO *pInfo = &Device_Info; //获取设备信息
  uint8_t B1, B0;					 //16bit的高8位和低8位
  uint16_t offset, returned;

  B0 = pInfo->USBwValues.bw.bb0;	 //获取请求的wValue的低8位数据
  B1 = pInfo->USBwValues.bw.bb1;	 //获取请求的wValue的高8位数据
  wBlockNum = (uint16_t)B1;
  wBlockNum = wBlockNum * 0x100;
  wBlockNum += (uint16_t)B0;		 //更新wBlockNum

  B0 = pInfo->USBwLengths.bw.bb0;	 //获取请求的wLength的低8位数据
  B1 =  pInfo->USBwLengths.bw.bb1;	 //获取请求的wLength的高8位数据
  wlength = (uint16_t)B0;
  wlength = wlength * 0x100;
  wlength += (uint16_t)B1;			 //更新wLength

  offset = pInfo->Ctrl_Info.Usb_wOffset;

  DeviceState = STATE_dfuDNLOAD_SYNC;
  DeviceStatus[4] = DeviceState;

  returned = wlength - offset;		 //指向数据的起始地址

  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = returned ;
    return NULL;
  }

  return((uint8_t*)MAL_Buffer + offset);
}

/*******************************************************************************
* Function Name  : GETSTATE.
* Description    : 获取STATE请求函数.
* Input          : Length.
* Output         : None.
* Return         : Pointer to data.
*******************************************************************************/
uint8_t *GETSTATE(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1 ;
    return NULL;
  }
  else
    return(&DeviceState);
}

/*******************************************************************************
* Function Name  : GETSTATUS.
* Description    : 获取Status请求函数
* Input          : Length.
* Output         : None.
* Return         : Pointer to data.
*******************************************************************************/
uint8_t *GETSTATUS(uint16_t Length)
{
  switch (DeviceState)
  {
    case   STATE_dfuDNLOAD_SYNC:
      if (wlength != 0)
      {
        DeviceState = STATE_dfuDNBUSY;
        DeviceStatus[4] = DeviceState;
        if ((wBlockNum == 0) && (MAL_Buffer[0] == CMD_ERASE))
        {
          MAL_GetStatus(Pointer, 0, DeviceStatus);
        }
        else
        {
          MAL_GetStatus(Pointer, 1, DeviceStatus);
        }
      }
      else  /* (wlength==0)*/
      {
        DeviceState = STATE_dfuDNLOAD_IDLE;
        DeviceStatus[4] = DeviceState;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;

      }
      break;
    case   STATE_dfuMANIFEST_SYNC :
      if (Manifest_State == Manifest_In_Progress)
      {
        DeviceState = STATE_dfuMANIFEST;
        DeviceStatus[4] = DeviceState;
        DeviceStatus[1] = 1;             /*bwPollTimeout = 1ms*/
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        //break;
      }
      else if (Manifest_State == Manifest_complete && Config_Descriptor.Descriptor[20]
               & 0x04)
      {
        DeviceState = STATE_dfuIDLE;
        DeviceStatus[4] = DeviceState;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        //break;
      }
      break;
    default :
      break;
  }

  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 6 ;
    return NULL;
  }
  else
    return(&(DeviceStatus[0]));
}

/*******************************************************************************
* Function Name  : DFU_write_crc.
* Description    : DFU 写CRC
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void DFU_write_crc(void)
{
  Manifest_State = Manifest_complete;

  if (Config_Descriptor.Descriptor[20] & 0x04)
  {
    DeviceState = STATE_dfuMANIFEST_SYNC;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;
    return;
  }
  else
  {
    DeviceState = STATE_dfuMANIFEST_WAIT_RESET;
    DeviceStatus[4] = DeviceState;
    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;

    //Reset_Device();

    return;
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
