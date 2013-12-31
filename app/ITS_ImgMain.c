/**
 * \file      	ITS_ImgMain.c 
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.3.4
 * \date      	2013.12.31
 * \brief     	���ܽ�ͨӦ��----ͼ�������߳�(������⹦�ܡ����г��ȼ�⹦��)
 * \update      ������ȡ������ϢBUG
**/

#include "EE3_common.h"
#include "SDFS_filesys.h"
/**************************************************************************/
extern ITS_ImgBufStruct      g_ImgBufofBackS;
extern ITS_ImgBufStruct      g_ImgBufofBackS2;
extern ITS_ImgBufStruct      g_ImgBufofFrontS;
extern ITS_ImgBufStruct  	 g_ImgBufofBackVD;
extern RoseekImgBufStruct		g_ImgBufStruct[]; //ͼ��ԭʼ���ݻ���ṹ��
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
extern EE3_CfgStruct g_EE3Cur;//�������״̬
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
Bool DecodingCfgData(char *bufstream, int n, char word[][30]);
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

/************************************************************************************/
/**
 * \function 	VD_SemOut_IO����
 * \brief    	���ճ�����������ݣ�����IO�����LED����
 * \					
 * \			1��ʹ��IO3��IO4�����ڣ��ֱ��Ӧ�����߿�
 * \            2��IOʹ������main�г�ʼ��
**/
void VD_SemOut_IO(Uint8 *isC) {
		static Bool Iscarcome[2] = {FALSE, FALSE};
		//��һ��
		switch(isC[0]){
			case 1:
				//����IO3���					
				Roseek_Set_Pio_Outvalue(2,1);//IO3,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
				Roseek_SetISOOut_AsSwitch_State(0, 1);//ISO-OUT1,����
				Iscarcome[0] = TRUE;
				break;
			case 0:
				Roseek_Set_Pio_Outvalue(2,0);//IO3,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
				Roseek_SetISOOut_AsSwitch_State(0, 0);//ISO-OUT1,�ر�
				if(Iscarcome[0])
				{
					(g_carInfo.num[0])++;
					Iscarcome[0] = FALSE;
				}
				break;
			default:
				Roseek_Set_Pio_Outvalue(2,0);//IO3,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
				Roseek_SetISOOut_AsSwitch_State(0, 0);//ISO-OUT1,�ر�
				break;
		}
		//�ڶ���
		switch(isC[1]){
			case 1:
				//����IO4���					
				Roseek_Set_Pio_Outvalue(3,1);//IO4,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
				Iscarcome[1] = TRUE;
				break;
			case 0:
				Roseek_Set_Pio_Outvalue(3,0);//IO4,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
				if(Iscarcome[1])
				{
					(g_carInfo.num[1])++;
					Iscarcome[1] = FALSE;
				}
				break;
			default:
				Roseek_Set_Pio_Outvalue(3,0);//IO4,1Ϊ�ߵ�ƽ��0Ϊ�͵�ƽ
				break;
		}	
}
	
/************************************************************************************/
/**
 * \function 	PointDataConfig����Ϊͼ�����㷨�����߿�Point������ʼ��������
 * \brief    	�����߿�Point���ʼ�����������㷨��ʹ�á�
 * \					
 * \			1���Կ���Ϊ���ģ���0��Ӧ�������򣬵�1��Ӧ���ϣ�2��Ӧ������3��Ӧ����
 * \            2��������
**/
void PointDataConfig(DpPoint *pt, int *pointdata) {
	
	int ptTmp[2][4][2];
	int i,j;
	//��ȡȫ��PointData�еĵ����꣬PointData��ϵͳ��ʼ��ʱ��Flash�ж�ȡ��
	for(i = 0; i < 2; i++)
		for(j = 0; j < 4; j++)
		{
			ptTmp[i][j][0] = pointdata[8 * i + 2 * j];//(x , y)����x
			ptTmp[i][j][1] = pointdata[8 * i + 2 * j + 1];//(x , y)����y
		}
	//����Point����
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
 * \function 	GetCfgDataFromFlash()����Ϊ��FLASH�ж�ȡ���ã������ù���ͨ�Ż�ȡ����
 * \brief    	��Flash�ж�ȡԭʼ�������ݣ����������ݽ�����
 * \					
 * \			1�������߿���������(��VD��QD�����߿�)
 * \            2�����������������
**/

void ReadPointDataFromFlash(){
	int 	i,j,k,n;
	char 	background[2][80];
	char    tmpNum[3] = {0,0,0};
	//��FLASH�ж�ȡ�����߿�����
	Roseek_Flash_Read((Uint8 *)background[0],CFGVDPOINTADD,80);//�������VD��
	Roseek_Flash_Read((Uint8 *)background[1],CFGQDPOINTADD,80);//�ӳ����QD��
	//���ݽ���
	j=0;
	k=0;
	for(i=0;i<2;i++)
	{
		for(n=0;n<80;n++){
			if(background[i][n]>='0'&&background[i][n]<='9'){//����ת��
			 	tmpNum[j]=10*tmpNum[j]+(background[i][n] - '0');//charת������
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
				//��ʱ������
				j=0;
				tmpNum[0]=0;
				tmpNum[1]=0;
				tmpNum[2]=0;
				++k;
			}
			if(k==16){//16���������ݶ�ȡ���
				k=0;
				break;
			}
		}
	}

}
/**
 * \function 	ReadSetInfFormFlash()����Ϊ��FLASH�ж�ȡ���������Ϣ��
 * \brief    	��Flash�ж�ȡԭʼ�������ݣ���ת������ǰ״̬�ṹ�塣
 * \					
 * \			1��Flash��ʼ��ַCFGCOMMONADD�洢��ǰ����״̬
 * \            2��Flash��ʼ��ַCFGVDADD�洢����������ã�Flash��ʼ��ַCFGQDADD�洢�ӳ��������
 * \update      13.12.31����BUG
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
	Bool decodefail[3] = {FALSE, FALSE, FALSE};//�������ñ��

	if(isReadflash)
	{	
		//��Flash�ж���������
		Roseek_Flash_Read((Uint8 *)Sdata, CFGCOMMONADD, 80);
		Roseek_Flash_Read((Uint8 *)scfgtmpVD, CFGVDADD, 30);
		Roseek_Flash_Read((Uint8 *)scfgtmpQD, CFGQDADD, 30);
		
		ee3mode = (int)(Sdata[0] - '0');
		if(ee3mode == VEHICLE_DETECT || ee3mode == QUEUE_DETECT || ee3mode == TEST_MODE)
		{
			//������������
			g_EE3Cur.RunMode = ee3mode;
			//ͨ������
			decodefail[0] = DecodingCfgData(Sdata, n1, word);
			if(decodefail[0] == TRUE)
			{
				strcpy(g_EE3Cur.CameraIp, word[0]);
				strcpy(g_EE3Cur.ServerIp, word[1]);
				g_EE3Cur.ServerPort = (Uint16)(StringConvertInt(word[2]));
			}
			//�����������
			decodefail[1] = DecodingCfgData(scfgtmpVD, n2, word);
			if(decodefail[1] == TRUE)
			{
				g_EE3Cur.UploadTime = StringConvertInt(word[0]);
				g_EE3Cur.NightTrafficStream = StringConvertInt(word[1]);
			}	
			//�ӳ��������
			decodefail[2] = DecodingCfgData(scfgtmpQD, n2, word);	
			if(decodefail[2] == TRUE)
			{		
				g_EE3Cur.RoadNum[0] = StringConvertInt(word[0]);
				g_EE3Cur.RoadNum[1] = StringConvertInt(word[1]);
			}
			if((decodefail[0] && decodefail[1] && decodefail[2]) == TRUE)
			{
				g_EE3Cur.Isreadflash = 1;//��ȡ���óɹ�
			}
			else
			{
				g_EE3Cur.Isreadflash = 0;//������ʧ��
			}			
		}
		else
		{
			g_EE3Cur.Isreadflash = 0;//������ʧ��	
		}
	}	
}
/**
 * \function 	DecodingCfgData()����Ϊ���ַ����н����ַ����ݲ�ת�档
 * \brief    	���룺����*bufstream�ַ�������ʽΪ\n Word \r��
 * \				  ����wordnumҪ�������ֶ���
 * \			���������word[][30]ת���ַ��������飨��ʽΪ Word \0��		
 * \			ע�⣺
 * \update      13.12.31����BUG
**/
Bool DecodingCfgData(char *bufstream, int wordnum, char word[][30])
{	
	int i;
	int nCountRow = 0;//�Ѷ�ȡ���ֶ���
	int nWord;//�ַ�������
	Bool isWord = FALSE;//���

	//ת����word������
	for(i = 0;;i++)
	{
		if(nCountRow >= wordnum)
		{
			break;
		}
		if(bufstream[i] == '\n')
		{
			nWord = 0;
			isWord = TRUE;
			continue;
		}
		if(bufstream[i] == '\r')
		{
			word[nCountRow][nWord] = '\0';//���β����ʶ
			nCountRow++;
			isWord = FALSE;
			continue;
		}
		//�Ƿ�����ȷ���ַ�
		if(( '0' <= bufstream[i] && bufstream[i] <= '9' ) || bufstream[i] == '.')
		{
			if(isWord)
			{
				word[nCountRow][nWord] = bufstream[i];
				nWord++;
			}
		}
		else
		{
			return FALSE;
		}
		
	}
	return TRUE;
			
}

/**
 * \function 	StringConvertInt()�������ַ���ת��Ϊ��Ӧ��int�ͱ�����
 * \brief    	���룺*p�ַ�������ʽ����1234 \0"��
 * \			�����Intp����ʽ�� 1234��
 * \			ע�⣺1����'\0'��β��2��ע�ⳤ�ȣ�intΪ32λ
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
 * \brief    	ÿ��ϵͳ����ʱ���ã��߿������������á�
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
 * \brief    	�������߿���ͼ���ϱ��,�����ο�
 * \					
 * \			1���Կ���Ϊ���ģ���0��Ӧ�������򣬵�1��Ӧ���ϣ�2��Ӧ������3��Ӧ����
 * \            2��ÿ�������߿����ĵ�ȷ����ȡ�ĵ������ǵ������ο�
 * \            3��ԭʼͼ��ΪYUV422��ʽ
**/
//ȡ4�������������Сֵ����
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
//������
void DrawPoint(Uint8 *in_data, int *PointData){

		int ColMin,ColMax,RowMin,RowMax;
		int ptTmp[2][4][2];
		int i,j;
		//��ȡȫ��PointData�еĵ����꣬PointData��ϵͳ��ʼ��ʱ��Flash�ж�ȡ��
		for(i = 0; i < 2; i++){
			for(j = 0; j < 4; j++)
			{
				ptTmp[i][j][0] = PointData[8 * i + 2 * j];//(x , y)����x
				ptTmp[i][j][1] = PointData[8 * i + 2 * j + 1];//(x , y)����y
			}
		}
		//����һ�߿�
		//ȡ������X�������Сֵ
		Sortminmax(ptTmp[0][0][0],ptTmp[0][1][0],ptTmp[0][2][0],ptTmp[0][3][0],&ColMin,&ColMax);
		//ȡ������y�������Сֵ
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
		//���ڶ��߿�
		//ȡ������X�������Сֵ
		Sortminmax(ptTmp[1][0][0],ptTmp[1][1][0],ptTmp[1][2][0],ptTmp[1][3][0],&ColMin,&ColMax);
		//ȡ������y�������Сֵ
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
 * \brief    	������������ʱ�亯��
 * \					
 * \			1����ȡ�����е�������ʱ�ڵ㣬��ʱ���
 * \            
**/
void Count_RunTime(Uint32 *nRT)
{
	static int 		nCountR = 0;
	static float    nSumRT = 0;
	if(nCountR < NUM_RUMTIME)
	{		
		RunTimeOnce[nCountR] = (abs(nRT[1] - nRT[0]))/1000;	//����
		nSumRT = nSumRT + RunTimeOnce[nCountR];
		nCountR++;		
	}
	if(nCountR == NUM_RUMTIME)
	{
		//��¼���
		g_CmdUdata.nSumRT = nSumRT;
		g_CmdUdata.nFrame = NUM_RUMTIME / nSumRT * 1000;
		//SEM_post( &sem_DataRunTimeN );//�����ϴ�����
		nSumRT = 0;
		nCountR = 0;
	}
}
/*****************************************************************************
******************************������⹦��***********************************
*****************************************************************************/
void ITS_VehicleDetect(DpPoint *dpPVD)
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
void ITS_QueueDetect(DpPoint *dpPQD)
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
		            //���ñ�����ģ�㷨
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


/************************************************************************************/
/**
 * \function 	Test_PointDPlus
 * \brief    	�ֶ����������ο��ĵ���Ĳ���
 * \					
 * \			
 * \            
**/
void Test_PointDPlus()
{
	static int step = 60;
	//�߿�ĵ�����
	PointDataVD[0] = 0;
	PointDataVD[1] = 0;
	PointDataVD[2] = 0;	
	PointDataVD[3] = step;
	PointDataVD[4] = ALG_COL;
	PointDataVD[5] = 0;
	PointDataVD[6] = ALG_COL;	
	PointDataVD[7] = step;

}
