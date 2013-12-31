/**
 * \file      	EE3_ImgAcq.c 
 * \author    	LQ
 * \version   	0.1.3
 * \date      	2013.12.31
 * \brief     	ͼ��ɼ�ģ��(��ͼ�����̴߳���)
 * \update      ��������              
**/

#include "EE3_common.h"

/**************************************************************************/
//�ⲿ����
extern volatile Bool            g_bIsDpDataClientConnect;
extern volatile	Bool			g_bIsTrgClientConnect;
extern volatile	Bool			g_bIsFconClientConnect;
extern volatile	Uint8			g_ui8TmpTrgMode;
extern volatile float			g_fTmpSpeed;
extern volatile Uint8			g_ui8TmpTrgTime[8];
extern volatile Bool            BIsTrgFrameofBack;
extern RoseekImgBufStruct		g_ImgBufStruct[]; //ͼ��ԭʼ���ݻ���ṹ��
extern RoseekImgInfStruct		g_ImgInfStruct;  //ץ����Ϣ�ṹ��
extern RoseekTrgCtrlInfStruct 	g_TrgCtrlInfStruct; //ץ�Ŀ�����Ϣ�ṹ��
/**************************************************************************/
//��������
int hwiImgAcqOverProcessFunc();
/**************************************************************************/
volatile enum Roseek_SensorRunMode enumRunMode;
Uint8 g_ui8BufIndex = 0;
Uint32 B1YRGB[4],B2YRGB[4],B3YRGB[4],B4YRGB[4],B5YRGB[4];
float ftmp;
/**************************************************************************/
/**
 * \function 	hwiFrameValidFunc
 * \brief    	֡��Ч�ж���Ӧ����(Ӳ���ж�)
 * \			ÿ��һ֡ͼ���������ݴ�FPGA��ʼ�����DSP�Ŀ�ʼ�����жϻᱻϵͳ�Զ�����		
 * \note		1	�û������ж���Ӧ�����ڽ��������ͼ��ԭʼ���ݵ�bufferָ�����õ�EDMA�����ú�����
 * \			2	�������û����ı�������
**/					
int hwiFrameValidFunc()
{
	Uint32 i,j;
	int iTrgSur;
	enum Roseek_ImgAcquisition_Format enumImgFormat;
	static Bool bFreeBufferHasBeenFound = FALSE;
	Uint8 ui8ScaleLel;

	//�ж�ͼ��ɼ����ں���ģʽ
	enumRunMode = Roseek_Get_CurRunMode();
	enumImgFormat = Roseek_Get_CurImgFormat();
	ui8ScaleLel = Roseek_Get_TrgFconMode_ScaleLevel();	

	//��ȡ����Դ��ʾ�����ڴ���ģʽ����Ч
	iTrgSur = Roseek_Get_TrgSurFlag();
	//Ѱ�ҿջ���
	j = 0;
	bFreeBufferHasBeenFound = FALSE;
	for( i=g_ui8BufIndex; i<g_ui8BufIndex+6; i++ )
	{
		j++;
		if( i>5 ){
			if( g_ImgBufStruct[i-6].bProcessLocked ){   //�������ݼ�����־��TRUE��ʾ�������ڱ�����FALSE��ʾ���ݿ���
			}
			else{
				bFreeBufferHasBeenFound = TRUE;
				g_ui8BufIndex = i-6;
				break;
			}
		}
		else{
			if( g_ImgBufStruct[i].bProcessLocked ){
			}
			else{
				bFreeBufferHasBeenFound = TRUE;
				g_ui8BufIndex = i;
				break;
			}
		}
	}
	if( ( j==6 )&&(!bFreeBufferHasBeenFound) )  //������û���ҵ��ջ���
	{
		return 0;
	}

	g_ImgBufStruct[g_ui8BufIndex].ui8RotateFlag = Roseek_Get_ImgRotateMode();//��ѯ��ǰͼ��ɼ���ת�Ƕ�

	if( enumRunMode==TrgMode ){
		if( g_TrgBrightnessAdj_Params.bIsAllowBrightnessAdj ){
			if( ((g_TrgBrightnessAdj_Params.ui32PIShutterWidthHigh_us - g_TrgBrightnessAdj_Params.ui32CurrentShutterWidth_us ) <= 200) ){//����ҹ��ģʽ
				Roseek_Enable_TrgFlashlamp_Syn_Out();
				Roseek_Set_ImgEnhancement_Level( IMG_ENHANCEMENT_LEVEL1 );
				g_TrgCtrlInfStruct.bEnableADLI = FALSE;
			}
			else{
				Roseek_Set_ImgEnhancement_Level( IMG_ENHANCEMENT_LEVEL3 );
				Roseek_Disable_TrgFlashlamp_Syn_Out();
				g_TrgCtrlInfStruct.bEnableADLI = TRUE;
			}
		}
	}
	g_ImgBufStruct[g_ui8BufIndex].CurImg_CorrespondingRunMode = enumRunMode;
	g_ImgBufStruct[g_ui8BufIndex].CurImg_Format = enumImgFormat;
	g_ImgBufStruct[g_ui8BufIndex].iTrgSurFlag = iTrgSur;
	g_ImgBufStruct[g_ui8BufIndex].ui8TrgFconScaleLel = ui8ScaleLel;
	//����EDMA����ԭʼ���ݵİ���P16����34��
	Roseek_ImgAcqChannel_Config( enumRunMode, enumImgFormat, g_ImgBufStruct[g_ui8BufIndex].pui8FrameBuf, EDMA_OPT_PRI_LOW );
	return 0;
}


/**
 * \function 	hwiImgAcqOverProcessFunc
 * \brief    	���ݲɼ�����ж�(Ӳ���ж�)
 * \			ÿ��һ֡ͼ���������ݱ�EDMAģ����Ƶ��û�ָ����buffer�к󣬸��жϻᱻϵͳ�Զ�����		
 * \			֪ͨ�û�ͼ�������Ѿ��������ָ����buffer�У��û����ڸú����н��Ѿ��ɼ���ͼ������������Ϣ��д��ͼ�����ݵĽṹ����
 * \note		1	���жϺ�������Ч���õ�ǰ��������Ч�ж��е�Roseek_ImgAcqChannel_Config����Ч����
 * \			2   �������û����ı�������
 * \            3   ��ʱ�������״̬
**/	
int hwiImgAcqOverProcessFunc()
{
	Bool	bClientConnect = FALSE;
	//����ɼ�����ж����ȱ��ݵ�ǰ����ģʽ�ͻ�������
	enum Roseek_SensorRunMode enumCurRunMode;
	Uint8 g_ui8CurBufIndex;
	enumCurRunMode = enumRunMode;
	g_ui8CurBufIndex = g_ui8BufIndex;
	//����EDMA�ɼ�ͨ��
	Roseek_Clear_ImgAcqChannel();
	//�ж��Ƿ������������Ӧ����
	Roseek_Get_MIBlockYRGBMean( 1, B1YRGB ); //P15����28 ��ȡ��������ƽ������ֵ
	switch( enumCurRunMode ){
		case TrgMode:
			//��Ϊ����ģʽ�����ڴ˽�AFE��������������һ��
			if( g_TrgBrightnessAdj_Params.bIsAllowBrightnessAdj ){//�Ƿ�������ӿ����Զ����ڣ�TRUE:����; FALSE:������
				Roseek_DetectIllum_Adj( enumCurRunMode, *B1YRGB );
			}
			break;
		case FconMode:
			if( g_FconBrightnessAdj_Params.bIsAllowBrightnessAdj ){
				Roseek_DetectIllum_Adj( enumCurRunMode, *B1YRGB );
			}
			break;
		default:
			break;
	}
	//�Ƿ��Զ���ƽ��
	if( g_CameraGeneralCtrlParams.bRunWhitebalance ){
		Roseek_WhiteBalance();//ִ�а�ƽ�����
	}
	//�Դ���ģʽ�����ж�
	if( enumCurRunMode==TrgMode )
	{
		//Ϊ��Чץ����������Ҫ����Ϣ
		g_ImgBufStruct[g_ui8CurBufIndex].ui8TrgMode = g_ui8TmpTrgMode;//������ʽ
		memcpy( g_ImgBufStruct[g_ui8CurBufIndex].ui8TrgShotTime, (Uint8*)g_ui8TmpTrgTime, 8 );//ץ��ʱ��
		switch( g_ui8TmpTrgMode )
		{
			case 0://����
			case 1://����Ȧ����
				g_ImgBufStruct[g_ui8CurBufIndex].fSpeed = 0.0;
				break;
			case 2://˫��Ȧ����
				g_ImgBufStruct[g_ui8CurBufIndex].fSpeed = g_fTmpSpeed;
				break;
			default:
				g_ImgBufStruct[g_ui8CurBufIndex].fSpeed = 0.0;
		}
		
	}
	
	//ȷ����ǰ�ɼ���ͼ�������ģʽ
	switch( g_ImgBufStruct[g_ui8CurBufIndex].CurImg_CorrespondingRunMode ){//��ǰͼ���ں���ģʽ�²ɼ�(TrgMode, FconMode, HfrMode)
		case TrgMode:
			bClientConnect = g_bIsTrgClientConnect;
			break;
		case FconMode:
			bClientConnect = g_bIsFconClientConnect;
			break;
		default:
			break;
	}
	//1������ͼ����ģ��	
    SEM_post(&sem_AcmdBDReady);   	
    //��ǰ֡�������
	g_ImgBufStruct[g_ui8CurBufIndex].bProcessLocked = TRUE;	 
     
	//2������JPEGģ��                                       
 	if( bClientConnect)
 	{                                        
 		SEM_post(&sem_JpegReady); //JPEG��ʼ�ź���            
 	} 
 	                                                         	                                                         	

    return 0;
}




