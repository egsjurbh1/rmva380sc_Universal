/**
 * \file      	EE3_main.c 
 * \author    	LQ
 * \version   	0.15
 * \date      	2013.11.24
 * \brief     	RMVA380SC µÄÏµÍ³³õÊ¼»¯Ä£¿é
 * \update      ĞÂµÄIO¿Ú³õÊ¼»¯
**/

#include "EE3_common.h"

/**************************************************************************/
//Íâ²¿±äÁ¿
extern Uint8 ui8SPIO1Config[16];
extern Uint8 ui8SPIO2Config[16];
extern Uint8 ui8SPIOUDConfig[16];
extern RoseekISOOutAtbStruct g_ISOOutAtb;
extern RoseekImgInfStruct g_ImgInfStruct; //×¥ÅÄĞÅÏ¢½á¹¹Ìå,ÊµÀı»¯¶ÔÏóÓÃÓÚ´æ´¢ÉÏ´«µÄÍ¼ÏñÃèÊöĞÅÏ¢
extern RoseekTrgCtrlInfStruct g_TrgCtrlInfStruct;//×¥ÅÄ¿ØÖÆĞÅÏ¢½á¹¹Ìå
extern RoseekImgBufStruct	g_ImgBufStruct[]; //Í¼ÏñÔ­Ê¼Êı¾İ»º´æ½á¹¹Ìå£¬ÆäÊµÀı»¯¶ÔÏóÓÃÀ´´æ´¢²É¼¯Ä£¿é½ÓÊÕµ½µÄÍ¼ÏñÔ­Ê¼Êı¾İ¼°Ïà¹ØĞÅÏ¢
extern RoseekJPGEncBufStruct g_JpgEncBufStruct; //JPEGÍ¼Ïñ»º´æ½á¹¹Ìå£¬ÆäÊµÀı»¯¶ÔÏóÓÃÀ´´æ´¢JPEG±àÂëÄ£¿éÓÃµ½µÄÖĞ¼äÊı¾İ£¬ÒÔ¼°Êä³öµÄJPEGÍ¼ÏñÊı¾İ
extern RoseekJPGEncBufStruct g_JpgEncBuftoSD;//JPEGÍ¼Ïñ»º´æ£¬ÆäÊµÀı»¯¶ÔÏó´æ´¢JPEG±àÂëÁÙÊ±Êı¾İ
extern ITS_ImgBufStruct   g_ImgBufofBackS; //Í¼Ïñ±³¾°Êı¾İ»º³å£¬¹©Í¼Ïñ´¦ÀíÄ£¿éÊ¹ÓÃ£¨×Ô¶¨Òå£©
extern ITS_ImgBufStruct   g_ImgBufofBackS2; //µÚ¶şÂ·£¨×Ô¶¨Òå£©
extern ITS_ImgBufStruct   g_ImgBufofFrontS; //Í¼ÏñÇ°¾°Êı¾İ»º³å£¬¹©Í¼Ïñ´¦ÀíÄ£¿éÊ¹ÓÃ£¨×Ô¶¨Òå£©
extern ITS_ImgBufStruct   g_ImgBufofBackVD;//Í¼Ïñ±³¾°Êı¾İ»º³å£¬³µÁ¾¼ì²âÊ¹ÓÃ
extern AlgfuncData g_algfuncData; //¶ÓÁĞ¼ì²âËã·¨Êı¾İ»º´æ
extern VDalgfuncData g_VDalgfuncData;//³µÁ¾¼ì²âËã·¨Êı¾İ»º´æ
extern BpalgData g_bpalgData; //±³¾°½¨Ä£Ëã·¨Êı¾İ»º´æ£¬µÚÒ»Â·
extern BpalgData g_bpalgData2;  //±³¾°½¨Ä£Ëã·¨Êı¾İ»º´æ£¬µÚ¶şÂ·
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
 * \brief    	EagleEyeÏµÁĞÖÇÄÜÏà»úÏµÍ³ÄÚÊ¹ÓÃÁËTIÌá¹©µÄDSP/BIOSÊÇÊµÊ±²Ù×÷ÏµÍ³ÄÚºË£¬mainº¯Êı×÷ÎªÏµÍ³µÄ³õÊ¼»¯º¯Êı´æÔÚ£¬ÔÚDSP/BIOSÄÚºË¿ªÊ¼¹¤×÷Ö®Ç°Íê³ÉÏµÍ³×´Ì¬µÄ³õÊ¼»¯
 * \note		ÓÃ»§×Ô¶¨ÒåµÄ³õÊ¼»¯ÄÚÈİĞèÒªÔÚmainº¯ÊıÖĞµÄ Roseek_SysParamInit£¨£©º¯ÊıÄÚµÄRoseek_Initialize_SysBufs£¨£©Ö®ºó½øĞĞ	
 * \			
**/	
void main()
{
	Roseek_SysParamInit();
}

/**
 * \function 	Roseek_SysParamInit
 * \brief    	ÓÃ»§²ÎÊı³õÊ¼»¯×Óº¯Êı
 * \note		ÓÃ»§×Ô¶¨ÒåµÄ³õÊ¼»¯ÄÚÈİĞèÔÚ¸Ãº¯ÊıÄÚµÄRoseek_Initialize_SysBufs£¨£©Ö®ºó½øĞĞ	
 * \			
**/	
void Roseek_SysParamInit()
{
  	Uint8 ui8Tmp = 0xff;  //ÉèÖÃ»º³åÇøµØÖ·	
  	Uint8 i;
	Uint32 codeRate[2] = {0};
	char    cha[104];

	//ÉêÇë¸÷»º³åÇøÊı¾İ´æ´¢¿Õ¼ä
	Roseek_Allocate_Memory();

	//ÉêÇëÏµÍ³Ê¹ÓÃµÄ»º³åÇøÊı¾İ´æ´¢¿Õ¼ä
	Roseek_Initialize_SysBufs();

	
	//ÅäÖÃH.264ÊÓÆµÁ÷Ñ¹Ëõ°å²ÎÊı,¸ù¾İµ±Ç°µÄÊÓÆµÄ£Ê½·Ö±æÂÊ´óĞ¡À´ÅäÖÃ
	Roseek_Set_EncResolution( ENC_380S_DS_1080P_CIF  ); //³õÊ¼ENC_380S_DS_1080P_CIF
	Roseek_Set_EncCodeRate(8000);   //ÉèÖÃÊÓÆµÊä³öÊ±µÄÂëÁ÷´óĞ¡£¬100~8000Kb/s£¬³õÉè8000
	Roseek_Get_ENCCodeRate(&codeRate[0]);
	Roseek_Set_SubCodeRate(400);    //ÉèÖÃCIF¸ñÊ½×ÓÊÓÆµÁ÷µÄÂëÁ÷´óĞ¡
	Roseek_Get_SubCodeRate(&codeRate[1]);
	for(i=0;i<2;i++)
	   CodeRate[i]=codeRate[i];
	//ÊÓÆµÁ÷ÉÏÏÔÊ¾º¬½á¹ûĞÅÏ¢
	memset( cha, '\0', 104 );
    sprintf(cha, "·Ö±æÂÊ:1920*1088,ÂëÂÊ:%dKbps",CodeRate[0]);
    Roseek_Set_EncOSDInf( cha );
	Roseek_Set_EncOSD_ShowOp(ENC_OSD_FONT2X,0,0,0,0,0,0,0,0,0);//Éè¶¨OSDÊÓÆµÎÄ×ÖĞÅÏ¢P41º¯Êı12
	//sprintf( cha, "EE3Ïà»úH.264×ÓÂëÁ÷,·Ö±æÂÊ:CIF,ÂëÂÊ:%dKbps", CodeRate[1] );
	//Roseek_Set_EncCIFOSDInf( cha );
	
	Roseek_Open_ENCDataChannel();  //´ò¿ª±àÂëASICÊı¾İ´«ÊäÍ¨µÀ

	//ÉèÖÃÍ¼ÏñÉ«²Ê±¥ºÍ¶È²ÎÊı
	Roseek_Set_ColorSaturation_Param( 1.6 );  //1.0Îª²»µ÷ÕûÉ«²Ê±¥ºÍ¶È£¬0ÎªºÚ°×,³õÉè1.3¡£
	//³õÊ¼»¯SD¿¨
	Roseek_SD_Initialize();
	
	//³õÊ¼»¯»º´æĞÅÏ¢½á¹¹ÌåµÄ¶ÔÏó
	for( i=0; i<6; i++ ){
		g_ImgBufStruct[i].bProcessLocked = FALSE;   //»º´æÊı¾İ¼ÓËø±êÖ¾£¬¿ÕÏĞ
		g_ImgBufStruct[i].ui8RotateFlag = IMGR_DEGREE0;  //Í¼ÏñĞı×ª±êÖ¾
		g_ImgBufStruct[i].ui8TrgFconScaleLel =1;         //´¥·¢ºÍÈ«·Ö±æÂÊÁ¬ĞøÄ£Ê½ÏÂµÄËõ·Å¼¶±ğ0Îª1920*1088£¬1Îª960*544
		g_ImgBufStruct[i].iTrgSurFlag = 0;               //ÈôÍ¼Ïñ²É¼¯ÓÚ´¥·¢Ä£Ê½ÏÂ£¬±íÊ¾±¾ìõÍ¼ÏñÀ´×ÔÓÚÄÄ¸ö´¥·¢Ô´
		memset( g_ImgBufStruct[i].ui8TrgShotTime, 0, 8 ); //´¥·¢Ä£Ê½ÏÂµÄ×¥ÅÄÊ±¼ä£¬Äê-ÔÂ-ÈÕ-ĞÇÆÚ-Ê±-·Ö-Ãë-Ê®±¶ºÁÃë£¬Á¬ĞøÄ£Ê½ÏÂ²»Ê¹ÓÃ£¬ÎŞĞ§
		g_ImgBufStruct[i].ui8TrgMode = 0;                 //´¥·¢·½Ê½£¬0->Èí´¥·¢;1->µ¥ÏßÈ¦;2->Ë«ÏßÈ¦(²âËÙ)£¬Á¬ĞøÄ£Ê½ÎŞĞ§
		g_ImgBufStruct[i].fSpeed = 0.0;                   //´¥·¢Ä£Ê½ÏÂ³µÁ¾ËÙ¶È£¬½öË«ÏÈÈ¦´¥·¢ÓĞĞ§£¬Á¬ĞøÄ£Ê½ÎŞĞ§
		g_ImgBufStruct[i].CurImg_CorrespondingRunMode = FconMode; //µ±Ç°Í¼ÏñÔÚºÎÖÖÄ£Ê½ÏÂ²É¼¯(TrgMode, FconMode, HfrMode)
		g_ImgBufStruct[i].CurImg_Format = YUV_ARRAY;       //µ±Ç°Í¼ÏñÎªºÎÖÖÄ£Ê½(RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY)
		g_ImgBufStruct[i].iTrgFrameofBack = FALSE;         //µ±Ç°Í¼Ïñ²É¼¯ÊÇ·ñ×÷Îª±³¾°Í¼Ïñ£¨new)
	}
	for( i=0;i<6;i++){
		g_JpgEncBufStruct.bTrgJpgImgBuf_Lock[i]=FALSE; //±àÂëÆ÷Êä³öµÄ´¥·¢·½Ê½JPEGÍ¼ÏñµÄ»º´æĞòÁĞÊı¾İÓĞĞ§ĞÔ±êÖ¾Î»
	}
	
	for( i=0;i<2;i++){
		g_JpgEncBufStruct.bJpgImgBuf_Lock[i]=FALSE;  //±àÂëÆ÷Êä³öµÄ·Ç´¥·¢·½Ê½JPEGÍ¼ÏñµÄ»º´æĞòÁĞµÄÊı¾İÓĞĞ§ĞÔ±êÖ¾Î»
	}

	BIsTrgFrameofBack = TRUE; //³õÉèÎªTRUE£¬ÓÃÓÚ´¥·¢ÊÕ¼¯±³¾°Í¼Ïñ´æÈëSD¿¨¡£
	
	//Ä¬ÈÏ²É¼¯Ä£Ê½ÎªYUVµãÕó
	Roseek_Set_ImgAcquisition_Format( YUV_ARRAY );//³õÉèYUV_ARRAY

	//ÉèÖÃÍ¼ÏñÔöÇ¿µÈ¼¶Îª3
	Roseek_Set_ImgEnhancement_Level(IMG_ENHANCEMENT_LEVEL3); //³õÉèIMG_ENHANCEMENT_LEVEL3
	
	//ÅäÖÃIO1ºÍIO2Îª1³µµÀÁ½¸öÏßÈ¦ÊäÈë£¬Á½¸öÏßÈ¦ÓÃÀ´²âËÙ£¬2ºÅÏßÈ¦´¥·¢×¥ÅÄ
	Roseek_SPIOInfConfig( 0, ui8SPIO1Config );//IO1
	Roseek_SPIOInfConfig( 1, ui8SPIO2Config );//IO2
	
	//ÅäÖÃIO3ºÍIO4ÎªÁ½¸öÏß¿ò³µÁ¾¼ì²âÊä³ö
	Roseek_SPIOInfConfig( 2, ui8SPIOUDConfig );//IO3
	Roseek_SPIOInfConfig( 3, ui8SPIOUDConfig );//IO4

	//ÅäÖÃIO5ºÍIO6ÎªÁ½¸ö³µµÀ¶Ó³¤¼ì²âÊä³ö£¬5ºÅ¶ÔÓ¦RoadNum[0]Â·£¬6ºÅ¶ÔÓ¦RoadNum[1]Â·
	Roseek_SPIOInfConfig( 4, ui8SPIOUDConfig );//IO5
	Roseek_SPIOInfConfig( 5, ui8SPIOUDConfig );//IO6

	//ÅäÖÃ¹âñîºÏÊä³ö¶Ë¿ÚISO1¡¢ISO2
	Roseek_ISOOUT_InfConfig( 0, g_ISOOutAtb );   //ISO1
	Roseek_ISOOUT_InfConfig( 1, g_ISOOutAtb );   //ISO1
	
	//Ê¹ÄÜÆØ¹âÍ¬²½ÖĞ¶Ï
	Roseek_Enable_ExpSynInt();

	//´Ó8MBµÄFlashÖĞ¶ÁÈ¡´æ´¢ĞÅÏ¢£¬³õÊ¼»¯ÏµÍ³ÔËĞĞ²ÎÊı
	Roseek_Flash_Read( &ui8Tmp, TRGINFFLAGADD, 1 ); //´ÓFlashµÄTRGINFFLAGADDµØÖ·¿ªÊ¼¶Á1¸ö×Ö½ÚÊı¾İµ½ui8Tmp=0xff»º³åÇø
	if( ui8Tmp!=0xff ){//0xff±íÊ¾Î´±£´æ¹ı×¥ÅÄĞÅÏ¢
		Roseek_Flash_Read( (Uint8*)&g_ImgInfStruct, TRGINFADD, sizeof(RoseekImgInfStruct) );
		g_ImgInfStruct.ui8LocusInf[100] = '\0'; //×¥ÅÄµØµãĞÅÏ¢£¬ÏŞ50¸öºº×Ö,³õÊ¼»¯\0
		Roseek_Flash_Read( (Uint8*)&g_TrgCtrlInfStruct, TRGCTRLINFADD, sizeof(RoseekTrgCtrlInfStruct) );
		Roseek_Flash_Read( (Uint8*)&g_TrgBrightnessAdj_Params, TRGCTRLPARAMADD, sizeof(RoseekImgYMeanAutoAdjParams) );
	}

	//¹Ø±Õ´¥·¢Ä£Ê½ÏÂµÄÉÁ¹âµÆÍ¬²½Êä³ö
	Roseek_Disable_TrgFlashlamp_Syn_Out();
	//¹Ø±ÕÁ¬ĞøÄ£Ê½ÏÂµÄÉÁ¹âµÆÍ¬²½Êä³ö
	Roseek_Disable_FconFlashlamp_Syn_Out();


	
	//ÅäÖÃ²â¹âÇøÓòp15º¯Êı26£©
	Roseek_Set_MIBlockParam( 1, 400, 400, 832, 1216 );//³õÉè1, 400, 400, 832, 1216
	//³õÊ¼»¯´«¸ĞÆ÷µÄÆØ¹âÊ±¼äºÍĞÅºÅÄÚ²¿ÔöÒæ
	switch( Roseek_Get_CurRunMode() ){
		case TrgMode:
			Roseek_Set_Afe_Pga( TrgMode,g_TrgBrightnessAdj_Params.fCurrentAfePga_db );//ÉèÖÃTrgModeÔËĞĞÄ£Ê½ÏÂ´«¸ĞÆ÷Êä³öĞÅºÅÔöÒæ
			Roseek_Set_TrgShutter_Time( g_TrgBrightnessAdj_Params.ui32CurrentShutterWidth_us );
			break;
		case FconMode:
			Roseek_Set_Afe_Pga( FconMode,g_FconBrightnessAdj_Params.fCurrentAfePga_db );
			Roseek_Set_FConShutter_Time( g_FconBrightnessAdj_Params.ui32CurrentShutterWidth_us );
			break;
		default:
			break;
	}

	//³õÊ¼»¯JPG±àÂëÆ÷µÄ²ÎÊı
	Roseek_JPGENC_Init( 2048, 2048 );//´ı±àÂëÍ¼ÏñµÄ×î´ó·Ö±æÂÊÎª2048*2048

	//³õÊ¼»¯´®¿Ú¹¤×÷Ä£Ê½
	Roseek_SetRS232RcvBufDepth( Roseek_RS232_PORT1, 256 );
	Roseek_RS232_SetBaudrate( Roseek_RS232_PORT1, Roseek_RS232_BAUDRATE_38400 );
	Roseek_RS232_Open( Roseek_RS232_PORT1 );
	Roseek_SetRS232RcvBufDepth( Roseek_RS232_PORT2, 256 );
	Roseek_RS232_SetBaudrate( Roseek_RS232_PORT2, Roseek_RS232_BAUDRATE_38400 );
	Roseek_RS232_Open( Roseek_RS232_PORT2 );
	Roseek_SetRS485RcvBufDepth( 256 );
	Roseek_RS485_SetBaudrate(Roseek_RS485_BAUDRATE_38400);
	Roseek_RS485_Open();

	//ÉèÖÃDMAÄ£¿éÏµÍ³×ÜÏßÓÅÏÈ¼¶£¬²»½¨ÒéÓÃ»§ĞŞ¸Ä¸ÃAPIµÄ²ÎÊı
	Roseek_Set_DMASysPriority(3);

	//ÉêÇëDAT×ÊÔ´

	g_hDAT0 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT1 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT2 = Roseek_DAT_Open(QDMA_OPT_PRI_HIGH);
	g_hDAT3 = Roseek_DAT_Open(QDMA_OPT_PRI_MEDIUM);
	g_hDAT4 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT5 = Roseek_DAT_Open(QDMA_OPT_PRI_URGENT);
	g_hDAT6 = Roseek_DAT_Open(QDMA_OPT_PRI_HIGH);
	g_hDAT7 = Roseek_DAT_Open(QDMA_OPT_PRI_MEDIUM);
	
	//ÉèÖÃÏà»ú³õÊ¼Ä£Ê½ÎªÁ¬ĞøÄ£Ê½,25fps
	Roseek_Mode_Switch_Ctrl( FconMode, EE3_380_DRCLK_HF);//³õÉèFconMode, EE3_380_DRCLK_HF
	//³õÉèÖÃ´¥·¢Ä£Ê½ÎªÈ«·Ö±æÂÊÍ¼Ïñ£¬²»¾­¹ıËõ·Å
	Roseek_Set_TrgFconMode_ScaleLevel(TrgMode,TRGFCON_SCALE_LEVEL_1);//³õÉèTrgMode,TRGFCON_SCALE_LEVEL_0
	//³õÉèÖÃÁ¬ĞøÄ£Ê½ÎªÈ«·Ö±æÂÊÍ¼Ïñ£¬²»¾­¹ıËõ·Å
	Roseek_Set_TrgFconMode_ScaleLevel(FconMode,TRGFCON_SCALE_LEVEL_1);//³õÉèFconMode,TRGFCON_SCALE_LEVEL_0
	
	//´ò¿ªÍ¼Ïñ²É¼¯Í¨µÀ
	Roseek_Open_Imagechannel();
}

/**
 * \function 	Roseek_Allocate_Memory
 * \brief    	ÉêÇëÊı¾İ»º´æ
 * \note		ÓÃ»§±ØĞë½«ÏµÍ³ÔËĞĞÊ±Ê¹ÓÃµÄ»º´æÔÚDSP/BIOSÏµÍ³ÔËĞĞ®Ç°½øĞĞÉêÇë£¬¾¡Á¿±ÜÃâÔÚÔËĞĞÖĞ¶¯Ì¬ÉêÇë¡¢ÊÍ·Å´æµÄ²Ù×÷	
 * \			
**/	
void Roseek_Allocate_Memory()
{
	// ÉêÇëÍ¼Ïñ²É¼¯Ô­Ê¼Êı¾İ»º´æ
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
	
	// ÉêÇëITSÓ¦ÓÃÍ¼Ïñ´¦ÀíËùĞèµÄÍ¼ÏñÊı¾İ»º´æ
	//³µÁ¾¼ì²â±³¾°
	g_ImgBufofBackVD.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL*2, 256);
	while( g_ImgBufofBackVD.pui8YCrCb_YBuf==NULL);
	//¶Ó³¤¼ì²â±³¾°
	g_ImgBufofBackS.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofBackS.pui8YCrCb_YBuf==NULL);

	g_ImgBufofBackS2.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofBackS2.pui8YCrCb_YBuf==NULL);
	
    // Ç°¾°
	g_ImgBufofFrontS.pui8FrameBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL*3, 256);
	while( g_ImgBufofFrontS.pui8FrameBuf==NULL);

	g_ImgBufofFrontS.pui8YCrCb_YBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofFrontS.pui8YCrCb_YBuf==NULL);

	g_ImgBufofFrontS.pui8YCrCb_CbBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofFrontS.pui8YCrCb_CbBuf==NULL);

	g_ImgBufofFrontS.pui8YCrCb_CrBuf = MEM_alloc( extHeap,  ALG_ROW*ALG_COL, 256);
	while( g_ImgBufofFrontS.pui8YCrCb_CrBuf==NULL);

	// ÉêÇë"¶ÓÁĞ¼ì²â"Ëã·¨¹ı³ÌËùĞè»º´æ
    g_algfuncData.ay_data = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_algfuncData.ay_data==NULL);

	g_algfuncData.aflag = MEM_alloc( extHeap,  sizeof(float)*ALG_ROW, 256);
	while( g_algfuncData.aflag==NULL);

	g_algfuncData.aforeground = MEM_alloc( extHeap,  sizeof(Lineinfo)*ALG_ROW, 256);
	while( g_algfuncData.aforeground==NULL);

	// ÉêÇë"³µÁ¾¼ì²â"Ëã·¨¹ı³ÌËùĞè»º´æ
    g_VDalgfuncData.f_data = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.f_data==NULL);

	g_VDalgfuncData.pro_imgback = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.pro_imgback==NULL);

	g_VDalgfuncData.tmpdata1 = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.tmpdata1==NULL);

	g_VDalgfuncData.tmpdata2 = MEM_alloc( extHeap,  ALG_COL*ALG_ROW, 256);
	while( g_VDalgfuncData.tmpdata2==NULL);
	
	// ÉêÇë±³¾°½¨Ä£Ëã·¨¹ı³ÌËùĞè»º´æ
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

	// ÉêÇë·ÇTrgModeµÄJPEGÊı¾İËùĞèµÄ»º´æ
	g_JpgEncBufStruct.pui8JpgImgBuf[0] = MEM_calloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8JpgImgBuf[0]==NULL);

	g_JpgEncBufStruct.pui8JpgImgBuf[1] = MEM_calloc( extHeap, FINE_ROW*FINE_COL, 256);
	while( g_JpgEncBufStruct.pui8JpgImgBuf[1]==NULL);

    // ÉêÇëTrgModeµÄJPEGÊı¾İËùĞèµÄ»º´æ
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
