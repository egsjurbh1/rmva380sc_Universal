/**
 * \file      	ITS_ImgFunc.c 
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.1.0
 * \date      	2014.1.15
 * \brief     	ͼ�����߳��õ���һЩ����
 * \update      
**/
/************************************************************************************/
#include "EE3_common.h"
#include "EE3_Imgfunc.h"
/************************************************************************************/
/**
 * \function 	PointDataConfig����Ϊͼ�����㷨�����߿�Point������ʼ��������
 * \brief    	�����߿�Point���ʼ�����������㷨��ʹ�á�
 * \					
 * \			1���Կ���Ϊ���ģ���0��Ӧ�������򣬵�1��Ӧ���ϣ�2��Ӧ������3��Ӧ����
 * \            2��������
**/
void PointDataConfig(DpPoint *pt, int *pointdata) 
{
	
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
 * \function 	ReadPointDataFromFlash()����Ϊ��FLASH�ж�ȡ�����߿����ã������ù���ͨ�Ż�ȡ����
 * \brief    	��Flash�ж�ȡԭʼ�������ݣ����������ݽ�����
 * \					
 * \			1�������߿���������(��VD��QD�����߿�)
 * \            2�����������������
 * \update		���Ӷ�ȡ�ж�2014.1.15
**/

void ReadPointDataFromFlash()
{
	int 	i,j,k,n;
	char 	background[2][81];
	char    tmpNum[3] = {0,0,0};
	//��FLASH�ж�ȡ�����߿�����
	Roseek_Flash_Read((Uint8 *)background[0],CFGVDPOINTADD,80);//�������VD��
	Roseek_Flash_Read((Uint8 *)background[1],CFGQDPOINTADD,80);//�ӳ����QD��
	//���ݽ���
	j=0;
	k=0;
	for(i=0;i<2;i++)
	{
		if(background[i][79] != 0x55)
			continue;
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
/************************************************************************************/
/**
 * \function 	StringConvertInt()�������ַ���ת��Ϊ��Ӧ��int�ͱ�����
 * \brief    	���룺*p�ַ�������ʽ����1234 \0"��
 * \			�����Intp����ʽ�� 1234��
 * \			ע�⣺1����'\0'��β��2��ע�ⳤ�ȣ�intΪ32λ
 * \
**/
static int StringConvertInt(char *p)
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
 * \function 	DecodingCfgData()����Ϊ���ַ����н����ַ����ݲ�ת�档
 * \brief    	���룺����*bufstream�ַ�������ʽΪ\n Word \r��
 * \				  ����wordnumҪ�������ֶ���
 * \			���������word[][30]ת���ַ��������飨��ʽΪ Word \0��		
 * \			ע�⣺
 * \update      13.12.31����BUG
**/
static Bool DecodingCfgData(char *bufstream, int wordnum, char word[][30])
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
/************************************************************************************/
/**
 * \function 	ReadSetInfFormFlash()����Ϊ��FLASH�ж�ȡ���������Ϣ��
 * \brief    	��Flash�ж�ȡԭʼ�������ݣ���ת������ǰ״̬�ṹ�塣
 * \					
 * \			1��Flash��ʼ��ַCFGCOMMONADD�洢��ǰ����״̬
 * \            2��Flash��ʼ��ַCFGVDADD�洢����������ã�Flash��ʼ��ַCFGQDADD�洢�ӳ��������
 * \update		���Ӷ�ȡ�ж�2014.1.15
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
			if(Sdata[79] == 0x55)//��д�����ж�
			{
				decodefail[0] = DecodingCfgData(Sdata, n1, word);
				if(decodefail[0] == TRUE)
				{
					strcpy(g_EE3Cur.CameraIp, word[0]);
					strcpy(g_EE3Cur.ServerIp, word[1]);
					g_EE3Cur.ServerPort = (Uint16)(StringConvertInt(word[2]));
				}
			}
			if(scfgtmpVD[29] == 0x55)//��д�����ж�
			{
				decodefail[1] = DecodingCfgData(scfgtmpVD, n2, word);
				if(decodefail[1] == TRUE)
				{
					g_EE3Cur.UploadTime = StringConvertInt(word[0]);
					g_EE3Cur.NightTrafficStream = StringConvertInt(word[1]);
				}
			}
			//�����������
			if(scfgtmpQD[29] == 0x55)//��д�����ж�
			{	
			//�ӳ��������
				decodefail[2] = DecodingCfgData(scfgtmpQD, n2, word);	
				if(decodefail[2] == TRUE)
				{		
					g_EE3Cur.RoadNum[0] = StringConvertInt(word[0]);
					g_EE3Cur.RoadNum[1] = StringConvertInt(word[1]);
				}
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
static void Sortminmax(int a,int b,int c,int d,int *min,int *max)
{
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
void DrawPoint(Uint8 *in_data, int *PointData)
{
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

/************************************************************************************/
/**
 * \function 	VD_SemOut_IO����
 * \brief    	���ճ�����������ݣ�����IO�����LED����
 * \					
 * \			1��ʹ��IO3��IO4�����ڣ��ֱ��Ӧ�����߿�
 * \            2��IOʹ������main�г�ʼ��
**/
void VD_SemOut_IO(Uint8 *isC) 
{
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
 * \function 	ResetPointData
 * \brief    	ÿ��ϵͳ����ʱ���ã��߿������������á�
 **/
void ResetPointData()
{
	int i;
	for(i=0; i<16; i++)
		PointDataVD[i] = 0;
	for(i=0; i<16; i++)
		PointDataQD[i] = 0;

}

/************************************************************************************/
/**
 * \function 	SendSemCycle
 * \brief    	�����Է����ź���
 **/
void SendSemCycle()
{
	Uint8   RTC_time[8];
	static  Uint8 temp_min = 0;
	static  Uint8 temp_hour = 0;
	
	Roseek_RTC_ReadTime(RTC_time);//��ǰϵͳʱ��
	if(RTC_time[5]>temp_min)
	{
		if(RTC_time[5]-temp_min > CONNECT_CYCLE)
		{	
			temp_min = RTC_time[5];
			temp_hour = RTC_time[4];
			SEM_post( &sem_SendRoadInfReconnect );//֪ͨ���ӷ������ź���
			
		}							
	}
	else
	{
		if( temp_min < (60 - CONNECT_CYCLE) || temp_hour != RTC_time[4])
		{	
			temp_min = RTC_time[5] ;
			temp_hour = RTC_time[4] ;
			SEM_post( &sem_SendRoadInfReconnect );//֪ͨ���ӷ������ź���
			
		}
	}
}

