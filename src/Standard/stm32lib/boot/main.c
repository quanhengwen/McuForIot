/****************************************************************************
����ʱ�䣺20120712
�����ˣ�����
�汾��stm32 v3.5
****************************************************************************/

#include <includes.h>
#include "UserappConfig.h"
#include "MPU6050.h"
#include "usb_devices.h"
/*********************PRI*****************************/
#define MAIN_TASK_PRI      		15
	
/*******************TASK_STK*****************************/
#define TASK_STK_SIZE       256
static  INT32U STK_MAIN_Manage[TASK_STK_SIZE];
/************************************************/

u32 SystemTime_10_ms;
u32 SystemTime_1000_ms;
void  	MainTask(void *pd);

int main(void)
{

	OSInit();
    OSTaskCreate(MainTask,    (void *)0,(OS_STK *)&STK_MAIN_Manage[TASK_STK_SIZE-1],  MAIN_TASK_PRI); //15
    OSStart();
}
 
void Delay(uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}
/***************************************************************************************
** ��������: MainTask
** ��������: ������
** ��    ��: None
** �� �� ֵ: None 
** ��  ����: 
****************************************************************************************/
void MainTask(void *pd)
{	
		pd=pd;


  	Bsp_Configuration();
	/*********************��������*****************************************************/
	AppCreate();

	while(1)
	{  

//		GPIO_SetBits(GPIOC, GPIO_Pin_13);
//		Delay(0xfffff);
//	  Delay(0xfffff);	
//		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//	  Delay(0xfffff);
//	  Delay(0xfffff);			
	  OSTimeDly(OS_TICKS_PER_SEC);
	   
	}	
}



