//////////////////////////////////////////////////////////////////////
// File name: EE3_common.h      		                            //    
//////////////////////////////////////////////////////////////////////

/**
 * \file      	EE3_common.h
 * \author    	LQ
 * \version   	0.05
 * \date      	2012.12.1
 * \brief     	DSP Demo³ÌĞòÖĞµÄÓÃµ½µÄºê¶¨Òå£¬ºÍÏà¹Ø½á¹¹Ìå¡£
 *				¸ÃÎÄ¼şÖĞµÄÄÚÈİÔÊĞíÓÃ»§¸ù¾İĞèÒª×Ô¶¨Òå¸ü¸Ä¡£
 *				ÓÃ»§´´½¨µÄÔ´ÎÄ¼şÖĞĞèÒª°üº¬±¾Í·ÎÄ¼ş
 *              £¡¸üĞÂ£º¼ÓÈë½á¹û½á¹¹Ìå
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


//Í¼Ïñ´¦ÀíËã·¨µÄĞĞÁĞ·Ö±æÂÊ	
#define ALG_ROW                         544u   
#define ALG_COL                         960u 
//¹¦ÄÜMode¶¨Òå
#define TEST_MODE                       0       //ÎŞÍ¼Ïñ´¦Àí¹¦ÄÜÄ£Ê½£¨²âÊÔÓÃ£©
#define QUEUE_DETECT					1      	//¹¦ÄÜÒ»£º¶Ó³¤¼ì²â
#define VEHICLE_DETECT					2		//¹¦ÄÜ¶ş£º³µÁ¾¼ì²â
#define WORD_NUM 	7

/**********³µÁ¾¼ì²âÓĞ¹Ø***********/
#define NUM_RUMTIME 	100     	//Í³¼ÆÔËĞĞ´ÎÊı
#define QUEUE_LENGTH 	11		//¸ßË¹ÂË²¨³¤¶È
#define COIL_NUM     	2  		//ÏßÈ¦Êı
#define FRAMEOFBACK_NUM	5		//Ã¿5Ö¡Ò»´Î±³¾°½¨Ä£	

/***ÓëºóÌ¨¿ØÖÆÏµÍ³µÄÍ¨ĞÅĞ­Òé***/
#define ACMDPORT 	6366		//¶Ë¿Ú
#define HEADCODE 	0xFFFF		//Ğ­ÒéÍ·
#define NETREQUEST 	100			//ÏòÉãÏñÍ·ÇëÇó¶Ó³¤
#define NETRETURN  	101			//ÉãÏñÍ··µ»Ø¶Ó³¤
#define NETINFORTG 	103			//Í¨Öªºì×ªÂÌ
#define NETINFOGTR 	104	    	//Í¨ÖªÂÌ×ªºì

/***ÓëÔ¶³ÌÅäÖÃ¹¤¾ßµÄÍ¨ĞÅĞ­Òé***/
//Í¨ĞÅ¶Ë¿Ú
#define CONTROLPORT 4500
#define VIDEOPORT 	4501
#define UDPPORT   	4502 
//ÅäÖÃÃüÁî 
#define DEVICETYPE		62  //Éè±¸ÀàĞÍEagleEye3ĞÍ
#define BROADCAST1 	   	0
#define GETBACKGROUND  	1  
#define SETPOINT    	3   //ĞéÄâÏßÈ¦ÅäÖÃÃüÁî
#define REBOOT      	4   //ÖØÆôÏà»ú
#define STARTVIDEO  	7   //¿ªÊ¼·¢ËÍVideo
#define ENDVIDEO    	8   //Í£Ö¹·¢ËÍVideo
#define CHOOSECAM   	10  //Ñ¡ÔñÏà»ú
#define EE3_SENDCONFIG	16  //Ïà»ú²ÎÊıÅäÖÃ·¢ËÍ
#define EE3_SETCONFIG	17	//Ïà»ú²ÎÊıÅäÖÃ½ÓÊÜ
#define CANCELCMD    	20  //È¡Ïûµ±Ç°²Ù×÷
#define UDPCON     		99  //UDPÇëÇó
#define UDPSETTIME 		100 //UDP¹ã²¥ÏµÍ³Ê±¼ä
/***Ïà»úÅäÖÃĞÅÏ¢±£´æµØÖ·£¬×Ô¶¨Òå***/
#define	CFGCOMMONADD					0x102000	//length = 80 Byte£¬´æ´¢Í¨ÓÃÅäÖÃĞÅÏ¢
#define CFGVDADD						0x102100	//length = 30 Byte£¬´æ´¢³µÁ¾¼ì²â»ù±¾ÅäÖÃ
#define CFGQDADD						0x102200	//length = 30 Byte£¬´æ´¢¶Ó³¤¼ì²â»ù±¾ÅäÖÃ
#define CFGVDPOINTADD					0x102300	//length = 80 Byte£¬´æ´¢³µÁ¾¼ì²âĞéÄâÏß¿òÅäÖÃ
#define CFGQDPOINTADD					0x102400	//length = 80 Byte£¬´æ´¢¶Ó³¤¼ì²âĞéÄâÏß¿òÅäÖÃ

/***×¥ÅÄĞÅÏ¢±£´æµØÖ·,DEMOÊ¹ÓÃ£¬ÓÃ»§¿ÉĞŞ¸Ä***/
#define	TRGINFFLAGADD					0x000000	//length = 1 Byte
#define	TRGINFADD						0x000001	//length = 256 Byte
#define	TRGCTRLINFADD					0x000101	//length = 256 Byte
#define	TRGCTRLPARAMADD					0x000201	//length = 256 Byte
#define	NETPORTINITFLAGADD				0x000301	//length = 1 Byte, 0x55ÓÃÀ´±íÊ¾ÒÑ´æĞÅÏ¢,ÏµÍ³ÉÏµç½«¶Á³öÒÑ´æĞÅÏ¢À´½øĞĞ³õÊ¼»¯,·ñÔò½«Ê¹ÓÃÄ¬ÈÏĞÅÏ¢³õÊ¼»¯
#define	NETPORTINFADD					0x000302	//length = 12 Byte, ±£´æË³ĞòÒÀ´ÎÎªTrg,Fcon,Hfr;¸÷Õ¼ÓÃ4¸ö×Ö½Ú£¬ÎªÎŞ·ûºÅÕûĞÍÊı

/**************************************************************************/
//Í¼ÏñÔ­Ê¼Êı¾İ»º´æ½á¹¹Ìå£¬ÆäÊµÀı»¯¶ÔÏóÓÃÀ´´æ´¢²É¼¯Ä£¿é½ÓÊÕµ½µÄÍ¼ÏñÔ­Ê¼Êı¾İ¼°Ïà¹ØĞÅÏ¢
typedef struct ImgBufStruct{
	Uint8	*pui8FrameBuf;				//Í¼Ïñ»º´æ
	Uint8	ui8RotateFlag;				//Í¼ÏñĞı×ª±êÖ¾
	Uint8	ui8TrgFconScaleLel;			//´¥·¢ºÍÈ«·Ö±æÂÊÁ¬ĞøÄ£Ê½ÏÂµÄËõ·Å¼¶±ğ
	Bool	bProcessLocked;				//»º´æÊı¾İ¼ÓËø±êÖ¾£¬TRUE±íÊ¾Êı¾İÕıÔÚ±»´¦Àí£¬FALSE±íÊ¾Êı¾İ¿ÕÏĞ
	int		iTrgSurFlag;				//ÈôÍ¼Ïñ²É¼¯ÓÚ´¥·¢Ä£Ê½ÏÂ£¬±íÊ¾±¾ìõÍ¼ÏñÀ´×ÔÓÚÄÄ¸ö´¥·¢Ô´
	Uint8	ui8TrgShotTime[8];			//´¥·¢Ä£Ê½ÏÂµÄ×¥ÅÄÊ±¼ä£¬Äê-ÔÂ-ÈÕ-ĞÇÆÚ-Ê±-·Ö-Ãë-Ê®±¶ºÁÃë£¬Á¬ĞøÄ£Ê½ÏÂ²»Ê¹ÓÃ£¬ÎŞĞ§
	Uint8	ui8TrgMode;					//´¥·¢·½Ê½£¬0->Èí´¥·¢;1->µ¥ÏßÈ¦;2->Ë«ÏßÈ¦(²âËÙ)£¬Á¬ĞøÄ£Ê½ÎŞĞ§
	float	fSpeed;						//´¥·¢Ä£Ê½ÏÂ³µÁ¾ËÙ¶È£¬½öË«ÏÈÈ¦´¥·¢ÓĞĞ§£¬Á¬ĞøÄ£Ê½ÎŞĞ§
	Bool    iTrgFrameofBack;             //ÊÇ·ñ×÷Îª±³¾°Í¼Ïñ
	enum Roseek_SensorRunMode CurImg_CorrespondingRunMode;	//µ±Ç°Í¼ÏñÔÚºÎÖÖÄ£Ê½ÏÂ²É¼¯(TrgMode, FconMode, HfrMode)
	enum Roseek_ImgAcquisition_Format CurImg_Format;			//µ±Ç°Í¼ÏñÎªºÎÖÖÄ£Ê½(RGB_ARRAY, YUV_ARRAY, BAYER_ARRAY)
}RoseekImgBufStruct;
/**************************************************************************/

/**************************************************************************/
//JPEGÍ¼Ïñ»º´æ½á¹¹Ìå£¬ÆäÊµÀı»¯¶ÔÏóÓÃÀ´´æ´¢JPEG±àÂëÄ£¿éÓÃµ½µÄÖĞ¼äÊı¾İ£¬ÒÔ¼°Êä³öµÄJPEGÍ¼ÏñÊı¾İ
typedef struct JPGEncBufStruct{
	Uint8	*pui8YCrCb_YBuf;			//±àÂëÆ÷ÊäÈëÍ¼ÏñµÄY·ÖÁ¿»º´æÖ¸Õë
	Uint8	*pui8YCrCb_CrBuf;			//±àÂëÆ÷ÊäÈëÍ¼ÏñµÄCr·ÖÁ¿»º´æÖ¸Õë
	Uint8	*pui8YCrCb_CbBuf;			//±àÂëÆ÷ÊäÈëÍ¼ÏñµÄCb·ÖÁ¿»º´æÖ¸Õë
	Uint8	*pui8JpgImgBuf[2];			//±àÂëÆ÷Êä³öµÄ·Ç´¥·¢·½Ê½JPEGÍ¼ÏñµÄ»º´æĞòÁĞ
	Uint8	*pui8TrgJpgImgBuf[6];		//±àÂëÆ÷Êä³öµÄ´¥·¢·½Ê½JPEGÍ¼ÏñµÄ»º´æĞòÁĞ
	Bool	bJpgImgBuf_Lock[2];			//±àÂëÆ÷Êä³öµÄ·Ç´¥·¢·½Ê½JPEGÍ¼ÏñµÄ»º´æĞòÁĞµÄÊı¾İÓĞĞ§ĞÔ±êÖ¾Î»
	Bool	bTrgJpgImgBuf_Lock[6];		//±àÂëÆ÷Êä³öµÄ´¥·¢·½Ê½JPEGÍ¼ÏñµÄ»º´æĞòÁĞÊı¾İÓĞĞ§ĞÔ±êÖ¾Î»
}RoseekJPGEncBufStruct;
/**************************************************************************/

/**************************************************************************/
//DEMOÖĞ¸ø³öµÄ¶¨Òå£¬¹²114¸ö×Ö½Ú£¬ÓÃ»§¿É¸ù¾İÊµ¼ÊÇé¿öĞŞ¸Ä
//×¥ÅÄĞÅÏ¢½á¹¹Ìå£¬ÇëÊµÀı»¯¶ÔÏóÓÃÓÚ´æ´¢ÉÏ´«µÄÍ¼ÏñÃèÊöĞÅÏ¢
typedef struct ImgInfStruct{
	char	ui8LocusInf[101];			//×¥ÅÄµØµãĞÅÏ¢£¬ÏŞ50¸öºº×Ö
	Uint8	ui8TimeInf[8];				//×¥ÅÄÊ±¼ä£¬Äê-ÔÂ-ÈÕ-ĞÇÆÚ-Ê±-·Ö-Ãë-10±¶ºÁÃë
	Uint8	ui8WayNum;					//³µµÀºÅ
	Uint8	ui8TrgMode;					//´¥·¢·½Ê½£¬0->Èí´¥·¢;1->µ¥ÏßÈ¦;2->Ë«ÏßÈ¦(²âËÙ)
	float	fSpeed;						//³µËÙ(µ¥Î»:¹«Àï/Ğ¡Ê±)   
	Bool    bIsTrgpic;                  //ÊÇ·ñ×÷Îª±³¾°Í¼Ïñ
}RoseekImgInfStruct;
/**************************************************************************/

/**************************************************************************/
//DEMOÖĞ¸ø³öµÄ¶¨Òå£¬ÓÃ»§¿É¸ù¾İÊµ¼ÊÇé¿öĞŞ¸Ä
//×¥ÅÄ¿ØÖÆĞÅÏ¢½á¹¹Ìå
typedef struct TrgCtrlInfStruct{
	Bool	bEnableADLI;				//ÊÇ·ñÔÊĞí×Ô²â¹â
	Uint8	ui8AutoDetcLigIllumPeriod;	//×Ô²â¹â×¥ÅÄÖÜÆÚ(µ¥Î»:Ãë)
	Uint32	ui32AutoDetcLigIllumCNT;	//×Ô²â¹â×¥ÅÄ¼ÆÊıÆ÷´¥·¢Öµ(ÓÉ×Ô²â¹â×¥ÅÄÖÜÆÚ»»ËãµÃµ½£¬DSP¶Ë¶¨ÒåµÄ¼ÆÊıÆ÷µİÔöÖÜÆÚÎª1Ãë)
	Bool	bADLIOccur;					//×Ô²â¹â±êÖ¾
	Bool	bExtTrgOccurWhenAutoDetcLig;//×Ô²â¹âÆÚ¼äÊÇ·ñ·¢ÉúÍâ´¥·¢
	Uint32	ui32FixedShutterTime;		//²»ĞèÒª×Ô²â¹âÊ±µÄ¹Ì¶¨¿ìÃÅÊ±¼ä(µ¥Î»:Î¢Ãë)
	float	fFixedPGA;					//²»ĞèÒª×Ô²â¹âÊ±µÄ¹Ì¶¨ÔöÒæ(µ¥Î»:dB)
	Uint8	ui8IsSingalCoin;			//ÊÇ·ñµ¥ÏßÈ¦ÏµÍ³
	float	fInteval;					//Èç¹ûÊÇË«ÏßÈ¦ÏµÍ³£¬ÏßÈ¦¼ä¾à(µ¥Î»:Ã×)	          
}RoseekTrgCtrlInfStruct;

/**************************************************************************
********Í¼Ïñ´¦Àí²¿·Ö
**************************************************************************/

//Í¼Ïñ´¦ÀíÊı¾İ»º³å£¬ÊµÀı»¯¶ÔÏóÓÃÀ´´æ´¢Ç°¾°Óë±³¾°Í¼ÏñµÄYUV·ÖÁ¿
typedef struct ImgBufStructITS{
        Uint8   *pui8FrameBuf;              //Ô­Ê¼Í¼ÏñYUV422»º´æ
        Uint8	*pui8YCrCb_YBuf;			//Ô­Ê¼Í¼Ïñ×ªÎªYUV420ºóµÄY·ÖÁ¿»º´æÖ¸Õë
	    Uint8	*pui8YCrCb_CrBuf;			//Ô­Ê¼Í¼Ïñ×ªÎªYUV420ºóµÄCr·ÖÁ¿»º´æÖ¸Õë
	    Uint8	*pui8YCrCb_CbBuf;			//Ô­Ê¼Í¼Ïñ×ªÎªYUV420ºóµÄCb·ÖÁ¿»º´æÖ¸Õë
}ITS_ImgBufStruct;
//½¨Ä£ËÄ±ßĞÎ½á¹¹Ìå,ÓÃÓÚËã·¨²¿·Ö
typedef struct Point{
	int xMax;    
	int xMin;
	int yMax;
	int yMin;    //½¨Á¢¾ØĞÎ¿ò¶Ëµã×ø±ê
	int pt5x;
	int pt5y;
	int pt6x;
	int pt6y;    //ËÄ±ßĞÎÁ½¶Ô±ßµÄÖĞµã×ø±ê
	float k1;
	float k2;
	float k3;
	float k4;
	float k5;
	float b1;
	float b2;
	float b3;
	float b4;
	float b5;    //ÓÃËÄÌõy=kx+bÖ±ÏßÔÚ¾ØĞÎ¿ò·¶Î§ÄÚ¹¹½¨Ò»¸öËÄ±ßĞÎ£¬k3ÎªÖĞÏß
}DpPoint;
//Ëã·¨²¿·ÖÊ¹ÓÃ
typedef struct LineInfo{
	int foreground_count;//Ç°¾°Êı
	int total_pixel;//ÏñËØ×ÜÖµ
}Lineinfo;
//¶ÓÁĞ¼ì²âËã·¨²¿·ÖËùĞè»º´æ
typedef struct algfuncData{
    Uint8       *ay_data; 
    Lineinfo    *aforeground;
	float       *aflag;
}AlgfuncData;
//³µÁ¾¼ì²âËã·¨²¿·ÖËùĞè»º´æ
typedef struct VDalgfuncData{
	Uint8      *f_data;
	Uint8      *pro_imgback;
	Uint8      *tmpdata1;
	Uint8      *tmpdata2;
}VDalgfuncData;
//±³¾°½¨Ä£Ëã·¨²¿·ÖËùĞè»º´æ
typedef struct bpalgData{
    float   *ww;
	float   *mean;
	float   *sd;
}BpalgData;
//Êı³µ½á¹¹Ìå
typedef struct carInfo{
	int num[5];
	float speed[5];
}CarInfo;
/**************************************************************************
******ºóÌ¨¿ØÖÆÍ¨Å
**************************************************************************/

//ÓëºóÌ¨¿ØÖÆ¶ËÍ¨ĞÅ£¬¿ØÖÆĞÅÏ¢´æ´¢½á¹¹Ìå
typedef struct CmdInputData{
    Uint8 	head1;   		//Ğ­ÒéÍ·
	Uint8 	head2;			//Ğ­ÒéÍ·
	Uint8 	length;   		//³¤¶È
	Uint8 	label1;       	//±ê¼Ç
	Uint8 	label2;       	//±ê¼Ç
	Uint8  	cameraN;		//³µµÀºÅ
	Uint8  	comm;			//ÃüÁî	
	Uint8   agree;			//±ê¼Ç¶Ó³¤½á¹ûÊÇ·ñÕıÈ·£¬ÕıÈ·Îª1£¬´íÎóÎª0
	Uint8 	dataR;			//Ìî³ä
	Uint8   data;			//¶ÓÁĞ³¤¶È
}PcInData;

//´«ÊäĞÅÏ¢Í¨ÓÃ½á¹¹Ìå
typedef struct CmdUData{
	float  nSumRT;     //ÔËĞĞÊ±¼ä
	float  nFrame;	   //Ã¿Ãë´¦ÀíÖ¡Êı
}CmdUData;

//µ±Ç°¿ØÖÆĞÅÏ¢½á¹¹Ìå
typedef struct ACmdSem{
	Uint8  	comm;			//ÃüÁî
	Bool    bStart;			//Æô¶¯±ê¼Ç
}AcmdSem;

//·¢ËÍµÄ³µÁ¾ĞÅÏ¢½á¹¹Ìå
typedef struct sendInfo{
	Int label;
	Int num;
	Float speed;
	Char local_ip[20];//new
}SendInfo;

/**************************************************************************
*******Ô¶³ÌÅäÖÃ
**************************************************************************/

//ÓëÔ¶³ÌÅäÖÃ¹¤¾ßÍ¨ĞÅ£¬¿ØÖÆÃüÁî´æ´¢½á¹¹Ìå
typedef struct conversationHead{
	int command;         //ÃüÁî
	int bufsize;         //³¤¶È
}ConversationHead;

//Ïà»úµ±Ç°ÅäÖÃĞÅÏ¢½á¹¹Ìå
typedef struct eE3_CfgStruct{
		int 	RunMode;         		//¹¦ÄÜÄ£Ê½£¨0 ÎŞ£»1 ¹¦ÄÜÒ» £»2¹¦ÄÜ¶ş£©
		char	CameraIp[20];       	//Ïà»úµ±Ç°IPµØÖ·
		char    ServerIp[20];			//¹ÜÀíÆ½Ì¨IP
		int  	ServerPort;				//¹ÜÀíÆ½Ì¨¶Ë¿ÚºÅ
		int  	RoadNum[2];				//¼à¿Ø³µµÀ±àºÅ£¨½ö¹¦ÄÜ¶ş£©
		int		UploadTime;				//ÉÏ´«ÖÜÆÚ£¨½ö¹¦ÄÜÒ»£©
		int  	NightTrafficStream;		//Ò¹Íí³µÁ÷£¨½ö¹¦ÄÜÒ»£©		
		Uint8 	RealTime[8];			//Ïà»úÏµÍ³Ê±¼ä
		int     Isreadflash;			//ÊÇ·ñ¶ÁÈ¡ÁËFlashÖĞµÄÅäÖÃ		
}EE3_CfgStruct;
//Ïà»úµ±Ç°²ÎÊı½á¹¹Ìå
typedef struct EE3_StateStruct{
		char 	Version[40];			//Ö÷³ÌĞò°æ±¾ºÅ
		int  	Resolution;				//Ïà»ú·Ö±æÂÊ£¨1 960*544£» 2 1920*1080£©
}EE3_StateStruct;

/**************************************************************************/

/**************************************************************************/
//¼ÓÃÜĞ¾Æ¬AT88SCXXCÅäÖÃÇø½á¹¹Ìå
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



