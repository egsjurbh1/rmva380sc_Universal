/**
 * \file      	EE3_SendFconData.c
 * \author    	Leo Bai
 * \version   	1.08
 * \date      	2012.07.13
 * \brief     	FconMode��JPEGͼ�����߳�
**/

#include "EE3_common.h"

/**************************************************************************/
//�ⲿ����
extern RoseekJPGEncBufStruct	g_JpgEncBufStruct;
extern volatile	Bool	g_bIsFconClientConnect;
/**************************************************************************/

/**
 * \function 	SendFconDataPort
 * \brief    	ʹ��TCP/IPЭ��ʵ��FconMode��JPEGͼ����
 * \			Ĭ����Ϊ�������ˣ�Port = 55000		
 * \
**/	
void SendFconDataPort()
{
	SOCKET	sockFconListen, sockFconSvr;
	int		size,i,cnt;
	Uint16	ui16Reply = 0;
	Bool	bClosed = FALSE;
	struct	sockaddr_in addr;
	Bool	bBreak=FALSE;
	static	Uint8	ui8JpgBufIndex=0;
	Uint8	ui8tmp;

	//Ϊ��ǰ�����������л���
	fdOpenSession( TaskSelf() );

	//��������socket����
	sockFconListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( sockFconListen == INVALID_SOCKET ){
		Roseek_Reset();//���������������ʧ�ܣ��������
	}
	bzero( &addr, sizeof(struct sockaddr_in) );
	addr.sin_family	= AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_len	= sizeof( addr );
	addr.sin_port	= htons( 55000 );
	//sockFconListen��
	if ( bind( sockFconListen, (PSA) &addr, sizeof(addr) ) < 0 ){
		Roseek_Reset();//�����ʧ�ܣ��������
	}
	//sockFconListen��ʼ������ͬһʱ�̽�֧��һ������
	if ( listen( sockFconListen, 1) < 0 ){
		Roseek_Reset();//�������ʧ�ܣ��������
	}
	//����ѭ��
	while( 1 ){
		size = sizeof( addr );
		sockFconSvr = accept( sockFconListen, (PSA)&addr, &size );
		if( sockFconSvr == INVALID_SOCKET){
			//����������ӳ�����رս��ܵ������Ӷ������³���
			fdClose(sockFconSvr);
			continue;
		}
		g_bIsFconClientConnect = TRUE; //�����߳�׼�����
		//����λ������MJPGͼ��
		while( !bClosed ){
			//�ȴ��źŵ�ͬ��
			SEM_pend( &sem_ConDataReady, SYS_FOREVER);
			//�ж��ϴ��ĸ�����������
			
			bBreak = FALSE;
			if(ui8JpgBufIndex==1){
				ui8tmp=0;
			}
			else{
				ui8tmp=ui8JpgBufIndex+1;
			}
			
			for( i=ui8tmp; i<ui8tmp+2; i++ ){
				if( i>1 ){
					if( g_JpgEncBufStruct.bJpgImgBuf_Lock[i-2] ){
						ui8JpgBufIndex = i-2;
						bBreak = TRUE;
						break;
					}
				}
				else{//i<=5
					if( g_JpgEncBufStruct.bJpgImgBuf_Lock[i] ){
						ui8JpgBufIndex = i;
						bBreak = TRUE;
						break;
					}
				}
			}
			
			if( !bBreak ){
				continue;
			}

			if( send( sockFconSvr, g_JpgEncBufStruct.pui8JpgImgBuf[ui8JpgBufIndex], *(Uint32*)g_JpgEncBufStruct.pui8JpgImgBuf[ui8JpgBufIndex] + 4, 0 ) < 0 )
			{
					break;
			}
			else
			{
				int a= 0;
				a++;
			}
			g_JpgEncBufStruct.bJpgImgBuf_Lock[ui8JpgBufIndex] = FALSE;

			//���ܻ�Ӧ�ж��Ƿ��˳�����ѭ���������ܵ�0xffff��ֹ����ѭ�������������������
			//�������һ��ִ�к��յ���0xffff(����ʱ��
			i = 0;
			while( i < 2 ){
				cnt = recv( sockFconSvr, (char*)&ui16Reply, 2-i, 0 );
				if( cnt <= 0 ){
					bClosed = TRUE;
					break;
				}
				i += cnt;
			}
			if( ui16Reply==0xffff ){
				bClosed = TRUE;
			}
		}
		//���Ʊ�־��λ���رշ��ͷ��������ӣ��ػص���ѭ���ȴ�������
		bClosed = FALSE;
		ui16Reply = 0x0000;
		fdClose(sockFconSvr);
		g_bIsFconClientConnect = FALSE;
		g_JpgEncBufStruct.bJpgImgBuf_Lock[0] = FALSE;
		g_JpgEncBufStruct.bJpgImgBuf_Lock[1] = FALSE;
		SEM_reset( &sem_ConDataReady, 0 );
	}//����ѭ��
}
