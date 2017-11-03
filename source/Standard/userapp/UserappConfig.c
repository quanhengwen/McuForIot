/****************************************************************************
����ʱ�䣺	20120712
�����ˣ�	����
�汾��		stm32 v3.5
****************************************************************************/
#include <includes.h>

#include "UserappConfig.h"

/*********************PRI*****************************/
/*******************TASK_STK**************************/
#define APP_TASK_STK_SIZE       256
#if CONFIG_DEBUG
#define DebugTask_PRI		 	17 
static  INT32U STK_DebugTask[APP_TASK_STK_SIZE]; 
#endif
#if CONFIG_LEDS
#define LedsTask_PRI		 	18 
static  INT32U STK_LedsTask[APP_TASK_STK_SIZE];
#endif
#if CONFIG_LCD5110
#define LCD5110Task_PRI		 	18 
static  INT32U STK_LCD5110Task[APP_TASK_STK_SIZE];
#endif
#if CONFIG_MPU6050
#define MPU6050Task_PRI		 	18 	
	static  INT32U STK_MPU6050Task[APP_TASK_STK_SIZE];
#endif
#ifdef CONFIG_USBDEVICES
#define USB_McuUSBTask_PRI		 	19 	
static  INT32U STK_USB_McuUSBTask[APP_TASK_STK_SIZE];
#endif

/*****************************************************/

/***************************************************************************************
** ��������: AppCreate
** ��������:
** ��    ��: None
** �� �� ֵ: None 
** ��  ����: 														
****************************************************************************************/
void AppCreate(void)
{	
	printf("******************************************************************************\r\n");
	printf("******************************************************************************\r\n");
	printf("**      Standard stm32/ucos2 Project	                                    **\r\n");	   
	printf("**      Author  :       zzhere                                              **\r\n");
	printf("**      Version :       Potatoes 1.0.0                                      **\r\n");
	printf("**      Time    :       2014.06.29                                          **\r\n");
	printf("******************************************************************************\r\n");
	printf("/*****************************************************************************\r\n");

#if CONFIG_DEBUG
	OSTaskCreate(DebugTask, (void *)0,(OS_STK *)&STK_DebugTask[APP_TASK_STK_SIZE-1],DebugTask_PRI); //17
#endif
#if CONFIG_LEDS
	OSTaskCreate(LedsTask, (void *)0,(OS_STK *)&STK_LedsTask[APP_TASK_STK_SIZE-1],LedsTask_PRI); 
#endif
#if CONFIG_LCD5110
	OSTaskCreate(LCD5110Task, (void *)0,(OS_STK *)&STK_LCD5110Task[APP_TASK_STK_SIZE-1],LCD5110Task_PRI); 
#endif	
	
	#if CONFIG_MPU6050
	OSTaskCreate(MPU6050Task, (void *)0,(OS_STK *)&STK_MPU6050Task[APP_TASK_STK_SIZE-1],MPU6050Task_PRI); 
	#endif 

#ifdef CONFIG_USBDEVICES
OSTaskCreate(USB_McuUSBTask, (void *)0,(OS_STK *)&STK_USB_McuUSBTask[APP_TASK_STK_SIZE-1],USB_McuUSBTask_PRI);
#endif

}



