/****************************************Copyright (c)**************************************************
**                               Shijiazhuang TIANYUAN technology Co.,LTD.
**                                     
** -----------------------------------------------------------------------------------------------------
** File Name    : bsp_ch374hft.c	
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

/* Includes ------------------------------------------------------------------*/
/*
#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"
#include "bsp_printf.h"
#include "bsp_button.h"
#include "bsp_hw_cfg.h"
*/
#include <includes.h>	// ���ļ����������б����.h�ļ� 


/* ���¶������ϸ˵���뿴CH374HFM.H�ļ� */
#define LIB_CFG_INT_EN			0		   // CH374��INT#�������ӷ�ʽ,0Ϊ"��ѯ��ʽ",1Ϊ"�жϷ�ʽ" 
//#define NO_DEFAULT_CH374_INT	1		   // ��ֹCH374HF6.H�е�Ĭ���жϴ������,ʹ���������б�д���жϳ��� 

// ֻʹ�õ�Ƭ�����õ�1KB�ⲿRAM: 0000H-01FFH Ϊ���̶�д������, ���ֽ�Ϊ��λ��д�ļ�����Ҫ�ļ����ݶ�д������FILE_DATA_BUF  
#define	DISK_BASE_BUF_ADDR		0x0000	// �ⲿRAM�Ĵ������ݻ���������ʼ��ַ,�Ӹõ�Ԫ��ʼ�Ļ���������ΪSECTOR_SIZE  
#define DISK_BASE_BUF_LEN		4096
#define CH374_INT_WIRE			GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)	// PD10, INT0, CH374���ж���INT#����,����CH374��INT#����,���ڲ�ѯ�ж�״̬  
// ���δ����CH374���ж�����,��ôӦ��ȥ����������,�Զ�ʹ�üĴ�����ѯ��ʽ  

#define NO_DEFAULT_CH374_F_ENUM		1		// δ����CH374FileEnumer����ʽ�ֹ�Խ�Լ����  
#define NO_DEFAULT_CH374_F_QUERY	1		// δ����CH374FileQuery����ʽ�ֹ�Խ�Լ����  
//#define NO_DEFAULT_CH374_RESET		1		// δ����CH374Reset����ʽ�ֹ�Խ�Լ����  

#include "CH374HFM.H"		// ����CORTEX-M3�Ŀ�ͷ�ļ�  
#include "bsp_CH374hft.h"

#define USB_DataBuf_size         512   

u8      USB_DataBuf[USB_DataBuf_size] = {0};

CH374_Flag_t    CH374_Flag;

USB_Flag_e     USB_Flag;
//************************�ڲ�����*********************************
extern u8 USB_FILE_READ(char *fname,u16 length ,u8* data_buf ); 
extern u8 USB_FILE_Create(char *fname);
extern u8 USB_FILE_WriteX(u8* addr);
/*
void xQueryInterrupt( void )
{
   while(USB_Flag.Transf_one ==FALSE);
   USB_Flag.Transf_one =FALSE;
}  */
/********************************************************************************************
**	�������� ��void EXTI15_10_IRQHandler(void)
**	�������� ��usb�жϴ���
**	����ӿ� ��
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/


void EXTI15_10_IRQHandler(void)
{
    u8 USB_EXTI_Flag;
	OS_CPU_SR  cpu_sr;

    OS_ENTER_CRITICAL();          //���� �ٽ��
    OSIntNesting++;
    OS_EXIT_CRITICAL();           //�˳��ٽ��

	//**************************************************************
	//***������*********
	//---------------------------------------------

	 if(EXTI_GetITStatus(EXTI_Line10) != RESET)
     {
	     //Write_To_Log( UDISK_TASK ,"Enter USB EXTI_IRQHandler !!");

		 USB_EXTI_Flag =CH374_READ_REGISTER( REG_INTER_FLAG );					//��ȡch374�ж�
		 //RS232_PutString("USB_EXTI_Flag:");
		 //RS232_PutWord(USB_EXTI_Flag,8);
		 if((USB_EXTI_Flag & BIT_IF_DEV_DETECT)!=0)						    	//�Ȳ���ж�
		 {
		     if((USB_EXTI_Flag & BIT_IF_DEV_ATTACH)!=0)
			 {
			     #ifdef EN_HUB_DISK
		           CH374vHubPortCount = 0;                 
                  #endif
        
			 	 CH374_WRITE_REGISTER( REG_USB_SETUP, BIT_SETP_HOST_MODE );	    //��������ģʽ
				 //Write_To_Log( UDISK_TASK ,"USB_Enter..........");
				 USB_Flag.In_Out =TRUE ;
				 RS232_PutString("\r\nUSB_Enter..........");
			 }
			 else
			 {
			 	 //Write_To_Log( UDISK_TASK ,"USB_Exit..........");
				 RS232_PutString("\r\nUSB_Exit_1..........");
				 //USB_Flag.In_Out =FALSE ;			 
			 }
			 CH374_WRITE_REGISTER( REG_INTER_FLAG,BIT_IF_USB_PAUSE| BIT_IF_DEV_DETECT ); //���
		 }
		 if((USB_EXTI_Flag & BIT_IF_TRANSFER)!=0)								//���ݴ�������ж�
		 {
		 	 if((USB_EXTI_Flag & BIT_IF_DEV_ATTACH)!=0)
			 {
			 	 //Write_To_Log( UDISK_TASK ,"USB_Trans_OK����one!!");
				 //RS232_PutString("\r\nUSB_Trans_OK����one!!");
				 USB_Flag.Transf_one =TRUE ;
			 }
			 else
			 {
			 	 //Write_To_Log( UDISK_TASK ,"USB_Exit..........");
				 RS232_PutString("\r\nUSB_Exit_2..........");
				 //USB_Flag.In_Out =FALSE ;			 
			 }	
			 //CH374_WRITE_REGISTER( REG_INTER_FLAG, BIT_IF_TRANSFER ); //���	 
		 }

	 
	     EXTI_ClearITPendingBit(EXTI_Line10);
	 }
	//**************************************************************

    OSIntExit();
}
//Ӳ�����ó�ʼ��
void USB_Init(void)
{
    SPI_CH374_Init();
}
/********************************************************************************************
**	�������� ��void USB_start(void)
**	�������� ��usb ����
**	����ӿ� ��
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/

void USB_start(void)
{
   	u8 temp;

	RS232_PutString("\r\nZZ--1\r\n");
    //CH374Reset();
    RS232_PutString("\r\nZZ--2\r\n");
	temp=CH374LibInit();               // ��ʼ��CH374������CH374оƬ,�����ɹ�����0 
    if(temp==0)
	{
	    Write_To_Log( UDISK_TASK , "USB_Init_OK..........");
		CH374_Flag.State= TRUE;
		RS232_PutString("\r\nZZ--3\r\n");
	}
	else
	{
	   Write_To_Log( UDISK_TASK , "USB_Init_Error..........");
	   CH374_Flag.State = FALSE;
	   RS232_PutString("\r\nZZ--4\r\n");
	}
	RS232_PutString("\r\nZZ--5\r\n");
}
/********************************************************************************************
**	�������� ��void USB_Dealing(void)
**	�������� ��usb������
**	����ӿ� ��
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/



void USB_Dealing(void)
{
	  RS232_PutString("\r\nUSB_Dealing..........");

	  App_Task_ClrDog(UDISK_TASK);         //UDISK_TASK ����ι�� 

	  if(USB_Flag.In_Out == TRUE)
      {
		   USB_Flag.State =TRUE;
		   RS232_PutString("\r\nUSB_Dealing1..........");
	  }
	  else if((USB_Flag.In_Out == FALSE)&&(USB_Flag.State == TRUE ))
      {
			USB_Flag.State =FALSE ;
			clrram((u8 *) &USB_Flag,sizeof(USB_Flag));
			RS232_PutString("\r\nUSB_Dealing2..........");
	  }

      if(CH374_Flag.State == FALSE )
	  {	  
	  	   USB_start();	 
		   RS232_PutString("\r\nUSB_Dealing3.........."); 
	  }
	  else if(USB_Flag.State == TRUE)
	  { 
		   RS232_PutString("\r\nUSB_Dealing4..........");
		   if(USB_Flag.ReadFlag_One==FALSE )	   //��1
		   {
		   		RS232_PutString("\r\nUSB_du1");
				if(0==USB_SetIC_READ())
				{
				     USB_Flag.ReadFlag_One=TRUE;
					 RS232_PutString("\r\nUSB_du2..........");
				}
				else
				{
					USB_Flag.State =FALSE ;
				//	CH374Reset();
				}
		   }
		   else if(USB_Flag.ReadFlag_Two==FALSE )	   //��2
		   { 
				 RS232_PutString("\r\nUSB_du3..........");
				 if(0==USB_SetXSJLY_READ())
				{
				    USB_Flag.ReadFlag_Two=TRUE;
					RS232_PutString("\r\nUSB_du4..........");
				} 
				else
				{
					USB_Flag.State =FALSE ;
				//	CH374Reset();
				}
		   }
		   else if(USB_Flag.WriteFlag ==FALSE)		   //д
		   {
		   		  RS232_PutString("\r\nUSB_xie1..........");
				  if(0==USB_CJData_Write())
				{
				    USB_Flag.WriteFlag=TRUE;
					RS232_PutString("\r\nUSB_xie2..........");
				} 
				else
				{
					USB_Flag.State =FALSE ;
				//	CH374Reset();
				}
		   }	 
	  }
	  
}

/********************************************************************************************
**	�������� ��u8 USB_FILE_READ((char *)fname,u16 length ,u8* data_buf ) 
**	�������� ����ȡu�̵�����ָ���ļ�������
**	����ӿ� �� 0 �ɹ�	1 ʧ�� 2δ����u�� 3u��δ׼����	4 �޷����ļ�
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/


u8 USB_FILE_READ(char* fname,u16 length ,u8* data_buf )  
{
    u8 success=1,temp,i;
	u8 GetNum;
    u16  Data_Count,j=0;
	RS232_PutString("\r\nUSB_du1_11");
	temp = CH374DiskConnect();
	if(temp != ERR_SUCCESS)
	{  
	    success=2; 
		RS232_PutString("\r\nUSB_du1_12");
	    return 	 success;
	}
	RS232_PutString("\r\nUSB_du1_13");
	temp = CH374DiskReady();
	RS232_PutString("\r\nUSB_du1_14");
	if(temp != ERR_SUCCESS)
	{  
	    success=3; 
		RS232_PutString("\r\nUSB_du1_15");
	    return 	 success;
	} 
    RS232_PutString("\r\nCH374DiskReady:");
    RS232_PutWord(temp,8);
    strcpy( (char *)mCmdParam.Open.mPathName,fname );
	RS232_PutString("\r\nUSB_du1_16");
	temp = CH374FileOpen();
	RS232_PutString("\r\nUSB_du1_17");
	if(temp != ERR_SUCCESS)
	{  
	    success=4; 
	   RS232_PutString("\r\nUSB_du1_18");
	    return 	 success;
	}                        		  
    RS232_PutString("\r\nUSB_du1_19");
	Data_Count = length +10;                       // ���ݳ��� 
	while ( Data_Count ) 
    { 
		App_Task_ClrDog(UDISK_TASK);               //UDISK_TASK ����ι�� 

		if ( Data_Count > MAX_BYTE_IO ) 		   //���㵱�ζ�ȡ����
        {   GetNum= MAX_BYTE_IO;  	  }
		else 
        {    GetNum = Data_Count;     }
        
		mCmdParam.ByteRead.mByteCount = GetNum;    //����������� 
		temp = CH374ByteRead( );  
	    if ( temp != ERR_SUCCESS ) 
	    {
            success=1;
			break;
	    }
		Data_Count -= mCmdParam.ByteRead.mByteCount;  //����,��ȥ��ǰʵ���Ѿ��������ַ��� 
		for ( i=0; i<mCmdParam.ByteRead.mByteCount; i++,j++ )
		{
            data_buf[j]=mCmdParam.ByteRead.mByteBuffer[i] ;
		}
        if ( mCmdParam.ByteRead.mByteCount < GetNum ) 
		{  
		    success=0;
			break;
		}
	}
	temp = CH374FileClose( );
	if(temp != ERR_SUCCESS)
    {  success=4; }
	return success;
}
/********************************************************************************************
**	�������� ��U8 USB_SetIC_READ(void)
**	�������� ��usb������
**	����ӿ� ��0 �ɹ�	 1 ʧ��
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/

u8 USB_SetIC_READ(void)
{
	u8  temp,success;

	clrram(USB_DataBuf,USB_DataBuf_size);
    temp = USB_FILE_READ("/ZZ.TY",0x68 ,USB_DataBuf );
		RS232_PutString("\r\nUSB_du1_01");
	if(temp ==0)
	{	
	    Write_To_Log2(UDISK_TASK,"Read USB_DRIVERID_Data:",1,(u32)USB_DataBuf,0x68);

	    RS232_PutString("\r\nUSB_du1_02");
	    success =0;
	}
	else if(temp ==4)   //���ļ���
	{
		 Write_To_Log( UDISK_TASK , "NO DRIVERID.TY");
		 success =0;
		 	RS232_PutString("\r\nUSB_du1_03");
	}
	else
	{
	    success =1;
	}
	return 	 success;
}
/********************************************************************************************
**	�������� ��U8 USB_SetXSJLY_READ(void)
**	�������� ��usb������
**	����ӿ� ��0 �ɹ�	 1 ʧ��
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/
u8 USB_SetXSJLY_READ(void)
{
	u8  temp,success;

	clrram(USB_DataBuf,USB_DataBuf_size);
    temp = USB_FILE_READ("/GBSETUP.TY",0x68 ,USB_DataBuf ); 
	if(temp ==0)
	{	
	    Write_To_Log2(UDISK_TASK,"Read USB_GBSETUP_Data:",1,(u32)USB_DataBuf,0x68);



	    success =0;
	}
	else if(temp ==4)   //���ļ���
	{
		 Write_To_Log( UDISK_TASK , "NO GBSETUP.TY");
		 success =0;
	}
	else
	{
	    success =1;
	}
	return 	 success;
}
/********************************************************************************************
**	�������� ��u8 USB_FILE_Create((char *)fname) 
**	�������� ����u�����½��ļ�����
**	����ӿ� �� 0 �ɹ� 1 �޷������ļ�	2δ����u�� 3u��δ׼����	
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/
u8 USB_FILE_Create(char* fname)
{   
    u8  temp,success;
	RS232_PutString("\r\nUSB_xie001..........");
	temp = CH374DiskConnect();
	RS232_PutString("\r\nUSB_xie002..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=2; 
		RS232_PutString("\r\nUSB_xie003..........");
	    return 	 success;
	}
	RS232_PutString("\r\nUSB_xie004..........");
	temp = CH374DiskReady();
	RS232_PutString("\r\nUSB_xie005..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=3; 
		RS232_PutString("\r\nUSB_xie006..........");
	    return 	 success;
	} 
	RS232_PutString("\r\nUSB_xie007..........");
    strcpy( (char *)mCmdParam.Create.mPathName,"/ZZ.TY" );
	RS232_PutString("\r\nUSB_xie008..........");
	temp = CH374FileCreate();
	RS232_PutString("\r\nUSB_xie009..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=1; 
		RS232_PutString("\r\nUSB_xie010..........");
	    return 	 success;
	}       
	RS232_PutString("\r\nUSB_xie011..........");                 		  
	success=0;
	return 	 success;
}
/********************************************************************************************
**	�������� ��u8 USB_FILE_WriteX(u8* addr) 
**	�������� ��д������
**	����ӿ� �� 0 �ɹ� 1 ʧ��
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/
u8 USB_FILE_WriteX(u8* addr)
{
    u8 success;

    App_Task_ClrDog(UDISK_TASK);                   //UDISK_TASK ����ι��
	
    mCmdParam.Locate.mSectorOffset = 0xffffffff; //�ƶ��ļ�ָ�뵽�ļ�ĩβ 
    CH374FileLocate();             
    mCmdParam.WriteX.mSectorCount = 1;
    mCmdParam.WriteX.mDataBuffer = addr;  // ������ 

	if ( CH374FileWriteX() != ERR_SUCCESS )
	{
	    success=1; 
	}                        		  
	success=0;
	return 	 success;

}

/********************************************************************************************
**	�������� ��U8 USB_CJData_Write(void)
**	�������� ��usbд���ļ�
**	����ӿ� ��0 �ɹ�	 1 ʧ��
**	��    �� ��  zz
**	ʱ    �� ��	20120823
*********************************************************************************************/
u8 USB_CJData_Write(void)
{
    u8    temp,success,Index;
	char  FileName[15]={0};
	u8    RTCTime_Cur[6];

  	Index=Car_info.Car_No[0];
    for(temp=0;temp< Index;temp++)
	{ FileName[temp]= Car_info.Car_No[temp+1]; }	
	FileName[Index++]='.';
	FileName[Index++]='T';
	FileName[Index++]='Y';
    RS232_PutString("\r\nUSB_xie01..........");
    temp = USB_FILE_Create("/ZZ.TY");
	RS232_PutString("\r\nUSB_xie02..........");
	if(temp != ERR_SUCCESS)
	{  
	    success=1; 
		RS232_PutString("\r\nUSB_xie03..........");
	    return 	 success;
	}
	RS232_PutString("\r\nUSB_xie04..........");
	//********************************************************
    //��������
		clrram(USB_DataBuf,USB_DataBuf_size);
		USB_DataBuf[0]=9;
		USB_DataBuf[1]=8;
		USB_DataBuf[2]=7;
		USB_DataBuf[3]=6;
		USB_DataBuf[4]=5;
		USB_DataBuf[5]=4;
		USB_DataBuf[6]=3;
		USB_DataBuf[7]=2;
		USB_DataBuf[8]=1;
	    //strcpy( (char *)USB_DataBuf,"zz-20120819" );
		RS232_PutString("\r\nUSB_xie05..........");
	    temp=USB_FILE_WriteX(USB_DataBuf);
		RS232_PutString("\r\nUSB_xie06..........");
		if(temp != ERR_SUCCESS)
	    {  
	    success=1; 
		RS232_PutString("\r\nUSB_xie07..........");
	    return 	 success;
	    }

		Write_To_Log2(UDISK_TASK,"write USB_DRIVERID_Data:",1,(u32)USB_DataBuf,20);
	//********************************************************

    mCmdParam.Locate.mSectorOffset = 0xffffffff; // �ƶ��ļ�ָ�뵽�ļ�ĩβ 
    CH374FileLocate();                          
    mCmdParam.WriteX.mSectorCount = 0;
    mCmdParam.WriteX.mDataBuffer = USB_DataBuf;  // ������ 
    CH374FileWriteX();                           // дĿ¼�ĵ�һ������ 


    //Get current time
    Time_Display(RTC_GetCounter(),RTCTime_Cur);           
	mCmdParam.Modify.mFileAttr = 0xff;            //   �������: �µ��ļ����Բ��޸�
	mCmdParam.Modify.mFileTime = MAKE_FILE_TIME( RTCTime_Cur[RTC_HOUR], RTCTime_Cur[RTC_MIN], RTCTime_Cur[RTC_SEC]);//0xffff;//   �������: �µ��ļ�ʱ��,Ϊ0FFFFH���޸�,ʹ���½��ļ�������Ĭ��ʱ��
	mCmdParam.Modify.mFileDate = MAKE_FILE_DATE( (2000+RTCTime_Cur[RTC_YEAR]), RTCTime_Cur[RTC_MONTH], RTCTime_Cur[RTC_DAY]);//  �������: �µ��ļ�����: 2004.05.18
	mCmdParam.Modify.mFileSize = 0xffffffff;      //�ļ�ʱ�Զ����³���,���Դ˴����޸�                                          
	temp=CH374FileModify( );                     // �޸ĵ�ǰ�ļ�����Ϣ
	if(temp != ERR_SUCCESS)
	{  
	    success=1; 
	    return 	 success;
	}
    if ( CH374FileClose()!= ERR_SUCCESS ) 			    // �ر��ļ�
    {  
	    success=1; 
	}    
	else
	{
	    success=0;
	}
	 return 	 success;                     
}  

