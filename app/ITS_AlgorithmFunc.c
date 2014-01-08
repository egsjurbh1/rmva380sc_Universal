/**
 * \file      	ITS_AlgorithmFunc.c 
 * \author    	L.Q.@Lab217.tongji
 * \version   	0.2.3
 * \date      	2014.1.8
 * \brief     	���ܽ�ͨӦ�á���ͼ�����㷨
 * \update      ��������ͷ�ļ�            
**/
#include "EE3_common.h"
#include "EE3_ITSAlg.h"
/************************************************************************************/
//�ⲿ��ʼ���ṹ��,�ڴ����
extern AlgfuncData 		g_algfuncData;
extern VDalgfuncData 	g_VDalgfuncData;
/************************************************************************************/
/**
 * \function 	gaussian
 * \brief    	��˹������ģ�㷨
 * \			��ģ���������ʵʱͼ�񣬶�ͼ����б�����ģ���������ͼ��		
 * \			
**/

void gaussian(Uint8 *b_data, Uint8 *i_data, Float *ww, Float *mean, Float *sd,int yMin, int yMax, int xMin, int xMax) 
{
	Uint32 C = 3, xp, yp, kp, match, movep, k; //��˹�ֲ�����3-5
	Float D = 2.5; //��ƫ�ֵ
	Float std_init = 0.001;
	Float x = 0.08; //ѧϰ��0.01-0.5,x��0.01����ǰ������Խ�죬��ģ������ڣ���СЧ����
	Float p = x * C, mid_data;
	Uint32 min_a_index = 1; //min_a_index�����趨ֵ
	Float sum_uu, min_a, tmp = 0;

  	for (yp = yMin; yp < yMax; yp++)
  	{
	  for (xp = xMin; xp < xMax; xp++) 
	  {
		  sum_uu = 1.0; //��ʼ��ֵ
		  min_a = *ww;
		  for (kp = 0; kp < C; kp++) {
			  match = 0;
			  movep = yp * WIDTH * 3 + xp * 3 + kp;

			  tmp = fabs((float) i_data[yp * WIDTH + xp] - mean[movep]);
			  sum_uu = tmp * tmp;
			  sd[movep] = D * D * sd[movep];

			  if (sum_uu <= sd[movep]) {
				  match = 1;
				  ww[movep] = (1 - x) * ww[movep] + x;
				  p = x / ww[movep];
				  mean[movep] = (1 - p) * mean[movep]
				  + p * (Float) i_data[yp * WIDTH + xp];
				  mid_data = ((Float) i_data[yp * WIDTH + xp] - mean[movep])
					  * ((Float) i_data[yp * WIDTH + xp] - mean[movep]);
				  sd[movep] = sqrt(
					  (1 - p) * sd[movep] * sd[movep] + p * mid_data);
			  } else {
				  ww[movep] = (1 - x) * ww[movep];
				  mean[movep] = (1 - p) * mean[movep];
			  }

			  if (match == 0) {
				  min_a = ww[movep - kp];
				  min_a_index = 0;

				  for (k = 1; k < C; k++) {
					  if (min_a >= ww[movep - kp + k]) {
						  min_a = ww[movep - kp + k];
						  ww[movep - kp + k] = ww[movep - kp + k - 1];
						  ww[movep - kp + k - 1] = min_a;
					  }
					  min_a_index = k - 1;
				  }
				  mean[movep - kp + min_a_index] = i_data[yp * WIDTH + xp];
				  sd[movep - kp + min_a_index] = std_init;
			  }
		  }
		  *(b_data + yp * WIDTH + xp) = (Uint8) mean[movep];
		  //*(i_data + yp * WIDTH + xp) = (Uint8) mean[movep];
	  }
	}
}

/**
 * \function 	setwwmeansd
 * \brief    	��˹������ģ�㷨������ĳ�ʼ������
 * \					
 * \			1������
 * \            2��
**/

void setwwmeansd(Float *ww, Float *mean, Float *sd) 
{
	int i,j,k;
	for (i = 0; i < 544; i++)  //��ʼ��
    	for (j = 0; j < 960; j++)
       		for (k = 0; k < 3; k++) {
	        	*(ww +  960 * i * 3 + j * 3 + k) = 0.33333;
	        	*(sd + 960 * i * 3 + j * 3 + k) = 6;
	       	 	*(mean + 960 * i * 3 + j * 3 + k) = 40;
 			}
}

/************************************************************************************/
/**
 * \function 	GrayscaleQD
 * \brief    	���м���㷨
 * \			��ģ�����뱳��ͼ���ǰ��ͼ�����ݣ��������ǰ������		
 * \			
**/

int GrayscaleQD(Uint8 *back_data, Uint8 *in_data, DpPoint *pt, Uint32 *firstip, Uint32 *lastip)
{

	Uint8 *y_data;
	Lineinfo *foreground;
	float *flag;
	DpPoint *ptP;
	int i, j, n;
	float occupancy;
	int firsti = 0, lasti = 0;
	Bool istart = TRUE;

	//y_data = (Uint8 *) malloc(1920 * 1088); //ԭΪ704*576
	//flag = (Float *) malloc(sizeof(Float) * 1088);
	//foreground = (Lineinfo *) malloc(1088 * sizeof(Lineinfo));

	y_data = g_algfuncData.ay_data;
	flag = g_algfuncData.aflag;
	foreground = g_algfuncData.aforeground;

	memset((Uint8 *) y_data, 0, ALG_COL * ALG_ROW); 
	memset((Uint8 *) foreground, 0, ALG_ROW * sizeof(Lineinfo));
	memset((Uint8 *) flag, 0, sizeof(Float) * ALG_ROW);
	ptP = pt; 
	*((int *) firstip) = 0;
	*((int *) lastip) = 0; 

	for (j = ptP->yMin; j < ptP->yMax; j++) {
		for (i = ptP->xMin; i < ptP->xMax; i++) {
			//n��ʾ�������ص�ͶӰ�������ϵ�yֵ,���ഹֱ������б�ʳ˻�-1
			int n = (int) ((ptP->k3 * ptP->k3 * j + ptP->k3 * i + ptP->b3)
					/ (ptP->k3 * ptP->k3 + 1)); //n��ʾ�������ص�ͶӰ�������ϵ�yֵ
			if (n >= ALG_ROW || n < 0)
				continue;
			if ((i > (j - ptP->b1) / ptP->k1) && (i < (j - ptP->b2) / ptP->k2)
					&& (j > ptP->k4 * i + ptP->b4) && (j < ptP->k5 * i
					+ ptP->b5)) {
				foreground[n].total_pixel++;
				if (fabs(*(in_data + i + j * ALG_COL) - *(back_data + i + j * ALG_COL))
						> THRESHOLD1)//�˵�ǰ�������
				{
					*(y_data + i + j * ALG_COL) = 255; //��Ϊ�׵㣬��ֵ��111
					foreground[n].foreground_count++; //ǰ��ֵ��1
				}
			}
		}


	}

	for (n = 0; n < ALG_ROW; n++) //������0ֵ��ʼɨ��
	{
		if (foreground[n].foreground_count != 0) //����ǰ��
		{
			occupancy = (float)foreground[n].foreground_count/foreground[n].total_pixel;//һ����ǰ��ռ��һ�������صı�ֵ

			if (occupancy < THRESHOLD2) {
				foreground[n].foreground_count = 0; //��Ϊ�������˵��˵ȿ�ȱ���С������
			} else {
				*(flag + n) = 1; //foreground flag
			}
		}
	}
	for (j = ptP->pt6y - 1; j >= ptP->pt5y; j--) //ͼ�� img->height-1�����߿�ʼ��� ,ʵ���п���Ҫ�����趨
	{

		if (flag[j] == 0) //��ǰ�����������
		{
			continue;
		} else if ((flag[j] != 0) && (istart))//��ǰ�����洢��һ��ǰ�������ڵ�yֵ
		{
			firsti = j;
			istart = FALSE;
		} else if (flag[j] != 0) //��ǰ�����洢ǰ��������yֵ,�Ҵ洢���һ��ǰ��������yֵ
		{
			lasti = j;
		}
	}
	//*resNump = scalep[lasti] - scalep[firsti];
	*((int *) firstip) = firsti;
	*((int *) lastip) = lasti;


	//free(foreground);
	//free(y_data);
	//free(flag);
	return 1;
}


/********************************************************
****************�������ͼ�����㷨*********************
********************************************************/
void GaussianPram(Float *nMat, int k) 
{
	Int s = (k + 1) / 2, m, n;
	Float bMat = 0.0;
	Float sMat = 0.0;
	Float pram;

	for (m = -k; m < k + 1; m++) {
		pram = ((Float) (m * m)) / (2 * s * s);
		bMat = exp(-pram);
		sMat += bMat;
	}
	for (m = 0; m < 2 * k + 1; m++) {
		n = m - k;
		pram = ((Float) (n * n)) / (2 * s * s);
		bMat = exp(-pram);
		nMat[m] = bMat / sMat;
	}
}

/*************************
��ռ��,��ȡǰ��ռ��Ȧ�İٷֱ�
f_data	out	ǰ�����Ӹ�˹ģ���м�ȥ��������ֵ����ǰ����

yMin	in	��Ȧ��ز���
yMax	in
xMin	in
xMax	in
*************************/
static float Percent(Uint8 *f_data, int yMin, int yMax, int xMin, int xMax) 
{
	int i, j;
	int fo = 0;
	int total = (yMax - yMin) * (xMax - xMin);
	float percentage = 0.0;

	for (i = yMin; i < yMax; i++) //bmp��
		for (j = xMin; j < xMax; j++) {
			if (f_data[i * WIDTH + j] == 255) {
				fo++;
			}
		}
	percentage = ((Float) fo) * 100 / total;
	if (percentage < PER_THR) {
		percentage = 0.0;
	}
	return percentage;

}

/*************************
��ʴ�����㷨
data	out	ԭʼͼ��

yMin	in	��Ȧ��ز���
yMax	in
xMin	in
xMax	in
*************************/
static void erosion(Uint8 *data, int yMin, int yMax, int xMin, int xMax, int mMat) 
{
	int i, j, flag, m, n;
	int center = mMat / 2;
	Uint8 *tmpdata;
	//tmpdata = (UInt8*) malloc(WIDTH * HEIGHT);
	tmpdata = g_VDalgfuncData.tmpdata1;
	memcpy((Uint8*) tmpdata, (Uint8*) data, WIDTH * HEIGHT);
	for (i = yMin; i < yMax; i++) {
		for (j = xMin; j < xMax; j++) {
			flag = 1;
			for (m = i - center; m < i + center + 1; m++) {
				for (n = j - center; n < j + center + 1; n++) {
					//��������������һ��Ϊ255
					//�򽫸õ���Ϊ255
					if (tmpdata[i * WIDTH + j] == 0
							|| tmpdata[m * WIDTH + n] == 0) {
						flag = 0;
						break;
					}
				} // for n
				if (flag == 0) {
					break;
				}
			} // for m
			if (flag == 0) {
				data[i * WIDTH + j] = 0;
			} else {
				data[i * WIDTH + j] = 255;
			}
		} // for j
	} // for i
	//free(tmpdata);
}
static void dilation(Uint8 *data, int yMin, int yMax, int xMin, int xMax, int mMat) 
{
	int i, j, flag, m, n;
	int center = mMat / 2;
	Uint8 *tmpdata;
	//tmpdata = (UInt8*) malloc(WIDTH * HEIGHT);
	tmpdata = g_VDalgfuncData.tmpdata2;
	memcpy((Uint8*) tmpdata, (Uint8*) data, WIDTH * HEIGHT);
	for (i = yMin; i < yMax; i++) {
		for (j = xMin; j < xMax; j++) {
			flag = 1;
			for (m = i - center; m < i + center + 1; m++) {
				for (n = j - center; n < j + center + 1; n++) {
					//��������������һ��Ϊ255
					//�򽫸õ���Ϊ255
					if (tmpdata[i * WIDTH + j] == 255
							|| tmpdata[m * WIDTH + n] == 255) {
						flag = 0;
						break;
					}
				} // for n
				if (flag == 0) {
					break;
				}
			} // for m
			if (flag == 0) {
				data[i * WIDTH + j] = 255;
			} else {
				data[i * WIDTH + j] = 0;
			}
		} // for j
	} // for i
	//free(tmpdata);
}

/*************************
ȡ��ǰ������,ǰ����ֵ��
in_data	in	��ǰԭʼͼ��
back_data in	��˹������ģ��ȡ�ı���

f_data	out 	��ֵ��ǰ��

yMin	in	��Ȧ��ز���
yMax	in
xMin	in
xMax	in
*************************/
static void ForeGround(Uint8 *in_data, Uint8 *back_data, Uint8 *f_data, int yMin,
		int yMax, int xMin, int xMax) 
{
	int i, j;

	for (i = yMin; i < yMax; i++)
		for (j = xMin; j < xMax; j++) {
			if (abs(
					in_data[i * WIDTH + j]
							- back_data[i * WIDTH + j]) > THRESHOLD1)
				f_data[i * WIDTH + j] = 255;
			else
				f_data[i * WIDTH + j] = 0;
		}
	dilation(f_data, yMin, yMax, xMin, xMax, 5); //5*5����
	erosion(f_data, yMin, yMax, xMin, xMax, 5);
	//dilation(f_data, 5);
	//erosion(f_data, 5);

}

/*************************
��˹�˲�	�԰ٷֱȶ�������˹�˲�
tempPercent	in	�������һ��ٷֱ�
nMat		in	��˹�˲�ϵ��
k		in	֡��

����ֵ	��˹�˲���İٷֱ�
*************************/
static float GaussianFilter(float *tempPercent, float *nMat, int k) 
{
	float Gaus_percent = 0.0;
	int i;

	for (i = 0; i < 2 * k + 1; i++) {
		Gaus_percent += (float) (nMat[i] * tempPercent[i]);
	}
	return Gaus_percent;
}

/*************************
��˹������ģ��ǰ�κͺ�αȽ�
pro_imgback	in	�������һ��ٷֱ�
b_data		in	��˹�˲�ϵ��

yMin		in	��Ȧ��ز���
yMax		in
xMin		in
xMax		in

diff_point	out	ǰ��������ͬ�ĵ���
*************************/
static void AbsDiff(Uint8 *pro_imgback, Uint8 *b_data, int yMin, int yMax, int xMin,
		int xMax, int *diff_point) 
{
	int yp, xp;
	*diff_point = 0;
	for(yp = yMin; yp < yMax; yp++)
		for(xp = xMin; xp < xMax; xp++) {
			if (abs(
					*(b_data + yp * WIDTH + xp)
							- (*(pro_imgback + yp * WIDTH + xp))) > SEG_THR) {
				(*diff_point)++;
			}
		}
}

/*************************
��ǰ�θ�˹������ģ�Ľ������һ��
src		in	��˹����ԭ��ַ
dest		in	��˹�������ݵ�ַ

yMin		in	��Ȧ��ز���
yMax		in
xMin		in
xMax		in

*************************/
static void memcpy_area(Uint8 *src, Uint8 *dest, int yMin, int yMax, int xMin,
		int xMax) 
{
	int yp, xp;
	for (yp = yMin; yp < yMax; yp++)
		for (xp = xMin; xp < xMax; xp++) {
			*(dest + yp * WIDTH + xp) = *(src + yp * WIDTH + xp);
		}
}

/************************************************
**
 * \function 	GrayscaleVD
 * \brief    	�������������
 * \			��ģ���������ʵʱͼ�񣬶�ͼ����б�����ģ���������ͼ��		
 * \
��ǰ�θ�˹������ģ�Ľ������һ��
in_data		in	ԭͼ

ģ����ز���
ww
mean
sd

pt				in	��Ȧ�����ṹ��

ptn				in	������Ȧ��

carNump			out	������Ȧ���Ƿ��г���1�г�2�޳�

nMatp			in	��˹�˲��ľ�����

back_data		out	�������ĸ�˹����

numofframe       in  ������ģƵ��

*************************/			
int GrayscaleVD(Uint8 *in_data, float *ww, float *mean, float *sd, DpPoint *ptP,
		 Uint8 *ptn, Uint8 *carNum,	float *nMat, Uint8 *back_data,	Uint8 numofframe) 
{
	static Uint8 count = 0;
	static Uint8 iStart = 0;
	static Uint8 framesTemp = 0;
	float tempPercent[5][QUEUE_LENGTH*2+1];
	int i, k,  gask = QUEUE_LENGTH;
	static int st = 0;

	Bool BG_flag[5]={TRUE,TRUE,TRUE,TRUE,TRUE};

	float percentTotal = 0;
	float percent[2];
	static int count2;

	int diff_point = 0, thr_diff_point;
	Uint8 *f_data = NULL, *pro_imgback = NULL;
		

	f_data = g_VDalgfuncData.f_data;		
	pro_imgback = g_VDalgfuncData.pro_imgback;
	
	if(st == 0)
	{
		memset(f_data, 0, WIDTH * HEIGHT * sizeof(Uint8));
		memset((Uint8 *) tempPercent, 0, sizeof(tempPercent));
		st++;
	}
		
	count = count + 1;
	if(count >= 25)
	{
		count2++;
	}
	if (count == 255)
		count = 25;
	if(count >= 20 || iStart)
	{
		for(i = 0; i < *ptn; i++)
		{
			ForeGround(in_data, back_data, f_data, (ptP + i)->yMin,
					(ptP + i)->yMax, (ptP + i)->xMin, (ptP + i)->xMax);//ȡ��ǰ��
			percent[i] = Percent(f_data, (ptP + i)->yMin, (ptP + i)->yMax,
					(ptP + i)->xMin, (ptP + i)->xMax);
			if (percent[i] < PER_THR) 
			{
				percent[i] = 0.0;
			}
			percentTotal += percent[i];
		}
		if (percentTotal <= BAC_READY) 
		{
			iStart = 1;
		}
		if (iStart) {
			
			framesTemp++;
			if (framesTemp == 255)
				framesTemp = gask + 2;

			//�ӵ�11֡��ʼ���˲�
			if (framesTemp <= gask + 1) {
				for (i = 0; i < *ptn; i++) {
					tempPercent[i][framesTemp + gask - 1] = percent[i];
					if (framesTemp == gask + 1) {
						 percent[i] = GaussianFilter(tempPercent[i], nMat, gask);
					}
				}
			} else { //tempPercentԪ��ֵ������һλ
				for (i = 0; i < *ptn; i++) {
					for (k = 0; k < (gask * 2); ++k) {
						tempPercent[i][k] = tempPercent[i][k + 1];
					}
					tempPercent[i][gask * 2] = percent[i];

					percent[i] = GaussianFilter(tempPercent[i], nMat, gask);
				}
			}
			//�г�ʱͣ��ģ
			for(i = 0; i < *ptn; i++)
			{
				if(*ptn > 5)
					break;
				if(percent[i] > BG_THR)
				{
					BG_flag[i] = FALSE;
				}
				else
				{
					BG_flag[i] = TRUE;
				}
			}
			for (i = 0; i < *ptn; i++) {
				if (percent[i] - PER_THR >= 0) //PER_THR����ʵ��������ĵ���
				{
					carNum[i] = 1;
				} 
				else 
				{
					carNum[i] = 0;
				}
			}
		}
	}

	if (count2 == numofframe || !(iStart)) { //(*count % 10) == 0 || *count < 20
		memcpy(pro_imgback, back_data, WIDTH * HEIGHT);
		for (i = 0; i < *ptn; i++) {
			//��˹������ģ
			if(BG_flag[i] == TRUE)
			{
				gaussian(back_data, in_data, (Float *) ww, (Float *) mean,
					(Float *) sd, (ptP + i)->yMin, (ptP + i)->yMax,
					(ptP + i)->xMin, (ptP + i)->xMax);
				//ĳ����
				AbsDiff(pro_imgback, back_data, (ptP + i)->yMin, (ptP + i)->yMax,
					(ptP + i)->xMin, (ptP + i)->xMax, &diff_point);
				thr_diff_point = ((ptP + i)->yMax - (ptP + i)->yMin)
					* ((ptP + i)->xMax - (ptP + i)->xMin) * 16 / 100;

				//��ͬ���ص���ܺʹ�����ֵ����ʹ��֮ǰ�ı���
				if (iStart) {
					if (diff_point > thr_diff_point) {
						memcpy_area(pro_imgback, back_data, (ptP + i)->yMin,
							(ptP + i)->yMax, (ptP + i)->xMin, (ptP + i)->xMax);
					}
				
				}
			}
		}
		count2 = 0;
	}

	return 1;
}

