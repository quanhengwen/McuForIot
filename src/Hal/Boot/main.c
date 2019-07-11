/****************************************************************************
����ʱ�䣺20120712
�����ˣ�����
�汾��stm32 v3.5
****************************************************************************/

#include <includes.h>
#include "UserappConfig.h"
#include "usb_devices.h"

#define MAIN_TASK_PRI      		15
#define TASK_STK_SIZE       	256
static  INT32U STK_MAIN_Manage[TASK_STK_SIZE];

void	MainTask(void *pd);

int main(void)
{
  	Board_Configuration();
	
	  OSInit();
    OSTaskCreate(MainTask,    (void *)0,(OS_STK *)&STK_MAIN_Manage[TASK_STK_SIZE-1],  MAIN_TASK_PRI); //15
    OSStart();
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
    
	AppCreate();

	while(1)
	{  
    
		GPIO_SetBits(GPIOB, GPIO_Pin_12);
    
		OSTimeDly(OS_TICKS_PER_SEC);

		GPIO_ResetBits(GPIOB, GPIO_Pin_12);

	  OSTimeDly(OS_TICKS_PER_SEC);  
	}	
}



