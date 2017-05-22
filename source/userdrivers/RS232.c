

/*******************************************************
* �ļ���     :RS232.c
* ����       :dongfangxiao     
* ��������   :2008.05.29
* ��������   :���ļ������������йص��Դ��ڵ���������
********************************************************
* ������ʷ:

�޸�ʱ�䣺20120405
�޸����ݣ�RS232_TX_Char()��'>'->'>=' 
�޸�ԭ�򣺷��ͻ������жϽ���Խ�磬�����жϻ��������ݳ���ʱ��
          ��������ʵ�ʳ����������жϵ�ʱ��ȴ��0.(��������֤)

***********************************************************************/
#include <includes.h>
#include <RS232.h>

//-------------------------------------------------------
//Private constants and types.
//-------------------------------------------------------
// Buffer sizes
#define RS232_TX_BUFSIZE        1000            // Transmit buffer size
#define RS232_RX_BUFSIZE        100            // Receive buffer size


//-------------------------------------------------------
//Module global variables.
//-------------------------------------------------------
u8   RS232_TXBuf[RS232_TX_BUFSIZE];           // Transmit buffer
u8   RS232_RXBuf[RS232_RX_BUFSIZE];           // Receive buffer
u8   RS232_RecBuf[RS232_Receive_BUFSIZE];  

// Buffer index values
s16  RS232_TX_TailIndex;                    // Transmit buffer tail index
s16  RS232_TX_HeadIndex;                    // Transmit buffer head index
s16  RS232_RX_TailIndex;                    // Receive buffer tail index
s16  RS232_RX_HeadIndex;                    // Receive buffer head index
s16  RS232_Receive_Index;

//-------------------------------------------------------
//Definition of API functions.
//-------------------------------------------------------
void RS232_Initialise( void )
{
  GPIO_InitTypeDef   GPIO_InitStructure;//I/O�ڳ�ʼ���ṹ��
  NVIC_InitTypeDef   NVIC_InitStructure;//�жϳ�ʼ���ṹ��
  USART_InitTypeDef  USART_InitStructure;//���ڳ�ʼ���ṹ��
  USART_ClockInitTypeDef  USART_ClockInitS;

   // Flush the buffers by setting the indexes to 0
   RS232_TX_HeadIndex = 0;
   RS232_TX_TailIndex = 0;
   RS232_RX_HeadIndex = 0;
   RS232_RX_TailIndex = 0;
   RS232_Receive_Index=0;

       // Configure USART1 Tx (PA.09) as alternate function push-pull 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_Init(GPIOA, &GPIO_InitStructure);

       // Configure USART1 Rx (PA.10) as input floating 
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
      // Enable the USART1 Interrupt 
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&NVIC_InitStructure);
  
   USART_StructInit(&USART_InitStructure);

     // �����������
   USART_InitStructure.USART_BaudRate = 115200;  
   USART_InitStructure.USART_WordLength = USART_WordLength_8b; // λ��
   USART_InitStructure.USART_StopBits = USART_StopBits_1;      //ֹͣλ
   USART_InitStructure.USART_Parity = USART_Parity_No;         //У��λ
   USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������
   USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //����ģʽ
   USART_Init(USART1, &USART_InitStructure);  // Configure USART1 ���� ���ڷ��ͷ�ʽ�Ĵ���


   USART_ClockInitS.USART_Clock = USART_Clock_Disable;          //SCLKʱ��ʹ�ܣ�ͬ��ģʽ�£�
   USART_ClockInitS.USART_CPOL = USART_CPOL_Low;                //ʱ�Ӽ��ԣ�ͬ��ģʽ�£�
   USART_ClockInitS.USART_CPHA = USART_CPHA_2Edge;              //ʱ����λ��ͬ��ģʽ�£�
   USART_ClockInitS.USART_LastBit = USART_LastBit_Disable;      //���һλʱ�����壨ͬ��ģʽ�£�
   USART_ClockInit(USART1, &USART_ClockInitS);

   // USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
   USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
   USART_Cmd(USART1, ENABLE);                // Enable USART1   
}

u16 RS232_TX_Count(void)
{
    s16 num;
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();      //    ���� �ٽ��
    num =RS232_TX_HeadIndex -RS232_TX_TailIndex;
    if (num <0)
    {  num +=RS232_TX_BUFSIZE; }

    OS_EXIT_CRITICAL();        //     �˳��ٽ��
    return num;
}

u16 RS232_RX_Count(void)
{
    s16 num;
    OS_CPU_SR  cpu_sr;
    OS_ENTER_CRITICAL();      //    ���� �ٽ��
    num =RS232_RX_HeadIndex -RS232_RX_TailIndex;
    if (num <0)
    {  num +=RS232_RX_BUFSIZE; }

    OS_EXIT_CRITICAL();        //     �˳��ٽ��
    return num;
}

bool  RS232_TX_Char(u8 c)
{
    bool success =TRUE;
    OS_CPU_SR  cpu_sr;
    
    //if(System.DebugEn !=TRUE)
    //{ return FALSE;}

    OS_ENTER_CRITICAL();          //    ���� �ٽ��      

    if(RS232_TX_Count() >=(RS232_TX_BUFSIZE-1))	 //20120405D+ '>'->'>='
    {  success =FALSE; }
    else
    {
       RS232_TXBuf[RS232_TX_HeadIndex++] = c;
       if(RS232_TX_HeadIndex >= RS232_TX_BUFSIZE)
       { RS232_TX_HeadIndex =0; }
       
        USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    }
     OS_EXIT_CRITICAL();        //     �˳��ٽ��
   return success;
}

s16 RS232_RX_Char(void)
{
   u8 ch;
   OS_CPU_SR  cpu_sr;
   OS_ENTER_CRITICAL();          //    ���� �ٽ��
   if(RS232_RX_Count() ==0)
   { 
     OS_EXIT_CRITICAL();        //     �˳��ٽ��
     return -1;
   }
   
   ch =RS232_RXBuf[RS232_RX_TailIndex++];
   if (RS232_RX_TailIndex >=RS232_RX_BUFSIZE )
   {  RS232_RX_TailIndex =0;}

   OS_EXIT_CRITICAL();            //     �˳��ٽ��
   return ch;
}


bool RS232_PutChar(u8 *s,u16 length)
{
   u16 i;
   bool success =TRUE;
   for(i=0; i<length; i++)
   {
      if(!RS232_TX_Char(*s++))
      {
          success =FALSE;
          break;
      }
      //wdg_clr_flag = 1;       //����ι������  ��ֹ���ݹ���Ӱ��ι��
   }
   return success;
}

bool RS232_PutWord(u32 ch,u8 bit)
{
   u8  con[5];
   bool success =TRUE;
   if(bit ==8)
   {
     con[0]=ch&0xff;
     RS232_PutHex(con,1); 
   }
   else if(bit ==16)
   {
     con[0]=(ch>>8) &0x0ff;
     con[1]=ch &0x0ff;
     RS232_PutHex(con,2);   
   }
   else if(bit ==32)
   {
     con[0]=(ch>>24) &0x0ff;
     con[1]=(ch>>16) &0x0ff;
     con[2]=(ch>>8) &0x0ff;
     con[3]=ch &0x0ff;
     RS232_PutHex(con,4); 
   }
   return success;
}

bool RS232_PutHex(u8 *s,u16 length)
{
   u16 i;
   u8  ch;
   bool success =TRUE;
   for(i=0; i<length; i++)
   {
      ch =s[i] >>4;
      if( ch<=9 )
      {  RS232_TX_Char(ch+'0'); }
      else
      {  RS232_TX_Char(ch+0x37);}

      ch =s[i] &0x0f;
      if( ch<=9 )
      { RS232_TX_Char(ch+'0'); }
      else
      {  RS232_TX_Char(ch+0x37);}
      //wdg_clr_flag = 1;     //����ι������  ��ֹ���ݹ���Ӱ��ι��
   }
   return success;
}

bool RS232_PutString(char *s)
{
   bool success =TRUE;
   while (*s)
   {
      if(!RS232_TX_Char(*s++))
      {
          success =FALSE;
          break;
      }
      //wdg_clr_flag = 1;    //����ι������  ��ֹ���ݹ���Ӱ��ι��
   }
  return success;
}


u16 Process_RS232_Receive(void)
{
   s8   c;
   bool success =FALSE;
   while(RS232_RX_Count())
   {
      c =RS232_RX_Char();
         if ((c>=0x20)&&(c<0x7F))
      {  RS232_RecBuf[RS232_Receive_Index++] =c;  }

      if((RS232_Receive_Index >=(RS232_Receive_BUFSIZE-2)) ||(c ==0x0D))
      {
        RS232_RecBuf[RS232_Receive_Index] =0;
        success =TRUE;
        break;
      }
   }
   return(success);
}


/*******************************************************************************
* Function Name  : RS232_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
/*---------------interrupt sub---------------------------------*/
void USART1_IRQHandler(void)
{
  u8 ch;
  OS_CPU_SR  cpu_sr;
  OS_ENTER_CRITICAL();      //    ���� �ٽ��
  OSIntNesting++;
  OS_EXIT_CRITICAL();        //     �˳��ٽ��
      
              
  if(USART_GetITStatus(USART1, USART_IT_RXNE) !=RESET)
  {
    ch =USART_ReceiveData(USART1);
    
    if(RS232_RX_Count() <(RS232_RX_BUFSIZE -1))
    {
      RS232_RXBuf[RS232_RX_HeadIndex++] =ch;
      if(RS232_RX_HeadIndex >=RS232_RX_BUFSIZE)
      {
        RS232_RX_HeadIndex =0;
      }
    }
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
  }
  
  if(USART_GetITStatus(USART1,USART_IT_TXE) !=RESET)
  {
    if(RS232_TX_Count() >0)
    {
       USART_ClearITPendingBit(USART1, USART_IT_TXE);			 //����������жϱ�־
	   USART_SendData(USART1, RS232_TXBuf[RS232_TX_TailIndex++]);
           
       if(RS232_TX_TailIndex >=RS232_TX_BUFSIZE)
       {  RS232_TX_TailIndex =0; }
    }
    else 
    { /* Disable the USART1 Transmit interrupt */
      USART_ITConfig(USART1, USART_IT_TXE, DISABLE);
    }
  }
   OSIntExit();    
}
/*--------------------------------------------------------------*/
