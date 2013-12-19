/**
 * \file      	RConfig_ConfigMain.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.21
 * \date      	2013.12.7
 * \brief     	远程配置功能——远程配置通信的网络线程
 * \update      新的配置逻辑
**/

#include "EE3_common.h"

/**************************************************************************/
//外部变量
extern volatile    Bool    g_bIsSendPictoDec;
extern volatile    ConversationHead 	g_DecCmdsem;
extern volatile    Uint8    readRTCtime[8];
extern volatile    float    Ftmpnow;
extern EE3_CfgStruct g_EE3Cur;//相机运行状态
extern EE3_StateStruct g_EE3State;
extern     char    basicCfgSend[60];
extern unsigned int	g_IPAddrCurUse;
/**************************************************************************/
void Read_DataEncoding(EE3_CfgStruct Cfg, EE3_StateStruct state, char *Sendbuf);
/**************************************************************************/

/**
 * \function 	ReceDecOrderPort
 * \brief    	使用TCP/IP协议，远程配置工具通信，配置信息的接收、解析与参数传回
 * \			默认作为服务器端，Port = 4500
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
	struct	sockaddr_in addr; //保存客户的地址信息
	ConversationHead reply;

	//为当前任务配置运行环境
	fdOpenSession( TaskSelf() );

	//创建侦听套接字socket对象
	sockReceDecOrderListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);//创建服务器监听socket,TCP协议
	if( sockReceDecOrderListen == INVALID_SOCKET ){
		Roseek_Reset();//如果创建侦听对象失败，重启相机
	}
	bzero( &addr, sizeof(struct sockaddr_in) );
	addr.sin_family	= AF_INET; //AF_INET指明使用TCP/IP协议族
	addr.sin_addr.s_addr = INADDR_ANY;//自动获取本机地址
	addr.sin_len	= sizeof( addr );
	addr.sin_port	= htons( CONTROLPORT );//指明连接服务器的端口号
	//sockReceDecOrderListen绑定
	if ( bind( sockReceDecOrderListen, (PSA) &addr, sizeof(addr) ) < 0 ){
		Roseek_Reset();//如果绑定失败，重启相机
	}
	//sockReceDecOrderListen开始监听，同一时刻仅支持一个连接
	if ( listen( sockReceDecOrderListen, 1) < 0 ){
		Roseek_Reset();//如果侦听失败，重启相机
	}
	//迭代接受配置命令
	while(1)
	{
		size = sizeof( addr );
		sockReceDecOrderSvr = accept( sockReceDecOrderListen, (PSA)&addr, &size );
		if( sockReceDecOrderSvr == INVALID_SOCKET){
			//如果接受连接出错则关闭接受到的连接对象，重新尝试
			fdClose(sockReceDecOrderSvr);
			continue;
		}
		if( bIsError ){
			//如果接受数据出错，则等待新的连接连接
			bIsError = FALSE;
			continue;
		}
		while(!bIsError)
		{	//接受命令
			cnt=recv( sockReceDecOrderSvr, (char *)(&g_DecCmdsem), sizeof(g_DecCmdsem), 0 );
			if(cnt<0){
			  	fdClose(sockReceDecOrderSvr);	
			  	bIsError = TRUE;
			  	break;
			}
			
			//配置功能命令解析
			switch(g_DecCmdsem.command){
				  //1选择相机
			      case CHOOSECAM:
				     reply.command = CHOOSECAM;
					 reply.bufsize = g_DecCmdsem.bufsize;
					 //返回信号
                     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 break;

				  //2请求发送图像
				  case STARTVIDEO:
				     reply.command = STARTVIDEO;
					 reply.bufsize = 1;
					 //返回信号
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 SEM_post(&sem_SendVideoReady);//开始建立传输图像连接信号
				     g_bIsSendPictoDec = TRUE; 
					 break;

				  //3终止发送图像
				  case ENDVIDEO:				     
				     reply.command = ENDVIDEO;
					 reply.bufsize = 1;
				     //返回信号
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 g_bIsSendPictoDec = FALSE;//停止传输图像信号
					 break;

				  //4虚拟线圈配置
				  case SETPOINT:
					 //接收配置数据
				     if(recv( sockReceDecOrderSvr, (char *)(&pointBuffer), sizeof(pointBuffer), 0 )<0){
				     	bIsError = TRUE;
				     	break;
					 }
					 switch(g_EE3Cur.RunMode){
						case VEHICLE_DETECT:
					 		//车辆检测VD框配置参数写入FLASH存储区地址CFGVDPOINTADD开始的前80字节
					 		Roseek_Flash_Burn(pointBuffer,CFGVDPOINTADD,80); 
							break;
						case QUEUE_DETECT:
					 		//队长检测QD框配置参数写入FLASH存储区地址CFGQDPOINTADD开始的前80字节
					 		Roseek_Flash_Burn(pointBuffer,CFGQDPOINTADD,80);	
					 		break;
					 	default:
					 		break;
					 }
					 //反馈信息
					 reply.command = SETPOINT;
					 reply.bufsize = 1;
					 //返回信号
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }																										
					 break;

				  //5发送相机状态信息
				  case EE3_SENDCONFIG:
					 reply.command = EE3_SENDCONFIG;
					 reply.bufsize = sizeof(sendEE3cfgbuf);//返回发送包的大小
					 //返回信号
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
					 //此处打包数据,准备发送
					 Read_DataEncoding(g_EE3Cur, g_EE3State, sendEE3cfgbuf);
					//发送配置信息
				     if(send( sockReceDecOrderSvr, &sendEE3cfgbuf, sizeof(sendEE3cfgbuf), 0 )<0){
				     	bIsError = TRUE;
					 	break;
					 }
					 break;

				  //6接收配置信息
				  case EE3_SETCONFIG:
					 //接收配置数据
				     if(recv( sockReceDecOrderSvr, (char *)(&ee3cfgBuffer), sizeof(ee3cfgBuffer), 0 )<0){
						bIsError = TRUE;
                     	break;
					 }
					 //读取运行功能
					 ee3_mode = (int)(ee3cfgBuffer[0] - '0');//读取功能模式
					 //配置信息转存
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

					 //读IP地址并修改
					 for(i=2;i<30;i++){
					 	if(ee3cfgBuffer[i]=='\r'){
							StringIP[i-2]='\0';
							break;
						}
						StringIP[i-2]=ee3cfgBuffer[i];
					 }
					 tempIP = inet_addr(StringIP);
					 //修改相机的IP地址
					 Roseek_Save_IP_Address( tempIP );

					 //写入Flash
					 Roseek_Flash_Burn((Uint8 *)scommonmode,CFGCOMMONADD,80);//存储通用配置信息
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
					 //反馈信息
					 reply.command = EE3_SETCONFIG;
					 reply.bufsize = 1;
					 //返回信号
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }

					 break;

				  //7重启相机
				  case REBOOT:
					 reply.command = REBOOT;
					 reply.bufsize = 1;
					 //返回信号
				     if(send(sockReceDecOrderSvr,&reply,sizeof(reply),0)<0){
						bIsError = TRUE;
                     	break;
					 }
				     Roseek_Reset();
					 break;

				  //8取消当前操作
				  case CANCELCMD:
					 reply.command = CANCELCMD;
					 reply.bufsize = 1;
					 //返回信号
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
		//关闭连接，回迭代循环
		fdClose(sockReceDecOrderSvr);
	}//迭代循环
	
}

/**
 * \function 	StateDataPackup读取相机当前配置信息，打包格式封装
 * \brief    	输入：Cfg、state当前相机运行参数
 * \			输出：*Sendbuf打包后的字符流
 * \			注意：1、参数行顺序不可调整，只能逐行添加。
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
		
	//请按顺序打包封装
	//第1行，RunMode
	sprintf(runmode,"%d",(int)(Cfg.RunMode));
	strcat(runmode,"\r\n");
	//第2行，CamraIp,相机IP
	sprintf(cameraip,"%d",g_IPAddrCurUse);
	strcat(cameraip,"\r\n");
	//第3行，Resolution,相机分辨率
	sprintf(resolution,"%d",(int)(state.Resolution));
	strcat(resolution,"\r\n");
	//第4行，Temperature，相机温度
	ftmpnow = Roseek_Read_CurTemperature(); 	
	Roseek_Waitusec( 10 );				 					
	sprintf(temperature,"%d",(int)(ftmpnow));
	strcat(temperature,"\r\n");
	//第5行，ServerIp
	serverIp = inet_addr(Cfg.ServerIp);
	sprintf(serverip,"%d",serverIp);
	strcat(serverip,"\r\n");
	//第6行，ServerPort
	sprintf(serverport,"%d",(int)(Cfg.ServerPort));
	strcat(serverport,"\r\n");
	//第7行，RoadNum
	sprintf(sroad1,"%d",Cfg.RoadNum[0]);
	sprintf(sroad2,"%d",Cfg.RoadNum[1]);
	strcat(sroad1,"\r\n");
	strcat(sroad2,"\r\n");
	strcpy((char *)(&roadnum),sroad1);
	strcat((char *)(&roadnum),sroad2);
	//第8行，UploadTime
	sprintf(uploadtime,"%d",(int)(Cfg.UploadTime));
	strcat(uploadtime,"\r\n");
	//第9行，NightTrafficStream
	sprintf(nighttrafficstream,"%d",(int)(Cfg.NightTrafficStream));
	strcat(nighttrafficstream,"\r\n");
	//第10行，Version
	strcpy(version,state.Version);
	strcat(version,"\r\n");
	//第11行，Isreadflash
	sprintf(isreadflash,"%d",(int)(Cfg.Isreadflash));
	strcat(isreadflash,"\r\n");
	//此处继续添加
	
	//信息封包
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
	//此处继续添加
	
}




