/**
 * \file      	RServer_SendRoadInf.c
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.04
 * \date      	2013.12.17
 * \brief     	向远程服务器发送消息功能——发送车辆检测数据的网络线程
 * \update      
**/				

#include "EE3_common.h"
/**************************************************************************/
//外部变量
extern EE3_CfgStruct g_EE3Cur;//相机运行状态
extern CarInfo g_carInfo;//车辆检测信息
extern volatile    Bool    g_bIsSendRoadInfConnect;
/**************************************************************************/

/**
 * \function 	SendRoadInfPort
 * \brief    	使用TCP/IP协议，向向远程服务器发送车辆检测数据
 * \			作为客户端	
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
	    	//为当前任务配置运行环境
			fdOpenSession( TaskSelf() );
			bzero( &connaddr,sizeof(struct sockaddr_in));
			connaddr.sin_family = AF_INET;
			connaddr.sin_addr.s_addr = INADDR_ANY;
			connaddr.sin_len = sizeof( connaddr );
			connaddr.sin_port = htons(0);
			//套接字socket对象,TCP协议
			sockTCPsendroadinf = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			if( sockTCPsendroadinf == INVALID_SOCKET ){
		         Roseek_Reset();//如果创建侦听对象失败，重启相机
	    	}
			if (bind(sockTCPsendroadinf, (PSA) &connaddr,sizeof(connaddr))){
			     Roseek_Reset();//如果绑定失败，重启相机
			}	 	
    		bzero( &serveraddr,sizeof(struct sockaddr_in));
			serveraddr.sin_family = AF_INET;
    		serveraddr.sin_len = sizeof( serveraddr );
    		serveraddr.sin_port = htons(g_EE3Cur.ServerPort);//端口号
			inet_aton(g_EE3Cur.ServerIp, &serveraddr.sin_addr); //指明服务端IP地址
			//连接服务端
			cnt=connect( sockTCPsendroadinf, (PSA)&serveraddr, serveraddr.sin_len);
			if(cnt == -1){
             	//如果连接出错则关闭接受到的连接对象，重新尝试
			 	fdClose(sockTCPsendroadinf);
			 	continue;
			}
			//网络线程准备完毕
			g_bIsSendRoadInfConnect = TRUE;
			//IP地址信息存入
			strcpy(info[0].local_ip, g_EE3Cur.CameraIp);
			strcpy(info[1].local_ip, g_EE3Cur.CameraIp);
	
			//循环计时
			t_start =  Roseek_GetTickCount();
			while(1)
			{						
	    		SEM_pend(&sem_SendRoadInfReady,SYS_FOREVER);
	    		t_end = Roseek_GetTickCount();
				//指定上传周期
	    		if( abs(t_end - t_start) >= (g_EE3Cur.UploadTime * 1000000))
	    		{      	    	           					          
			    	//生成发送信息		
					for(i = 0;i < 2;i++)
					{
						info[i].label = i + 1;
						info[i].num = g_carInfo.num[i];
						info[i].speed = g_carInfo.speed[i];			
						g_carInfo.num[i] = 0;
						g_carInfo.speed[i] = 0;
					}
					//发送数据至服务端
			    	len = send( sockTCPsendroadinf, &(info), 2 * sizeof(SendInfo), 0 );
					if(len < 0)
					{
						//关闭重连
						break;
					}
					//重新计时					
					t_start = Roseek_GetTickCount();
			    	
			    }  		    
	  		}
	  		//控制标志复位，关闭发送服务器连接，重回迭代循环等待新连接		
			fdClose(sockTCPsendroadinf);
			g_bIsSendRoadInfConnect = FALSE;
			SEM_reset( &sem_SendRoadInfReady, 0 );
									
	}
}
