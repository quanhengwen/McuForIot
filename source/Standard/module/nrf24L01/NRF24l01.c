

#include "includes.h"
#include "NRF24l01.h"
#include "SPI1.h"
/*****************************************************************************
      NRF24L01�Ĵ���ָ��
*****************************************************************************/
#define NRF_READ_REG        0x00  	// ���Ĵ���ָ��
#define NRF_WRITE_REG       0x20 	// д�Ĵ���ָ��
#define RD_RX_PLOAD     0x61  	// ��ȡ��������ָ��
#define WR_TX_PLOAD     0xA0  	// д��������ָ��
#define FLUSH_TX        0xE1 	// ��ϴ���� FIFOָ��
#define FLUSH_RX        0xE2  	// ��ϴ���� FIFOָ��
#define REUSE_TX_PL     0xE3  	// �����ظ�װ������ָ��
#define NOP             0xFF  	// ����
/*****************************************************************************
      SPI(nRF24L01)�Ĵ�����ַ
******************************************************************************/
#define CONFIG          0x00  // �����շ�״̬��CRCУ��ģʽ�Լ��շ�״̬��Ӧ��ʽ
#define EN_AA           0x01  // �Զ�Ӧ��������
#define EN_RXADDR       0x02  // �����ŵ�����
#define SETUP_AW        0x03  // �շ���ַ�������
#define SETUP_RETR      0x04  // �Զ��ط���������
#define RF_CH           0x05  // ����Ƶ������
#define RF_SETUP        0x06  // �������ʡ����Ĺ�������
#define NRFRegSTATUS    0x07  // ״̬�Ĵ���
#define OBSERVE_TX      0x08  // ���ͼ�⹦��
#define CD              0x09  // ��ַ���           
#define RX_ADDR_P0      0x0A  // Ƶ��0�������ݵ�ַ
#define RX_ADDR_P1      0x0B  // Ƶ��1�������ݵ�ַ
#define RX_ADDR_P2      0x0C  // Ƶ��2�������ݵ�ַ
#define RX_ADDR_P3      0x0D  // Ƶ��3�������ݵ�ַ
#define RX_ADDR_P4      0x0E  // Ƶ��4�������ݵ�ַ
#define RX_ADDR_P5      0x0F  // Ƶ��5�������ݵ�ַ
#define TX_ADDR         0x10  // ���͵�ַ�Ĵ���
#define RX_PW_P0        0x11  // ����Ƶ��0�������ݳ���
#define RX_PW_P1        0x12  // ����Ƶ��1�������ݳ���
#define RX_PW_P2        0x13  // ����Ƶ��2�������ݳ���
#define RX_PW_P3        0x14  // ����Ƶ��3�������ݳ���
#define RX_PW_P4        0x15  // ����Ƶ��4�������ݳ���
#define RX_PW_P5        0x16  // ����Ƶ��5�������ݳ���
#define FIFO_STATUS     0x17  // FIFOջ��ջ��״̬�Ĵ�������
/****************************************************************************************/
u8 TxBuf[32]={0};

/******************************************************************************************/
u8   sta;   //״̬��־
#define	RX_DR	sta & 0x40
#define	TX_DS	sta & 0x20
#define	MAX_RT	sta & 0x10
/****************************************IO�˿ڶ���***************************************/
/*	CE  	B10
**  CSN 	B11
**	SCK 	B5
**	MISO  	B6
**  MISI  	B7
**	IRQ     B0
*/
//Chip Enable Activates RX or TX mode
#define CE_H()   GPIO_SetBits(GPIOB, GPIO_Pin_10) 
#define CE_L()   GPIO_ResetBits(GPIOB, GPIO_Pin_10)

//SPI Chip Select
#define CSN_H()  GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define CSN_L()  GPIO_ResetBits(GPIOB, GPIO_Pin_11)
/*
#define CE_H()   ( GPIOB->ODR |= (1<<10) )
#define CE_L()   ( GPIOB->ODR &= ~(1<<10) )
#define CSN_H()  ( GPIOB->ODR |= (1<<11) )
#define CSN_L()  ( GPIOB->ODR &= ~(1<<11) )	*/

/*********************************************NRF24L01*************************************/
#define TX_ADR_WIDTH    5   	// 5 uints TX address width
#define RX_ADR_WIDTH    5   	// 5 uints RX address width

#define RX_PLOAD_WIDTH  32  	// 32  uints TX payload	
#define TX_PLOAD_WIDTH  32  	// 32 uints TX payload

u8 NRF24SPI_Send_Byte(u8 dat)
{
   return SPI1_Send_Byte(dat);
}

u8  TX_ADDRESS[TX_ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};	//���ص�ַ
u8  RX_ADDRESS[RX_ADR_WIDTH]= {0xE7,0xE7,0xE7,0xE7,0xE7};	//���յ�ַ

u8 	SPI_RD_Reg(u8 reg);
u8 	SPI_WR_Reg(u8 reg, u8 value);
u8 	SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len);
u8 	SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len);

u8 	nRF24L01_RxPacket(unsigned char* rx_buf);
void nRF24L01_TxPacket(unsigned char * tx_buf);


/******************************************************************************************
*��ʱ����,�Ǿ�ȷ��ʱ
******************************************************************************************/
#define Delay_us OSTimeDly(1)

/****************************************************************************************
**
**                NRF24L01��ʼ��
***************************************************************************************/
void NRF24L01_int(void)
{
 	GPIO_InitTypeDef   GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure; 
	u8 buf[5]={0};

	Delay_us;
	SPI1_Init();
  
  	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10|GPIO_Pin_11;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	EXTI_ClearITPendingBit(EXTI_Line0);   //����жϱ�־   
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);      
	EXTI_InitStructure.EXTI_Line = EXTI_Line0; //ѡ���ж���·0
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //����Ϊ�ж����󣬷��¼�����    
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //�����жϴ�����ʽΪ���� ���ش���   
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;            //�ⲿ�ж�ʹ��    
	EXTI_Init(&EXTI_InitStructure); 

  	CE_L();    // chip enable
  	CSN_H();   // Spi disable 
  
  	SPI_Read_Buf(TX_ADDR, buf, TX_ADR_WIDTH);	//debug ����ԭ���ı��ص�ַ����λֵ�ǣ�0xE7 0xE7 0xE7 0xE7 0xE7
  
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	
	SPI_WR_Reg(NRF_WRITE_REG + EN_AA, 0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_WR_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_WR_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); // �����Զ��ط�ʱ��ʹ�����500us + 86us, 10 retrans...
	SPI_WR_Reg(NRF_WRITE_REG + RF_CH, 22);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_WR_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_WR_Reg(NRF_WRITE_REG + RF_SETUP, 0x07);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB

}
/***************************************************************************************************
**
**			  ��һ���֣��Ĵ������ݲ�������
u8 SPI_RD_Reg(u8 reg)
u8 SPI_WR_Reg(u8 reg, u8 value)
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len)
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
****************************************************************************************************/
//���Ĵ���
u8 SPI_RD_Reg(u8 reg)
{
	u8 reg_val;
	
	CSN_L();                			// CSN low, initialize SPI communication...
	NRF24SPI_Send_Byte(reg);            // Select register to read from..
	reg_val = NRF24SPI_Send_Byte(0);    // ..then read registervalue
	CSN_H();                			// CSN high, terminate SPI communication
	
	return(reg_val);       	 			// return register value
}

//NRF24L01д�Ĵ�������
u8 SPI_WR_Reg(u8 reg, u8 value)
{
	u8 status;
	
	CSN_L();                   			// CSN low, init SPI transaction
	status = NRF24SPI_Send_Byte(reg);	// select register
	NRF24SPI_Send_Byte(value);    		// ..and write value to it..
	CSN_H();                   			// CSN high again
	
	return(status);            			// return nRF24L01 status u8
}
//���ڶ�����
u8 SPI_Read_Buf(u8 reg, u8 *pBuf, u8 Len)
{
	u8 status,i;
	
	CSN_L();                    				// Set CSN low, init SPI tranaction
	status = NRF24SPI_Send_Byte(reg);      		// Select register to write to and read status u8
	
  	for(i=0;i<Len;i++)
  	{
     	pBuf[i] = NRF24SPI_Send_Byte(0);
  	}
	
	CSN_H();                           
	
	return(status);                    			// return nRF24L01 status u8
}
//����д����
u8 SPI_Write_Buf(u8 reg, u8 *pBuf, u8 Len)
{
	u8 status,i;
	
	CSN_L();            //SPIʹ��       
	status = NRF24SPI_Send_Byte(reg);   
	for(i=0; i<Len; i++) //
	{
	   NRF24SPI_Send_Byte(*pBuf);
		 pBuf ++;
	}
	CSN_H();           //�ر�SPI
	return(status);    // 
}
/***************************************************************************************************
**
**           �ڶ����� ����ʵ�ֺ���
void RX_Mode(void)
void TX_Mode(void)
****************************************************************************************************/
//���ݽ���ģʽ
void RX_Mode(void)
{u8 buf[5]={0};
	CE_L();
	
	SPI_Read_Buf(TX_ADDR, buf, TX_ADR_WIDTH);//debug ����ԭ���ı��ص�ַ����λֵ�ǣ�0xE7 0xE7 0xE7 0xE7 0xE7
	//SPI_WR_Reg(WRITE_REG + CONFIG, 0x03);//SPI_WR_Reg(WRITE_REG + CONFIG, 0x0f); // IRQ�շ�����ж���Ӧ��16λCRC	��������
	
	//SPI_Write_Buf(WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	
	SPI_WR_Reg(NRF_WRITE_REG + EN_AA, 0);//0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_WR_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_WR_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); // �����Զ��ط�ʱ��ʹ�����500us + 86us, 10 retrans...
	SPI_WR_Reg(NRF_WRITE_REG + RF_CH, 40);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
	SPI_WR_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_WR_Reg(NRF_WRITE_REG + RF_SETUP, 0x0F);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB
	
	SPI_WR_Reg(NRF_WRITE_REG + CONFIG, 0x03);//0x0F);	
	CE_H(); 
	Delay_us;
}

void TX_Mode(void)
{
 	CE_L();
	//SPI_WR_Reg(WRITE_REG + CONFIG, 0x02);   //0x0E		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	
	SPI_Write_Buf(NRF_WRITE_REG + TX_ADDR, TX_ADDRESS, TX_ADR_WIDTH);    // д���ص�ַ	
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, RX_ADDRESS, RX_ADR_WIDTH); // д���ն˵�ַ
	
	SPI_WR_Reg(NRF_WRITE_REG + EN_AA, 0);//0x01);      //  Ƶ��0�Զ�	ACKӦ������	
	SPI_WR_Reg(NRF_WRITE_REG + EN_RXADDR, 0x01);  //  ������յ�ַֻ��Ƶ��0�������Ҫ��Ƶ�����Բο�Page21  
	SPI_WR_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a); // �����Զ��ط�ʱ��ʹ�����500us + 86us, 10 retrans...
	SPI_WR_Reg(NRF_WRITE_REG + RF_CH, 40);        //   �����ŵ�����Ϊ2.4GHZ���շ�����һ��
//	SPI_WR_Reg(NRF_WRITE_REG + RX_PW_P0, RX_PLOAD_WIDTH); //���ý������ݳ��ȣ���������Ϊ32�ֽ�
	SPI_WR_Reg(NRF_WRITE_REG + RF_SETUP, 0x0F);   		//���÷�������Ϊ1MHZ�����书��Ϊ���ֵ0dB

	SPI_WR_Reg(NRF_WRITE_REG + CONFIG, 0x02);//0x0E);
 	CE_H();
}
//***********************************************************************************************************
//*������void nRF24L01_TxPacket(unsigned char * tx_buf)
//*���ܣ����� tx_buf������
//**********************************************************************************************************/
void nRF24L01_TxPacket(unsigned char * tx_buf)
{
	CE_L();			//StandBy Iģʽ	
	SPI_Write_Buf(NRF_WRITE_REG + RX_ADDR_P0, TX_ADDRESS, TX_ADR_WIDTH); // װ�ؽ��ն˵�ַ
	SPI_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH); 			 // װ������	
//	SPI_WR_Reg(WRITE_REG + CONFIG, 0x0e);   		 // IRQ�շ�����ж���Ӧ��16λCRC��������
	CE_H();		 //�ø�CE���������ݷ���
	//Delay_us(10);
}

//******************************************************************************************************/
//*������unsigned char nRF24L01_RxPacket(unsigned char* rx_buf)
//*���ܣ����ݶ�ȡ�����rx_buf���ջ�������
//******************************************************************************************************/
u8 nRF24L01_RxPacket(unsigned char* rx_buf)
{
	u8 flag=0;
 	u8 status;
 	
	status=SPI_RD_Reg(NRFRegSTATUS);	// ��ȡ״̬�Ĵ������ж����ݽ���״��
	
	if(status & 0x40)				// �ж��Ƿ���յ�����
	{
	   //CE_L(); 			//SPIʹ��
		 SPI_Read_Buf(RD_RX_PLOAD,rx_buf,TX_PLOAD_WIDTH);// read receive payload from RX_FIFO buffer
		 flag =1;			//��ȡ������ɱ�־
	}
	SPI_WR_Reg(NRF_WRITE_REG+NRFRegSTATUS, status);   //���յ����ݺ�RX_DR,TX_DS,MAX_RT���ø�Ϊ1��ͨ��д1������жϱ�־
	return flag;
}
///************************************������************************************************************
void NRF_Send(void)
{	
	//static u32 		counter=0;
 	static u8 		flag=0;  
   
   //TX_Mode();
   
   //IntervalTimems(100);
   if(flag==0)
   {
      flag=1;
      //memcpy(TxBuf, "1234567890abcdefghij", 20);
      nRF24L01_TxPacket(TxBuf);
   }
   else
   {
      flag=0;
      //memcpy(TxBuf, "abcdefghij1234567890", 20);
      nRF24L01_TxPacket(TxBuf);
   }
   //GPIO_Write(GPIOC, (u16)~GPIO_ReadOutputData(GPIOC));
}

