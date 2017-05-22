				

#ifndef __DRIVER_PRINTF_H
#define __DRIVER_PRINTF_H

#include "Printf.h"
#include "RS232.h"
#include "DriverConfig.h"

//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if CONFIG_DEV_PRINTF
	#pragma import(__use_no_semihosting)             
	//��׼����Ҫ��֧�ֺ���                 
	typedef struct
	{ 
		int handle; 
		/* Whatever you require here. If the only file you are using is */ 
		/* standard output using printf() for debugging, no file handling */ 
		/* is required. */ 
	}FILE; 
	/* FILE is typedef�� d in stdio.h. */ 
	FILE __stdout;
	       
	/*************����_sys_exit()�Ա���ʹ�ð�����ģʽ********************/    
	_sys_exit(int x) 
	{ 
		x = x; 
	} 
	/*************�ض���fputc����*****************************************/
	int fputc(int ch, FILE *f)
	{      
		u8 val = (u8)ch;
		RS232_PutChar(&val,1);      
		return ch;
	}
#else
	#define	 printf   RS232_PutString
#endif 
#endif

