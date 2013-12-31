/**
 * \file      	Rconfig_SendYuvData.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.0.6
 * \date      	2013.11.24
 * \brief     	Զ�����ù��ܡ���YUVͼ�����ݷ��������ù��ߵ������߳�
 * \update      
**/				

#include "EE3_common.h"
/**************************************************************************/
//�ⲿ����
extern ITS_ImgBufStruct      g_ImgBufofFrontS;//ͼ��ԭʼ���ݻ���ṹ��
extern volatile	Bool	g_bIsFcontoDecClientConnect;
extern volatile Bool    g_bIsSendPictoDec;
extern volatile    struct  sockaddr g_ConfigSetaddr;
/**************************************************************************/

/**
 * \function 	SendFcontoDecPort
 * \brief    	ʹ��TCP/IPЭ�飬�����ù��߷���YUV422��ʽ��ԭʼͼ������
 * \			��Ϊ�ͻ��ˣ�Port = 4501		
 * \
**/	
void SendFcontoDecPort()
{
	SOCKET	sockTCPsendVideo;
	int		cnt;
	struct	sockaddr_in connaddr,clientaddr;	
	
	while(1)
	{
		SEM_pend(&sem_SendVideoReady,SYS_FOREVER);			    			    
	    //Ϊ��ǰ�����������л���
		fdOpenSession( TaskSelf() );
		//�׽���socket����,TCPЭ��
		sockTCPsendVideo = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( sockTCPsendVideo == INVALID_SOCKET )
		{
		    Roseek_Reset();//���������������ʧ�ܣ��������
	    }
		bzero( &connaddr,sizeof(struct sockaddr_in));
		connaddr.sin_family = AF_INET;
		connaddr.sin_addr.s_addr = INADDR_ANY;
		connaddr.sin_len = sizeof( connaddr );
		connaddr.sin_port = htons(0);
		if (bind(sockTCPsendVideo, (PSA) &connaddr,sizeof(connaddr)))
		{
			Roseek_Reset();//�����ʧ�ܣ��������
		}	 	
    	bzero( &clientaddr,sizeof(struct sockaddr_in));
		clientaddr.sin_family = AF_INET;
    	clientaddr.sin_len = sizeof( connaddr );
    	clientaddr.sin_port = htons(VIDEOPORT);//�˿ں�
		clientaddr.sin_addr = ((struct sockaddr_in*) &g_ConfigSetaddr)->sin_addr; //ָ�������IP��ַ
		//���ӷ����
		cnt=connect( sockTCPsendVideo, (PSA)&clientaddr, clientaddr.sin_len);
		if(cnt == -1)
		{
             //������ӳ�����رս��ܵ������Ӷ������³���
			 fdClose(sockTCPsendVideo);
			 continue;
		}
		g_bIsFcontoDecClientConnect = TRUE; //�����߳�׼�����
			
		while(g_bIsSendPictoDec)
		{				
			//����YUV422ԭʼͼ�������������
	    	SEM_pend( &sem_FcontoDecReady, SYS_FOREVER);		    			      	    	           					          
			if( send( sockTCPsendVideo, g_ImgBufofFrontS.pui8FrameBuf, ALG_ROW*ALG_COL*2, 0 ) < 0 )
			{
				break;
			}			  		    
	  	}
	  	//�رշ��ͷ��������ӣ��ػص���ѭ���ȴ�������		
		fdClose(sockTCPsendVideo);
		g_bIsFcontoDecClientConnect = FALSE;
		SEM_reset( &sem_FcontoDecReady, 0 );
		SEM_reset( &sem_SendVideoReady, 0 );							
	}
}
