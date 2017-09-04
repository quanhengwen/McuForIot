/*****************************************************************************
**	�ļ����ƣ� MPU6050.c
**	�������ߣ� zzhere2007
**	�޸�ʱ�䣺 20130820
**	�����ܣ� MPU6050����
**	�ܹ����ţ�  SDA PE0 , SCL PE1 , vcc-3.3 ,GND
*******************************************************************************/ 
#include "MPU6050.h"

//#include "LED.h"

typedef struct int16_xyz{
				int16_t X;
				int16_t Y;
				int16_t Z;}S_INT16_XYZ;

uint8_t 		mpu6050_buffer[14];					
S_INT16_XYZ 	GYRO_OFFSET,ACC_OFFSET;			
uint8_t 		GYRO_OFFSET_OK = 1;
uint8_t 		ACC_OFFSET_OK = 1;
S_INT16_XYZ 	MPU6050_ACC_LAST,MPU6050_GYRO_LAST;	
int16_t 		MPU6050_tem;
/**************************ʵ�ֺ���********************************************
//��iic��ȡ�������ݷֲ�,������Ӧ�Ĵ���
*******************************************************************************/
void MPU6050_Dataanl(void)	   //ȡֵ-��׼
{
	//���ٶ�
	MPU6050_ACC_LAST.X=((((int16_t)mpu6050_buffer[0]) << 8) | mpu6050_buffer[1]) - ACC_OFFSET.X;
	MPU6050_ACC_LAST.Y=((((int16_t)mpu6050_buffer[2]) << 8) | mpu6050_buffer[3]) - ACC_OFFSET.Y;
	MPU6050_ACC_LAST.Z=((((int16_t)mpu6050_buffer[4]) << 8) | mpu6050_buffer[5]) - ACC_OFFSET.Z;
	//�¶�ADC
	MPU6050_tem		  =(int16_t)((((int16_t)mpu6050_buffer[6]) << 8) | mpu6050_buffer[7])/340 +36;
   	//������
	MPU6050_GYRO_LAST.X=((((int16_t)mpu6050_buffer[8]) << 8) | mpu6050_buffer[9]) - GYRO_OFFSET.X;
	MPU6050_GYRO_LAST.Y=((((int16_t)mpu6050_buffer[10]) << 8) | mpu6050_buffer[11]) - GYRO_OFFSET.Y;
	MPU6050_GYRO_LAST.Z=((((int16_t)mpu6050_buffer[12]) << 8) | mpu6050_buffer[13]) - GYRO_OFFSET.Z;
	//MPU6050_tem =(MPU6050_tem-32)/1.8;
	RS232_PutString("\r\n-----MPU6050--ACC---------------");
	RS232_PutString("\r\nMPU6050_ACC_LAST.X:");
	RS232_PutWord(MPU6050_ACC_LAST.X,16);
	RS232_PutString("\r\nMPU6050_ACC_LAST.Y:");
	RS232_PutWord(MPU6050_ACC_LAST.Y,16);
	RS232_PutString("\r\nMPU6050_ACC_LAST.Z:");
	RS232_PutWord(MPU6050_ACC_LAST.Z,16);
	RS232_PutString("\r\n-----MPU6050--tem---------------");
	RS232_PutString("\r\nMPU6050_tem:");
	RS232_PutWord(MPU6050_tem,16);
	RS232_PutString("\r\n-----MPU6050--GYRO---------------");
	RS232_PutString("\r\nMPU6050_GYRO_LAST.X:");
	RS232_PutWord(MPU6050_GYRO_LAST.X,16);
	RS232_PutString("\r\nMPU6050_GYRO_LAST.Y:");
	RS232_PutWord(MPU6050_GYRO_LAST.Y,16);
	RS232_PutString("\r\nMPU6050_GYRO_LAST.Z:");
	RS232_PutWord(MPU6050_GYRO_LAST.Z,16);

	/**************************������һ��200�ε��˲�****************************************/
	if(!GYRO_OFFSET_OK)
	{
		static int32_t	tempgx=0,tempgy=0,tempgz=0;
		static uint8_t cnt_g=0;

		if(cnt_g==0)
		{
			GYRO_OFFSET.X=0;
			GYRO_OFFSET.Y=0;
			GYRO_OFFSET.Z=0;
			tempgx = 0;
			tempgy = 0;
			tempgz = 0;
			cnt_g = 1;
			return;
		}
		tempgx+= MPU6050_GYRO_LAST.X;
		tempgy+= MPU6050_GYRO_LAST.Y;
		tempgz+= MPU6050_GYRO_LAST.Z;
		if(cnt_g==200)
		{
			GYRO_OFFSET.X=tempgx/cnt_g;
			GYRO_OFFSET.Y=tempgy/cnt_g;
			GYRO_OFFSET.Z=tempgz/cnt_g;
			cnt_g = 0;
			GYRO_OFFSET_OK = 1;
			return;
		}
		cnt_g++;
	}
	if(!ACC_OFFSET_OK)
	{
		static int32_t	tempax=0,tempay=0,tempaz=0;
		static uint8_t cnt_a=0;

		if(cnt_a==0)
		{
			ACC_OFFSET.X = 0;
			ACC_OFFSET.Y = 0;
			ACC_OFFSET.Z = 0;
			tempax = 0;
			tempay = 0;
			tempaz = 0;
			cnt_a = 1;
			return;
		}
		tempax+= MPU6050_ACC_LAST.X;
		tempay+= MPU6050_ACC_LAST.Y;
		tempaz+= MPU6050_ACC_LAST.Z;
		if(cnt_a==200)
		{
			ACC_OFFSET.X=tempax/cnt_a;
			ACC_OFFSET.Y=tempay/cnt_a;
			ACC_OFFSET.Z=tempaz/cnt_a;
			cnt_a = 0;
			ACC_OFFSET_OK = 1;
			return;
		}
		cnt_a++;		
	}
}
/**************************�����ݺ���********************************************
//��iic��ȡ�������ݷֲ�,������Ӧ�Ĵ���,����MPU6050_Last
*******************************************************************************/
void MPU6050_READ(void)
{
	i2cRead(devAddr,MPU6050_RA_ACCEL_XOUT_H,14,mpu6050_buffer);
}
/******************************************************************************
*����ԭ��:		u8 IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �е�1��λ
 ����	dev     Ŀ���豸��ַ
        reg	    �Ĵ�����ַ
        bitNum  Ҫ�޸�Ŀ���ֽڵ�bitNumλ
        data    Ϊ0 ʱ��Ŀ��λ������0 ���򽫱���λ
����   �ɹ� Ϊ1  ʧ��Ϊ0
*******************************************************************************/ 
void IICwriteBit(u8 dev, u8 reg, u8 bitNum, u8 data){
	u8 b;
	i2cRead(dev, reg, 1, &b);
	b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
	i2cWrite(dev, reg, b);
}
/******************************************************************************
*����ԭ��:		u8 IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
*��������:	    �� �޸� д ָ���豸 ָ���Ĵ���һ���ֽ� �еĶ��λ
����	dev  Ŀ���豸��ַ
reg	   �Ĵ�����ַ
bitStart  Ŀ���ֽڵ���ʼλ
length   λ����
data    ��Ÿı�Ŀ���ֽ�λ��ֵ
����   �ɹ� Ϊ1 
ʧ��Ϊ0
*******************************************************************************/ 
void IICwriteBits(u8 dev,u8 reg,u8 bitStart,u8 length,u8 data)
{
	
	u8 b;
	u8 mask;

	i2cRead(dev, reg, 1, &b);
	mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
	data <<= (8 - length);
	data >>= (7 - bitStart);
	b &= mask;
	b |= data;
	i2cWrite(dev, reg, b);
}
/**************************���ú���*************************************************************************************
*����ԭ��:		void MPU6050_setClockSource(uint8_t source)
*��������:	    ����  MPU6050 ��ʱ��Դ
* CLK_SEL | Clock Source
* --------+--------------------------------------
* 0       | Internal oscillator
* 1       | PLL with X Gyro reference
* 2       | PLL with Y Gyro reference
* 3       | PLL with Z Gyro reference
* 4       | PLL with external 32.768kHz reference
* 5       | PLL with external 19.2MHz reference
* 6       | Reserved
* 7       | Stops the clock and keeps the timing generator in reset
*******************************************************************************/
void MPU6050_setClockSource(uint8_t source){
	IICwriteBits(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_CLKSEL_BIT, MPU6050_PWR1_CLKSEL_LENGTH, source);
	
}
/** Set full-scale gyroscope range.	  //�������������
* @param range New full-scale gyroscope range value
* @see getFullScaleRange()
* @see MPU6050_GYRO_FS_250
* @see MPU6050_RA_GYRO_CONFIG
* @see MPU6050_GCONFIG_FS_SEL_BIT
* @see MPU6050_GCONFIG_FS_SEL_LENGTH
*/
void MPU6050_setFullScaleGyroRange(uint8_t range) {
	IICwriteBits(devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

/**************************���ú���********************************************
*����ԭ��:		void MPU6050_setFullScaleAccelRange(uint8_t range)
*��������:	    ����  MPU6050 ���ٶȼƵ��������
*******************************************************************************/
void MPU6050_setFullScaleAccelRange(uint8_t range) {
	IICwriteBits(devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}
/**************************���ú���********************************************
*����ԭ��:		void MPU6050_setSleepEnabled(uint8_t enabled)
*��������:	    ����  MPU6050 �Ƿ����˯��ģʽ
enabled =1   ˯��
enabled =0   ����
*******************************************************************************/
void MPU6050_setSleepEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

/**************************���ú���********************************************
*����ԭ��:		void MPU6050_setI2CMasterModeEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CMasterModeEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_USER_CTRL, MPU6050_USERCTRL_I2C_MST_EN_BIT, enabled);
}

/**************************���ú���********************************************
*����ԭ��:		void MPU6050_setI2CBypassEnabled(uint8_t enabled)
*��������:	    ���� MPU6050 �Ƿ�ΪAUX I2C�ߵ�����
*******************************************************************************/
void MPU6050_setI2CBypassEnabled(uint8_t enabled) {
	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_I2C_BYPASS_EN_BIT, enabled);
}

void MPU6050_setDLPF(uint8_t mode)
{
	IICwriteBits(devAddr, MPU6050_RA_CONFIG, MPU6050_CFG_DLPF_CFG_BIT, MPU6050_CFG_DLPF_CFG_LENGTH, mode);
}
/**************************��ʼ������********************************************
*����ԭ��:		void MPU6050_initialize(void)
*��������:	    ��ʼ�� 	MPU6050 �Խ������״̬��
*******************************************************************************/
void MPU6050_INIT(void)
{
//		I2C2_Write_1Byte(0xd0,0x19,0x07);//1khz
//		I2C2_Write_1Byte(0xd0,0x1a,0x03);//44hz�˲�
//		I2C2_Write_1Byte(0xd0,0x1b,0x00);//gyro 250du/s
//		I2C2_Write_1Byte(0xd0,0x1c,0x08);//acc +-4g
//		I2C2_Write_1Byte(0xd0,0x24,13);//400khz
//		I2C2_Write_1Byte(0xd0,0x6a,0x00);//mst_en=0
//		I2C2_Write_1Byte(0xd0,0x6b,0x00);//sleep=0
	
	MPU6050_setClockSource(MPU6050_CLOCK_PLL_XGYRO); //����ʱ��  0x6b   0x01
	OSTimeDly(2);
	MPU6050_setFullScaleGyroRange(MPU6050_GYRO_FS_500);//������������� +-500��ÿ��
	OSTimeDly(2);
	MPU6050_setFullScaleAccelRange(MPU6050_ACCEL_FS_4);	//���ٶȶ�������� +-4G
	OSTimeDly(2);
	MPU6050_setDLPF(MPU6050_DLPF_BW_42);
	OSTimeDly(2);
	MPU6050_setSleepEnabled(0); //���빤��״̬
	OSTimeDly(2);
	MPU6050_setI2CMasterModeEnabled(0);	 //����MPU6050 ����AUXI2C
	OSTimeDly(2);
	MPU6050_setI2CBypassEnabled(1);	 //����������I2C��	MPU6050��AUXI2C	ֱͨ������������ֱ�ӷ���HMC5883L
	OSTimeDly(2);
	
	//	//����MPU6050 ���ж�ģʽ ���жϵ�ƽģʽ
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_LEVEL_BIT, 0);
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_OPEN_BIT, 0);
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_LATCH_INT_EN_BIT, 1);
	//	IICwriteBit(devAddr, MPU6050_RA_INT_PIN_CFG, MPU6050_INTCFG_INT_RD_CLEAR_BIT, 1);
	//	//������ת������ж�
	//    IICwriteBit(devAddr, MPU6050_RA_INT_ENABLE, MPU6050_INTERRUPT_DATA_RDY_BIT, 1);
}
/***********************************************************************************************
**
**			   test������ͨ��
***********************************************************************************************/
void MPU6050_test(void)
{	
	i2cInit();
	MPU6050_INIT();
	while(1)
	{  
	   	MPU6050_READ();
	   	MPU6050_Dataanl();
	   	
	   	printf("\r\test����loop");
	   	OSTimeDly(OS_TICKS_PER_SEC);
	}	
}

