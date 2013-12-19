/**
 * \file      	EE3_main.c 
 * \author    	LQ
 * \version   	0.15
 * \date      	2013.11.24
 * \brief     	RMVA380SC ��ϵͳ��ʼ��ģ��
 * \update      �µ�IO�ڳ�ʼ��
**/

#include "EE3_common.h"

/**************************************************************************/
//�ⲿ����
extern Uint8 ui8SPIO1Config[16];
extern Uint8 ui8SPIO2Config[16];
extern Uint8 ui8SPIOUDConfig[16];
extern RoseekISOOutAtbStruct g_ISOOutAtb;
extern RoseekImgInfStruct g_ImgInfStruct; //ץ����Ϣ�ṹ��,ʵ�����������ڴ洢�ϴ���ͼ��������Ϣ
extern RoseekTrgCtrlInfStruct g_TrgCtrlInfStruct;//ץ�Ŀ�����Ϣ�ṹ��
extern RoseekImgBufStruct	g_ImgBufStruct[]; //ͼ��ԭʼ���ݻ���ṹ�壬��ʵ�������������洢�ɼ�ģ����յ���ͼ��ԭʼ���ݼ������Ϣ
extern RoseekJPGEncBufStruct g_JpgEncBufStruct; //JPEGͼ�񻺴�ṹ�壬��ʵ�������������洢JPEG����ģ���õ����м����ݣ��Լ������JPEGͼ������
extern RoseekJPGEncBufStruct g_JpgEncBuftoSD;//JPEGͼ�񻺴棬��ʵ��������洢JPEG������ʱ����
extern ITS_ImgBufStruct   g_ImgBufofBackS; //ͼ�񱳾����ݻ��壬��ͼ����ģ��ʹ�ã��Զ��壩
extern ITS_ImgBufStruct   g_ImgBufofBackS2; //�ڶ�·���Զ��壩
extern ITS_ImgBufStruct   g_ImgBufofFrontS; //ͼ��ǰ�����ݻ��壬��ͼ����ģ��ʹ�ã��Զ��壩
extern ITS_ImgBufStruct   g_ImgBufofBackVD;//ͼ�񱳾����ݻ��壬�������ʹ��
extern AlgfuncData g_algfuncData; //���м���㷨���ݻ���
extern VDalgfuncData g_VDalgfuncData;//��������㷨���ݻ���
extern BpalgData g_bpalgData; //������ģ�㷨���ݻ��棬��һ·
extern BpalgData g_bpalgData2;  //������ģ�㷨���ݻ��棬�ڶ�·
extern int	extHeap;
extern DAT_HANDLE g_hDAT0;
extern DAT_HANDLE g_hDAT1;
extern DAT_HANDLE g_hDAT2;
extern DAT_HANDLE g_hDAT3;
extern DAT_HANDLE g_hDAT4;
extern DAT_HANDLE g_hDAT5;
extern DAT_HANDLE g_hDAT6;
extern DAT_HANDLE g_hDAT7;
extern volatile    Uint32  CodeRate[2];
extern volatile    Bool    BIsTrgFrameofBack;
/**************************************************************************/
void Roseek_SysParamInit();
void Roseek_Allocate_Memory();
/**************************************************************************/


/**
 * \function 	main
 * \brief    	EagleEyeϵ���������ϵͳ��ʹ����TI�ṩ��DSP/BIOS��ʵʱ����ϵͳ�ںˣ�main������Ϊϵͳ�ĳ�ʼ���������ڣ���DSP/BIOS�ں˿�ʼ����֮ǰ���ϵͳ״̬�ĳ�ʼ��
 * \note		�û��Զ���ĳ�ʼ��������Ҫ��main�����е� Roseek_SysParamInit���������ڵ�Roseek_Initialize_SysBufs����֮�����	
 * \			
**/	
void main()
{
	Roseek_SysParamInit();
}

/**
 * \function 	Roseek_SysParamInit
 * \brief    	�û�������ʼ���Ӻ���
 * \note		�û��Զ���ĳ�ʼ���������ڸú����ڵ�Roseek_Initialize_SysBufs����֮�����	
 * \			
**/	
void Roseek_SysParamInit()
{
  	Uint8 ui8Tmp = 0xff;  //���û�������ַ	
  	Uint8 i;
	Uint32 codeRate[2] = {0};
	char    cha[104];

	//��������������ݴ洢�ռ�
	Roseek_Allocate_Memory();

	//����ϵͳʹ�õĻ��������ݴ洢�ռ�
	Roseek_Initialize_SysBufs();

	
	//����H.264��Ƶ��ѹ�������,���ݵ�ǰ����Ƶģʽ�ֱ��ʴ�С������
	Roseek_Set_EncResolution( ENC_380S_DS_1080P_CIF  ); //��ʼENC_380S_DS_1080P_CIF
	Roseek_Set_EncCodeRate(8000);   //������Ƶ���ʱ��������С��100~8000Kb/s������8000
	Roseek_Get_ENCCodeRate(&codeRate[0]);
	Roseek_Set_SubCodeRate(400);    //����CIF��ʽ����Ƶ����������С
	Roseek_Get_SubCodeRate(&codeRate[1]);
	for(i=0;i<2;i++)
	   CodeRate[i]=codeRate[i];
	//��Ƶ������ʾ�������Ϣ
	memset( cha, '\0', 104 );
    sprintf(cha, "�ֱ���:1920*1088,����:%dKbps",CodeRate[0]);
    Roseek_Set_EncOSDInf( cha );
	Roseek_Set_EncOSD_ShowOp(ENC_OSD_FONT2X,0,0,0,0,0,0,0,0,0);//�趨OSD��Ƶ������ϢP41����12
	//sprintf( cha, "EE3���H.264������,�ֱ���:CIF,����:%dKbps", CodeRate[1] );
	//Roseek_Set_EncCIFOSDInf( cha );
	
	Roseek_Open_ENCDataChannel();  //�򿪱���ASIC���ݴ���ͨ��

	//����ͼ��ɫ�ʱ��ͶȲ���
	Roseek_Set_ColorSaturation_Param( 1.6 );  //1.0Ϊ������ɫ�ʱ��Ͷȣ�0Ϊ�ڰ�,����1.3��
	//��ʼ��SD��
	Roseek_SD_Initialize();
	
	//��ʼ��������Ϣ�ṹ��Ķ���
	for( i=0; i<6; i++ ){
		g_ImgBufStruct[i].bProcessLocked = FALSE;   //�������ݼ�����־������
		g_ImgBufStruct[i].ui8RotateFlag = IMGR_DEGREE0;  //ͼ����ת��־
		g_ImgBufStruct[i].ui8TrgFconScaleLel =1;         //������ȫ�ֱ�������ģʽ�µ����ż���0Ϊ1920*1088��1Ϊ960*544
		g_ImgBufStruct[i].iTrgSurFlag = 0;               //��ͼ��ɼ��ڴ���ģʽ�£���ʾ����ͼ���������ĸ�����Դ
		memset( g_ImgBufStruct[i].ui8TrgShotTime, 0, 8 ); //����ģʽ�µ�ץ��ʱ�䣬��-��-��-����-ʱ-��-��-ʮ�����룬����ģʽ�²�ʹ�ã���Ч
		g_ImgBufStruct[i].ui8TrgMode = 0;                 //������ʽ��0->����;1->����Ȧ;2->˫��Ȧ(����)������ģʽ��Ч
		g_ImgBufStruct[i].fSpeed = 0.0;                   //����ģʽ�³����ٶȣ���˫��Ȧ������Ч������ģʽ��Ч
		g_ImgBufStruct[i].CurImg_CorrespondingRunMode = FconMode; //��ǰͼ���ں���ģʽ�²ɼ�(TrgMode, FconMode, HfrMode)
		g_ImgBufStruct[i].CurImg_Format = YUV_ARRAY;       //��ǰͼ��Ϊ����ģʽ(RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY)
		g_ImgBufStruct[i].iTrgFrameofBack = FALSE;         //��ǰͼ��ɼ��Ƿ���Ϊ����ͼ��new)
	}
	for( i=0;i<6;i++){
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[i]=FALSE; //����������Ĵ�����ʽJPEGͼ��Ļ�������������Ч�Ա�־λ
	}
	
	for( i=0;i<2;i++){
		g_JpgEncBufStruct.bJpgImgBuf_Lock[i]=FALSE;  //����������ķǴ�����ʽJPEGͼ��Ļ������е�������Ч�Ա�־λ
	}

	BIsTrgFrameofBack = TRUE; //����ΪTRUE�����ڴ����ռ�����ͼ�����SD����
	
	//Ĭ�ϲɼ�ģʽΪYUV����
	Roseek_Set_ImgAcquisition_Format( YUV_ARRAY );//����YUV_ARRAY

	//����ͼ����ǿ�ȼ�Ϊ3
	Roseek_Set_ImgEnhancement_Level(IMG_ENHANCEMENT_LEVEL3); //����IMG_ENHANCEMENT_LEVEL3
	
	//����IO1��IO2Ϊ1����������Ȧ���룬������Ȧ�������٣�2����Ȧ����ץ��
	Roseek_SPIOInfConfig( 0, ui8SPIO1Config );//IO1
	Roseek_SPIOInfConfig( 1, ui8SPIO2Config );//IO2
	
	//����IO3��IO4Ϊ�����߿���������
	Roseek_SPIOInfConfig( 2, ui8SPIOUDConfig );//IO3
	Roseek_SPIOInfConfig( 3, ui8SPIOUDConfig );//IO4

	//����IO5��IO6Ϊ���������ӳ���������5�Ŷ�ӦRoadNum[0]·��6�Ŷ�ӦRoadNum[1]·
	Roseek_SPIOInfConfig( 4, ui8SPIOUDConfig );//IO5
	Roseek_SPIOInfConfig( 5, ui8SPIOUDConfig );//IO6

	//���ù��������˿�ISO1��ISO2
	Roseek_ISOOUT_InfConfig( 0, g_ISOOutAtb );   //ISO1
	Roseek_ISOOUT_InfConfig( 1, g_ISOOutAtb );   //ISO1
	
	//ʹ���ع�ͬ���ж�
	Roseek_Enable_ExpSynInt();

	//��8MB��Flash�ж�ȡ�洢��Ϣ����ʼ��ϵͳ���в���
	Roseek_Flash_Read( &ui8Tmp, TRGINFFLAGADD, 1 ); //��Flash��TRGINFFLAGADD��ַ��ʼ��1���ֽ����ݵ�ui8Tmp=0xff������
	if( ui8Tmp!=0xff ){//0xff��ʾδ�����ץ����Ϣ
		Roseek_Flash_Read( (Uint8*)&g_ImgInfStruct, TRGINFADD, sizeof(RoseekImgInfStruct) );
		g_ImgInfStruct.ui8LocusInf[100] = '\0'; //ץ�ĵص���Ϣ����50������,��ʼ��\0
		Roseek_Flash_Read( (Uint8*)&g_TrgCtrlInfStruct, TRGCTRLINFADD, sizeof(RoseekTrgCtrlInfStruct) );
		Roseek_Flash_Read( (Uint8*)&g_TrgBrightnessAdj_Params, TRGCTRLPARAMADD, sizeof(RoseekImgYMeanAutoAdjParams) );
	}

	//�رմ���ģʽ�µ������ͬ�����
	Roseek_Disable_TrgFlashlamp_Syn_Out();
	//�ر�����ģʽ�µ������ͬ�����
	Roseek_Disable_FconFlashlamp_Syn_Out();


	
	//���ò������p15����26��
	Roseek_Set_MIBlockParam( 1, 400, 400, 832, 1216 );//����1, 400, 400, 832, 1216
	//��ʼ�����������ع�ʱ����ź��ڲ�����
	switch( Roseek_Get_CurRunMode() ){
		case TrgMode:
			Roseek_Set_Afe_Pga( TrgMode,g_TrgBrightnessAdj_Params.fCurrentAfePga_db );//����TrgMode����ģʽ�´���������ź�����
			Roseek_Set_TrgShutter_Time( g_TrgBrightnessAdj_Params.ui32CurrentShutterWidth_us );
			break;
		case FconMode:
			Roseek_Set_Afe_Pga( FconMode,g_FconBrightnessAdj_Params.fCurrentAfePga_db );
			Roseek_Set_FConShutter_Time( g_FconBrightnessAdj_Params.ui32CurrentShutterWidth_us );
			break;
		default:
			break;
	}

	//��ʼ��JPG�������Ĳ���
	Roseek_JPGENC_Init( 2048, 2048 );//������ͼ������ֱ���Ϊ2048*2048

	//��ʼ�����ڹ���ģʽ
	Roseek_SetRS232RcvBufDepth( Roseek_RS232_PORT1, 256 );
	Roseek_RS232_SetBaudrate( Roseek_RS232_PORT1, Roseek_RS232_BAUDRATE_38400 );
	Roseek_RS232_Open( Roseek_RS232_PORT1 );
	Roseek_SetRS232RcvBufDepth( Roseek_RS232_PORT2, 256 );
	Roseek_RS232_SetBaudrate( Roseek_RS232_PORT2, Roseek_RS232_BAUDRATE_38400 );
	Roseek_RS232_Open( Roseek_RS232_PORT2 );
	Roseek_SetRS485RcvBufDepth( 256 );
	Roseek_RS485_SetBaudrate(Roseek_RS485_BAUDRATE_38400);
	Roseek_RS485_Open();

	//����DMAģ��ϵͳ�������ȼ����������û��޸ĸ�API�Ĳ���
	Roseek_Set_DMASysPriority(3);

	//����DAT��Դ

	g_hDAT0 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT1 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT2 = Roseek_DAT_Open(QDMA_OPT_PRI_HIGH);
	g_hDAT3 = Roseek_DAT_Open(QDMA_OPT_PRI_MEDIUM);
	g_hDAT4 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT5 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT6 = Roseek_DAT_Open(QDMA_OPT_PRI_HIGH);
	g_hDAT7 = Roseek_DAT_Open(QDMA_OPT_PRI_MEDIUM);
	
	//���������ʼģʽΪ����ģʽ,25fps
	Roseek_Mode_Switch_Ctrl( FconMode, EE3_380_DRCLK_HF);//����FconMode, EE3_380_DRCLK_HF
	//�����ô���ģʽΪȫ�ֱ���ͼ�񣬲���������
	Roseek_Set_TrgFconMode_ScaleLevel(TrgMode,TRGFCON_SCALE_LEVEL_1);//����TrgMode,TRGFCON_SCALE_LEVEL_0
	//����������ģʽΪȫ�ֱ���ͼ�񣬲���������
	Roseek_Set_TrgFconMode_ScaleLevel(FconMode,TRGFCON_SCALE_LEVEL_1);//����FconMode,TRGFCON_SCALE_LEVEL_0
	
	//��ͼ��ɼ�ͨ��
	Roseek_Open_Imagechannel();
}

/**
 * \function 	Roseek_Allocate_Memory
 * \brief    	�������ݻ���
 * \note		�û����뽫ϵͳ����ʱʹ�õĻ�����DSP/BIOSϵͳ���Юǰ�������룬���������������ж�̬���롢�ͷŴ�Ĳ���	
 * \			
**/	
void Roseek_Allocate_Memory()
{
	// ����ͼ��ɼ�ԭʼ���ݻ���
	g_ImgBufStruct[0].pui8FrameBuf = MEM_alloc( extHeap,  FINE_ROW*FINE_COL*3, 256);
	while( g_ImgBufStruct[0].pui8FrameBuf==NULL);

	g_ImgBufStruct[1].pui8FrameBuf = MEM_alloc( extHeap,  FINE_ROW*FINE_COL*3, 256);
	while( g_ImgBufStruct[1].pui8FrameBuf==NULL);

	g_ImgBufStruct[2].pui8FrameBuf = MEM_alloc( extHeap,  FINE_ROW*FINE_COL*3, 256);
	while( g_ImgBufStruct[2].pui8FrameBuf==NULL);

	g_ImgBufStruct[3].pui8FrameBuf = MEM_alloc( extHeap,  FINE_ROW*FINE_COL*3, 256);
	while( g_ImgBufStruct[3].pui8FrameBuf==NULL);

	g_ImgBufStruct[4].pui8FrameBuf = MEM_alloc( extHeap,  FINE_ROW*FINE_COL*3, 256);
	while( g_ImgBufStruct[4].pui8FrameBuf==NULL);

	g_ImgBufStruct[5].pui8FrameBuf = MEM_alloc( extHeap,  FINE_ROW*FINE_COL*3, 256);
	while( g_ImgBufStruct[5].pui8FrameBuf==NULL);
	
	// ����ITSӦ��ͼ���������ͼ�����ݻ���
	//������ⱳ��
	g_ImgBufofBackVD.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL*2, 256);
	while( g_ImgBufofBackVD.pui8YCrCb_YBuf==NULL);
	//�ӳ���ⱳ��
	g_ImgBufofBackS.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofBackS.pui8YCrCb_YBuf==NULL);

	g_ImgBufofBackS2.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofBackS2.pui8YCrCb_YBuf==NULL);
	
    // ǰ��
	g_ImgBufofFrontS.pui8FrameBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL*3, 256);
	while( g_ImgBufofFrontS.pui8FrameBuf==NULL);

	g_ImgBufofFrontS.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofFrontS.pui8YCrCb_YBuf==NULL);

	g_ImgBufofFrontS.pui8YCrCb_CbBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofFrontS.pui8YCrCb_CbBuf==NULL);

	g_ImgBufofFrontS.pui8YCrCb_CrBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofFrontS.pui8YCrCb_CrBuf==NULL);

	// ����"���м��"�㷨�������軺��
    g_algfuncData.ay_data = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_algfuncData.ay_data==NULL);

	g_algfuncData.aflag = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW, 256);
	while( g_algfuncData.aflag==NULL);

	g_algfuncData.aforeground = MEM_alloc( extHeap,  sizeof(Lineinfo)*ALG_ROW, 256);
	while( g_algfuncData.aforeground==NULL);

	// ����"�������"�㷨�������軺��
    g_VDalgfuncData.f_data = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.f_data==NULL);

	g_VDalgfuncData.pro_imgback = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.pro_imgback==NULL);

	g_VDalgfuncData.tmpdata1 = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.tmpdata1==NULL);

	g_VDalgfuncData.tmpdata2 = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.tmpdata2==NULL);
	
	// ���뱳����ģ�㷨�������軺��
	g_bpalgData.mean = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW*ALG_COL*3, 256);
	while( g_bpalgData.mean==NULL);

	g_bpalgData.sd = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW*ALG_COL*3, 256);
	while( g_bpalgData.sd==NULL);

	g_bpalgData.ww = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW*ALG_COL*3, 256);
	while( g_bpalgData.ww==NULL);

	g_bpalgData2.mean = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW*ALG_COL*3, 256);
	while( g_bpalgData2.mean==NULL);

	g_bpalgData2.sd = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW*ALG_COL*3, 256);
	while( g_bpalgData2.sd==NULL);

	g_bpalgData2.ww = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW*ALG_COL*3, 256);
	while( g_bpalgData2.ww==NULL);

	// �����TrgMode��JPEG��������Ļ���
	g_JpgEncBufStruct.pui8JpgImgBuf[0] = MEM_calloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8JpgImgBuf[0]==NULL);

	g_JpgEncBufStruct.pui8JpgImgBuf[1] = MEM_calloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8JpgImgBuf[1]==NULL);

    // ����TrgMode��JPEG��������Ļ���
	g_JpgEncBufStruct.pui8TrgJpgImgBuf[0] = MEM_alloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8TrgJpgImgBuf[0]==NULL);

	g_JpgEncBufStruct.pui8TrgJpgImgBuf[1] = MEM_alloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8TrgJpgImgBuf[1]==NULL);
	
	g_JpgEncBufStruct.pui8TrgJpgImgBuf[2] = MEM_alloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8TrgJpgImgBuf[2]==NULL);

	g_JpgEncBufStruct.pui8TrgJpgImgBuf[3] = MEM_alloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8TrgJpgImgBuf[3]==NULL);
	
	g_JpgEncBufStruct.pui8TrgJpgImgBuf[4] = MEM_alloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8TrgJpgImgBuf[4]==NULL);

	g_JpgEncBufStruct.pui8TrgJpgImgBuf[5] = MEM_alloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8TrgJpgImgBuf[5]==NULL);

}
