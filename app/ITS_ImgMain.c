/**
 * \file      	ITS_ImgMain.c 
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.3.3
 * \date      	2013.12.13
 * \brief     	ÖÇÄÜ½»Í¨Ó¦ÓÃ----Í¼Ïñ´¦ÀíÖ÷Ïß³Ì(³µÁ¾¼ì²â¹¦ÄÜ¡¢¶ÓÁĞ³¤¶È¼ì²â¹¦ÄÜ)
 * \update      Ä£¿é»¯
**/

#include "EE3_common.h"
#include "SDFS_filesys.h"
/**************************************************************************/
extern ITS_ImgBufStruct      g_ImgBufofBackS;
extern ITS_ImgBufStruct      g_ImgBufofBackS2;
extern ITS_ImgBufStruct      g_ImgBufofFrontS;
extern ITS_ImgBufStruct  	 g_ImgBufofBackVD;
extern RoseekImgBufStruct		g_ImgBufStruct[]; //Í¼ÏñÔ­Ê¼Êı¾İ»º´æ½á¹¹Ìå
extern int     PointDataVD[16];
extern int     PointDataQD[16];
extern BpalgData g_bpalgData;
extern BpalgData g_bpalgData2;
extern AcmdSem 		g_AmcdSem1;
extern AcmdSem 		g_AmcdSem2;
extern PcInData     g_BpInputData;
extern PcInData 	g_BpOutputData1;
extern PcInData 	g_BpOutputData2;
extern Uint32  nRunT[2];
extern volatile     float  RunTimeOnce[NUM_RUMTIME];
extern volatile    float   runTimeT[60];
extern CmdUData g_CmdUdata;
extern volatile	Bool	        g_bIsFcontoDecClientConnect;
extern volatile    Bool    g_bIsSendRoadInfConnect;
extern EE3_CfgStruct g_EE3Cur;//Ïà»úÔËĞĞ×´Ì¬
extern volatile Bool IsReadFlash;
extern CarInfo g_carInfo;
/**************************************************************************/
extern int GrayscaleQD(Uint8 *back_data,Uint8 *in_data,DpPoint *pt,Uint32 *firstip,Uint32 *lastip);
extern int GrayscaleVD(Uint8 *in_data, float *ww, float *mean, float *sd, DpPoint *ptP,
		 Uint8 *ptn, Uint8 *carNum,	float *nMat, Uint8 *back_data,	Uint8 numofframe);
extern void gaussian(Uint8 *b_data, Uint8 *i_data, float *ww, float *mean, float *sd,int yMin, int yMax, int xMin, int xMax);
extern void setwwmeansd(Float *ww, Float *mean, Float *sd);
extern void EE3_YUV2YCrCb420( Uint8*, Uint8*, Uint8*, Uint8*, Uint32, Uint32 );
extern void GaussianPram(Float *nMat, int k);
void DrawPoint(Uint8 *in_data, int *PointData);
void ReadPointDataFromFlash();
void ReadSetInfFormFlash(Bool isReadflash);
void PointDataConfig(DpPoint *pt, int *pointdata);
void VD_SemOut_IO(Uint8 *isC);
void ResetPointData();
void Count_RunTime(Uint32 *nRT);
void ITS_VehicleDetect(DpPoint *dpPVD);
void ITS_QueueDetect(DpPoint  *dpPQD);
int StringConvertInt(char *p);
void Test_PointDPlus();
void DecodingCfgData(char *bufstream, int n, char word[][30]);
/**************************************************************************/
/**
 * \function 	tskAcmdBDmainFunc
 * \brief    	Í¼Ïñ´¦ÀíÏµÍ³Ïß³Ì
 * \			¸ÃÄ£¿é½ÓÊÜImgAcqÖĞĞÅºÅ´¥·¢£¬Ã¿Ö¡Í¼Ïñ´¥·¢		
 * \            1¡¢		
 * \            2¡¢
**/	
void tskAcmdBDmainFunc()
{
	 Uint8   *pui8IMGbuffer;//µ±Ç°²É¼¯Ö¡Í¼Ïñ»º´æ
	 Uint8	 i;
	 DpPoint dpPointVD[2];
	 DpPoint dpPointQD[2];	 
	 Uint8	 ui8tmp = 0;
	 Bool	 bBreak = FALSE;			//ÇĞ³ö±êÖ¾
	 Bool    bReadOnce = TRUE;
	 Bool    bsemSendtoDecready = FALSE;        
	 static	 Uint8	ui8FrameIndex = 0; //Ö¡»º´æĞòºÅ

	 //×¼±¸Ëã·¨ËùĞèÊı¾İ
	 setwwmeansd( g_bpalgData.ww, g_bpalgData.mean, g_bpalgData.sd);
	 setwwmeansd( g_bpalgData2.ww, g_bpalgData2.mean, g_bpalgData2.sd);	
	 ResetPointData();//ÏßÈ¦ÇåÁã
	     
     while(TRUE)
	 {
	    SEM_pend(&sem_AcmdBDReady,SYS_FOREVER);
		TSK_sleep( 2 );
		nRunT[0] = Roseek_GetTickCount();//¿ªÊ¼¼ÆÊı
		//******¶ÁÈ¡FLASHÖĞµÄÅäÖÃ²ÎÊı£¬Æô¶¯ºó¶ÁÈ¡Ò»´Î¡£
		if(bReadOnce){
			ReadPointDataFromFlash();//¶ÁÏß¿òÅäÖÃÊı¾İ
			ReadSetInfFormFlash(IsReadFlash);//¶ÁÏà»úÔËĞĞ²ÎÊıÊı¾İ
			//Test_PointDPlus();//²âÊÔÓÃ£¬ÊÖ¶¯Éè¶¨Ïß¿ò´óĞ¡
			PointDataConfig(dpPointVD, PointDataVD);//³µÁ¾¼ì²âµÄÏß¿òÅäÖÃ
			PointDataConfig(dpPointQD, PointDataQD);//¶ÓÁĞ¼ì²âµÄÏß¿òÅäÖÃ
			bReadOnce=FALSE;
		}
				
		//ÅĞ¶Ïµ±Ç°´¦ÀíÄÄ¸öÔ­Ê¼Í¼Ïñ»º´æ
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
		if( !bBreak ){
			g_ImgBufStruct[ui8FrameIndex].bProcessLocked = FALSE;
			continue;
		}
		g_ImgBufofFrontS.pui8FrameBuf=g_ImgBufStruct[ui8FrameIndex].pui8FrameBuf;//¸´ÖÆÇ°¾°Í¼Ïñ
		bsemSendtoDecready = g_bIsFcontoDecClientConnect; //·¢ËÍÍ¼ÏñÏß³Ì×¼±¸Íê±Ï
		//¼¤»îÏòÅäÖÃ¹¤¾ß´«ÊäÍ¼ÏñÏß³ÌĞÅºÅÁ¿
		if(bsemSendtoDecready){
	    	SEM_post(&sem_FcontoDecReady);
		}
		
		//´Ë´¦µ÷ÓÃ»­ĞéÄâÏß¿òº¯Êı
		pui8IMGbuffer=g_ImgBufStruct[ui8FrameIndex].pui8FrameBuf;
		switch(g_EE3Cur.RunMode){
			//³µÁ¾¼ì²â
			case VEHICLE_DETECT:
				DrawPoint(pui8IMGbuffer, PointDataVD);
				break;
			//¶Ó³¤¼ì²â
			case QUEUE_DETECT:
				DrawPoint(pui8IMGbuffer, PointDataQD);
				break;
			default:
				break;
		}
 
		//Çå³ı L2 Cache
		BCACHE_wbAll();

		//******×ª»»Ô­Ê¼Í¼ÏñYUV422ÖÁYUV420¸ñÊ½£¬²¢ÌáÈ¡Y·ÖÁ¿
		EE3_YUV2YCrCb420(	pui8IMGbuffer,
							g_ImgBufofFrontS.pui8YCrCb_YBuf, g_ImgBufofFrontS.pui8YCrCb_CrBuf, g_ImgBufofFrontS.pui8YCrCb_CbBuf,
							ALG_COL, ALG_ROW );
		
		g_ImgBufStruct[ui8FrameIndex].bProcessLocked = FALSE;//Ô­Ê¼Í¼Ïñ½âËø	

		//¹¦ÄÜÅĞ¶Ï
		switch(g_EE3Cur.RunMode){
			//³µÁ¾¼ì²â
			case VEHICLE_DETECT:
				ITS_VehicleDetect(dpPointVD);
				if(g_bIsSendRoadInfConnect)
				{
					SEM_post( &sem_SendRoadInfReady );//Í¨Öª·¢ËÍÊı¾İĞÅºÅÁ¿
				}
				break;
			//¶Ó³¤¼ì²â
			case QUEUE_DETECT:
				ITS_QueueDetect(dpPointQD);
				break;
			//²âÊÔµ÷ÊÔ
			case TEST_MODE:
				break;
			default:
				break;
		}

   }//while
   
}//tsk

/************************************************************************************/
/**
 * \function 	VD_SemOut_IOº¯Êı
 * \brief    	½ÓÊÕ³µÁ¾¼ì²â½á¹ûÊı¾İ£¬´¥·¢IO¿ÚÊä³öLEDµÆÁÁ
 * \					
 * \			1¡¢Ê¹ÓÃIO3¡¢IO4Á½¸ö¿Ú£¬·Ö±ğ¶ÔÓ¦Á½¸öÏß¿ò¡£
 * \            2¡¢IOÊ¹ÓÃĞèÔÚmainÖĞ³õÊ¼»¯
**/
void VD_SemOut_IO(Uint8 *isC) {
		static Bool Iscarcome[2] = {FALSE, FALSE};
		//µÚÒ»¿ò
		switch(isC[0]){
			case 1:
				//¼¤»îIO3Êä³ö					
				Roseek_Set_Pio_Outvalue(2,1);//IO3,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
				Roseek_SetISOOut_AsSwitch_State(0, 1);//ISO-OUT1,¿ªÆô
				Iscarcome[0] = TRUE;
				break;
			case 0:
				Roseek_Set_Pio_Outvalue(2,0);//IO3,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
				Roseek_SetISOOut_AsSwitch_State(0, 0);//ISO-OUT1,¹Ø±Õ
				if(Iscarcome[0])
				{
					(g_carInfo.num[0])++;
					Iscarcome[0] = FALSE;
				}
				break;
			default:
				Roseek_Set_Pio_Outvalue(2,0);//IO3,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
				Roseek_SetISOOut_AsSwitch_State(0, 0);//ISO-OUT1,¹Ø±Õ
				break;
		}
		//µÚ¶ş¿ò
		switch(isC[1]){
			case 1:
				//¼¤»îIO4Êä³ö					
				Roseek_Set_Pio_Outvalue(3,1);//IO4,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
				Iscarcome[1] = TRUE;
				break;
			case 0:
				Roseek_Set_Pio_Outvalue(3,0);//IO4,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
				if(Iscarcome[1])
				{
					(g_carInfo.num[1])++;
					Iscarcome[1] = FALSE;
				}
				break;
			default:
				Roseek_Set_Pio_Outvalue(3,0);//IO4,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
				break;
		}	
}
	
/************************************************************************************/
/**
 * \function 	PointDataConfigº¯ÊıÎªÍ¼Ïñ´¦ÀíËã·¨ĞéÄâÏß¿òPoint²ÎÊı³õÊ¼»¯º¯Êı¡£
 * \brief    	ĞéÄâÏß¿òPointµã³õÊ¼»¯º¯Êı£¬ÔÚËã·¨ÖĞÊ¹ÓÃ¡£
 * \					
 * \			1¡¢ÒÔ¿òĞÄÎªÖĞĞÄ£¬µã0¶ÔÓ¦Î÷±±·½Ïò£¬µã1¶ÔÓ¦Î÷ÄÏ£¬2¶ÔÓ¦¶«±±£¬3¶ÔÓ¦¶«ÄÏ
 * \            2¡¢Á½¸ö¿ò
**/
void PointDataConfig(DpPoint *pt, int *pointdata) {
	
	int ptTmp[2][4][2];
	int i,j;
	//¶ÁÈ¡È«¾ÖPointDataÖĞµÄµã×ø±ê£¬PointDataÔÚÏµÍ³³õÊ¼»¯Ê±´ÓFlashÖĞ¶ÁÈ¡¡£
	for(i = 0; i < 2; i++)
		for(j = 0; j < 4; j++)
		{
			ptTmp[i][j][0] = pointdata[8 * i + 2 * j];//(x , y)×ø±êx
			ptTmp[i][j][1] = pointdata[8 * i + 2 * j + 1];//(x , y)×ø±êy
		}
	//¼ÆËãPoint²ÎÊı
	for(i = 0; i <2; i++)
	{
		(pt + i)->pt5x = (ptTmp[i][0][0] + ptTmp[i][2][0]) / 2;
		(pt + i)->pt5y = (ptTmp[i][0][1] + ptTmp[i][2][1]) / 2;
		(pt + i)->pt6x = (ptTmp[i][1][0] + ptTmp[i][3][0]) / 2;
		(pt + i)->pt6y = (ptTmp[i][1][1] + ptTmp[i][3][1]) / 2;

		(pt + i)->k1 = ((float) (ptTmp[i][1][1] - ptTmp[i][0][1])) / (ptTmp[i][1][0]
				- ptTmp[i][0][0] == 0 ? 1 : ptTmp[i][1][0] - ptTmp[i][0][0]);
		(pt + i)->k2 = ((float) (ptTmp[i][3][1] - ptTmp[i][2][1])) / (ptTmp[i][3][0]
				- ptTmp[i][2][0] == 0 ? 1 : ptTmp[i][3][0] - ptTmp[i][2][0]);
		(pt + i)->k3 = ((float) ((pt + i)->pt6y - (pt + i)->pt5y)) / ((pt + i)->pt6x - (pt + i)->pt5x);
		(pt + i)->k4 = ((float) (ptTmp[i][2][1] - ptTmp[i][0][1])) / (ptTmp[i][2][0]
				- ptTmp[i][0][0] == 0 ? 1 : ptTmp[i][2][0] - ptTmp[i][0][0]);
		(pt + i)->k5 = ((float) (ptTmp[i][3][1] - ptTmp[i][1][1])) / (ptTmp[i][3][0]
				- ptTmp[i][1][0] == 0 ? 1 : ptTmp[i][3][0] - ptTmp[i][1][0]);
		(pt + i)->b1 = (ptTmp[i][1][1] - (pt + i)->k1 * ptTmp[i][1][0]);
		(pt + i)->b2 = (ptTmp[i][3][1] - (pt + i)->k2 * ptTmp[i][3][0]);
		(pt + i)->b3 = ((pt + i)->pt6y - (pt + i)->k3 * (pt + i)->pt6x);
		(pt + i)->b4 = (ptTmp[i][2][1] - (pt + i)->k4 * ptTmp[i][2][0]);
		(pt + i)->b5 = (ptTmp[i][3][1] - (pt + i)->k5 * ptTmp[i][3][0]);

		(pt + i)->xMax = 0;
		(pt + i)->yMax = 0;
		(pt + i)->xMin = 99999;
		(pt + i)->yMin = 99999;
		for (j = 0; j < 4; j++) {
			if ((pt + i)->xMax < ptTmp[i][j][0])
				(pt + i)->xMax = ptTmp[i][j][0];
			if ((pt + i)->yMax < ptTmp[i][j][1])
				(pt + i)->yMax = ptTmp[i][j][1];
			if ((pt + i)->xMin > ptTmp[i][j][0])
				(pt + i)->xMin = ptTmp[i][j][0];
			if ((pt + i)->yMin > ptTmp[i][j][1])
				(pt + i)->yMin = ptTmp[i][j][1];
		}
	}
}

/************************************************************************************/
/**
 * \function 	GetCfgDataFromFlash()º¯ÊıÎª´ÓFLASHÖĞ¶ÁÈ¡ÅäÖÃ£¨ÓëÅäÖÃ¹¤¾ßÍ¨ĞÅ»ñÈ¡£©¡£
 * \brief    	´ÓFlashÖĞ¶ÁÈ¡Ô­Ê¼ÅäÖÃÊı¾İ£¬²¢½øĞĞÊı¾İ½âÎö¡£
 * \					
 * \			1¡¢ĞéÄâÏß¿òÅäÖÃÊı¾İ(·ÖVDºÍQDÁ½¸öÏß¿ò)
 * \            2¡¢³µµÀ±àºÅÅäÖÃÊı¾İ
**/

void ReadPointDataFromFlash(){
	int 	i,j,k,n;
	char 	background[2][80];
	char    tmpNum[3] = {0,0,0};
	//´ÓFLASHÖĞ¶ÁÈ¡ĞéÄâÏß¿òÅäÖÃ
	Roseek_Flash_Read((Uint8 *)background[0],CFGVDPOINTADD,80);//³µÁ¾¼ì²âVD¿ò
	Roseek_Flash_Read((Uint8 *)background[1],CFGQDPOINTADD,80);//¶Ó³¤¼ì²âQD¿ò
	//Êı¾İ½âÎö
	j=0;
	k=0;
	for(i=0;i<2;i++)
	{
		for(n=0;n<80;n++){
			if(background[i][n]>='0'&&background[i][n]<='9'){//Êı×Ö×ª´æ
			 	tmpNum[j]=10*tmpNum[j]+(background[i][n] - '0');//char×ªÕûĞÍÊı
			 	++j;
			}
			if(background[i][n]==' '||background[i][n]=='\r'){
				switch(j){
					case 1:
						if(i == 0)
							PointDataVD[k]=tmpNum[0];
						else if(i == 1)
							PointDataQD[k]=tmpNum[0];
						break;
					case 2:
						if(i == 0)
							PointDataVD[k]=10*tmpNum[0]+tmpNum[1];
						else if(i == 1)
							PointDataQD[k]=10*tmpNum[0]+tmpNum[1];
						break;
					case 3:
						if(i == 0)
							PointDataVD[k]=100*tmpNum[0]+10*tmpNum[1]+tmpNum[2];
						else if(i == 1)
							PointDataQD[k]=100*tmpNum[0]+10*tmpNum[1]+tmpNum[2];
						break;
					default:
						break;
				}
				//ÁÙÊ±Á¿¹éÁã
				j=0;
				tmpNum[0]=0;
				tmpNum[1]=0;
				tmpNum[2]=0;
				++k;
			}
			if(k==16){//16¸ö×ø±êÊı¾İ¶ÁÈ¡Íê±Ï
				k=0;
				break;
			}
		}
	}

}
/**
 * \function 	ReadSetInfFormFlash()º¯ÊıÎª´ÓFLASHÖĞ¶ÁÈ¡Ïà»úÅäÖÃĞÅÏ¢¡£
 * \brief    	´ÓFlashÖĞ¶ÁÈ¡Ô­Ê¼ÅäÖÃÊı¾İ£¬²¢×ª´æÖÁµ±Ç°×´Ì¬½á¹¹Ìå¡£
 * \					
 * \			1¡¢FlashÆğÊ¼µØÖ·CFGCOMMONADD´æ´¢µ±Ç°ÔËĞĞ×´Ì¬
 * \            2¡¢FlashÆğÊ¼µØÖ·CFGVDADD´æ´¢³µÁ¾¼ì²âÅäÖÃ£¬FlashÆğÊ¼µØÖ·CFGQDADD´æ´¢¶Ó³¤¼ì²âÅäÖÃ
**/
void ReadSetInfFormFlash(Bool isReadflash)
{
	static int ee3mode = 0;
	char scfgtmpVD[30];
	char scfgtmpQD[30];
	char Sdata[80];
	char word[3][30];
	int n1 = 3;
	int n2 = 2;

	if(isReadflash)
	{	
		//´ÓFlashÖĞ¶ÁÅäÖÃÊı¾İ
		Roseek_Flash_Read((Uint8 *)Sdata, CFGCOMMONADD, 80);
		Roseek_Flash_Read((Uint8 *)scfgtmpVD, CFGVDADD, 30);
		Roseek_Flash_Read((Uint8 *)scfgtmpQD, CFGQDADD, 30);
		
		ee3mode = (int)(Sdata[0] - '0');
		if(ee3mode == VEHICLE_DETECT || ee3mode == QUEUE_DETECT || ee3mode == TEST_MODE)
		{
			//½âÎöÅäÖÃÊı¾İ
			g_EE3Cur.RunMode = ee3mode;
			DecodingCfgData(Sdata, n1, word);
			strcpy(g_EE3Cur.CameraIp, word[0]);
			strcpy(g_EE3Cur.ServerIp, word[1]);
			g_EE3Cur.ServerPort = (Uint16)(StringConvertInt(word[2]));
			DecodingCfgData(scfgtmpVD, n2, word);
			g_EE3Cur.UploadTime = StringConvertInt(word[0]);
			g_EE3Cur.NightTrafficStream = StringConvertInt(word[1]);
			DecodingCfgData(scfgtmpQD, n2, word);			
			g_EE3Cur.RoadNum[0] = StringConvertInt(word[0]);
			g_EE3Cur.RoadNum[1] = StringConvertInt(word[1]);
			g_EE3Cur.Isreadflash = 1;//±ê¼ÇÒÑ¶ÁÈ¡
		}
		else
		{
			g_EE3Cur.Isreadflash = 0;//Ä¬ÈÏÅäÖÃ		
		}
	}	
}
/**
 * \function 	DecodingCfgData()º¯ÊıÎª´Ó×Ö·ûÁ÷ÖĞ½âÎö×Ö·ûÊı¾İ²¢×ª´æ¡£
 * \brief    	ÊäÈë£º²ÎÊı*bufstream×Ö·ûÁ÷£¨¸ñÊ½Îª\n Word \r£©
 * \				  ²ÎÊıwordnumÒª½âÎöµÄ×Ö¶ÎÊı
 * \			Êä³ö£º²ÎÊıword[][30]×ª´æ×Ö·ûÊı¾İÊı×é£¨¸ñÊ½Îª Word \0£©		
 * \			×¢Òâ£º
**/
void DecodingCfgData(char *bufstream, int wordnum, char word[][30])
{	
	int i;
	int nCountRow = 0;//ÒÑ¶ÁÈ¡µÄ×Ö¶ÎÊı
	int nWord;//×Ö·ûÊı¼ÆÊı
	Bool isWord = FALSE;//±ê¼Ç

	//×ª´æÖÁwordÊı×éÖĞ
	for(i = 0;;i++)
	{
		if(bufstream[i] == '\n')
		{
			nWord = 0;
			isWord = TRUE;
			continue;
		}
		if(bufstream[i] == '\r')
		{
			word[nCountRow][nWord] = '\0';//Ìí¼ÓÎ²²¿±êÊ¶
			nCountRow++;
			isWord = FALSE;
			continue;
		}
		if(isWord)
		{
			word[nCountRow][nWord] = bufstream[i];
			nWord++;
		}
		if(nCountRow >= wordnum)
		{
			break;
		}
	}
			
}

/**
 * \function 	StringConvertInt()½«Êı×Ö×Ö·ûÁ÷×ª»»Îª¶ÔÓ¦µÄintĞÍ±äÁ¿¡£
 * \brief    	ÊäÈë£º*p×Ö·ûÁ÷£¨¸ñÊ½Àı¡°1234 \0"£©
 * \			Êä³ö£ºIntp£¨¸ñÊ½Àı 1234£©
 * \			×¢Òâ£º1¡¢ÒÔ'\0'½áÎ²£»2¡¢×¢Òâ³¤¶È£¬intÎª32Î»
 * \
**/
int StringConvertInt(char *p)
{
	int i;
	int nWord = 0;
	int Intp = 0;
	int tmp[10];
	for(i = 0;;i++)
	{
		if(p[i] == '\0')
			break;
		tmp[i] = (int)(p[i] - '0');
		nWord++;		
	}
	for(i = 0;i<nWord;i++)
	{
		Intp += tmp[i] * pow(10, (nWord - i - 1));
	}
	return(Intp);
}
	


/************************************************************************************/
/**
 * \function 	ResetPointData
 * \brief    	Ã¿´ÎÏµÍ³Æô¶¯Ê±µ÷ÓÃ£¬Ïß¿òÅäÖÃÊı¾İÖØÖÃ¡£
 **/
void ResetPointData(){
	int i;
	for(i=0; i<16; i++)
		PointDataVD[i] = 0;
	for(i=0; i<16; i++)
		PointDataQD[i] = 0;

}


/************************************************************************************/
/**
 * \function 	DrawPoint
 * \brief    	½«ĞéÄâÏß¿òÔÚÍ¼ÏñÉÏ±ê¼Ç,»­¾ØĞÎ¿ò
 * \					
 * \			1¡¢ÒÔ¿òĞÄÎªÖĞĞÄ£¬µã0¶ÔÓ¦Î÷±±·½Ïò£¬µã1¶ÔÓ¦Î÷ÄÏ£¬2¶ÔÓ¦¶«±±£¬3¶ÔÓ¦¶«ÄÏ
 * \            2¡¢Ã¿¸öĞéÄâÏß¿òÓÉËÄµãÈ·¶¨£¬È¡ËÄµãËù¸²¸ÇµÄ×î´ó¾ØĞÎ¿ò
 * \            3¡¢Ô­Ê¼Í¼ÏñÎªYUV422¸ñÊ½
**/
//È¡4¸ö±äÁ¿µÄ×î´ó¡¢×îĞ¡Öµº¯Êı
void Sortminmax(int a,int b,int c,int d,int *min,int *max){
	int e[4];
	int i;
	*min=a;
	*max=a;
	e[0]=a;
	e[1]=b;
	e[2]=c;
	e[3]=d;
	for(i=0;i<4;i++){
		if(e[i]<*min)
			*min=e[i];
		if(e[i]>*max)
			*max=e[i];
	}
}
//»­¿òº¯Êı
void DrawPoint(Uint8 *in_data, int *PointData){

		int ColMin,ColMax,RowMin,RowMax;
		int ptTmp[2][4][2];
		int i,j;
		//¶ÁÈ¡È«¾ÖPointDataÖĞµÄµã×ø±ê£¬PointDataÔÚÏµÍ³³õÊ¼»¯Ê±´ÓFlashÖĞ¶ÁÈ¡¡£
		for(i = 0; i < 2; i++){
			for(j = 0; j < 4; j++)
			{
				ptTmp[i][j][0] = PointData[8 * i + 2 * j];//(x , y)×ø±êx
				ptTmp[i][j][1] = PointData[8 * i + 2 * j + 1];//(x , y)×ø±êy
			}
		}
		//»­µÚÒ»Ïß¿ò
		//È¡ºá×ø±êXµÄ×î´ó¡¢×îĞ¡Öµ
		Sortminmax(ptTmp[0][0][0],ptTmp[0][1][0],ptTmp[0][2][0],ptTmp[0][3][0],&ColMin,&ColMax);
		//È¡×İ×ø±êyµÄ×î´ó¡¢×îĞ¡Öµ
		Sortminmax(ptTmp[0][0][1],ptTmp[0][1][1],ptTmp[0][2][1],ptTmp[0][3][1],&RowMin,&RowMax);
		for(i=(ALG_COL*RowMin+ColMin)*2;i<=(ALG_COL*RowMin+ColMax)*2;i=i+2){
		    *(in_data+i)=255;
			}
	    for(i=(ALG_COL*RowMax+ColMin)*2;i<=(ALG_COL*RowMax+ColMax)*2;i=i+2){
		    *(in_data+i)=255;
			}
		for(i=(ALG_COL*RowMin+ColMin)*2;i<=(ALG_COL*RowMax+ColMin)*2;i=i+2*ALG_COL){
		    *(in_data+i)=255;
			}
	    for(i=(ALG_COL*RowMin+ColMax)*2;i<=(ALG_COL*RowMax+ColMax)*2;i=i+2*ALG_COL){
		    *(in_data+i)=255;
			}
		//»­µÚ¶şÏß¿ò
		//È¡ºá×ø±êXµÄ×î´ó¡¢×îĞ¡Öµ
		Sortminmax(ptTmp[1][0][0],ptTmp[1][1][0],ptTmp[1][2][0],ptTmp[1][3][0],&ColMin,&ColMax);
		//È¡×İ×ø±êyµÄ×î´ó¡¢×îĞ¡Öµ
		Sortminmax(ptTmp[1][0][1],ptTmp[1][1][1],ptTmp[1][2][1],ptTmp[1][3][1],&RowMin,&RowMax);
		for(i=(ALG_COL*RowMin+ColMin)*2;i<=(ALG_COL*RowMin+ColMax)*2;i=i+2){
		    *(in_data+i)=255;
			}
	    for(i=(ALG_COL*RowMax+ColMin)*2;i<=(ALG_COL*RowMax+ColMax)*2;i=i+2){
		    *(in_data+i)=255;
			}
		for(i=(ALG_COL*RowMin+ColMin)*2;i<=(ALG_COL*RowMax+ColMin)*2;i=i+2*ALG_COL){
		    *(in_data+i)=255;
			}
	    for(i=(ALG_COL*RowMin+ColMax)*2;i<=(ALG_COL*RowMax+ColMax)*2;i=i+2*ALG_COL){
		    *(in_data+i)=255;
			}
	    //*(in_data+ALG_COL*P0y+P0x)=255;
	    //*(in_data+ALG_COL*P1y+P1x)=255;
	    //*(in_data+ALG_COL*P2y+P2x)=255;
	    //*(in_data+ALG_COL*P3y+P3x)=255;
}

/************************************************************************************/
/**
 * \function 	Count_RunTime
 * \brief    	¼ÆËã³ÌĞò¶ÎÔËĞĞÊ±¼äº¯Êı
 * \					
 * \			1¡¢¶ÁÈ¡³ÌĞòÖĞµÄÁ½¸ö¼ÆÊ±½Úµã£¬ËãÊ±¼ä²î
 * \            
**/
void Count_RunTime(Uint32 *nRT)
{
	static int 		nCountR = 0;
	static float    nSumRT = 0;
	if(nCountR < NUM_RUMTIME)
	{		
		RunTimeOnce[nCountR] = (abs(nRT[1] - nRT[0]))/1000;	//ºÁÃë
		nSumRT = nSumRT + RunTimeOnce[nCountR];
		nCountR++;		
	}
	if(nCountR == NUM_RUMTIME)
	{
		//¼ÇÂ¼½á¹û
		g_CmdUdata.nSumRT = nSumRT;
		g_CmdUdata.nFrame = NUM_RUMTIME / nSumRT * 1000;
		SEM_post( &sem_DataRunTimeN );//¼¤»îÉÏ´«Êı¾İ
		nSumRT = 0;
		nCountR = 0;
	}
}
/*****************************************************************************
******************************³µÁ¾¼ì²â¹¦ÄÜ***********************************
*****************************************************************************/
void ITS_VehicleDetect(DpPoint *dpPVD)
{
	//³µÁ¾¼ì²âËã·¨±äÁ¿
	static Uint8 ptn = COIL_NUM;
	static Uint8 frame_n = FRAMEOFBACK_NUM;
	static Uint8 gsk = QUEUE_LENGTH;
	float *nMatp;
	Uint8   *pui8inFrameBufY;//µ±Ç°Ç°¾°»º´æY·ÖÁ¿
	Uint8   *pui8BFrameBufYVD;//³µÁ¾¼ì²â±³¾°Í¼Ïñ»º´æY·ÖÁ¿
	static Uint8 isCar[2];

	//³µÁ¾¼ì²â²ÎÊı³õÊ¼»¯
	nMatp = (float *)malloc(sizeof(float) * ( gsk * 2 + 1));
	memset(nMatp, 0, sizeof(float) * ( gsk * 2 + 1));		
	GaussianPram(nMatp, gsk);
	pui8inFrameBufY = g_ImgBufofFrontS.pui8YCrCb_YBuf;//µ±Ç°Ç°¾°µÄY·ÖÁ¿
	pui8BFrameBufYVD = g_ImgBufofBackVD.pui8YCrCb_YBuf;//±³¾°Y·ÖÁ¿
    
    //³µÁ¾¼ì²âº¯Êı
	GrayscaleVD(pui8inFrameBufY, g_bpalgData.ww, g_bpalgData.mean, g_bpalgData.sd, dpPVD,
		&ptn, isCar, nMatp, pui8BFrameBufYVD, frame_n);
	//IOÊä³ö½á¹û
	VD_SemOut_IO(isCar);					
	free(nMatp);
	nRunT[1] = Roseek_GetTickCount();//ÖÕÖ¹¼ÆÊı
	//Ê±¼äÍ³¼Æ
	Count_RunTime(nRunT);	
}

/*****************************************************************************
***************************¶ÓÁĞ³¤¶È¼ì²â¹¦ÄÜ**********************************
*****************************************************************************/
void ITS_QueueDetect(DpPoint *dpPQD)
{
	Uint8   *pui8inFrameBufY;//µ±Ç°Ç°¾°»º´æY·ÖÁ¿
	Uint8   *pui8BFrameBufY1;//¶ÓÁĞ¼ì²â±³¾°Í¼Ïñ»º´æY·ÖÁ¿
	Uint8   *pui8BFrameBufY2;
	static  Bool    bDpSem1 = TRUE;           //±êÖ¾
	static  Bool    bDpSem2 = TRUE;
	Uint32    ui8Firstip1;
	Uint32    ui8Firstip2;
	Uint32    ui8Lastip1;
	Uint32    ui8Lastip2;
	Uint32    ui8RoadData1;
	Uint32    ui8RoadData2;

	pui8inFrameBufY = g_ImgBufofFrontS.pui8YCrCb_YBuf;//µ±Ç°Ç°¾°µÄY·ÖÁ¿
	
	//µÚÒ»Â·×´Ì¬¼ì²â
	if(g_AmcdSem1.bStart){
	      switch(g_AmcdSem1.comm){

			    //"ºì×ªÂÌ"×´Ì¬,¼¤»î±³¾°½¨Ä£
	            case NETINFORTG: 
	            	pui8BFrameBufY1 = g_ImgBufofBackS.pui8YCrCb_YBuf;//±³¾°½¨Ä£»ñµÃµÄ±³¾°Í¼ÏñY·ÖÁ¿           	          
		            //µ÷ÓÃ±³¾°½¨Ä£Ëã·¨
                    gaussian(pui8BFrameBufY1, pui8inFrameBufY, 
                    	g_bpalgData.ww, g_bpalgData.mean, g_bpalgData.sd, 
                    	dpPQD->yMin, dpPQD->yMax, dpPQD->xMin, dpPQD->xMax );
                    break;

			 	//¿ØÖÆ¶ËÇëÇó"¶ÓÁĞ¼ì²â"£¬·¢ËÍ½á¹û
		      	case NETREQUEST: 
		      		while(bDpSem1){//Ò»¸öÖÜÆÚ½öÖ´ĞĞÒ»´Î               		               					  
		                  //µ÷ÓÃ¶Ó³¤¼ì²âËã·¨
                          GrayscaleQD(pui8BFrameBufY1, pui8inFrameBufY,
                          		dpPQD, &ui8Firstip1, &ui8Lastip1 );
		                  //¶Ó³¤½á¹ûÉú³É
						  ui8RoadData1=ui8Firstip1-ui8Lastip1;
						  //Èİ´íÅĞ¶Ï
						  if(ui8RoadData1>0&&ui8RoadData1<999){
                          		g_BpOutputData1.data=abs(ui8RoadData1);//¶Ó³¤¼ì²âá¹û
								g_BpOutputData1.agree=1;
						  }
						  else{
								g_BpOutputData1.data=0;
								g_BpOutputData1.agree=0;
						  }
						  g_BpOutputData1.cameraN=g_EE3Cur.RoadNum[0];//³µµÀºÅ
                          g_BpOutputData1.comm=NETRETURN; //·µ»ØÃüÁî    			 			   
		  				  bDpSem1=FALSE;
		  			}
		  			//¼¤»î¶ÓÁĞ¼ì²â½á¹ûÊı¾İÉÏ´«
		  			SEM_post( &sem_DpDataReady1 ); 								
		  			//¼¤»îJPEG±àÂëºó´æÈëSD¿¨Ïß³Ì
					//SEM_post( &sem_JpegtoSDReady ); 
					//¼¤»îIO5Êä³ö					
					Roseek_Set_Pio_Outvalue(4,1);//IO5,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½ 					
					//Roseek_SetISOOut_AsSwitch_State( 0, 1);//ISO-OUT1,¿ªÆô
					TSK_sleep( 2 );            		                                    
                    break; 

				//"ÂÌ×ªºì"×´Ì¬£¬ÔòµÈ´ı£¬¶ÓÁĞ¼ì²â×¼±¸,Ö´ĞĞ´ÎÊıÇåÁã
	         	case NETINFOGTR:
					bDpSem1=TRUE;
					Roseek_Set_Pio_Outvalue(4,0);//IO4,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
					//Roseek_SetISOOut_AsSwitch_State( 0, 0);//ISO-OUT1,¹Ø±Õ
					TSK_sleep( 2 );
		        	break;

	         	default:
	            	break;
	   	  }
		  
	    }

		//µÚ¶şÂ·×´Ì¬¼ì²â
		if(g_AmcdSem2.bStart){
	        switch(g_AmcdSem2.comm){

				//"ºì×ªÂÌ"×´Ì¬,¼¤»î±³¾°½¨Ä£
	            case NETINFORTG:
	            	pui8BFrameBufY2 = g_ImgBufofBackS2.pui8YCrCb_YBuf;//±³¾°½¨Ä£»ñµÃµÄ±³¾°Í¼ÏñY·ÖÁ¿    	          		             	
		            //µ÷ÓÃ±³¾°½¨Ä£Ëã·¨
                    gaussian(pui8BFrameBufY2, pui8inFrameBufY, 
                    	g_bpalgData2.ww, g_bpalgData2.mean, g_bpalgData2.sd, 
                    	(dpPQD + 1)->yMin, (dpPQD + 1)->yMax, (dpPQD + 1)->xMin, (dpPQD + 1)->xMax );
                    break;

			  	//¿ØÖÆ¶ËÇëÇó"¶ÓÁĞ¼ì²â"
		      	case NETREQUEST:
		      		while(bDpSem2){//Ò»¸öÖÜÆÚ½öÖ´ĞĞÒ»´Î    		                 						  
		                  //µ÷ÓÃ¶ÓÁĞ¼ì²âËã·¨
                          GrayscaleQD(pui8BFrameBufY2, pui8inFrameBufY,
                          		(dpPQD + 1), &ui8Firstip2, &ui8Lastip2 );
						  
		                  //½á¹ûÉú³É²¢×÷Èİ´íÅĞ¶Ï
						  ui8RoadData2=ui8Firstip2-ui8Lastip2;
						  if(ui8RoadData2>0&&ui8RoadData2<999){
                          		g_BpOutputData2.data=abs(ui8RoadData2);//¶Ó³¤¼ì²â½á¹û
								g_BpOutputData2.agree=1;
						  }
						  else{
								g_BpOutputData2.data=0;
								g_BpOutputData2.agree=0;
						  }
						  g_BpOutputData2.cameraN=g_EE3Cur.RoadNum[1];//³µµÀºÅ			  
                          g_BpOutputData2.comm=NETRETURN; //·µ»ØÃüÁî		  			 			   
		  				  bDpSem2=FALSE;
		  			}
		  			//¼¤»î¶ÓÁĞ¼ì²â½á¹ûÊı¾İÉÏ´«
		  			SEM_post( &sem_DpDataReady2 ); 
		  			//¼¤»îJPEG±àÂëºó´æÈëSD¿¨Ïß³Ì
		  			//SEM_post( &sem_JpegtoSDReady ); 
		  			//¼¤»îIO6Êä³ö
					Roseek_Set_Pio_Outvalue(5,1);//IO6,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½ 
					//Roseek_SetISOOut_AsSwitch_State( 1, 1);//ISO-OUT2,¿ªÆô
					TSK_sleep( 2 );                                                        		                  
                    break; 

				//"ÂÌ×ªºì"×´Ì¬£¬ÔòµÈ´ı£¬¶ÓÁĞ¼ì²â×¼±¸,Ö´ĞĞ´ÎÊıÇåÁã
	         	case NETINFOGTR:
					bDpSem2=TRUE;
					Roseek_Set_Pio_Outvalue(5,0);//IO6,1Îª¸ßµçÆ½¡¢0ÎªµÍµçÆ½
					//Roseek_SetISOOut_AsSwitch_State( 1, 0);//ISO-OUT2,¹Ø±Õ
					TSK_sleep( 2 );
		       		break;

	         	default:
	           		break;
	     	}

	  	}
}


/************************************************************************************/
/**
 * \function 	Test_PointDPlus
 * \brief    	ÊÖ¶¯»­¿òº¯Êı£¬²Î¿¼ÎÄµµµãµÄ²¼¾Ö
 * \					
 * \			
 * \            
**/
void Test_PointDPlus()
{
	static int step = 60;
	//Ïß¿òµÄµã×ø±ê
	PointDataVD[0] = 0;
	PointDataVD[1] = 0;
	PointDataVD[2] = 0;	
	PointDataVD[3] = step;
	PointDataVD[4] = ALG_COL;
	PointDataVD[5] = 0;
	PointDataVD[6] = ALG_COL;	
	PointDataVD[7] = step;

}
