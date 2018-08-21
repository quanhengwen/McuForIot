/****************************************Copyright (c)**************************************************
**                               Shijiazhuang TIANYUAN technology Co.,LTD.
**                                     
** -----------------------------------------------------------------------------------------------------
** File Name    : PARA_HW.c	
** Version      : V1.0
** Description  : ��Ӳ����׼SPI�ӿڲ���CH374��4��SPI���ڶ�дʱ�� 

** -----------------------------------------------------------------------------------------------------
** Author       : WuPengbo  
** Date         : 04/22/2011
** -----------------------------------------------------------------------------------------------------
** Modify Person:
** Modify Date  :
** Description  :
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include	"HAL.H"
#define		CH374HF_NO_CODE		1
#include	"CH374HFM.H"
#include	"CH374INC.H"
#include	"bsp_spi_ch374.h"

#include <RS232.h>


/*******************************************************************************
* Function Name  : CH374_PORT_INIT
* Description    : Initializes the peripherals used by the SPI CH374 driver.
                   PB13--SPI2_SCK
                   PB14--SPI2_MISO
                   PB15--SPI2_MOSI
                   PB12--SPI2_/CS
                   PD10---/USB_INT
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void CH374_PORT_INIT( )  
{
    //SPI_CH374_Init();

}

/* SPI���8��λ���� */
#define	Spi374OutByte( d )		SPI_CH374_SendByte(d)   
/* SPI����8��λ���� */
#define	Spi374InByte( )			SPI_CH374_ReadByte()	


/* SPI��ʼ */
static	void	Spi374Start( UINT8 addr, UINT8 cmd )  
{
	Select_CH374();          /* SPIƬѡ��Ч */
	Spi374OutByte( addr );
	Spi374OutByte( cmd );
}

/* SPI���� */
#define	Spi374Stop( )	{ NotSelect_CH374(); }  



//======================================================================================
/* SPI�ӿ��ӳ���,��CH374�ӳ������� */
//======================================================================================

UINT8	CH374_READ_REGISTER( UINT8 mAddr )  /* �ⲿ����ı�CH374�������õ��ӳ���,��ָ���Ĵ�����ȡ���� */
{
	UINT8	d;
	//RS232_PutString("\r\nspi_1");
	Spi374Start( mAddr, CMD_SPI_374READ );
	d = Spi374InByte( );
	Spi374Stop( );
	return( d );
}

void	CH374_WRITE_REGISTER( UINT8 mAddr, UINT8 mData )  /* �ⲿ����ı�CH374�������õ��ӳ���,��ָ���Ĵ���д������ */
{
	//RS232_PutString("\r\nspi_2");
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	Spi374OutByte( mData );
	Spi374Stop( );
}

void	CH374_READ_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8 mBuf )  /* �ⲿ����ı�CH374�������õ��ӳ���,��ָ����ʼ��ַ�������ݿ� */
{
	RS232_PutString("\r\nspi_3");
	Spi374Start( mAddr, CMD_SPI_374READ );
	while ( mLen -- ) *mBuf++ = Spi374InByte( );
	Spi374Stop( );
}

void	CH374_WRITE_BLOCK( UINT8 mAddr, UINT8 mLen, PUINT8  mBuf )  /* �ⲿ����ı�CH374�������õ��ӳ���,��ָ����ʼ��ַд�����ݿ� */
{
	RS232_PutString("\r\nspi_4");
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	while ( mLen -- ) Spi374OutByte( *mBuf++ );
	Spi374Stop( );
}

PUINT8 	CH374_READ_BLOCK64( UINT8 mAddr, PUINT8  mBuf )  /* �ⲿ����ı�CH374�������õ��ӳ���,��˫����������64�ֽڵ����ݿ�,���ص�ǰ��ַ */
{
	UINT8	i;
	RS232_PutString("\r\nspi_5");
	Spi374Start( mAddr, CMD_SPI_374READ );
	for ( i = CH374_BLOCK_SIZE; i != 0; i -- ) *mBuf++ = Spi374InByte( );
	Spi374Stop( );
	return( mBuf );
}

PUINT8 	CH374_WRITE_BLOCK64( UINT8 mAddr, PUINT8  mBuf )  /* �ⲿ����ı�CH374�������õ��ӳ���,��˫������д��64�ֽڵ����ݿ�,���ص�ǰ��ַ */
{
	UINT8	i;
	RS232_PutString("\r\nspi_6");
	Spi374Start( mAddr, CMD_SPI_374WRITE );
	for ( i = CH374_BLOCK_SIZE; i != 0; i -- ) Spi374OutByte( *mBuf++ );
	Spi374Stop( );
	return( mBuf );
}

void	CH374_WRITE_BLOCK_C( UINT8 mLen, PUINT8C mBuf )  /* �ⲿ����ı�CH374�������õ��ӳ���,��RAM_HOST_TRANд�볣�������ݿ� */
{
	RS232_PutString("\r\nspi_7");
	Spi374Start( RAM_HOST_TRAN, CMD_SPI_374WRITE );
	do {
		Spi374OutByte( *mBuf );
		mBuf ++;
	} while ( -- mLen );
	Spi374Stop( );
}




/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
