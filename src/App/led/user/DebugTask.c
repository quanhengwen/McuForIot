
#include <includes.h>	
#include "DriverConfig.h"

#include <DebugTask.h>


void DebugTask(void *pd)
{
   	while(1)
	{
       	if(Process_RS232_Receive())
       	{
//         	if(FALSE !=Find_Str(RS232_RecBuf,"#",RS232_RecBuf+RS232_Receive_BUFSIZE) )    //����
//	      	{   
//	     	  	RS232_PutString("\r\nGC_ZZ");
//	      	}
//	      	else if(FALSE !=Find_Str(RS232_RecBuf,"$",RS232_RecBuf+RS232_Receive_BUFSIZE) )//ע��
//	      	{   
//	        	RS232_PutString("\r\n*********************");
//	      	}	
	   	}
    	OSTimeDly(OS_TICKS_PER_SEC*5);

	}
}


