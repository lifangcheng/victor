/**
  ******************************************************************************
  * @file    usbd_hid_core.c
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    19-March-2012
  * @brief   This file provides the HID core functions.
  *
  * @verbatim
  *      
  *          ===================================================================      
  *                                HID Class  Description
  *          =================================================================== 
  *           This module manages the HID class V1.11 following the "Device Class Definition
  *           for Human Interface Devices (HID) Version 1.11 Jun 27, 2001".
  *           This driver implements the following aspects of the specification:
  *             - The Boot Interface Subclass
  *             - The Mouse protocol
  *             - Usage Page : Generic Desktop
  *             - Usage : Joystick)
  *             - Collection : Application 
  *      
  * @note     In HS mode and when the DMA is used, all variables and data structures
  *           dealing with the DMA during the transaction process should be 32-bit aligned.
  *           
  *      
  *  @endverbatim
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
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
#include "usbd_hid_core.h"
#include "usbd_desc.h"
#include "usbd_req.h"


/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @{
  */


/** @defgroup USBD_HID 
  * @brief usbd core module
  * @{
  */ 

/** @defgroup USBD_HID_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USBD_HID_Private_Defines
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup USBD_HID_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 




/** @defgroup USBD_HID_Private_FunctionPrototypes
  * @{
  */

#define CUSTOM_HID_EPOUT_SIZE                0x40
uint8_t USB_Rx_Buffer[0xFF];
uint8_t UsbHidReceiveComplete = 0;

static uint8_t  USBD_HID_Init (void  *pdev, 
                               uint8_t cfgidx);

static uint8_t  USBD_HID_DeInit (void  *pdev, 
                                 uint8_t cfgidx);

static uint8_t  USBD_HID_Setup (void  *pdev, 
                                USB_SETUP_REQ *req);

static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length);

static uint8_t  USBD_HID_DataIn (void  *pdev, uint8_t epnum);

static uint8_t  USBD_HID_DataOut (void  *pdev, uint8_t epnum);
/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Variables
  * @{
  */ 

USBD_Class_cb_TypeDef  USBD_HID_cb = 
{
  USBD_HID_Init,
  USBD_HID_DeInit,
  USBD_HID_Setup,
  NULL, /*EP0_TxSent*/  
  NULL, /*EP0_RxReady*/
  USBD_HID_DataIn, /*DataIn*/
  USBD_HID_DataOut, /*DataOut*/
  NULL, /*SOF */
  NULL,
  NULL,      
  USBD_HID_GetCfgDesc,
#ifdef USB_OTG_HS_CORE  
  USBD_HID_GetCfgDesc, /* use same config as per FS */
#endif  
};

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */        
__ALIGN_BEGIN static uint32_t  USBD_HID_AltSet  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */      
__ALIGN_BEGIN static uint32_t  USBD_HID_Protocol  __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */  
__ALIGN_BEGIN static uint32_t  USBD_HID_IdleState __ALIGN_END = 0;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */ 
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZ] __ALIGN_END =
{
	0x09, /* bLength: Configuation Descriptor size */
    USB_CONFIGURATION_DESCRIPTOR_TYPE, /* bDescriptorType: Configuration */
    USB_HID_CONFIG_DESC_SIZ,//CUSTOMHID_SIZ_CONFIG_DESC,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /* bNumInterfaces: 1 interface */
    0x01,         /* bConfigurationValue: Configuration value */
    0x00,         /* iConfiguration: Index of string descriptor describing
                                 the configuration*/
    0xC0,         /* bmAttributes: Bus powered */
                  /*Bus powered: 7th bit, Self Powered: 6th bit, Remote wakeup: 5th bit, reserved: 4..0 bits */
    0x32,         /* MaxPower 100 mA: this current is used for detecting Vbus */
//    0x96,         /* MaxPower 300 mA: this current is used for detecting Vbus */
    /************** Descriptor of Custom HID interface ****************/
    /* 09 */
    0x09,         /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,/* bDescriptorType: Interface descriptor type */
    0x00,         /* bInterfaceNumber: Number of Interface */
    0x00,         /* bAlternateSetting: Alternate setting */
    0x02,         /* bNumEndpoints */
    0x03,         /* bInterfaceClass: HID */
    0x00,         /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,         /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,            /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID HID ********************/
    /* 18 */
    0x09,         /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE, /* bDescriptorType: HID */
    0x10,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
	HID_MOUSE_REPORT_DESC_SIZE,
    //CUSTOMHID_SIZ_REPORT_DESC,/* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom HID endpoints ******************/
    /* 27 */
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE, /* bDescriptorType: */
	
  /******************** Descriptor of Custom HID endpoints ********************/
//    0x82,          /* bEndpointAddress: Endpoint Address (IN) */
	0x81,          /* bEndpointAddress: Endpoint Address (IN) */
                   // bit 3...0 : the endpoint number
                   // bit 6...4 : reserved
                    // bit 7     : 0(OUT), 1(IN)
    0x03,          /* bmAttributes: Interrupt endpoint */
    CUSTOM_HID_EPOUT_SIZE,//0x02,          /* wMaxPacketSize: 20 Bytes max */
    0x00,
  0x20,	/* bInterval: Polling Interval (20 ms) */

/* 34 */   	
    0x07,	/* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,	/* bDescriptorType: */
			/*	Endpoint descriptor type */
    0x01,	/* bEndpointAddress: */
			/*	Endpoint Address (OUT) */
    0x03,	/* bmAttributes: Interrupt endpoint */
    CUSTOM_HID_EPOUT_SIZE,//0x02,	/* wMaxPacketSize: 20 Bytes max  */
  0x00,
  0x20,	/* bInterval: Polling Interval (20 ms) */
    /* 41 */
} ;

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
/* USB HID device Configuration Descriptor */
__ALIGN_BEGIN static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZ] __ALIGN_END=
{
  /* 18 */
  0x09,         /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x11,         /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,         /*bCountryCode: Hardware target country*/
  0x01,         /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,         /*bDescriptorType*/
  HID_MOUSE_REPORT_DESC_SIZE,/*wItemLength: Total length of Report descriptor*/
  0x00,
};
#endif 


#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
  #if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4   
  #endif
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */  
__ALIGN_BEGIN static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE] __ALIGN_END =
{
		/*short Item   D7~D4:bTag;D3~D2:bType;D1~D0:bSize
	**bTag ---?¯足??? 1000:那?豕?(Input) 1001:那?3?(Output) 1011:足?D?(Feature)	1010:?‘o?(Collection) 1100:1??‘o?(End Collection) 
	**	豕???足??? 0000:車?赤?辰3(Usage Page) 0001:???-℅?D??米(Logical Minimum) 0010:???-℅?∩車?米(Logical Maximum) 0011:??角赤℅?D??米(Physical Minimum)
	**	0100:??角赤℅?∩車?米(Physical Maximum) 0101:米ㄓ?a??那y(Unit Exponet) 0110:米ㄓ?a(Unit) 0111:那y?Y車辰∩車D?(Report Size)
	**	1000:㊣“??ID(Report ID) 1001:那y?Y車辰那y芍?(Report Count) 1010:?1??(Push) 1011:3???(Pop) 1100~1111:㊣㏒芍?(Reserved)
	**	??2?足??? 0000:車?赤?(Usage) 0001:車?赤?℅?D??米(Usage Minimum) 0010:車?赤?℅?∩車?米(Usage Maximum) 0011:㊣那那?﹞??¯辰y(Designator Index)
	**		0100:㊣那那?﹞?℅?D??米(Designator Minimum) 0101:㊣那那?﹞?℅?∩車?米(Designator Maximum) 0111:℅?﹞?∩??¯辰y(String Index) 1000:℅?﹞?∩?℅?D??米(String Minimum)   
	**		1001:℅?﹞?∩?℅?∩車?米(String Maximum) 1010:﹞???﹞?(Delimiter) ????㏒o㊣㏒芍?(Reserved)
	**bType---00:?¯足???(main)  01:豕???足???(globle)  10:??2?足???(local)  11:㊣㏒芍?(reserved)
	**bSize---00:0℅??迆  01:1℅??迆  10:2℅??迆  11:4℅??迆*/
	
	//0x05:0000 01 01 ?a那???豕???足???㏒?車?赤?辰3?aST辰3
	0x05, 0x8c, /* USAGE_PAGE (ST Page) */ 
	//0x09:0000 10 01 ?a那?????2?㊣?芍?㏒?車?赤??aDemo Kit
	0x09, 0x01, /* USAGE (Demo Kit) */ 
	//0xa1:1010 00 01 ?a那?辰????¯足???㏒??‘o??a車|車??‘o?
	0xa1, 0x01, /* COLLECTION (Application) */ 
	
	/* 那?豕?㊣“??*/ 
	//0x09:0000 10 01 ?a那?????2?足???㏒?車?赤??a3∫谷足ID
	0x09,0x03, // USAGE ID - Vendor defined 
	//0x15:0001 01 01 ?a那???豕???足???㏒????-℅?D??米?a0
	0x15,0x00, // LOGICAL_MINIMUM (0) 
	//0x26:0010 01 10 ?a那???豕???足???㏒????-℅?∩車?米?a255
	0x26,0x00, 0xFF, // LOGICAL_MAXIMUM (255) 
	//0x75:0111 01 01 ?a那???豕???足???㏒?㊣“??∩車D??a8??
	0x75,0x08, // REPORT_SIZE (8bit) 
	//0x95:1001 01 01 ?a那???豕???足???㏒?㊣“??那y芍??a64
	0x95,0x40, // REPORT_COUNT (64Byte) 
	//0x81:1000 00 01 ?a那????¯足???㏒?℅?那?豕?㏒?Data㊣赤那??aD?那y?Y?谷㊣?㏒?Var㊣赤那??aD?D足?車車迆那??角芍⊿米?㊣?芍?㏒?Abs㊣赤那??????米
	0x81,0x02, // INPUT (Data,Var,Abs) 

	/*那?3?㊣“??*/ 
	//0x09:0000 10 01 ?a那?????2?足???㏒?車?赤??a3∫谷足ID
	0x09,0x04, // USAGE ID - Vendor defined 
	//0x15:0001 01 01 ?a那???豕???足???㏒????-℅?D??米?a0
	0x15,0x00, // LOGICAL_MINIMUM (0) 
	//0x26:0010 01 10 ?a那???豕???足???㏒????-℅?∩車?米?a255
	0x26,0x00,0xFF, // LOGICAL_MAXIMUM (255) 
	//0x75:0111 01 01 ?a那???豕???足???㏒?㊣“??∩車D??a8??
	0x75,0x08, // REPORT_SIZE (8bit) 
	//0x95:1001 01 01 ?a那???豕???足???㏒?㊣“??那y芍??a64
	0x95,0x40, // REPORT_COUNT (64Byte) 
	//0x91:1001 00 01 ?a那???豕???足???㏒?℅?那?3?㏒?Data㊣赤那??aD?那y?Y?谷㊣?㏒?Var㊣赤那??aD?D足?車車迆那??角芍⊿米?㊣?芍?㏒?Abs㊣赤那??????米
	0x91,0x02, // OUTPUT (Data,Var,Abs) 

	0xc0 /* END_COLLECTION */
}; 

/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Functions
  * @{
  */ 

/**
  * @brief  USBD_HID_Init
  *         Initialize the HID interface
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_Init (void  *pdev, 
                               uint8_t cfgidx)
{
  
  /* Open EP IN */
  DCD_EP_Open((USB_OTG_CORE_HANDLE *)pdev,
              HID_IN_EP,
              HID_IN_PACKET,
              USB_OTG_EP_INT);
  
  /* Open EP OUT */
  DCD_EP_Open((USB_OTG_CORE_HANDLE *)pdev,
              HID_OUT_EP,
              HID_OUT_PACKET,
              USB_OTG_EP_INT);
  /* Prepare Out endpoint to receive next packet */
  DCD_EP_PrepareRx((USB_OTG_CORE_HANDLE *)pdev,
                   HID_OUT_EP,
                   (uint8_t*)(USB_Rx_Buffer),
                   HID_OUT_PACKET);	
  
  return USBD_OK;
}

/**
  * @brief  USBD_HID_Init
  *         DeInitialize the HID layer
  * @param  pdev: device instance
  * @param  cfgidx: Configuration index
  * @retval status
  */
static uint8_t  USBD_HID_DeInit (void  *pdev, 
                                 uint8_t cfgidx)
{
  /* Close HID EPs */
  DCD_EP_Close ((USB_OTG_CORE_HANDLE *)pdev , HID_IN_EP);
  DCD_EP_Close ((USB_OTG_CORE_HANDLE *)pdev , HID_OUT_EP);
  
  
  return USBD_OK;
}

/**
  * @brief  USBD_HID_Setup
  *         Handle the HID specific requests
  * @param  pdev: instance
  * @param  req: usb requests
  * @retval status
  */
static uint8_t  USBD_HID_Setup (void  *pdev, 
                                USB_SETUP_REQ *req)
{
  uint16_t len = 0;
  uint8_t  *pbuf = NULL;
  
  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {
  case USB_REQ_TYPE_CLASS :  
    switch (req->bRequest)
    {
      
      
    case HID_REQ_SET_PROTOCOL:
      USBD_HID_Protocol = (uint8_t)(req->wValue);
      break;
      
    case HID_REQ_GET_PROTOCOL:
      USBD_CtlSendData ((USB_OTG_CORE_HANDLE *)pdev, 
                        (uint8_t *)&USBD_HID_Protocol,
                        1);    
      break;
      
    case HID_REQ_SET_IDLE:
      USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
      break;
      
    case HID_REQ_GET_IDLE:
      USBD_CtlSendData ((USB_OTG_CORE_HANDLE *)pdev, 
                        (uint8_t *)&USBD_HID_IdleState,
                        1);        
      break;      
      
    default:
      USBD_CtlError ((USB_OTG_CORE_HANDLE *)pdev, req);
      return USBD_FAIL; 
    }
    break;
    
  case USB_REQ_TYPE_STANDARD:
    switch (req->bRequest)
    {
    case USB_REQ_GET_DESCRIPTOR: 
      if( req->wValue >> 8 == HID_REPORT_DESC)
      {
        len = MIN(HID_MOUSE_REPORT_DESC_SIZE , req->wLength);
        pbuf = HID_MOUSE_ReportDesc;
      }
      else if( req->wValue >> 8 == HID_DESCRIPTOR_TYPE)
      {
        
#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
        pbuf = USBD_HID_Desc;   
#else
        pbuf = USBD_HID_CfgDesc + 0x12;
#endif 
        len = MIN(USB_HID_DESC_SIZ , req->wLength);
      }
      
      USBD_CtlSendData ((USB_OTG_CORE_HANDLE *)pdev, 
                        pbuf,
                        len);
      
      break;
      
    case USB_REQ_GET_INTERFACE :
      USBD_CtlSendData ((USB_OTG_CORE_HANDLE *)pdev,
                        (uint8_t *)&USBD_HID_AltSet,
                        1);
      break;
      
    case USB_REQ_SET_INTERFACE :
      USBD_HID_AltSet = (uint8_t)(req->wValue);
      break;
    }
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_SendReport 
  *         Send HID Report
  * @param  pdev: device instance
  * @param  buff: pointer to report
  * @retval status
  */
uint8_t USBD_HID_SendReport     (USB_OTG_CORE_HANDLE  *pdev, 
                                 uint8_t *report,
                                 uint16_t len)
{
  if (pdev->dev.device_status == USB_OTG_CONFIGURED )
  {
    DCD_EP_Tx (pdev, HID_IN_EP, report, len);
  }
  return USBD_OK;
}

/**
  * @brief  USBD_HID_GetCfgDesc 
  *         return configuration descriptor
  * @param  speed : current device speed
  * @param  length : pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetCfgDesc (uint8_t speed, uint16_t *length)
{
  *length = sizeof (USBD_HID_CfgDesc);
  return USBD_HID_CfgDesc;
}

/**
  * @brief  USBD_HID_DataIn
  *         handle data IN Stage
  * @param  pdev: device instance
  * @param  epnum: endpoint index
  * @retval status
  */
static uint8_t  USBD_HID_DataIn (void  *pdev, 
                              uint8_t epnum)
{
  
  /* Ensure that the FIFO is empty before a new transfer, this condition could 
  be caused by  a new transfer before the end of the previous transfer */
  DCD_EP_Flush((USB_OTG_CORE_HANDLE *)pdev, HID_IN_EP);
  return USBD_OK;
}

static uint8_t  USBD_HID_DataOut (void  *pdev, uint8_t epnum)
{
  uint16_t USB_Rx_Cnt;
	uint8_t i;
  
	if ( epnum != (HID_OUT_EP & 0x0F) )
    return USBD_FAIL;

  /* Get the received data buffer and update the counter */
  USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].xfer_count;
	
  /* Prepare Out endpoint to receive next packet */
  DCD_EP_PrepareRx((USB_OTG_CORE_HANDLE*)pdev,
                   HID_OUT_EP,
                   (uint8_t*)(USB_Rx_Buffer),
                   HID_OUT_PACKET);

	UsbHidReceiveComplete=1;
  return USBD_OK;
}
/**
  * @}
  */ 


/**
  * @}
  */ 


/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
