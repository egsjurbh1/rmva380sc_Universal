/**
 * \file      	EE3_SendTrgData.c
 * \author    	Leo Bai
 * \version   	1.08
 * \date      	2012.07.13
 * \brief     	TrgMode��JPEGͼ�����߳�
**/

#include "EE3_common.h"

/**************************************************************************/
//�ⲿ����
extern volatile	Bool		g_bIsStopTrgSvr;
extern volatile	Bool		g_bIsTrgClientConnect;
extern volatile	Bool		g_bTrgSvrIsLink;
extern RoseekJPGEncBufStruct	g_JpgEncBufStruct;
/**************************************************************************/

/**************************************************************************/
/**
 * \function 	SendTrgDataPort
 * \brief    	ʹ��TCP/IPЭ��ʵ��TrgMode��JPEGͼ����
 * \			Ĭ����Ϊ�������ˣ�Port = 45000		
 * \
**/	
void SendTrgDataPort()
{
	SOCKET	sockTrgListen, sockTrgSvr;
	int		size;
	Bool	bClosed = FALSE;
	struct	sockaddr_in addr;
	Uint32	ui32Reply = 0;
	Bool	bBreak=FALSE;
	static	Uint8	ui8TrgJpgBufIndex=0;
	Uint8	i,ui8tmp;
	//Ϊ��ǰ�����������л���
	fdOpenSession( TaskSelf() );

	//��������socket����
	sockTrgListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if( sockTrgListen == INVALID_SOCKET ){
		Roseek_Reset();//���������������ʧ�ܣ��������
	}
	bzero( &addr, sizeof(struct sockaddr_in) );
	addr.sin_family	= AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_len	= sizeof( addr );
	addr.sin_port	= htons( 45000 );
	//sockFconListen��
	if ( bind( sockTrgListen, (PSA) &addr, sizeof(addr) ) < 0 ){
		Roseek_Reset();//�����ʧ�ܣ��������
	}
	//sockFconListen��ʼ������ͬһʱ�̽�֧��һ������
	if ( listen( sockTrgListen, 1) < 0 ){
		Roseek_Reset();//�������ʧ�ܣ��������
	}
	//����ѭ��
	while( 1 ){
		size = sizeof( addr );
		sockTrgSvr = accept( sockTrgListen, (PSA)&addr, &size );
		if( sockTrgSvr == INVALID_SOCKET ){
			//����������ӳ�����رս��ܵ������Ӷ������³���
			fdClose(sockTrgSvr);
			continue;
		}
		g_bIsTrgClientConnect = TRUE;
		g_bTrgSvrIsLink = TRUE;
		//����λ������MJPGͼ��
		while( !bClosed ){
			//�ȴ��źŵ�ͬ��
			SEM_pend( &sem_TrgDataReady, SYS_FOREVER);
			//�ж��Ƿ��˳���ǰ����
			if( g_bIsStopTrgSvr ){
				bClosed = TRUE;
				g_bIsStopTrgSvr = FALSE;
				ui32Reply = 0x0;
				if( send( sockTrgSvr, (char*)&ui32Reply, 4, 0 ) < 0 ){
					break;
				}
				continue;
			}
			
			
			bBreak = FALSE;
			if(ui8TrgJpgBufIndex==5){
				ui8tmp=0;
			}
			else{
				ui8tmp=ui8TrgJpgBufIndex+1;
			}
			
			for( i=ui8tmp; i<ui8tmp+6; i++ ){
				if( i>5 ){
					if( g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[i-6] ){
						ui8TrgJpgBufIndex = i-6;
						bBreak = TRUE;
						break;
					}
				}
				else{//i<=5
					if( g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[i] ){
						ui8TrgJpgBufIndex = i;
						bBreak = TRUE;
						break;
					}
				}
			}
			
			if( !bBreak ){
				continue;
			}
			
			if( send( sockTrgSvr, g_JpgEncBufStruct.pui8TrgJpgImgBuf[ui8TrgJpgBufIndex], *(Uint32*)g_JpgEncBufStruct.pui8TrgJpgImgBuf[ui8TrgJpgBufIndex] + 4 + sizeof(RoseekImgInfStruct), 0 ) < 0 ){
					break;
				}
			g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[ui8TrgJpgBufIndex] = FALSE;

		}
		//���Ʊ�־��λ���رշ��ͷ��������ӣ��ػص���ѭ���ȴ�������
		bClosed = FALSE;
		g_bTrgSvrIsLink = FALSE;
		fdClose(sockTrgSvr);
		g_bIsTrgClientConnect = FALSE;
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[0] = FALSE;
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[1] = FALSE;
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[2] = FALSE;
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[3] = FALSE;
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[4] = FALSE;
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[5] = FALSE;
		SEM_reset( &sem_TrgDataReady, 0 );
	}//����ѭ��
}

