#include "STM32_I2C.h"

#define CMD_Write     0		 //mpu6050 д����λ���λ��Ч
#define CMD_Read      1		 //mpu6050 ������λ���λ��Ч
/*****
***	 	IIc���ţ�SDA PE0 , SCL PE1 
***
*******/
#define SCL_H         GPIOE->BSRR = GPIO_Pin_1 /* GPIO_SetBits(GPIOB , GPIO_Pin_10)   */
#define SCL_L         GPIOE->BRR  = GPIO_Pin_1 /* GPIO_ResetBits(GPIOB , GPIO_Pin_10) */

#define SDA_H         GPIOE->BSRR = GPIO_Pin_0 /* GPIO_SetBits(GPIOB , GPIO_Pin_11)   */
#define SDA_L         GPIOE->BRR  = GPIO_Pin_0 /* GPIO_ResetBits(GPIOB , GPIO_Pin_11) */

#define SCL_read      GPIOE->IDR  & GPIO_Pin_1 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_10) */
#define SDA_read      GPIOE->IDR  & GPIO_Pin_0 /* GPIO_ReadInputDataBit(GPIOB , GPIO_Pin_11) */

void i2cInit(void)
{
    GPIO_InitTypeDef gpio;

    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_Init(GPIOE, &gpio);
}

/*****************************************************************************************************/
static void I2C_delay(void)
{
    volatile int i =10 ;	  //zz 7->10
    while (i)
        i--;
}

static bool I2C_Start(void)
{
    SDA_H;
    SCL_H;
    I2C_delay();
    if (!SDA_read)
        return FALSE;
    SDA_L;
    I2C_delay();
    if (SDA_read)
        return FALSE;
    SDA_L;
    I2C_delay();
    return TRUE;
}

static void I2C_Stop(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SDA_H;
    I2C_delay();
}

static void I2C_Ack(void)
{
    SCL_L;
    I2C_delay();
    SDA_L;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}

static void I2C_NoAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    SCL_L;
    I2C_delay();
}

static bool I2C_WaitAck(void)
{
    SCL_L;
    I2C_delay();
    SDA_H;
    I2C_delay();
    SCL_H;
    I2C_delay();
    if (SDA_read) {
        SCL_L;
        return FALSE;
    }
    SCL_L;
    return TRUE;
}

static void I2C_SendByte(uint8_t byte)
{
    uint8_t i = 8;
    while (i--) {
        SCL_L;
        I2C_delay();
        if (byte & 0x80)
            SDA_H;
        else
            SDA_L;
        byte <<= 1;
        I2C_delay();
        SCL_H;
        I2C_delay();
    }
    SCL_L;
}

static uint8_t I2C_ReceiveByte(void)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    SDA_H;
    while (i--) {
        byte <<= 1;
        SCL_L;
        I2C_delay();
        SCL_H;
        I2C_delay();
        if (SDA_read) {
            byte |= 0x01;
        }
    }
    SCL_L;
    return byte;
}

/********************************************************************************
** �������� bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
** �������ܣ�д�����ݿ�
** ������
      uint8_t addr,       ģ���ַ
	  uint8_t reg, 		  �Ĵ�����ַ
	  uint8_t len, 		  ���ݳ���
	  uint8_t * data	  ����
**����ֵ true �ɹ�  false ʧ��
*********************************************************************************/
bool i2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data)
{
    int i;
    if (!I2C_Start())
        return FALSE;
    I2C_SendByte(addr << 1 | CMD_Write);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    for (i = 0; i < len; i++) {
        I2C_SendByte(data[i]);
        if (!I2C_WaitAck()) {
            I2C_Stop();
            return FALSE;
        }
    }
    I2C_Stop();
    return TRUE;
}
/********************************************************************************
** �������� bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
** �������ܣ�д��һ���ֽڵ�ָ���Ĵ���
** ������
      uint8_t addr,       ģ���ַ
	  uint8_t reg, 		  �Ĵ�����ַ
	  uint8_t data  	  ����
**����ֵ true �ɹ�  false ʧ��
*********************************************************************************/
bool i2cWrite(uint8_t addr, uint8_t reg, uint8_t data)
{
    if (!I2C_Start())
        return FALSE;
    I2C_SendByte(addr << 1 | CMD_Write);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_SendByte(data);
    I2C_WaitAck();
    I2C_Stop();
    return TRUE;
}
/********************************************************************************
** �������� bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
** �������ܣ������ݴ�ָ���Ĵ���
** ������
      uint8_t addr,       ģ���ַ
	  uint8_t reg, 		  �Ĵ�����ַ
	  uint8_t len, 	      ��ȡ����
	  uint8_t *buf		  ��ȡ���Ŀռ�
**����ֵ true �ɹ�  false ʧ��
*********************************************************************************/
bool i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!I2C_Start())
        return FALSE;
    I2C_SendByte(addr << 1 | CMD_Write);
    if (!I2C_WaitAck()) {
        I2C_Stop();
        return FALSE;
    }
    I2C_SendByte(reg);
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(addr << 1 | CMD_Read);
    I2C_WaitAck();
    while (len) {
        *buf = I2C_ReceiveByte();
        if (len == 1)
            I2C_NoAck();
        else
            I2C_Ack();
        buf++;
        len--;
    }
    I2C_Stop();
    return TRUE;
}

uint16_t i2cGetErrorCounter(void)
{
    // TODO maybe fix this, but since this is test code, doesn't matter.
    return 0;
}
