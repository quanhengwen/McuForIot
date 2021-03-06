/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : hw_config.h
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <includes.h>	
#include "stm32f10x.h"
#include "usb_lib.h"
#include "usb_type.h"
#include "platform_config.h"
#include "usb_core.h"
#include "usb_regs.h"
#include "usb_mem.h"
#include "usb_init.h"

#include "mouse_usb_conf.h"
#include "mouse_usb_desc.h"
#include "mouse_usb_pwr.h"
#include "mouse_usb_prop.h"
#include "mouse_usb_pwr.h"
#include "mouse_usb_istr.h"

#include "usb_conf.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define CURSOR_STEP     5

#define DOWN            1
#define LEFT            2
#define RIGHT           3
#define UP              4
#define LEFT_BUTTON     5
#define RIGHT_BUTTON    6



/* Exported functions ------------------------------------------------------- */
void Mouse_Set_System(void);
void Mouse_Set_USBClock(void);
void GPIO_AINConfig(void);
void Mouse_Enter_LowPowerMode(void);
void Mouse_Leave_LowPowerMode(void);
void Mouse_USB_Interrupts_Config(void);
void Mouse_USB_Cable_Config (FunctionalState NewState);
void Joystick_Send(uint8_t Keys);
uint8_t JoyState(void);
void Mouse_Get_SerialNum(void);

#endif  /*__HW_CONFIG_H*/

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
