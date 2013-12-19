/**
 * \file      	RConfig_ConfigMain.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.21
 * \date      	2013.12.7
 * \brief     	Զ�����ù��ܡ���Զ������ͨ�ŵ������߳�
 * \update      �µ������߼�
**/

#include "EE3_common.h"

/**************************************************************************/
//�ⲿ����
extern volatile    Bool    g_bIsSendPictoDec;
extern volatile    ConversationHead 	g_DecCmdsem;
extern volatile    Uint8    readRTCtime[8];
extern volatile    float    Ftmpnow;
extern EE3_CfgStruct g_EE3Cur;//�������״̬
extern EE3_StateStruct g_EE3State;
extern     char    basicCfgSend[60];
extern unsigned int	g_IPAddrCurUse;
/**************************************************************************/
void Read_DataEncoding(EE3_CfgStruct Cfg, EE3_StateStruct state, char *Sendbuf);
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
	int		size,i,j,cnt;
	Uint8 	pointBuffer[80];
	char    sendEE3cfgbuf[150];
	Uint8   ee3cfgBuffer[100];	
	unsigned int  tempIP;
	char    *StringIP;
	char    scommonmode[80];
	char    sfmode[30];
	int     ee3_mode;
	Bool	bIsError = FALSE;
	Bool    bSchange = FALSE;
	struct	sockaddr_in addr; //����ͻ��ĵ�ַ��Ϣ
	ConversationHead reply;

	//Ϊ��ǰ�����������л���
	fdOpenSession( TaskSelf() );

	//���������׽���socket����
	sockReceDecOrderListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//��������������socket,TCPЭ��
	if( sockReceDecOrderListen == INVALID_SOCKET ){
		Roseek_Reset();//���������������ʧ�ܣ��������
	}
	bzero( &addr, sizeof(struct sockaddr_in) );
	addr.sin_family	= AF_INET; //AF_INETָ��ʹ��TCP/IPЭ����
	addr.sin_addr.s_addr = INADDR_ANY;//�Զ���ȡ������ַ
	addr.sin_len	= sizeof( addr );
	addr.sin_port	= htons( CONTROLPORT );//ָ�����ӷ������Ķ˿ں�
	//sockReceDecOrderListen��
	if ( bind( sockReceDecOrderListen, (PSA) &addr, sizeof(addr) ) < 0 ){
		Roseek_Reset();//�����ʧ�ܣ��������
	}
	//sockReceDecOrderListen��ʼ������ͬһʱ�̽�֧��һ������
	if ( listen( sockReceDecOrderListen, 1) < 0 ){
		Roseek_Reset();//�������ʧ�ܣ��������
	}
	//����������������
	while(1)
	{
		size = sizeof( addr );
		sockReceDecOrderSvr = accept( sockReceDecOrderListen, (PSA)&addr, &size );
		if( sockReceDecOrderSvr == INVALID_SOCKET){
			//����������ӳ�����رս��ܵ������Ӷ������³���
			fdClose(sockReceDecOrderSvr);
			continue;
		}
		if( bIsError ){
			//����������ݳ�����ȴ��µ���������
			bIsError = FALSE;
			continue;
		}
		while(!bIsError)
		{	//��������
			cnt=recv( sockReceDecOrderSvr, (char *)(&g_DecCmdsem), sizeof(g_DecCmdsem), 0 );
			if(cnt<0){
			  	fdClose(sockReceDecOrderSvr);	
			  	bIsError = TRUE;
			  	break;
			}
			
			//���ù����������
			switch(g_DecCmdsem.command){
				  //1ѡ�����
			      case CHOOSECAM:
				     reply.command = CHOOSECAM;
					 reply.bufsize = g_DecCmdsem.bufsize;
					 //�����ź�
                     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 break;

				  //2������ͼ��
				  case STARTVIDEO:
				     reply.command = STARTVIDEO;
					 reply.bufsize = 1;
					 //�����ź�
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 SEM_post(&sem_SendVideoReady);//��ʼ��������ͼ�������ź�
				     g_bIsSendPictoDec = TRUE; 
					 break;

				  //3��ֹ����ͼ��
				  case ENDVIDEO:				     
				     reply.command = ENDVIDEO;
					 reply.bufsize = 1;
				     //�����ź�
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 g_bIsSendPictoDec = FALSE;//ֹͣ����ͼ���ź�
					 break;

				  //4������Ȧ����
				  case SETPOINT:
					 //������������
				     if(recv( sockReceDecOrderSvr, (char *)(&pointBuffer), sizeof(pointBuffer), 0 )<0){
				     	bIsError = TRUE;
				     	break;
					 }
					 switch(g_EE3Cur.RunMode){
						case VEHICLE_DETECT:
					 		//�������VD�����ò���д��FLASH�洢����ַCFGVDPOINTADD��ʼ��ǰ80�ֽ�
					 		Roseek_Flash_Burn(pointBuffer,CFGVDPOINTADD,80); 
							break;
						case QUEUE_DETECT:
					 		//�ӳ����QD�����ò���д��FLASH�洢����ַCFGQDPOINTADD��ʼ��ǰ80�ֽ�
					 		Roseek_Flash_Burn(pointBuffer,CFGQDPOINTADD,80);	
					 		break;
					 	default:
					 		break;
					 }
					 //������Ϣ
					 reply.command = SETPOINT;
					 reply.bufsize = 1;
					 //�����ź�
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }																										
					 break;

				  //5�������״̬��Ϣ
				  case EE3_SENDCONFIG:
					 reply.command = EE3_SENDCONFIG;
					 reply.bufsize = sizeof(sendEE3cfgbuf);//���ط��Ͱ��Ĵ�С
					 //�����ź�
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 //�˴��������,׼������
					 Read_DataEncoding(g_EE3Cur, g_EE3State, sendEE3cfgbuf);
					//����������Ϣ
				     if(send( sockReceDecOrderSvr, &sendEE3cfgbuf, sizeof(sendEE3cfgbuf), 0 )<0){
				     	bIsError = TRUE;
					 	break;
					 }
					 break;

				  //6����������Ϣ
				  case EE3_SETCONFIG:
					 //������������
				     if(recv( sockReceDecOrderSvr, (char *)(&ee3cfgBuffer), sizeof(ee3cfgBuffer), 0 )<0){
						bIsError = TRUE;
                     	break;
					 }
					 //��ȡ���й���
					 ee3_mode = (int)(ee3cfgBuffer[0] - '0');//��ȡ����ģʽ
					 //������Ϣת��
					 for(i = 0,j = 0;i < sizeof(ee3cfgBuffer);i++)
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
					 for(i=2;i<30;i++){
					 	if(ee3cfgBuffer[i]=='\r'){
							StringIP[i-2]='\0';
							break;
						}
						StringIP[i-2]=ee3cfgBuffer[i];
					 }
					 tempIP = inet_addr(StringIP);
					 //�޸������IP��ַ
					 Roseek_Save_IP_Address( tempIP );

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
					 //������Ϣ
					 reply.command = EE3_SETCONFIG;
					 reply.bufsize = 1;
					 //�����ź�
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }

					 break;

				  //7�������
				  case REBOOT:
					 reply.command = REBOOT;
					 reply.bufsize = 1;
					 //�����ź�
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
				     Roseek_Reset();
					 break;

				  //8ȡ����ǰ����
				  case CANCELCMD:
					 reply.command = CANCELCMD;
					 reply.bufsize = 1;
					 //�����ź�
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }				     
					 break;

			      default:
	                 break;
			}
			

		}		
		bIsError=FALSE;
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
void Read_DataEncoding(EE3_CfgStruct Cfg, EE3_StateStruct state, char *Sendbuf)
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




