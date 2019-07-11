

#include <includes.h>	

void NVIC_Configuration(void);
void RCC_Configuration(void);
void SysTick_Configuration(void);
void Timer_Configuration(void);
void GPIO_Configuration(void);
void ADC_Configuration(void);
void DMA_Configuration(void);
void EXTI_Configuration(void);
void WWdg_Configuration(void);
void Bsp_RS232_Initialise(void);


//-----------------------------------------------------------
//��������: Bsp_Configuration
//��������: ϵͳ�������ú���
//��    ��: None
//�� �� ֵ: None 
//��    ע��None
//-----------------------------------------------------------

void Bsp_Configuration(void)
{   
   	SystemInit();	

    RCC_Configuration();
    NVIC_Configuration();
    GPIO_Configuration();
    Timer_Configuration();
    ADC_Configuration();
    DMA_Configuration();
    SysTick_Configuration();
    EXTI_Configuration();     
    WWdg_Configuration(); 
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 
	
	Bsp_RS232_Initialise();
}

/***************************************************************************************
** ��������: RCC_Configuration
** ��������: ʱ�����ú���
** ��    ��: None
** �� �� ֵ: None 
****************************************************************************************/
void RCC_Configuration(void)
{

   u16 cn=0;
  ErrorStatus HSEStartUpStatus;
 
  RCC_DeInit();                                //RCC system reset(for debug purpose)
  RCC_HSEConfig(RCC_HSE_ON);                   // Enable HSE  
  HSEStartUpStatus = RCC_WaitForHSEStartUp();  // Wait till HSE is ready 

  if(HSEStartUpStatus == SUCCESS)
  {
    
    RCC_HCLKConfig(RCC_SYSCLK_Div1);     // HCLK = SYSCLK  
    RCC_PCLK2Config(RCC_HCLK_Div1);      // PCLK2 = HCLK 
    RCC_PCLK1Config(RCC_HCLK_Div2);      // PCLK1 = HCLK/2 
  
    FLASH_SetLatency(FLASH_Latency_2);   // Flash 2 wait state    
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable); // Enable Prefetch Buffer 
   
    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); 
    RCC_PLLCmd(ENABLE);                  // Enable PLL  
    for(cn=0;cn<20;cn++)
    {
      if(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
      { break;}
    }
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);           // Select PLL as system clock source 
    for(cn=0;cn<20;cn++)
    {
      if(RCC_GetSYSCLKSource() == 0x08)break;
    }
  }

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                         RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);

  //RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3 |RCC_APB1Periph_UART4, ENABLE); 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
 
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG , ENABLE);       // Enable WDG clocks 

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);


}



/***************************************************************************************
** ��������: SysTick_Config
** ��������: ϵͳʱ�����ú���  ��OSϵͳ����ʱ��    10ms�ж�
** ��    ��: None
** �� �� ֵ: None 
** ��  ����: 2008��11��27��
****************************************************************************************/
void SysTick_Configuration(void)
{
     RCC_ClocksTypeDef  rcc_clocks;
    INT32U      cnts;
    
    RCC_GetClocksFreq(&rcc_clocks);                              //�õ�RCCʱ��    
    cnts = (INT32U)rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC; //���ü���ֵ   72M/100    
    
    SysTick_Config(cnts);
    //SysTick_SetReload(cnts);                                     //��cnts���ó�Ϊ��װֵ
    //SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);             //����ϵͳʱ��ԴΪHCLK
    //SysTick_CounterCmd(SysTick_Counter_Enable);                  //����ʹ��
                                           
    //SysTick_ITConfig(ENABLE);                                    //ʹ���ж�    
}

/***************************************************************************************
** ��������: GPIO_Configuration
** ��������: I/O���ú���
** ��    ��: None
** �� �� ֵ: None 
** ��  ����: 2008��11��27��
****************************************************************************************/
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_AFIODeInit();
   
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE); 						 
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_13);
 
}

/***************************************************************************************
** ��������: GPIO_Configuration
** ��������: �ж����ú���
** ��    ��: None
** �� �� ֵ: None 
** ��  ����: 2008��11��27��
****************************************************************************************/
void NVIC_Configuration(void)
{
   NVIC_InitTypeDef   NVIC_InitStructure;
	
	#ifdef  VECT_TAB_RAM  
  /* Set the Vector Table base location at 0x20000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif 
	
NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
  NVIC_Init(&NVIC_InitStructure);
}

/***************************************************************************************
** ��������: Timer_Configuration
** ��������: �ж����ú���
** ��    ��: None
** �� �� ֵ: None 
** ��  ����: 2008��11��27��
****************************************************************************************/
void Timer_Configuration(void)
{
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStruct;

   TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
   TIM_TimeBaseInitStruct.TIM_Period =6000;           //TIM update rate =TIM counter clock/TIM_Period =6MHz/6000=1000Hz   
   TIM_TimeBaseInitStruct.TIM_Prescaler =11;           //TIM counter clock =TIMxCLK/(Prescaler +1) =72MHz/12=6MHz       
   TIM_TimeBaseInitStruct.TIM_ClockDivision = 0;       //ʱ�ӱ�Ƶ  
   TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;  //�����������ϼ���
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);       
   
   //TIM_Cmd(TIM2, ENABLE); 
                                          
   TIM_ClearFlag(TIM2, TIM_FLAG_Update);
   TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);           // Enable TIM2 Update interrupt [TIM2����ж�����]
   TIM_Cmd(TIM2, ENABLE);                               // TIM2 enable counter [����tim2����]

}


void ADC_Configuration(void)
{
   
} 


void DMA_Configuration(void)
{
   
}

 

 /***************************************************************************************
** ��������: EXTI_Configuration
** ��������: �ⲿ�����ж����ú���
** ��    ��: None
** �� �� ֵ: None 
** ��  ����: 2008��11��27��
****************************************************************************************/
 void EXTI_Configuration(void )
{
 
  
} 
/***************************************************************************************
** ��������: Wdg_Init
** ��������: ���Ź���ʼ������
** ��    ��: None
** �� �� ֵ: None   
****************************************************************************************/
void WWdg_Configuration(void)
{

}
//*******************************************************************************
// ��������: RCC_GetFlagStatus
// ��������: ���ָ����RCC��־�������.
// �������: RCC_FLAG:Ҫ���ı�־.
//                    ������������������ֵ֮һ:
//                       - RCC_FLAG_HSIRDY: HIS��ʱ�Ӿ���
//                       - RCC_FLAG_HSERDY: HSE��ʱ�Ӿ���
//                       - RCC_FLAG_PLLRDY: PLLʱ�Ӿ���
//                       - RCC_FLAG_LSERDY: LSE��ʱ�Ӿ���
//                       - RCC_FLAG_LSIRDY: LSI��ʱ�Ӿ���
//                       - RCC_FLAG_PINRST: ���Ÿ�λ
//                       - RCC_FLAG_PORRST: POR/PDR��λ
//                       - RCC_FLAG_SFTRST: �����λ
//                       - RCC_FLAG_IWDGRST: �������Ź���λ
//                       - RCC_FLAG_WWDGRST: ���ڿ��Ź���λ
//                      - RCC_FLAG_LPWRRST: �͵�����λ
// �������: ��
// ���ز���: RCC_FLAG����״̬(SET��RESET).
//*******************************************************************************
//FlagStatus RCC_GetFlagStatus(u8 RCC_FLAG)
//*******************************************************************************
//* ��������: RCC_ClearFlag
//* ��������: ���RCC��λ��־.
//*           ��λ��־Ϊ: RCC_FLAG_PINRST, RCC_FLAG_PORRST,
//*                  RCC_FLAG_SFTRST, RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST,
//*                  RCC_FLAG_LPWRRST
//* �������: ��
//* �������: ��
//* ���ز���: ��
//*******************************************************************************/
//��ȡ��λֵ
void Fetch_Reset_Cause(void)
{

}
void Bsp_RS232_Initialise(void)
{
	 RS232_Initialise();
}

