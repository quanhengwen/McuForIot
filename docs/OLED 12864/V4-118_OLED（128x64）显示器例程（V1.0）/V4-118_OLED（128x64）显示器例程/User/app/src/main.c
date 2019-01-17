/*
*********************************************************************************************************
*
*	ģ������ : ������ģ�顣
*	�ļ����� : main.c
*	��    �� : V1.2
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2015-08-30 armfly  �׷�
*
*
*	Copyright (C), 2013-2014, ���������� www.armfly.com
*
*********************************************************************************************************
*/

#include "bsp.h"				/* �ײ�Ӳ������ */

/* ���������������̷������� */
#define EXAMPLE_NAME	"V4-118_OLED��128x64����ʾ������"
#define EXAMPLE_DATE	"2015-08-30"
#define DEMO_VER		"1.0"


#define DEMO_PAGE_COUNT		7	/* ��ʾҳ��ĸ��� */

/* �������ļ��ڵ��õĺ������� */
static void PrintfLogo(void);
static void DispMenu(void);

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
	FONT_T tFont12, tFont16;
	uint8_t ucKeyCode;
	uint8_t ucItem;
	uint8_t ucDispReq;	/* ˢ�������־ */
	uint8_t ucContrast = 0x80;
	uint8_t ucDir = 0;	/* ��ʾ����, 0 ��ʾ��������1��ʾ��180�� */

	/*
		ST�̼����е������ļ��Ѿ�ִ���� SystemInit() �������ú����� system_stm32f4xx.c �ļ�����Ҫ������
		����CPUϵͳ��ʱ�ӣ��ڲ�Flash����ʱ������FSMC�����ⲿSRAM
	*/

	bsp_Init();		/* Ӳ����ʼ�� */
	PrintfLogo();	/* ��ӡ������Ϣ������1 */

	DispMenu();		/* ��ӡ������ʾ */

	OLED_InitHard();	 /* ��ʼ��OLEDӲ�� */
	OLED_ClrScr(0x00);	 /* ������0x00��ʾ�ڵף� 0xFF ��ʾ�׵� */

	/* ����������� */
	{
		tFont16.FontCode = FC_ST_16;	/* ������� 16���� */
		tFont16.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont16.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont16.Space = 0;			/* ���ּ�࣬��λ = ���� */

		tFont12.FontCode = FC_ST_12;	/* ������� 12���� */
		tFont12.FrontColor = 1;		/* ������ɫ 0 �� 1 */
		tFont12.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
		tFont12.Space = 1;			/* ���ּ�࣬��λ = ���� */
	}
	ucItem = 0;
	ucDispReq = 1;
	while (1)
	{
		bsp_Idle();		/* ���������bsp.c�ļ����û������޸��������ʵ��CPU���ߺ�ι�� */

		if (ucDispReq == 1)
		{
			ucDispReq = 0;

			switch (ucItem)
			{
				case 0:
					OLED_ClrScr(0);			 /* �������ڵ� */
					OLED_DrawRect(0, 0, 64, 128, 1);
					OLED_DispStr(8, 3, "������OLED����", &tFont16);		/* ��(8,3)���괦��ʾһ������ */
					OLED_DispStr(10, 22, "�����ҡ��!", &tFont16);
					OLED_DispStr(5, 22 + 20, "www.ARMfly.com", &tFont16);
					break;

				case 1:
					//OLED_StartDraw();	  ���øĺ�����ֻˢ�»�������������ʾ
					OLED_ClrScr(0);
					OLED_DispStr(0, 0,  "�������ǻƺ�¥��", &tFont16);
					OLED_DispStr(0, 16, "�̻����������ݡ�", &tFont16);
					OLED_DispStr(0, 32, "�·�ԶӰ�̿վ���", &tFont16);
					OLED_DispStr(0, 48, "Ψ�������������", &tFont16);
					//OLED_EndDraw();	  ���øĺ�����������������������ʾ
					break;

				case 2:
					OLED_ClrScr(0);
					OLED_DispStr(5, 0,  "�����Ϻ�Ȼ֮���꡷", &tFont12);
					OLED_DispStr(0, 13, "�������ǻƺ�¥��", &tFont12);
					OLED_DispStr(0, 26, "�̻����������ݡ�", &tFont12);
					OLED_DispStr(0, 39, "�·�ԶӰ�̿վ���", &tFont12);
					OLED_DispStr(0, 52, "Ψ�������������", &tFont12);

					OLED_DispStr(110, 14, "��", &tFont16);
					OLED_DispStr(110, 30, "��", &tFont16);
					OLED_DispStr(110, 46, "��", &tFont16);
					OLED_DrawRect(109,13, 50, 17,1);
					break;

				case 3:
					/* �׵׺��� */
					tFont12.FrontColor = 0;		/* ������ɫ 0 �� 1 */
					tFont12.BackColor = 1;		/* ���ֱ�����ɫ 0 �� 1 */
					OLED_ClrScr(0xFF);
					OLED_DispStr(5, 0,  "�����Ϻ�Ȼ֮���꡷", &tFont12);
					OLED_DispStr(0, 13, "�������ǻƺ�¥��", &tFont12);
					OLED_DispStr(0, 26, "�̻����������ݡ�", &tFont12);
					OLED_DispStr(0, 39, "�·�ԶӰ�̿վ���", &tFont12);
					OLED_DispStr(0, 52, "Ψ�������������", &tFont12);

					OLED_DispStr(110, 14, "��", &tFont16);
					OLED_DispStr(110, 30, "��", &tFont16);
					OLED_DispStr(110, 46, "��", &tFont16);
					OLED_DrawRect(109,13, 50, 17, 0);

					/* �ָ��ڵװ��� */
					tFont12.FrontColor = 1;		/* ������ɫ 0 �� 1 */
					tFont12.BackColor = 0;		/* ���ֱ�����ɫ 0 �� 1 */
					break;

				case 4:
					OLED_ClrScr(0);
					OLED_DrawRect(0,0, 10,10,1);	/* ��(0,0)���괦����һ����10��10�ľ��� */
					OLED_DrawRect(10,10, 20,30,1);	/* ��(10,10)���괦����һ����20��30�ľ��� */
					OLED_DrawCircle(64,32,30,1);	/* ��(64,32)���ư뾶30��Բ */
					OLED_DrawLine(127,0,0,63,1);	/* ��(127,0) �� (0,63) ֮�����һ��ֱ�� */
					break;

				case 5:
					OLED_ClrScr(0x00);				/* �������ڵ� */
					break;

				case 6:
					OLED_ClrScr(0xFF);				/* �������׵� */
					{
						//char buf[32];

						//sprintf(buf, "%d", ucContrast);
						//OLED_DispStr(10,10,buf,&tFont16);
					}
					break;
			}
		}

		ucKeyCode = bsp_GetKey();
		if (ucKeyCode != KEY_NONE)
		{
			/* �м����� */
			switch (ucKeyCode)
			{
				case JOY_DOWN_U:		/* ҡ���ϼ����� */
					if (ucContrast < 255)
					{
						ucContrast++;
					}
					OLED_SetContrast(ucContrast);
					ucDispReq = 1;
					break;

				case JOY_DOWN_D:		/* ҡ���¼����� */
					if (ucContrast > 0)
					{
						ucContrast--;
					}
					OLED_SetContrast(ucContrast);
					ucDispReq = 1;
					break;

				case JOY_DOWN_L:		/* ҡ��LEFT������ */
					if (ucItem > 0)
					{
						ucItem--;
					}
					else
					{
						ucItem = DEMO_PAGE_COUNT - 1;
					}
					ucDispReq = 1;
					break;

				case JOY_DOWN_R:	/* ҡ��RIGHT������ */
					if (ucItem < DEMO_PAGE_COUNT - 1)
					{
						ucItem++;
					}
					else
					{
						ucItem = 0;
					}
					ucDispReq = 1;
					break;

				case JOY_DOWN_OK:		/* ҡ��OK�� */
					if (ucDir == 0)
					{
						ucDir = 1;
						OLED_SetDir(1);	/* ������ʾ���� */
					}
					else
					{
						ucDir = 0;
						OLED_SetDir(0);	/* ������ʾ���� */
					}
					ucDispReq = 1;
					break;
			}
		}
	}
}

/*
*********************************************************************************************************
*	�� �� ��: DispMenu
*	����˵��: ��ʾ������ʾ�˵�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispMenu(void)
{
	printf("\r\n��������Ҫ����OLED��ʾģ��������У�\r\n");
	printf("�����ҡ��:\r\n");
	printf("ҡ�����¼� �� ���ڶԱȶ�\r\n");
	printf("ҡ�����Ҽ� �� �л���ʾ����\r\n");
	printf("ҡ��OK��   �� �л���ʾ������ʾ���ݷ�ת180�ȣ�\r\n");
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
	printf("\n\r");
	printf("*************************************************************\n\r");
	printf("* ��������   : %s\r\n", EXAMPLE_NAME);	/* ��ӡ�������� */
	printf("* ���̰汾   : %s\r\n", DEMO_VER);		/* ��ӡ���̰汾 */
	printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	/* ��ӡST�̼���汾����3���������stm32f10x.h�ļ��� */
	printf("* �̼���汾 : V%d.%d.%d (STM32F10x_StdPeriph_Driver)\r\n", __STM32F10X_STDPERIPH_VERSION_MAIN,
			__STM32F10X_STDPERIPH_VERSION_SUB1,__STM32F10X_STDPERIPH_VERSION_SUB2);
	printf("* \n\r");	/* ��ӡһ�пո� */
	printf("* QQ    : 1295744630 \r\n");
	printf("* ����  : armfly\r\n");
	printf("* Email : armfly@qq.com \r\n");
	printf("* �Ա���: armfly.taobao.com\r\n");
	printf("* Copyright www.armfly.com ����������\r\n");
	printf("*************************************************************\n\r");
}

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
