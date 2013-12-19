/**
 * \file      	ITS_ReceiveCmd.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.07
 * \date      	2013.11.24
 * \brief     	���ܽ�ͨӦ�á������պ��̵ƿ����������źŵ������߳�
 * \            ��UDP_Client_CMDMonitor.exe����ͨ��ʹ��
 * \update
**/

#include "EE3_common.h"
/**************************************************************************/
//�ⲿ����
extern PcInData     g_BpInputData;
extern PcInData 	g_BpOutputData1;
extern PcInData 	g_BpOutputData2;
extern AcmdSem 		g_AmcdSem1;
extern AcmdSem 		g_AmcdSem2;
extern volatile    Bool            g_bIsDpDataClientConnect;
extern EE3_CfgStruct g_EE3Cur;//�������״̬
extern CmdUData g_CmdUdata;
extern volatile    int   bText;
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
	Bool	bClosed = FALSE;
	Bool    bSendDp1 = FALSE;
	Bool    bSendDp2 = FALSE;
	struct	sockaddr_in sin; //����ͻ��ĵ�ַ��Ϣ
	struct  sockaddr pcliaddr;

	//Ϊ��ǰ�����������л���
	fdOpenSession( TaskSelf() );

	//���������׽���socket����
	sockBpSemListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);//��������������socket,UDPЭ��
	if( sockBpSemListen == INVALID_SOCKET ){
		Roseek_Reset();//���������������ʧ�ܣ��������
	}
	bzero( &sin, sizeof(struct sockaddr_in) );
	sin.sin_family	= AF_INET; 
	sin.sin_addr.s_addr = INADDR_ANY;//�Զ���ȡ������ַ
	sin.sin_len	= sizeof( sin );
	sin.sin_port	= htons( ACMDPORT );//ָ�����ӷ������Ķ˿ں�
	//sockBpSemListen��
	if ( bind( sockBpSemListen, (PSA) &sin, sizeof(sin) ) < 0 ){
		Roseek_Reset();//�����ʧ�ܣ��������
	}
	//����ѭ��
	while( 1 ){
	    clilen = sizeof(struct sockaddr);
		g_bIsDpDataClientConnect = TRUE;//�����߳�׼�����
		while( !bClosed ){
			//�ȴ��źŵ�ͬ��
			SEM_pend( &sem_CmdDataReady, SYS_FOREVER);
			//��������ʱ��
			SEM_pend( &sem_DataRunTimeN, SYS_FOREVER);
			if( sendto( sockBpSemListen,(char *)(&g_CmdUdata),sizeof(g_CmdUdata), 0,(PSA)&pcliaddr,clilen )<0){
			     		break;
			}
			//��ʼ��������
			cnt=recvfrom( sockBpSemListen, (char *)(&g_BpInputData), sizeof(g_BpInputData), 0, (PSA)&pcliaddr,&clilen);
			if(cnt<0){
			   	bClosed = TRUE;
			    break;
			}
			
			//�ж��ź�����
			if(g_BpInputData.label1 == 0xCC&&g_BpInputData.label2 == 0xCC){//�ź�����ƥ��
				//��һ·
				if(g_BpInputData.cameraN == g_EE3Cur.RoadNum[0]){
					g_AmcdSem1.bStart=TRUE;
					switch(g_BpInputData.comm){
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
				if(g_BpInputData.cameraN == g_EE3Cur.RoadNum[1]){
					g_AmcdSem2.bStart=TRUE;
					switch(g_BpInputData.comm){
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
				if(bSendDp1){
					SEM_pend( &sem_DpDataReady1, SYS_FOREVER);
			     	if( sendto( sockBpSemListen,(char *)(&g_BpOutputData1),sizeof(g_BpOutputData1), 0,(PSA)&pcliaddr,clilen )<0){
			     		break;
					}
					bSendDp1=FALSE;
					//bTextȫ�ֱ�������sendto�Ƿ�ɹ�
				}
				if(bSendDp2){
					SEM_pend( &sem_DpDataReady2, SYS_FOREVER);
			     	if( sendto( sockBpSemListen,(char *)(&g_BpOutputData2),sizeof(g_BpOutputData2), 0,(PSA)&pcliaddr,clilen )<0){
			     		break;
					}
					bSendDp2=FALSE;
				}
		    }
		}
		//���Ʊ�־��λ���رշ��ͷ��������ӣ��ػص���ѭ���ȴ�������
		bClosed = FALSE;
		fdClose(sockBpSemListen);
		SEM_reset( &sem_CmdDataReady, 0 );
		SEM_reset( &sem_DpDataReady1, 0 );
		SEM_reset( &sem_DpDataReady1, 0 );
	}//����ѭ��
}






