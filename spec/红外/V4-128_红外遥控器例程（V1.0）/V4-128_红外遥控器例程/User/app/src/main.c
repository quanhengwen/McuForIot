/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.2
*	˵    �� : ������
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2015-08-30 armfly  �׷�
*	
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"		/* ���Ҫ��ST�Ĺ̼��⣬�����������ļ� */
#include "math.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"V4-128_����ң��������"
#define EXAMPLE_DATE	"2015-08-30"
#define DEMO_VER		"1.0"

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
	uint8_t ucKeyCode;
	/*
		����ST�̼���������ļ��Ѿ�ִ����CPUϵͳʱ�ӵĳ�ʼ�������Բ����ٴ��ظ�����ϵͳʱ�ӡ�
		�����ļ�������CPU��ʱ��Ƶ�ʡ��ڲ�Flash�����ٶȺͿ�ѡ���ⲿSRAM FSMC��ʼ����

		ϵͳʱ��ȱʡ����Ϊ72MHz�������Ҫ���ģ������޸ģ�
		\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\system_stm32f10x.c
		������ϵͳʱ�ӵĺꡣ
	*/

	bsp_Init();
	PrintfLogo();		/* ��ӡ����Logo������1 */

	//bsp_InitIRD();		/* ����GPIO  */

	/* ��ӡ�˵� */
	printf("���������ң�������۲��ֵ\r\n");
	printf(" ����K1�� = ��ʼ��������ź�\r\n");
	printf(" ����K2�� = ֹͣ��������ź�\r\n");

	IRD_StartWork();
	while (1)
	{
		bsp_Idle();		/* ����ʱִ�еĺ���,����ι��. ��bsp.c�� */

		/* �����˲��ͼ���ɺ�̨systick�жϷ������ʵ�֣�����ֻ��Ҫ����bsp_GetKey��ȡ��ֵ���ɡ� */
		ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
		if (ucKeyCode != KEY_NONE)
		{
			switch (ucKeyCode)
			{
				case KEY_DOWN_K1:			/* ����K1������ */
					IRD_StartWork();		/* ��ʼ�������ң���ź� */
					printf("K1 ��ʼ�������\r\n");
					break;

				case KEY_DOWN_K2:			/* ����K2������ */
					IRD_StopWork();			/* ֹͣ�������ң���ź� */
					printf("K1 ֹͣ�������\r\n");
					break;

				/* �����Ǻ���ң�������¼� */
				case IR_KEY_POWER:
					printf("IR Power\r\n");
					break;

				case IR_KEY_MENU:
					printf("IR Menu\r\n");
					break;

				case IR_KEY_TEST:
					printf("IR Test\r\n");
					break;

				case IR_KEY_UP:
					printf("IR +\r\n");
					break;

				case IR_KEY_RETURN:
					printf("IR Return\r\n");
					break;

				case IR_KEY_LEFT:
					printf("IR Left\r\n");
					break;

				case IR_KEY_OK:
					printf("IR Ok\r\n");
					break;

				case IR_KEY_RIGHT:
					printf("IR Right\r\n");
					break;

				case IR_KEY_0:
					printf("IR 0\r\n");
					break;

				case IR_KEY_DOWN:
					printf("IR -\r\n");
					break;

				case IR_KEY_C:
					printf("IR C\r\n");
					break;

				case IR_KEY_1:
					printf("IR 1\r\n");
					break;

				case IR_KEY_2:
					printf("IR 2\r\n");
					break;

				case IR_KEY_3:
					printf("IR 3\r\n");
					break;

				case IR_KEY_4:
					printf("IR 4\r\n");
					break;

				case IR_KEY_5:
					printf("IR 5\r\n");
					break;

				case IR_KEY_6:
					printf("IR 6\r\n");
					break;

				case IR_KEY_7:
					printf("IR 7\r\n");
					break;

				case IR_KEY_8:
					printf("IR 8\r\n");
					break;

				case IR_KEY_9:
					printf("IR 9\r\n");
					break;

				default:
					if (ucKeyCode >= IR_KEY_STRAT)
					{
						printf("IR �޷�ʶ���ң�ذ���\r\n");
						printf("%02X %02X %02X %02X\r\n", g_tIR.RxBuf[0], g_tIR.RxBuf[1],
							g_tIR.RxBuf[2], g_tIR.RxBuf[3]);
					}
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
	printf("*************************************************************\n\r");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */
	printf("* �̼���汾 : V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
			__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("* \n\r");	/* ��ӡһ�пո� */
	printf("* QQ    : 1295744630 \r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* Copyright www.armfly.com ����������\r\n");
	printf("*************************************************************\n\r");
}
