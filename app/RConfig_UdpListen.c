/**
 * \file      	RConfig_UdpListen.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.1.4
 * \date      	2013.11.24
 * \brief     	Զ�����ù��ܡ���UDP����Զ�����ù��ߵ������߳�
 * \update      
**/

#include "EE3_common.h"
/**************************************************************************/

/**************************************************************************/
//�ⲿ����
extern volatile    struct  sockaddr g_ConfigSetaddr;
extern volatile    int     bTextConnect;
extern volatile    int     bTextCount;
extern volatile    int     bTextRecev;
/**************************************************************************/

/**
 * \function 	UDPlistensendPort
 * \brief    	ʹ��UDPЭ��ʵ�ֹ㲥����Զ�����ù��ߣ����������ӽ����IP��ַ��֪
 * \			Ĭ����Ϊ����ˣ�Port = 4502;��������ƽ̨ʱ����Ϊ�ͻ��ˡ�
 * \
**/	
void UDPlistensendPort()
{
	SOCKET	sockUDPsendListen,sockTCPsendIP;
	int		clilen,len;
	int     label;
	struct	sockaddr_in sin,connaddr, clientaddr; //����ͻ��ĵ�ַ��Ϣ
	struct  sockaddr pcliaddr;
	ConversationHead replySem;

	//Ϊ��ǰ�����������л���
	fdOpenSession( TaskSelf() );

	//���������׽���socket����
	sockUDPsendListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//��������������socket,UDPЭ��
	if( sockUDPsendListen == INVALID_SOCKET )
	{
		Roseek_Reset();//���������������ʧ�ܣ��������
	}
	bzero( &sin, sizeof(struct sockaddr_in) );
	sin.sin_family	= AF_INET; 
	sin.sin_addr.s_addr = INADDR_ANY;//�Զ���ȡ������ַ
	sin.sin_len	= sizeof( sin );
	sin.sin_port= htons( UDPPORT );//ָ�����ӷ������Ķ˿ں�
	//sockUDPsendListen��
	if ( bind( sockUDPsendListen, (PSA) &sin, sizeof(sin) ) < 0 )
	{
		Roseek_Reset();//�����ʧ�ܣ��������
	}

	//�������ܿ�����Ϣ
	while(1)
	{
		label = 10; 
	    clilen = sizeof(struct sockaddr);		
		//��ʼ��������
		bTextRecev=recvfrom(sockUDPsendListen, (char *)(&replySem), sizeof(ConversationHead), 0, (PSA)&pcliaddr,&clilen);			
		if( bTextRecev < sizeof(ConversationHead) )
		{
			continue;
		}
		//�洢���ù��ߵ�IP
		g_ConfigSetaddr= pcliaddr;	
					
		//��Ϊ�ͻ��ˣ�����TCPЭ������Զ��ƽ̨���˿�4502����֪�����IP
		sockTCPsendIP = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//�����ͻ���socket.TCPЭ��
		if( sockTCPsendIP == INVALID_SOCKET )
		{
		    Roseek_Reset();//���������������ʧ�ܣ��������
	    }
		bzero( &connaddr,sizeof(struct sockaddr_in));
		connaddr.sin_family = AF_INET;
		connaddr.sin_addr.s_addr = INADDR_ANY;
		connaddr.sin_len = sizeof( connaddr );
		connaddr.sin_port = htons(0);
		if (bind(sockTCPsendIP, (PSA) &connaddr,sizeof(connaddr)))
		{
			Roseek_Reset();//�����ʧ�ܣ��������
		} 
		bzero(&clientaddr, sizeof(struct sockaddr_in));
		clientaddr.sin_family = AF_INET;
		clientaddr.sin_addr = ((struct sockaddr_in*) &pcliaddr)->sin_addr;
		clientaddr.sin_len = sizeof( clientaddr );
		clientaddr.sin_port = htons(UDPPORT);		
		if(connect( sockTCPsendIP, (PSA)&clientaddr, clientaddr.sin_len)<0)
		{
			bTextConnect = 1;
			++bTextCount;
			continue;
		}
		else
		{
			bTextConnect = 0;
			if(replySem.command==UDPCON)
			{
				//������Ϣ1
				replySem.command = BROADCAST1;
				replySem.bufsize = DEVICETYPE;//�豸���Ͳ���
		      	len=send( sockTCPsendIP,&replySem, sizeof(replySem), 0 );
		      	//������Ϣ2
		      	replySem.command = UDPSETTIME;
		      	replySem.bufsize = label;
		      	len=send( sockTCPsendIP,&replySem, sizeof(replySem), 0);
		      	fdClose(sockTCPsendIP);
				if(len<0)
				{
					continue;
				}
			}
		}			

	}//����ѭ��

}
