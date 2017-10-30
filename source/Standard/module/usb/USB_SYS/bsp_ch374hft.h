/****************************************Copyright (c)**************************************************
**                               Shijiazhuang TIANYUAN technology Co.,LTD.
**                                     
** -----------------------------------------------------------------------------------------------------
** File Name    : bsp_ch374hft.h	
** Version      : V1.0
** Description  : CH374 Host Filesystem interface
                  Support:  FAT12/FAT16/FAT32

** -----------------------------------------------------------------------------------------------------
** Author       : WuPengbo  
** Date         : 02/01/2011
** -----------------------------------------------------------------------------------------------------
** Modify Person:
** Modify Date  :
** Description  :
*********************************************************************************************************/

/* Define to prevent recursive inclusion ----------------------------------------------*/
#ifndef __BSP_CH374HFT_H 
#define __BSP_CH374HFT_H

#include <includes.h>	


#define USB_DataBuf_size         512   


typedef struct{
				u8  State		   :1;			//0 �ر�      1 ����

              }CH374_Flag_t;


typedef struct{
			    
				u8  State              :1;			//0��u��	  1 ������
				u8  In_Out             :1;			//0out        1  in	   //�жϲ���
				u8  Data_Error		   :1;			//0���ݲ����޴���   1 ���ݲ�������
				u8  ReadFlag_One       :1;			//0��δ���   1 �����
				u8  ReadFlag_Two       :1;			//0��δ���   1 �����
				u8	WriteFlag          :1;		    //0дδ���	  1 д���

				u8  Transf_one         :1;			//����д���	0дδ���	  1 д���

               }USB_Flag_e;

extern void EXTI15_10_IRQHandler(void);
extern void USB_Init(void);
extern void USB_start(void);
extern void USB_Dealing(void);
extern u8 USB_SetIC_READ(void);
extern u8 USB_SetXSJLY_READ(void);
extern u8 USB_CJData_Write(void);



#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/


