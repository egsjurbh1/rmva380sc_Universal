/**
 * \file      	EE3_uart_process.c
 * \author    	Leo Bai
 * \version   	1.08
 * \date      	2012.07.13
 * \brief     	UART���ݽ���ģ�顣
**/

#include "EE3_common.h"

Uint8 uint8485RcvBuf[512];
Uint8 uint8232_1_RcvBuf[512];
Uint8 uint8232_2_RcvBuf[512];

/**
 * \function 	hwiUartRerdyFunc
 * \brief    	�����ж���Ӧ����(Ӳ���ж�)
 * \			ÿ���������ݴ����DSP�󣬸��жϻᱻϵͳ�Զ�����		
 * \note		1	�û�����ʹ�ø��ж�֮ǰ��ʼ������
 * \			2	�����û����������ݽ��������ַ�����Ӧ���߳���ִ��
**/	
void hwiUartRerdyFunc()
{
	Uint32 cnt;
	//������ȡUART���ܻ�����źŵ�
	if( Roseek_IsRS485RcvReady() ){
		//��ȡUART���ܻ���
		cnt = Roseek_GetRS485_Recvcnt();
		Roseek_RS485_ReadBuf( (char*)uint8485RcvBuf, cnt );
		Roseek_RS485_WriteData( (char*)uint8485RcvBuf, cnt );
	}
	if( Roseek_IsRS232_1_RcvReady() ){
		//��ȡUART���ܻ���
		cnt = Roseek_GetRS232_1_Recvcnt();
		Roseek_RS232_ReadBuf( Roseek_RS232_PORT1, (char*)uint8232_1_RcvBuf, cnt );
		Roseek_RS232_WriteData( Roseek_RS232_PORT1, (char*)uint8232_1_RcvBuf, cnt );
	}
	if( Roseek_IsRS232_2_RcvReady() ){
		//��ȡUART���ܻ���
		cnt = Roseek_GetRS232_2_Recvcnt();
		Roseek_RS232_ReadBuf( Roseek_RS232_PORT2, (char*)uint8232_2_RcvBuf, cnt );
		Roseek_RS232_WriteData( Roseek_RS232_PORT2, (char*)uint8232_2_RcvBuf, cnt );
	}
}

