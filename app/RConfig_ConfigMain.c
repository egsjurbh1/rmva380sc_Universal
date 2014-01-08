/**
 * \file      	RConfig_ConfigMain.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.3.3
 * \date      	2014.1.8
 * \brief     	Զ�����ù��ܡ���Զ������ͨ�ŵ������߳�
 * \update      
**/

#include "EE3_common.h"

/**************************************************************************/
//�ⲿ����
extern volatile    Bool    g_bIsSendPictoDec;
extern EE3_CfgStruct g_EE3Cur;//�������״̬
extern EE3_StateStruct g_EE3State;
extern unsigned int	g_IPAddrCurUse;//���IP
/**************************************************************************/
static void Read_DataEncoding(EE3_CfgStruct Cfg, EE3_StateStruct state, char *Sendbuf);
static void Write_ConfigData(Uint8 *ee3cfgBuffer);
/**************************************************************************/

/**
 * \function 	ReceDecOrderPort
 * \brief    	ʹ��TCP/IPЭ�飬Զ�����ù���ͨ�ţ�������Ϣ�Ľ��ա��������������
 * \			Ĭ����Ϊ�������ˣ�Port = 4500
 * \
**/	
void ReceDecOrderSemPort()
{
	SOCKET	sockReceDecOrderListen, sockReceDecOrderSvr;
	struct	sockaddr_in addr; //����ͻ��ĵ�ַ��Ϣ
	int		size,cnt;
	Uint8 	pointBuffer[80];
	char    sendEE3cfgbuf[150];
	Uint8   ee3cfgBuffer[100];		
	Bool	bIsError = FALSE;
	ConversationHead reply;
	ConversationHead g_DecCmdsem;

	//Ϊ��ǰ�����������л���
	fdOpenSession( TaskSelf() );
	//���������׽���socket����
	sockReceDecOrderListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//��������������socket,TCPЭ��
	if( sockReceDecOrderListen == INVALID_SOCKET )
	{
		Roseek_Reset();//���������������ʧ�ܣ��������
	}
	bzero( &addr, sizeof(struct sockaddr_in) );
	addr.sin_family	= AF_INET; //AF_INETָ��ʹ��TCP/IPЭ����
	addr.sin_addr.s_addr = INADDR_ANY;//�Զ���ȡ������ַ
	addr.sin_len = sizeof( addr );
	addr.sin_port = htons( CONTROLPORT );//ָ�����ӷ������Ķ˿ں�
	//Listen��
	if ( bind( sockReceDecOrderListen, (PSA) &addr, sizeof(addr) ) < 0 )
	{
		Roseek_Reset();//�����ʧ�ܣ��������
	}
	//��ʼ������ͬһʱ�̽�֧��һ������
	if ( listen( sockReceDecOrderListen, 1) < 0 )
	{
		Roseek_Reset();//�������ʧ�ܣ��������
	}

	//������������
	while(1)
	{
		size = sizeof( addr );
		sockReceDecOrderSvr = accept( sockReceDecOrderListen, (PSA) &addr, &size );
		if( sockReceDecOrderSvr == INVALID_SOCKET)
		{
			//����������ӳ�����رս��ܵ������Ӷ������³���
			fdClose(sockReceDecOrderSvr);
			continue;
		}
		bIsError = FALSE;//���ӳɹ����
		
		//������������
		while(!bIsError)
		{	
			cnt=recv( sockReceDecOrderSvr, (char *)(&g_DecCmdsem), sizeof(g_DecCmdsem), 0 );
			if(cnt<0)
			{
			  	bIsError = TRUE;
			  	break;
			}
			
			//���ù����������
			switch(g_DecCmdsem.command)
			{
				  //1ѡ�����
			      case CHOOSECAM:
					 //������Ϣ
				     reply.command = CHOOSECAM;
					 reply.bufsize = g_DecCmdsem.bufsize;					 
                     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
                     {
						bIsError = TRUE;
                     	break;
					 }
					 break;

				  //2������ͼ��
				  case STARTVIDEO:
					 //������Ϣ
				     reply.command = STARTVIDEO;
					 reply.bufsize = 1;
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
				     {
						bIsError = TRUE;
                     	break;
					 }
					 SEM_post(&sem_SendVideoReady);//����ͼ���߳̿����ź�
				     g_bIsSendPictoDec = TRUE; 
					 break;

				  //3��ֹ����ͼ��
				  case ENDVIDEO:
				  	 //������Ϣ				     
				     reply.command = ENDVIDEO;
					 reply.bufsize = 1;
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
				     {
						bIsError = TRUE;
                     	break;
					 }
					 g_bIsSendPictoDec = FALSE;//ֹͣ����ͼ��
					 break;

				  //4������Ȧ����
				  case SETPOINT:
					 //������������
				     if(recv( sockReceDecOrderSvr, (char *)(&pointBuffer), sizeof(pointBuffer), 0 )<0)
				     {
				     	bIsError = TRUE;
				     	break;
					 }
					 //���ݴ���FLASH
					 switch(g_EE3Cur.RunMode)
					 {
						case VEHICLE_DETECT:
					 		Roseek_Flash_Burn(pointBuffer,CFGVDPOINTADD,80); 
							break;
						case QUEUE_DETECT:
					 		Roseek_Flash_Burn(pointBuffer,CFGQDPOINTADD,80);	
					 		break;
					 	default:
					 		break;
					 }
					 //������Ϣ
					 reply.command = SETPOINT;
					 reply.bufsize = 1;
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
				     {
						bIsError = TRUE;
                     	break;
					 }																										
					 break;

				  //5�������״̬��Ϣ
				  case EE3_SENDCONFIG:
					 //������Ϣ
					 reply.command = EE3_SENDCONFIG;
					 reply.bufsize = sizeof(sendEE3cfgbuf);//���ط��Ͱ��Ĵ�С
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
				     {
						bIsError = TRUE;
                     	break;
					 }
					 //�������,׼������
					 Read_DataEncoding(g_EE3Cur, g_EE3State, sendEE3cfgbuf);
					 //����������Ϣ
				     if(send( sockReceDecOrderSvr, &sendEE3cfgbuf, sizeof(sendEE3cfgbuf), 0 )<0)
				     {
				     	bIsError = TRUE;
					 	break;
					 }
					 break;

				  //6����������Ϣ
				  case EE3_SETCONFIG:
					 //������������
				     if(recv( sockReceDecOrderSvr, (char *)(&ee3cfgBuffer), sizeof(ee3cfgBuffer), 0 )<0)
				     {
						bIsError = TRUE;
                     	break;
					 }
					 //д����������
					 Write_ConfigData(ee3cfgBuffer);
					 //������Ϣ
					 reply.command = EE3_SETCONFIG;
					 reply.bufsize = 1;
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
				     {
						bIsError = TRUE;
                     	break;
					 }
					 break;

				  //7�������
				  case REBOOT:
					 //������Ϣ
					 reply.command = REBOOT;
					 reply.bufsize = 1;
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
				     {
						bIsError = TRUE;
                     	break;
					 }
				     Roseek_Reset();
					 break;

				  //8ȡ����ǰ����
				  case CANCELCMD:
					 //������Ϣ
					 reply.command = CANCELCMD;
					 reply.bufsize = 1;
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0)
				     {
						bIsError = TRUE;
                     	break;
					 }
					 //������ɽ����������ӣ�new��
					 bIsError = TRUE;				     
					 break;

			      default:
	                 break;
			}
			

		}//while recv
				
		//�ر����ӣ��ص���ѭ��
		fdClose(sockReceDecOrderSvr);
	}//����ѭ��
	
}

/**
 * \function 	StateDataPackup��ȡ�����ǰ������Ϣ�������ʽ��װ
 * \brief    	���룺Cfg��state��ǰ������в���
 * \			�����*Sendbuf�������ַ���
 * \			ע�⣺1��������˳�򲻿ɵ�����ֻ��������ӡ�
 * \
**/
static void Read_DataEncoding(EE3_CfgStruct Cfg, EE3_StateStruct state, char *Sendbuf)
{
	char runmode[5];
	char cameraip[15];
	char resolution[5];
	char temperature[5];
	char serverip[15];
	char serverport[7];
	char roadnum[10];
	char sroad1[5];
	char sroad2[5];
	char uploadtime[5];
	char nighttrafficstream[5];
	char version[40];
	char isreadflash[5];
	float   ftmpnow;
	unsigned int serverIp;
		
	//�밴˳������װ
	//��1�У�RunMode
	sprintf(runmode,"%d",(int)(Cfg.RunMode));
	strcat(runmode,"\r\n");
	//��2�У�CamraIp,���IP
	sprintf(cameraip,"%d",g_IPAddrCurUse);
	strcat(cameraip,"\r\n");
	//��3�У�Resolution,����ֱ���
	sprintf(resolution,"%d",(int)(state.Resolution));
	strcat(resolution,"\r\n");
	//��4�У�Temperature������¶�
	ftmpnow = Roseek_Read_CurTemperature(); 	
	Roseek_Waitusec( 10 );				 					
	sprintf(temperature,"%d",(int)(ftmpnow));
	strcat(temperature,"\r\n");
	//��5�У�ServerIp
	serverIp = inet_addr(Cfg.ServerIp);
	sprintf(serverip,"%d",serverIp);
	strcat(serverip,"\r\n");
	//��6�У�ServerPort
	sprintf(serverport,"%d",(int)(Cfg.ServerPort));
	strcat(serverport,"\r\n");
	//��7�У�RoadNum
	sprintf(sroad1,"%d",Cfg.RoadNum[0]);
	sprintf(sroad2,"%d",Cfg.RoadNum[1]);
	strcat(sroad1,"\r\n");
	strcat(sroad2,"\r\n");
	strcpy((char *)(&roadnum),sroad1);
	strcat((char *)(&roadnum),sroad2);
	//��8�У�UploadTime
	sprintf(uploadtime,"%d",(int)(Cfg.UploadTime));
	strcat(uploadtime,"\r\n");
	//��9�У�NightTrafficStream
	sprintf(nighttrafficstream,"%d",(int)(Cfg.NightTrafficStream));
	strcat(nighttrafficstream,"\r\n");
	//��10�У�Version
	strcpy(version,state.Version);
	strcat(version,"\r\n");
	//��11�У�Isreadflash
	sprintf(isreadflash,"%d",(int)(Cfg.Isreadflash));
	strcat(isreadflash,"\r\n");
	//�˴��������
	
	//��Ϣ���
	strcpy((char *)(Sendbuf),runmode);//1
	strcat((char *)(Sendbuf),cameraip);//2
	strcat((char *)(Sendbuf),resolution);//3
	strcat((char *)(Sendbuf),temperature);//4
	strcat((char *)(Sendbuf),serverip);//5
	strcat((char *)(Sendbuf),serverport);//6
	strcat((char *)(Sendbuf),roadnum);//7
	strcat((char *)(Sendbuf),uploadtime);//8
	strcat((char *)(Sendbuf),nighttrafficstream);//9
	strcat((char *)(Sendbuf),version);//10
	strcat((char *)(Sendbuf),isreadflash);//11
	//�˴��������
	
}

/**
 * \function 	Write_ConfigData��ȡ������Ϣ��д��FLASH
 * \brief    	���룺ee3cfgBuffer���ܵ���������Ϣ
 * \			������IP��ַ�����޸�IP
 * \			ע�⣺
 * \update		13.12.31����BUG����ֹ�����IP��ַ����
**/
static void Write_ConfigData(Uint8 *ee3cfgBuffer)
{
	int i, j;
	static  int     ee3_mode;
	static  Bool  bSchange = FALSE;
	static  Bool  bIsIP = FALSE;
	char    scommonmode[80];
	char    sfmode[30];
	char    StringIP[30];
	unsigned int  tempIP;

	//��ȡ���й���
	ee3_mode = (int)(ee3cfgBuffer[0] - '0');//��ȡ����ģʽ
	//������Ϣת��
	for(i = 0,j = 0; ;i++)
	{
		if(ee3cfgBuffer[i] == '\v')
			break;
		if(ee3cfgBuffer[i] == '\f')
		{
			bSchange = TRUE;
			continue;							
		}
		if(!bSchange)
			scommonmode[i] = ee3cfgBuffer[i];
		else
		{
			sfmode[j] = ee3cfgBuffer[i];
			j++;
		}						
	}

	//��IP��ַ���޸�
	for(i = 2;i < 30;i++)
	{
		//'\r'Ϊ������
		if(ee3cfgBuffer[i]!='\r')
		{
			//�ж��Ƿ�ΪIP
			if(('0' <= ee3cfgBuffer[i] && ee3cfgBuffer[i] <= '9' )|| ee3cfgBuffer[i] == '.')
			{
				StringIP[i-2]=ee3cfgBuffer[i];
				bIsIP = TRUE;
			}
			else
			{
				bIsIP = FALSE;
			}
		}
		else
		{
			StringIP[i-2]='\0';//����ַ���β
			break;
		}
	}
	if(bIsIP == TRUE)
	{
		tempIP = inet_addr(StringIP);
		//�޸������IP��ַ
		Roseek_Save_IP_Address( tempIP );
	}
	//д��Flash
	Roseek_Flash_Burn((Uint8 *)scommonmode,CFGCOMMONADD,80);//�洢ͨ��������Ϣ
	switch(ee3_mode)
	{
		case VEHICLE_DETECT:
			Roseek_Flash_Burn((Uint8 *)sfmode,CFGVDADD,30);
			break;
		case QUEUE_DETECT:
			Roseek_Flash_Burn((Uint8 *)sfmode,CFGQDADD,30);
			break;
		case TEST_MODE:
			break;
		default:
			break;
	}
	
}




