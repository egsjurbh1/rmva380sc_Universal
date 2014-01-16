/**
 * \file      	ITS_ImgMain.c 
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.3.6
 * \date      	2014.1.15
 * \brief     	���ܽ�ͨӦ��----ͼ�������߳�(������⹦�ܡ����г��ȼ�⹦��)
 * \update      �����sendroadinf�߳�ͬ���ź��߼�
**/

#include "EE3_common.h"
#include "SDFS_filesys.h"
#include "EE3_ITSAlg.h"
#include "EE3_Imgfunc.h"
#include "EE3_Imgmain.h"
/**************************************************************************/
static void ITS_VehicleDetect(DpPoint *dpPVD);
static void ITS_QueueDetect(DpPoint  *dpPQD);
/**************************************************************************/
/**
 * \function 	tskAcmdBDmainFunc
 * \brief    	ͼ����ϵͳ�߳�
 * \			��ģ�����ImgAcq���źŴ�����ÿ֡ͼ�񴥷�		
 * \            1��		
 * \            2��
**/	
void tskImgmainFunc()
{
	 Uint8   *pui8IMGbuffer;//��ǰ�ɼ�֡ͼ�񻺴�
	 Uint8	 i;
	 DpPoint dpPointVD[2];
	 DpPoint dpPointQD[2];	 
	 Uint8	 ui8tmp = 0;
	 Bool	 bBreak = FALSE;			//�г���־
	 Bool    bReadOnce = TRUE;
	 Bool    bsemSendtoDecready = FALSE;        
	 static	 Uint8	ui8FrameIndex = 0; //֡�������

	 //׼���㷨��������
	 setwwmeansd( g_bpalgData.ww, g_bpalgData.mean, g_bpalgData.sd);
	 setwwmeansd( g_bpalgData2.ww, g_bpalgData2.mean, g_bpalgData2.sd);	
	 ResetPointData();//��Ȧ����
	     
     while( 1 )
	 {
	    SEM_pend(&sem_AcmdBDReady,SYS_FOREVER);
		TSK_sleep( 2 );
				
		nRunT[0] = Roseek_GetTickCount();//��ʼ����
		
		//******��ȡFLASH�е����ò������������ȡһ�Ρ�
		if(bReadOnce)
		{
			ReadPointDataFromFlash();//���߿���������
			PointDataConfig(dpPointVD, PointDataVD);//���������߿�����
			PointDataConfig(dpPointQD, PointDataQD);//���м����߿�����
			ReadSetInfFormFlash(IsReadFlash);//��������в�������
			//Test_PointDPlus();//�����ã��ֶ��趨�߿��С
			bReadOnce=FALSE;
		}
			
		//�жϵ�ǰ�����ĸ�ԭʼͼ�񻺴�
		bBreak = FALSE;
		if(ui8FrameIndex==5){
			ui8tmp=0;
		}
		else{
			ui8tmp=ui8FrameIndex+1;
		}
		for( i=ui8tmp; i<ui8tmp+6; i++ ){
			if( i>5 ){
				if( g_ImgBufStruct[i-6].bProcessLocked ){
					ui8FrameIndex = i-6;
					bBreak = TRUE;
					break;
				}
			}
			else{//i<=5
				if( g_ImgBufStruct[i].bProcessLocked ){
					ui8FrameIndex = i;
					bBreak = TRUE;
					break;
				}
			}
		}
		if( !bBreak )
		{
			g_ImgBufStruct[ui8FrameIndex].bProcessLocked = FALSE;
			continue;
		}
		g_ImgBufofFrontS.pui8FrameBuf = g_ImgBufStruct[ui8FrameIndex].pui8FrameBuf;//����ǰ��ͼ��
		pui8IMGbuffer = g_ImgBufStruct[ui8FrameIndex].pui8FrameBuf;
		bsemSendtoDecready = g_bIsFcontoDecClientConnect; //����ͼ���߳�׼�����
		//���������ù��ߴ���ͼ���߳��ź���
		if(bsemSendtoDecready)
		{
	    	SEM_post(&sem_FcontoDecReady);
		}
 		//�˴����û������߿���		
		switch(g_EE3Cur.RunMode)
		{
			//�������
			case VEHICLE_DETECT:
				DrawPoint(pui8IMGbuffer, PointDataVD);
				break;
			//�ӳ����
			case QUEUE_DETECT:
				DrawPoint(pui8IMGbuffer, PointDataQD);
				break;
			default:
				break;
		}
		//��� L2 Cache
		BCACHE_wbAll();

		//******ת��ԭʼͼ��YUV422��YUV420��ʽ������ȡY����
		EE3_YUV2YCrCb420(	pui8IMGbuffer,
							g_ImgBufofFrontS.pui8YCrCb_YBuf, g_ImgBufofFrontS.pui8YCrCb_CrBuf, g_ImgBufofFrontS.pui8YCrCb_CbBuf,
							ALG_COL, ALG_ROW );
		
		g_ImgBufStruct[ui8FrameIndex].bProcessLocked = FALSE;//ԭʼͼ�����	

		//�����ж�
		switch(g_EE3Cur.RunMode)
		{
			//�������
			case VEHICLE_DETECT:
				ITS_VehicleDetect(dpPointVD);
				if(g_bIsSendRoadInfConnect)
				{
					SEM_post( &sem_SendRoadInfReady );//֪ͨ���������ź���
				}
				else
				{					
					SendSemCycle();//ÿ��һ��ʱ�䷢�����ź�
				}
				break;
			//�ӳ����
			case QUEUE_DETECT:
				ITS_QueueDetect(dpPointQD);
				break;
			//���Ե���
			case TEST_MODE:
				break;
			default:
				break;
		}	

   }//while
   
}//tsk


/*****************************************************************************
******************************������⹦��***********************************
*****************************************************************************/
static void ITS_VehicleDetect(DpPoint *dpPVD)
{
	//��������㷨����
	static Uint8 ptn = COIL_NUM;
	static Uint8 frame_n = FRAMEOFBACK_NUM;
	static Uint8 gsk = QUEUE_LENGTH;
	float *nMatp;
	Uint8   *pui8inFrameBufY;//��ǰǰ������Y����
	Uint8   *pui8BFrameBufYVD;//������ⱳ��ͼ�񻺴�Y����
	static Uint8 isCar[2];

	//������������ʼ��
	nMatp = (float *)malloc(sizeof(float) * ( gsk * 2 + 1));
	memset(nMatp, 0, sizeof(float) * ( gsk * 2 + 1));		
	GaussianPram(nMatp, gsk);
	pui8inFrameBufY = g_ImgBufofFrontS.pui8YCrCb_YBuf;//��ǰǰ����Y����
	pui8BFrameBufYVD = g_ImgBufofBackVD.pui8YCrCb_YBuf;//����Y����
    
    //������⺯��
	GrayscaleVD(pui8inFrameBufY, g_bpalgData.ww, g_bpalgData.mean, g_bpalgData.sd, dpPVD,
		&ptn, isCar, nMatp, pui8BFrameBufYVD, frame_n);
	//IO������
	VD_SemOut_IO(isCar);					
	free(nMatp);
	nRunT[1] = Roseek_GetTickCount();//��ֹ����
	//ʱ��ͳ��
	Count_RunTime(nRunT);	
}

/*****************************************************************************
***************************���г��ȼ�⹦��**********************************
*****************************************************************************/
static void ITS_QueueDetect(DpPoint *dpPQD)
{
	Uint8   *pui8inFrameBufY;//��ǰǰ������Y����
	Uint8   *pui8BFrameBufY1;//���м�ⱳ��ͼ�񻺴�Y����
	Uint8   *pui8BFrameBufY2;
	static  Bool    bDpSem1 = TRUE;           //��־
	static  Bool    bDpSem2 = TRUE;
	Uint32    ui8Firstip1;
	Uint32    ui8Firstip2;
	Uint32    ui8Lastip1;
	Uint32    ui8Lastip2;
	Uint32    ui8RoadData1;
	Uint32    ui8RoadData2;

	pui8inFrameBufY = g_ImgBufofFrontS.pui8YCrCb_YBuf;//��ǰǰ����Y����
	
	//��һ·״̬���
	if(g_AmcdSem1.bStart){
	      switch(g_AmcdSem1.comm){

			    //"��ת��"״̬,�������ģ
	            case NETINFORTG: 
	            	pui8BFrameBufY1 = g_ImgBufofBackS.pui8YCrCb_YBuf;//������ģ��õı���ͼ��Y����           	          
		            //���ñ�����ģ�㷨
                    gaussian(pui8BFrameBufY1, pui8inFrameBufY, 
                    	g_bpalgData.ww, g_bpalgData.mean, g_bpalgData.sd, 
                    	dpPQD->yMin, dpPQD->yMax, dpPQD->xMin, dpPQD->xMax );
                    break;

			 	//���ƶ�����"���м��"�����ͽ��
		      	case NETREQUEST: 
		      		while(bDpSem1){//һ�����ڽ�ִ��һ��               		               					  
		                  //���öӳ�����㷨
                          GrayscaleQD(pui8BFrameBufY1, pui8inFrameBufY,
                          		dpPQD, &ui8Firstip1, &ui8Lastip1 );
		                  //�ӳ��������
						  ui8RoadData1=ui8Firstip1-ui8Lastip1;
						  //�ݴ��ж�
						  if(ui8RoadData1>0&&ui8RoadData1<999){
                          		g_BpOutputData1.data=abs(ui8RoadData1);//�ӳ�����?
								g_BpOutputData1.agree=1;
						  }
						  else{
								g_BpOutputData1.data=0;
								g_BpOutputData1.agree=0;
						  }
						  g_BpOutputData1.cameraN=g_EE3Cur.RoadNum[0];//������
                          g_BpOutputData1.comm=NETRETURN; //��������    			 			   
		  				  bDpSem1=FALSE;
		  			}
		  			//������м���������ϴ�
		  			SEM_post( &sem_DpDataReady1 ); 								
		  			//����JPEG��������SD���߳�
					//SEM_post( &sem_JpegtoSDReady ); 
					//����IO5���					
					Roseek_Set_Pio_Outvalue(4,1);//IO5,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ 					
					//Roseek_SetISOOut_AsSwitch_State( 0, 1);//ISO-OUT1,����
					TSK_sleep( 2 );            		                                    
                    break; 

				//"��ת��"״̬����ȴ������м��׼��,ִ�д�������
	         	case NETINFOGTR:
					bDpSem1=TRUE;
					Roseek_Set_Pio_Outvalue(4,0);//IO4,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
					//Roseek_SetISOOut_AsSwitch_State( 0, 0);//ISO-OUT1,�ر�
					TSK_sleep( 2 );
		        	break;

	         	default:
	            	break;
	   	  }
		  
	    }

		//�ڶ�·״̬���
		if(g_AmcdSem2.bStart){
	        switch(g_AmcdSem2.comm){

				//"��ת��"״̬,�������ģ
	            case NETINFORTG:
	            	pui8BFrameBufY2 = g_ImgBufofBackS2.pui8YCrCb_YBuf;//������ģ��õı���ͼ��Y����    	          		             	
		            //���ñ����ģ�㷨
                    gaussian(pui8BFrameBufY2, pui8inFrameBufY, 
                    	g_bpalgData2.ww, g_bpalgData2.mean, g_bpalgData2.sd, 
                    	(dpPQD + 1)->yMin, (dpPQD + 1)->yMax, (dpPQD + 1)->xMin, (dpPQD + 1)->xMax );
                    break;

			  	//���ƶ�����"���м��"
		      	case NETREQUEST:
		      		while(bDpSem2){//һ�����ڽ�ִ��һ��    		                 						  
		                  //���ö��м���㷨
                          GrayscaleQD(pui8BFrameBufY2, pui8inFrameBufY,
                          		(dpPQD + 1), &ui8Firstip2, &ui8Lastip2 );
						  
		                  //������ɲ����ݴ��ж�
						  ui8RoadData2=ui8Firstip2-ui8Lastip2;
						  if(ui8RoadData2>0&&ui8RoadData2<999){
                          		g_BpOutputData2.data=abs(ui8RoadData2);//�ӳ������
								g_BpOutputData2.agree=1;
						  }
						  else{
								g_BpOutputData2.data=0;
								g_BpOutputData2.agree=0;
						  }
						  g_BpOutputData2.cameraN=g_EE3Cur.RoadNum[1];//������			  
                          g_BpOutputData2.comm=NETRETURN; //��������		  			 			   
		  				  bDpSem2=FALSE;
		  			}
		  			//������м���������ϴ�
		  			SEM_post( &sem_DpDataReady2 ); 
		  			//����JPEG��������SD���߳�
		  			//SEM_post( &sem_JpegtoSDReady ); 
		  			//����IO6���
					Roseek_Set_Pio_Outvalue(5,1);//IO6,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ 
					//Roseek_SetISOOut_AsSwitch_State( 1, 1);//ISO-OUT2,����
					TSK_sleep( 2 );                                                        		                  
                    break; 

				//"��ת��"״̬����ȴ������м��׼��,ִ�д�������
	         	case NETINFOGTR:
					bDpSem2=TRUE;
					Roseek_Set_Pio_Outvalue(5,0);//IO6,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
					//Roseek_SetISOOut_AsSwitch_State( 1, 0);//ISO-OUT2,�ر�
					TSK_sleep( 2 );
		       		break;

	         	default:
	           		break;
	     	}

	  	}
}



