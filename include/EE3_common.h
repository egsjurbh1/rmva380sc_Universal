//////////////////////////////////////////////////////////////////////
// File name: EE3_common.h      		                            //    
//////////////////////////////////////////////////////////////////////

/**
 * \file      	EE3_common.h
 * \author    	LQ
 * \version   	0.05
 * \date      	2012.12.1
 * \brief     	DSP Demo�����е��õ��ĺ궨�壬����ؽṹ�塣
 *				���ļ��е����������û�������Ҫ�Զ�����ġ�
 *				�û�������Դ�ļ�����Ҫ������ͷ�ļ�
 *              �����£��������ṹ��
**/

#ifndef	_EE3_COMMON_H
#define	_EE3_COMMON_H

#include <std.h>
#include <tsk.h>
#include <c64.h>
#include <stdlib.h>
#include <csl_types.h>
#include <soc.h>
#include <bcache.h>
#include <cslr.h>
#include <stdio.h>
#include <log.h>
#include <tistdtypes.h>
#include <cslr_cache.h>
#include <c6x.h>
#include <cslr_i2c.h>
#include <cslr_pllc1.h>
#include <cslr_tmr.h>
#include <cslr_psc.h>
#include <cslr_sys.h>
#include <string.h>
#include <math.h>
#include <netmain.h>
#include "EE3_380_lib.h"
#include "EE3cfg.h"
#include "roseek_ee3_rtsp.h"


//ͼ�����㷨�����зֱ���	
#define ALG_ROW                         544u   
#define ALG_COL                         960u 
//����Mode����
#define TEST_MODE                       0       //��ͼ������ģʽ�������ã�
#define QUEUE_DETECT					1      	//����һ���ӳ����
#define VEHICLE_DETECT					2		//���ܶ����������
#define WORD_NUM 	7

/**********��������й�***********/
#define NUM_RUMTIME 	100     	//ͳ�����д���
#define QUEUE_LENGTH 	11		//��˹�˲�����
#define COIL_NUM     	2  		//��Ȧ��
#define FRAMEOFBACK_NUM	5		//ÿ5֡һ�α�����ģ	

/***���̨����ϵͳ��ͨ��Э��***/
#define ACMDPORT 	6366		//�˿�
#define HEADCODE 	0xFFFF		//Э��ͷ
#define NETREQUEST 	100			//������ͷ����ӳ�
#define NETRETURN  	101			//����ͷ���ضӳ�
#define NETINFORTG 	103			//֪ͨ��ת��
#define NETINFOGTR 	104	    	//֪ͨ��ת��

/***��Զ�����ù��ߵ�ͨ��Э��***/
//ͨ�Ŷ˿�
#define CONTROLPORT 4500
#define VIDEOPORT 	4501
#define UDPPORT   	4502 
//�������� 
#define DEVICETYPE		62  //�豸����EagleEye3��
#define BROADCAST1 	   	0
#define GETBACKGROUND  	1  
#define SETPOINT    	3   //������Ȧ��������
#define REBOOT      	4   //�������
#define STARTVIDEO  	7   //��ʼ����Video
#define ENDVIDEO    	8   //ֹͣ����Video
#define CHOOSECAM   	10  //ѡ�����
#define EE3_SENDCONFIG	16  //����������÷���
#define EE3_SETCONFIG	17	//����������ý���
#define CANCELCMD    	20  //ȡ����ǰ����
#define UDPCON     		99  //UDP����
#define UDPSETTIME 		100 //UDP�㲥ϵͳʱ��
/***���������Ϣ�����ַ���Զ���***/
#define	CFGCOMMONADD					0x102000	//length = 80 Byte���洢ͨ��������Ϣ
#define CFGVDADD						0x102100	//length = 30 Byte���洢��������������
#define CFGQDADD						0x102200	//length = 30 Byte���洢�ӳ�����������
#define CFGVDPOINTADD					0x102300	//length = 80 Byte���洢������������߿�����
#define CFGQDPOINTADD					0x102400	//length = 80 Byte���洢�ӳ���������߿�����

/***ץ����Ϣ�����ַ,DEMOʹ�ã��û����޸�***/
#define	TRGINFFLAGADD					0x000000	//length = 1 Byte
#define	TRGINFADD						0x000001	//length = 256 Byte
#define	TRGCTRLINFADD					0x000101	//length = 256 Byte
#define	TRGCTRLPARAMADD					0x000201	//length = 256 Byte
#define	NETPORTINITFLAGADD				0x000301	//length = 1 Byte, 0x55������ʾ�Ѵ���Ϣ,ϵͳ�ϵ罫�����Ѵ���Ϣ�����г�ʼ��,����ʹ��Ĭ����Ϣ��ʼ��
#define	NETPORTINFADD					0x000302	//length = 12 Byte, ����˳������ΪTrg,Fcon,Hfr;��ռ��4���ֽڣ�Ϊ�޷���������

/**************************************************************************/
//ͼ��ԭʼ���ݻ���ṹ�壬��ʵ�������������洢�ɼ�ģ����յ���ͼ��ԭʼ���ݼ������Ϣ
typedef struct ImgBufStruct{
	Uint8	*pui8FrameBuf;				//ͼ�񻺴�
	Uint8	ui8RotateFlag;				//ͼ����ת��־
	Uint8	ui8TrgFconScaleLel;			//������ȫ�ֱ�������ģʽ�µ����ż���
	Bool	bProcessLocked;				//�������ݼ�����־��TRUE��ʾ�������ڱ�����FALSE��ʾ���ݿ���
	int		iTrgSurFlag;				//��ͼ��ɼ��ڴ���ģʽ�£���ʾ����ͼ���������ĸ�����Դ
	Uint8	ui8TrgShotTime[8];			//����ģʽ�µ�ץ��ʱ�䣬��-��-��-����-ʱ-��-��-ʮ�����룬����ģʽ�²�ʹ�ã���Ч
	Uint8	ui8TrgMode;					//������ʽ��0->����;1->����Ȧ;2->˫��Ȧ(����)������ģʽ��Ч
	float	fSpeed;						//����ģʽ�³����ٶȣ���˫��Ȧ������Ч������ģʽ��Ч
	Bool    iTrgFrameofBack;             //�Ƿ���Ϊ����ͼ��
	enum Roseek_SensorRunMode CurImg_CorrespondingRunMode;	//��ǰͼ���ں���ģʽ�²ɼ�(TrgMode, FconMode, HfrMode)
	enum Roseek_ImgAcquisition_Format CurImg_Format;			//��ǰͼ��Ϊ����ģʽ(RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY)
}RoseekImgBufStruct;
/**************************************************************************/

/**************************************************************************/
//JPEGͼ�񻺴�ṹ�壬��ʵ�������������洢JPEG����ģ���õ����м����ݣ��Լ������JPEGͼ������
typedef struct JPGEncBufStruct{
	Uint8	*pui8YCrCb_YBuf;			//����������ͼ���Y��������ָ��
	Uint8	*pui8YCrCb_CrBuf;			//����������ͼ���Cr��������ָ��
	Uint8	*pui8YCrCb_CbBuf;			//����������ͼ���Cb��������ָ��
	Uint8	*pui8JpgImgBuf[2];			//����������ķǴ�����ʽJPEGͼ��Ļ�������
	Uint8	*pui8TrgJpgImgBuf[6];		//����������Ĵ�����ʽJPEGͼ��Ļ�������
	Bool	bJpgImgBuf_Lock[2];			//����������ķǴ�����ʽJPEGͼ��Ļ������е�������Ч�Ա�־λ
	Bool	bTrgJpgImgBuf_Lock[6];		//����������Ĵ�����ʽJPEGͼ��Ļ�������������Ч�Ա�־λ
}RoseekJPGEncBufStruct;
/**************************************************************************/

/**************************************************************************/
//DEMO�и����Ķ��壬��114���ֽڣ��û��ɸ���ʵ������޸�
//ץ����Ϣ�ṹ�壬��ʵ�����������ڴ洢�ϴ���ͼ��������Ϣ
typedef struct ImgInfStruct{
	char	ui8LocusInf[101];			//ץ�ĵص���Ϣ����50������
	Uint8	ui8TimeInf[8];				//ץ��ʱ�䣬��-��-��-����-ʱ-��-��-10������
	Uint8	ui8WayNum;					//������
	Uint8	ui8TrgMode;					//������ʽ��0->����;1->����Ȧ;2->˫��Ȧ(����)
	float	fSpeed;						//����(��λ:����/Сʱ)   
	Bool    bIsTrgpic;                  //�Ƿ���Ϊ����ͼ��
}RoseekImgInfStruct;
/**************************************************************************/

/**************************************************************************/
//DEMO�и����Ķ��壬�û��ɸ���ʵ������޸�
//ץ�Ŀ�����Ϣ�ṹ��
typedef struct TrgCtrlInfStruct{
	Bool	bEnableADLI;				//�Ƿ������Բ��
	Uint8	ui8AutoDetcLigIllumPeriod;	//�Բ��ץ������(��λ:��)
	Uint32	ui32AutoDetcLigIllumCNT;	//�Բ��ץ�ļ���������ֵ(���Բ��ץ�����ڻ���õ���DSP�˶���ļ�������������Ϊ1��)
	Bool	bADLIOccur;					//�Բ���־
	Bool	bExtTrgOccurWhenAutoDetcLig;//�Բ���ڼ��Ƿ����ⴥ��
	Uint32	ui32FixedShutterTime;		//����Ҫ�Բ��ʱ�Ĺ̶�����ʱ��(��λ:΢��)
	float	fFixedPGA;					//����Ҫ�Բ��ʱ�Ĺ̶�����(��λ:dB)
	Uint8	ui8IsSingalCoin;			//�Ƿ���Ȧϵͳ
	float	fInteval;					//�����˫��Ȧϵͳ����Ȧ���(��λ:��)	          
}RoseekTrgCtrlInfStruct;

/**************************************************************************
********ͼ������
**************************************************************************/

//ͼ�������ݻ��壬ʵ�������������洢ǰ���뱳��ͼ���YUV����
typedef struct ImgBufStructITS{
        Uint8   *pui8FrameBuf;              //ԭʼͼ��YUV422����
        Uint8	*pui8YCrCb_YBuf;			//ԭʼͼ��תΪYUV420���Y��������ָ��
	    Uint8	*pui8YCrCb_CrBuf;			//ԭʼͼ��תΪYUV420���Cr��������ָ��
	    Uint8	*pui8YCrCb_CbBuf;			//ԭʼͼ��תΪYUV420���Cb��������ָ��
}ITS_ImgBufStruct;
//��ģ�ı��νṹ��,�����㷨����
typedef struct Point{
	int xMax;    
	int xMin;
	int yMax;
	int yMin;    //�������ο�˵�����
	int pt5x;
	int pt5y;
	int pt6x;
	int pt6y;    //�ı������Աߵ��е�����
	float k1;
	float k2;
	float k3;
	float k4;
	float k5;
	float b1;
	float b2;
	float b3;
	float b4;
	float b5;    //������y=kx+bֱ���ھ��ο�Χ�ڹ���һ���ı��Σ�k3Ϊ����
}DpPoint;
//�㷨����ʹ��
typedef struct LineInfo{
	int foreground_count;//ǰ����
	int total_pixel;//������ֵ
}Lineinfo;
//���м���㷨�������軺��
typedef struct algfuncData{
    Uint8       *ay_data; 
    Lineinfo    *aforeground;
	float       *aflag;
}AlgfuncData;
//��������㷨�������軺��
typedef struct VDalgfuncData{
	Uint8      *f_data;
	Uint8      *pro_imgback;
	Uint8      *tmpdata1;
	Uint8      *tmpdata2;
}VDalgfuncData;
//������ģ�㷨�������軺��
typedef struct bpalgData{
    float   *ww;
	float   *mean;
	float   *sd;
}BpalgData;
//�����ṹ��
typedef struct carInfo{
	int num[5];
	float speed[5];
}CarInfo;
/**************************************************************************
******��̨����ͨ�
**************************************************************************/

//���̨���ƶ�ͨ�ţ�������Ϣ�洢�ṹ��
typedef struct CmdInputData{
    Uint8 	head1;   		//Э��ͷ
	Uint8 	head2;			//Э��ͷ
	Uint8 	length;   		//����
	Uint8 	label1;       	//���
	Uint8 	label2;       	//���
	Uint8  	cameraN;		//������
	Uint8  	comm;			//����	
	Uint8   agree;			//��Ƕӳ�����Ƿ���ȷ����ȷΪ1������Ϊ0
	Uint8 	dataR;			//���
	Uint8   data;			//���г���
}PcInData;

//������Ϣͨ�ýṹ��
typedef struct CmdUData{
	float  nSumRT;     //����ʱ��
	float  nFrame;	   //ÿ�봦��֡��
}CmdUData;

//��ǰ������Ϣ�ṹ��
typedef struct ACmdSem{
	Uint8  	comm;			//����
	Bool    bStart;			//�������
}AcmdSem;

//���͵ĳ�����Ϣ�ṹ��
typedef struct sendInfo{
	Int label;
	Int num;
	Float speed;
	Char local_ip[20];//new
}SendInfo;

/**************************************************************************
*******Զ������
**************************************************************************/

//��Զ�����ù���ͨ�ţ���������洢�ṹ��
typedef struct conversationHead{
	int command;         //����
	int bufsize;         //����
}ConversationHead;

//�����ǰ������Ϣ�ṹ��
typedef struct eE3_CfgStruct{
		int 	RunMode;         		//����ģʽ��0 �ޣ�1 ����һ ��2���ܶ���
		char	CameraIp[20];       	//�����ǰIP��ַ
		char    ServerIp[20];			//����ƽ̨IP
		int  	ServerPort;				//����ƽ̨�˿ں�
		int  	RoadNum[2];				//��س�����ţ������ܶ���
		int		UploadTime;				//�ϴ����ڣ�������һ��
		int  	NightTrafficStream;		//ҹ������������һ��		
		Uint8 	RealTime[8];			//���ϵͳʱ��
		int     Isreadflash;			//�Ƿ��ȡ��Flash�е�����		
}EE3_CfgStruct;
//�����ǰ�����ṹ��
typedef struct EE3_StateStruct{
		char 	Version[40];			//������汾��
		int  	Resolution;				//����ֱ��ʣ�1 960*544�� 2 1920*1080��
}EE3_StateStruct;

/**************************************************************************/

/**************************************************************************/
//����оƬAT88SCXXC�������ṹ��
typedef struct {
	Uint8 AnswerToReset[8];		//00-07
	Uint8 FabCode[2];			//08-09
	Uint8 MTZ[2];				//0A-0B
	Uint8 CMC[4];				//0C-0F
	Uint8 LostHistoryCode[8];	//10-17
	Uint8 DCR[1];				//18
	Uint8 IdentificationCode[7];//19-1F
	///////////////////////
	Uint8 AR0[1];				//20
	Uint8 PR0[1];				//21
	Uint8 AR1[1];				//22
	Uint8 PR1[1];				//23
	Uint8 AR2[1];				//24
	Uint8 PR2[1];				//25
	Uint8 AR3[1];				//26
	Uint8 PR3[1];				//27
	Uint8 AR4[1];				//28
	Uint8 PR4[1];				//29
	Uint8 AR5[1];				//2A
	Uint8 PR5[1];				//2B
	Uint8 AR6[1];				//2C
	Uint8 PR6[1];				//2D
	Uint8 AR7[1];				//2E
	Uint8 PR7[1];				//2F
	Uint8 AR8[1];				//30
	Uint8 PR8[1];				//31
	Uint8 AR9[1];				//32
	Uint8 PR9[1];				//33
	Uint8 AR10[1];				//34
	Uint8 PR10[1];				//35
	Uint8 AR11[1];				//36
	Uint8 PR11[1];				//37
	Uint8 AR12[1];				//38
	Uint8 PR12[1];				//39
	Uint8 AR13[1];				//3A
	Uint8 PR13[1];				//3B
	Uint8 AR14[1];				//3C
	Uint8 PR14[1];				//3D
	Uint8 AR15[1];				//3E
	Uint8 PR15[1];				//3F
	///////////////////////
	Uint8 IssuerCode[16];		//40-4F
	///////////////////////
	Uint8 ACC0[1];				//50
	Uint8 CryptoCi0[7];			//51-57
	Uint8 SessionKeySk0[8];		//58-5F
	Uint8 ACC1[1];				//60
	Uint8 CryptoCi1[7];			//61-67
	Uint8 SessionKeySk1[8];		//68-6F
	Uint8 ACC2[1];				//70
	Uint8 CryptoCi2[7];			//71-77
	Uint8 SessionKeySk2[8];		//78-7F
	Uint8 ACC3[1];				//80
	Uint8 CryptoCi3[7];			//81-87
	Uint8 SessionKeySk3[8];		//88-8F
	///////////////////////
	Uint8 SecretSeedG0[8];		//90-97
	Uint8 SecretSeedG1[8];		//98-9F
	Uint8 SecretSeedG2[8];		//A0-A7
	Uint8 SecretSeedG3[8];		//A8-AF
	///////////////////////
	Uint8 PAC_Write0[1];		//B0
	Uint8 WritePassword0[3];	//B1-B3
	Uint8 PAC_Read0[1];			//B4
	Uint8 ReadPassword0[3];		//B5-B7
	Uint8 PAC_Write1[1];		//B8
	Uint8 WritePassword1[3];	//B9-BB
	Uint8 PAC_Read1[1];			//BC
	Uint8 ReadPassword1[3];		//BD-BF
	///////////////////////
	Uint8 PAC_Write2[1];		//C0
	Uint8 WritePassword2[3];	//C1-C3
	Uint8 PAC_Read2[1];			//C4
	Uint8 ReadPassword2[3];		//C5-C7
	Uint8 PAC_Write3[1];		//C8
	Uint8 WritePassword3[3];	//C9-CB
	Uint8 PAC_Read3[1];			//CC
	Uint8 ReadPassword3[3];		//CD-CF
	///////////////////////
	Uint8 PAC_Write4[1];		//D0
	Uint8 WritePassword4[3];	//D1-D3
	Uint8 PAC_Read4[1];			//D4
	Uint8 ReadPassword4[3];		//D5-D7
	Uint8 PAC_Write5[1];		//D8
	Uint8 WritePassword5[3];	//D9-DB
	Uint8 PAC_Read5[1];			//DC
	Uint8 ReadPassword5[3];		//DD-DF
	///////////////////////
	Uint8 PAC_Write6[1];		//E0
	Uint8 WritePassword6[3];	//E1-E3
	Uint8 PAC_Read6[1];			//E4
	Uint8 ReadPassword6[3];		//E5-E7
	Uint8 PAC_Write7[1];		//E8
	Uint8 WritePassword7[3];	//E9-EB
	Uint8 PAC_Read7[1];			//EC
	Uint8 ReadPassword7[3];		//ED-EF
	///////////////////////
	Uint8 SystemReserved[16];	//F0-FF
} AT88SC_Config;
/**************************************************************************/

#endif



