/**
 * \file      	RServer_SendRoadInf.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.1.0
 * \date      	2014.1.15
 * \brief     	��Զ�̷�����������Ϣ���ܡ������ͳ���������ݵ������߳�
 * \update      �޸��߳�ͬ��BUG
**/				

#include "EE3_common.h"
/**************************************************************************/
//�ⲿ����
extern EE3_CfgStruct g_EE3Cur;//�������״̬
extern CarInfo g_carInfo;//���������Ϣ
extern volatile    Bool    g_bIsSendRoadInfConnect;
/**************************************************************************/

/**
 * \function 	SendRoadInfPort
 * \brief    	ʹ��TCP/IPЭ�飬����Զ�̷��������ͳ����������
 * \			��Ϊ�ͻ���	
 * \
**/	
void SendRoadInfPort()
{
	SOCKET	sockTCPsendroadinf;
	int		cnt, i, len;
	struct	sockaddr_in connaddr,serveraddr;
	SendInfo info[2];	
	Uint32 t_start, t_end;
	
	while(1)
	{			    			      	
    	//�ȴ��ź�ͬ��
    	SEM_pend(&sem_SendRoadInfReconnect,SYS_FOREVER);
    	//Ϊ��ǰ�����������л���
		fdOpenSession( TaskSelf() );
		//�׽���socket����,TCPЭ��
		sockTCPsendroadinf = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if( sockTCPsendroadinf == INVALID_SOCKET )
		{
	         Roseek_Reset();//���������������ʧ�ܣ��������
    	}
		bzero( &connaddr,sizeof(struct sockaddr_in));
		connaddr.sin_family = AF_INET;
		connaddr.sin_addr.s_addr = INADDR_ANY;
		connaddr.sin_len = sizeof( connaddr );
		connaddr.sin_port = htons(0);			
		if (bind(sockTCPsendroadinf, (PSA) &connaddr,sizeof(connaddr)))
		{
		     Roseek_Reset();//�����ʧ�ܣ��������
		}	 	
		bzero( &serveraddr,sizeof(struct sockaddr_in));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_len = sizeof( serveraddr );
		serveraddr.sin_port = htons(g_EE3Cur.ServerPort);//�˿ں�
		inet_aton(g_EE3Cur.ServerIp, &serveraddr.sin_addr); //ָ�������IP��ַ
		//���ӷ����
		cnt=connect( sockTCPsendroadinf, (PSA)&serveraddr, serveraddr.sin_len);
		if(cnt == -1)
		{
         	//������ӳ�����رս��ܵ������Ӷ������³���
		 	fdClose(sockTCPsendroadinf);
			
		 	continue;
		}
		//���ӳɹ����
		g_bIsSendRoadInfConnect = TRUE;
		SEM_reset( &sem_SendRoadInfReconnect, 0 );

		//IP��ַ��Ϣ����
		strcpy(info[0].local_ip, g_EE3Cur.CameraIp);
		strcpy(info[1].local_ip, g_EE3Cur.CameraIp);
		
		//ѭ����ʱ
		t_start =  Roseek_GetTickCount();

		while(1)
		{						
    		SEM_pend(&sem_SendRoadInfReady,SYS_FOREVER);
    		t_end = Roseek_GetTickCount();
			//ָ���ϴ�����
    		if( abs(t_end - t_start) >= (g_EE3Cur.UploadTime * 1000000))
    		{      	    	           					          
		    	//���ɷ�����Ϣ		
				for(i = 0;i < 2;i++)
				{
					info[i].label = i + 1;
					info[i].num = g_carInfo.num[i];
					info[i].speed = g_carInfo.speed[i];			
					g_carInfo.num[i] = 0;
					g_carInfo.speed[i] = 0;
				}
				//���������������
		    	len = send( sockTCPsendroadinf, &(info), 2 * sizeof(SendInfo), 0 );
				if(len < 0)
				{
					//�ر�����
					break;
				}
				//���¼�ʱ					
				t_start = Roseek_GetTickCount();
		    	
		    }  		    
  		}
  		//�رշ��ͷ��������ӣ��ػص���ѭ���ȴ�������	
  		g_bIsSendRoadInfConnect = FALSE;	
		fdClose(sockTCPsendroadinf);
		SEM_reset( &sem_SendRoadInfReady, 0 );
									
	}
}




