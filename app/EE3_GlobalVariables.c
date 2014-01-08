/**
 * \file      	EE3_GlobalVariables.c
 * \author    	LQ
 * \version   	0.1.0
 * \date      	2013.12.31
 * \brief     	�������ʱʹ�õ�ȫ�ֱ������弰��ʼ��Դ�ļ�
 * \update      ͼ��buffer��ʼ��
**/

#include "EE3_common.h"

/**************************************************************************/
//ͼ��ԭʼ���ݻ��壬ϵͳĬ������6��buffer����mian.c�е��ڴ����
RoseekImgBufStruct	g_ImgBufStruct[6];
//���м���㷨���ֽṹ��,���ڴ����
AlgfuncData g_algfuncData;
//��������㷨���ֽṹ�壬���ڴ����
VDalgfuncData g_VDalgfuncData;
//������ģ�㷨���ֽṹ��,���ڴ����
BpalgData g_bpalgData;  //��һ·
BpalgData g_bpalgData2;  //�ڶ�·
//ͼ�񱳾����ݻ���, �洢�������VDͼ�񱳾����ݣ����ڴ����
ITS_ImgBufStruct  g_ImgBufofBackVD =
{
	NULL,
	NULL,
	NULL,
	NULL
};
//ͼ�񱳾����ݻ��壬�洢�ӳ����QD��һ·ͼ�񱳾����ݹ�ͼ����ģ��ʹ��,���ڴ����
ITS_ImgBufStruct  g_ImgBufofBackS =
{
	NULL,
	NULL,
	NULL,
	NULL
};
//ͼ�񱳾����ݻ��壬�洢�ӳ����QD�ڶ�·ͼ�񱳾����ݹ�ͼ����ģ��ʹ��,���ڴ����
ITS_ImgBufStruct  g_ImgBufofBackS2 =
{
	NULL,
	NULL,
	NULL,
	NULL
};
//ͼ��ǰ�����ݻ��壬�洢ͼ��ǰ�����ݹ�ͼ����ģ��ʹ�ã����ڴ����
ITS_ImgBufStruct  g_ImgBufofFrontS =
{
	NULL,           //*pui8FrameBuf;
	NULL,           //*pui8YCrCb_YBuf;
	NULL,           //*pui8YCrCb_CrBuf;
	NULL            //*pui8YCrCb_CbBuf;
};
//JPG����ά������
RoseekJPGEncBufStruct g_JpgEncBufStruct =
{
	NULL,			//*pui8YCrCb_YBuf;
	NULL,			//*pui8YCrCb_CrBuf;
	NULL,			//*pui8YCrCb_CbBuf;
	{NULL},			//*pui8JpgImgBuf[2];
	{NULL},			//*pui8TrgJpgImgBuf[6];
	{FALSE},		//bJpgImgBuf_Lock[2];
	{FALSE}			//bTrgJpgImgBuf_Lock[6];
};

//JPG�������SD������
RoseekJPGEncBufStruct g_JpgEncBuftoSD =
{
	NULL,			//*pui8YCrCb_YBuf;
	NULL,			//*pui8YCrCb_CrBuf;
	NULL,			//*pui8YCrCb_CbBuf;
	{NULL},			//*pui8JpgImgBuf[2];
	{NULL},			//*pui8TrgJpgImgBuf[6];
	{FALSE},		//bJpgImgBuf_Lock[2];
	{FALSE}			//bTrgJpgImgBuf_Lock[6];
};

//ץ�Ŀ�����Ϣ
RoseekTrgCtrlInfStruct g_TrgCtrlInfStruct =
{
	TRUE,			//bEnableADLI
	5,				//ui8AutoDetcLigIllumPeriod
	5,				//ui32AutoDetcLigIllumCNT
	FALSE,			//bADLIOccur
	FALSE,			//bExtTrgOccurWhenAutoDetcLig
	1500,			//ui32FixedShutterTime;
	30.0,			//fFixedPGA
	1,				//ui8IsSingalCoin
	5.0				//fInteval
};

//ץ����Ϣ
RoseekImgInfStruct g_ImgInfStruct =
{
	{"�Ϻ���ͬ�ô�ѧ�ζ�У��"},	//ui8LocusInf[102]
	{0},						//ui8TimeInf[8]
	1,							//ui8WayNum
	1,							//ui8TrgMode
	0.0,							//fSpeed
	FALSE                       //�Ǳ���ͼ��
};


//���տ��ƶ���Ϣ�洢�ṹ��
PcInData g_BpInputData =
{
	0xFF,		//Э��ͷ
	0xFF,		//Э��ͷ
	0x07,		//����
	0xCC,		//���
	0xCC,		//���
	0,			//������
	0,   		//����
	0,			//������
	0x00,     	//���
	0			//�ӳ�
};
//����ƶ˷�����Ϣ�洢�ṹ��
PcInData g_BpOutputData1 =
{
	0xFF,		//Э��ͷ
	0xFF,		//Э��ͷ
	0x07,		//����
	0xCC,		//���
	0xCC,		//���
	0,			//������
	0,   		//����
	0,			//������
	0x00,     	//���
	0			//�ӳ�

};
PcInData g_BpOutputData2 =
{
	0xFF,		//Э��ͷ
	0xFF,		//Э��ͷ
	0x07,		//����
	0xCC,		//���
	0xCC,		//���
	0,			//������
	0,   		//����
	0,			//������
	0x00,     	//���
	0			//�ӳ�

};

//CmdUData������Ϣͨ�ýṹ��
CmdUData g_CmdUdata =
{
	0,            //������ʱ��
	0
};
//��һ·��ǰ������Ϣ�ṹ��
AcmdSem g_AmcdSem1 = 
{
	0,          	//������
	FALSE 			//�������
};
//�ڶ�·��ǰ������Ϣ�ṹ��
AcmdSem g_AmcdSem2 = 
{
	0,          	//������
	FALSE 			//�������
};

//����Զ��ƽ̨��������洢�ṹ��
ConversationHead g_DecCmdsem =
{
  0,
  0
};

//�����ṹ�壨5������
CarInfo g_carInfo = 
{				
	{0,22,0,0,0},		//������
	{0,22,0,0,0}			//�ٶ�
};
//�����ǰ���в���״̬�ṹ��
EE3_CfgStruct g_EE3Cur = 
{
	0,					//����ģʽ��0 �ޣ�1 ����һ ��2���ܶ���
	"192.168.1.218",	//�����ǰIP��ַ
	"10.60.139.195",	//����ƽ̨IP
	4545,				//����ƽ̨�˿ں�
	{6,7},				//��س�����ţ������ܶ���
	30,					//�ϴ����ڣ�������һ��
	12,					//ҹ������������һ��
	{0,0,0,0,0,0,0,0},	//���ϵͳʱ��
	0					//�Ƿ��ȡ��Flash�е�����		
};

volatile Bool IsReadFlash = TRUE;//�Ƿ��Flash�ж�ȡ������в�����Ĭ�Ͽ�����

//�����ǰ״̬����
EE3_StateStruct g_EE3State = 
{
	"rmva380sc_Universal_V0.29",	//������汾��
	1								//����ֱ��ʣ�1 960*544�� 2 1920*1080��
};

//���������ṹ��
RoseekISOOutAtbStruct g_ISOOutAtb =
{
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

DAT_HANDLE			g_hDAT0;
DAT_HANDLE			g_hDAT1;
DAT_HANDLE			g_hDAT2;
DAT_HANDLE			g_hDAT3;
DAT_HANDLE			g_hDAT4;
DAT_HANDLE			g_hDAT5;
DAT_HANDLE			g_hDAT6;
DAT_HANDLE			g_hDAT7;
volatile	Bool	g_bIsTrgClientConnect = FALSE;
volatile 	Bool	g_bIsFconClientConnect = FALSE;
volatile	Bool	g_bIsFcontoDecClientConnect = FALSE;
volatile	Bool	g_bIsHfrClientConnect = FALSE;
volatile	Bool	g_bIsHfrBinClientConnect = FALSE;
volatile	Bool	g_bIsStopTrgSvr = FALSE;
volatile	Bool	g_bTrgSvrIsLink = FALSE;
volatile    Bool    g_bIsSendRoadInfConnect = FALSE;
volatile	Uint8	ui8SPIO1Config[16] = { 0, 2, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile	Uint8	ui8SPIO2Config[16] = { 0, 2, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
volatile	Uint8	ui8SPIOUDConfig[16] = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


//L1/2 CAHCE SIZE
volatile	Uint32	g_ui32L1PSize = EE3_L1_CACHE_SIZE_32K;
volatile	Uint32	g_ui32L1DSize = EE3_L1_CACHE_SIZE_32K;
volatile	Uint32	g_ui32L2Size = EE3_L2_CACHE_SIZE_128K; 
//ȫ�ֱ���
volatile    Uint32  CodeRate[2] = {0};    //����ȫ�ִ洢
volatile    float   Ftmpnow=0;         //��ǰ�¶�
volatile    Bool    BIsTrgFrameofBack = FALSE;      //�Ƿ���Ϊ����ͼ��
volatile 	Bool    g_bIsSendPictoDec = FALSE;		//�Ƿ���ͼ��      
volatile    struct  sockaddr g_ConfigSetaddr;
volatile    int     bTextConnect = 0;
volatile    int     bTextCount = 0;
volatile    int     bTextRecev = 0;
volatile    int     PointDataVD[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile    int     PointDataQD[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
volatile    Uint32  nRunT[2];
volatile    float   RunTimeOnce[NUM_RUMTIME];
volatile    float   runTimeT[60];



