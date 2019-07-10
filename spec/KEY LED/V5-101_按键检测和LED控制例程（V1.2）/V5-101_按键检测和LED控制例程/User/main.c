/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.2
*	˵    �� : ��������LED�������̡������ļ�Ϊ bsp_timer.c bsp_key.c �� bsp_led.c
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2013-02-01 armfly  �׷�
*		v1.1    2013-06-20 armfly  ����bsp����; ���´�ӡ��ʾ��Ϣ��
*		V1.2    2014-02-28 armfly  �����̼��⵽V1.3.0
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"			/* �ײ�Ӳ������ */

/* ���������������̷������� */
#define EXAMPLE_NAME	"V5-101_��������LED��������"
#define EXAMPLE_DATE	"2014-02-28"
#define DEMO_VER		"1.2"

/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: c�������
*	��    �Σ���
*	�� �� ֵ: �������(���账��)
*********************************************************************************************************
*/
int main(void)
{
	uint8_t ucKeyCode;		/* �������� */

	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
	����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM
	*/

	bsp_Init();		/* Ӳ����ʼ�� */
	PrintfLogo();	/* ��ӡ������Ϣ������1 */

	printf("LED4������˸(��˸Ƶ�� = 1Hz)\r\n");
	printf("����K1������LED1\r\n");
	printf("����K2������LED2\r\n");
	printf("����K3������LED3\r\n");
	printf("����������ҡ�˻��ӡ�����¼�\r\n");

	bsp_StartAutoTimer(0, 500);	/* ����1��500ms���Զ���װ�Ķ�ʱ�� */

	/* ����������ѭ���� */
	while (1)
	{
		bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */

		if (bsp_CheckTimer(0))	/* �ж϶�ʱ����ʱʱ�� */
		{
			/* ÿ��500ms ����һ�� */
			bsp_LedToggle(4);	/* ��תLED4��״̬ */
		}

		/* �����˲��ͼ���ɺ�̨systick�жϷ������ʵ�֣�����ֻ��Ҫ����bsp_GetKey��ȡ��ֵ���ɡ� */
		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:			/* K1������ */
					bsp_LedOn(1);
					printf("K1������, LED1����\r\n");
					break;

				case KEY_UP_K1:				/* K1������ */
					bsp_LedOff(1);
					printf("K1������, LED1Ϩ��\r\n");
					break;

				case KEY_DOWN_K2:			/* K2������ */
					bsp_LedOn(2);
					printf("K2������, LED2����\r\n");
					break;

				case KEY_UP_K2:				/* K2������ */
					bsp_LedOff(2);
					printf("K2������, LED2Ϩ��\r\n");
					break;

				case KEY_DOWN_K3:			/* K3������ */
					bsp_LedOn(3);
					printf("K3������, LED3����\r\n");
					break;

				case KEY_UP_K3:				/* K3������ */
					bsp_LedOff(3);
					printf("K3������, LED3Ϩ��\r\n");
					break;

				case JOY_DOWN_U:			/* ҡ��UP������ */
					printf("ҡ���ϼ�����\r\n");
					break;

				case JOY_DOWN_D:			/* ҡ��DOWN������ */
					printf("ҡ���¼�����\r\n");
					break;

				case JOY_DOWN_L:			/* ҡ��LEFT������ */
					printf("ҡ���������\r\n");
					break;

				case JOY_DOWN_R:			/* ҡ��RIGHT������ */
					printf("ҡ���Ҽ�����\r\n");
					break;

				case JOY_DOWN_OK:			/* ҡ��OK������ */
					printf("ҡ��OK������\r\n");
					break;

				case JOY_UP_OK:				/* ҡ��OK������ */
					printf("ҡ��OK������\r\n");
					break;

				default:
					/* �����ļ�ֵ������ */
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	/* ���CPU ID */
	{
		/* �ο��ֲ᣺
			32.6.1 MCU device ID code
			33.1 Unique device ID register (96 bits)
		*/
		uint32_t CPU_Sn0, CPU_Sn1, CPU_Sn2;

		CPU_Sn0 = *(__IO uint32_t*)(0x1FFF7A10);
		CPU_Sn1 = *(__IO uint32_t*)(0x1FFF7A10 + 4);
		CPU_Sn2 = *(__IO uint32_t*)(0x1FFF7A10 + 8);

		printf("\r\nCPU : STM32F407IGT6, LQFP176, UID = %08X %08X %08X\n\r"
			, CPU_Sn2, CPU_Sn1, CPU_Sn0);
	}

	printf("\n\r");
	printf("*************************************************************\n\r");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */
	printf("* �̼���汾 : V%d.%d.%d (STM32F4xx_StdPeriph_Driver)\r\n", __STM32F4XX_STDPERIPH_VERSION_MAIN,
			__STM32F4XX_STDPERIPH_VERSION_SUB1,__STM32F4XX_STDPERIPH_VERSION_SUB2);
	printf("* \r\n");	/* ��ӡһ�пո� */
	printf("* QQ    : 1295744630 \r\n");
	printf("* ����  : armfly\r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* �Ա���: armfly.taobao.com\r\n");
	printf("* Copyright www.armfly.com ����������\r\n");
	printf("*************************************************************\n\r");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
