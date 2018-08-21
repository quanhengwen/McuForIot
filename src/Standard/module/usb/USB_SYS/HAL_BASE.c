/* CH374оƬ Ӳ������� V1.0 */
/* �ṩ�����ӳ���,�ṩ�жϲ�ѯ�ӳ��� */

#include	"HAL.H"
#include <RS232.h>
//#include "bsp_CH374hft.h"
/* ��ʱָ��΢��ʱ��,���ݵ�Ƭ����Ƶ����,����ȷ */
void	mDelayuS( UINT8 us )
{
	while ( us -- );  // 24MHz MCS51
}



/* ��ʱָ������ʱ�� */
void	mDelaymS( UINT8 ms )
{
	RS232_PutString("\r\nspi_delay");
	while ( ms -- ) {
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
		mDelayuS( 250 );
	}
}

// ��ѯCH374�ж�(INT#�͵�ƽ)
BOOL	Query374Interrupt( void )
{  RS232_PutString("\r\nspi_int");
#ifdef CH374_INT_WIRE
	return( CH374_INT_WIRE ? FALSE : TRUE );  /* ���������CH374���ж�������ֱ�Ӳ�ѯ�ж����� */
#else
	return( Read374Byte( REG_INTER_FLAG ) & BIT_IF_INTER_FLAG ? TRUE : FALSE );  /* ���δ����CH374���ж��������ѯ�жϱ�־�Ĵ��� */
#endif
}

// �ȴ�CH374�ж�(INT#�͵�ƽ)����ʱ�򷵻�ERR_USB_UNKNOWN
/*
UINT8	Wait374Interrupt( void )
{
	UINT16	i;

	RS232_PutString("\r\nspi_int_wait");
	for ( i = 0; i < 10000; i ++ ) {  // ������ֹ��ʱ
		if ( Query374Interrupt( ) ) return( 0 );
	}
	return( ERR_USB_UNKNOWN );  // ��Ӧ�÷��������
}	 */
/*	 
void xQueryInterrupt( void )
{
   while(USB_Flag.Transf_one ==FALSE);
   USB_Flag.Transf_one =FALSE;
   } */

