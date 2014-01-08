/**
 * \file      	ITS_ReceiveCmd.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.1.1
 * \date      	2014.1.8
 * \brief     	���ܽ�ͨӦ�á������պ��̵ƿ����������źŵ������߳�
 * \            ��UDP_Client_CMDMonitor.exe����ͨ��ʹ��
 * \update		��������ͷ�ļ�
**/

#include "EE3_common.h"
#include "EE3_Imgmain.h"
/**************************************************************************/

/**
 * \function 	ReceBpStartSemPort
 * \brief    	ʹ��UDPЭ��ʵ�ֿ����źŵĽ���
 * \			Ĭ����Ϊ�������ˣ�Port = 6366	
 * \
**/	
void ReceBpStartSemPort()
{
	SOCKET	sockBpSemListen;
	int		cnt,clilen;
	Bool	bError = FALSE;
	Bool    bSendDp1 = FALSE;
	Bool    bSendDp2 = FALSE;
	struct	sockaddr_in sin; //����ͻ��ĵ�ַ��Ϣ
	struct  sockaddr pcliaddr;

	//����ѭ��
	while( 1 )
	{
		//Ϊ��ǰ�����������л���
		fdOpenSession( TaskSelf() );
		//���������׽���socket����
		sockBpSemListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//��������������socket,UDPЭ��
		if( sockBpSemListen == INVALID_SOCKET )
		{
			Roseek_Reset();//���������������ʧ�ܣ��������
		}
		bzero( &sin, sizeof(struct sockaddr_in) );
		sin.sin_family	= AF_INET; 
		sin.sin_addr.s_addr = INADDR_ANY;//�Զ���ȡ������ַ
		sin.sin_len	= sizeof( sin );
		sin.sin_port	= htons( ACMDPORT );//ָ�����ӷ������Ķ˿ں�
		//sockBpSemListen��
		if ( bind( sockBpSemListen, (PSA) &sin, sizeof(sin) ) < 0 )
		{
			Roseek_Reset();//�����ʧ�ܣ��������
		}
		clilen = sizeof(struct sockaddr);
		bError = FALSE;//�������ӳɹ�
    
		while( !bError )
		{
			//������������
			cnt=recvfrom( sockBpSemListen, (char *)(&g_BpInputData), sizeof(g_BpInputData), 0, (PSA)&pcliaddr,&clilen);
			if(cnt < 0)
			{
			   	bError = TRUE;
			    break;
			}
			
			//�ж��ź�����
			if(g_BpInputData.label1 == 0xCC&&g_BpInputData.label2 == 0xCC){//�ź�����ƥ��
				//��һ·
				if(g_BpInputData.cameraN == g_EE3Cur.RoadNum[0])
				{
					g_AmcdSem1.bStart=TRUE;
					switch(g_BpInputData.comm)
					{
					   case NETINFORTG://֪ͨ��ת��
							g_AmcdSem1.comm=NETINFORTG;//��ȡ�����ź�
							break;
					   case NETINFOGTR://֪ͨ��ת��
							g_AmcdSem1.comm=NETINFOGTR;
							break;
					   case NETREQUEST://����ӳ����
							g_AmcdSem1.comm=NETREQUEST;	
							bSendDp1=TRUE;		     			
							break;
					   default:
					        break;
					}			     	                	
				}
				//�ڶ�·
				if(g_BpInputData.cameraN == g_EE3Cur.RoadNum[1])
				{
					g_AmcdSem2.bStart=TRUE;
					switch(g_BpInputData.comm)
					{
					   case NETINFORTG://֪ͨ��ת��
							g_AmcdSem2.comm=NETINFORTG;
							break;
					   case NETINFOGTR://֪ͨ��ת��
							g_AmcdSem2.comm=NETINFOGTR;
							break;
					   case NETREQUEST://����ӳ����
							g_AmcdSem2.comm=NETREQUEST;	
							bSendDp2=TRUE;		     			
							break;
					   default:
					        break;
					}			     
				}
					
				//���Ͷӳ������
				if(bSendDp1)
				{
					SEM_pend( &sem_DpDataReady1, SYS_FOREVER);
			     	if( sendto( sockBpSemListen,(char *)(&g_BpOutputData1),sizeof(g_BpOutputData1), 0,(PSA)&pcliaddr,clilen )<0)
			     	{
			     		bError = TRUE;
			     		break;
					}
					bSendDp1=FALSE;
				}
				if(bSendDp2)
				{
					SEM_pend( &sem_DpDataReady2, SYS_FOREVER);
			     	if( sendto( sockBpSemListen,(char *)(&g_BpOutputData2),sizeof(g_BpOutputData2), 0,(PSA)&pcliaddr,clilen )<0)
			     	{
			     		bError = TRUE;
			     		break;
					}
					bSendDp2=FALSE;
				}
		    }
		}
		//�رշ��ͷ��������ӣ��ػص���ѭ���ȴ�������
		fdClose(sockBpSemListen);
		SEM_reset( &sem_DpDataReady1, 0 );
		SEM_reset( &sem_DpDataReady1, 0 );
	}//����ѭ��
}






