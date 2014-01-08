//////////////////////////////////////////////////////////////////////
// File name: EE3_ITSAlg.h      		                            //    
//////////////////////////////////////////////////////////////////////

/**
 * \file      	EE3_ITSAlg.h
 * \author    	LQ
 * \version   	0.0.1
 * \date      	2014.1.8
 * \brief     	���ܽ�ͨͼ�����㷨��
 *				
 *              �����£�
**/
#ifndef	_EE3_ITSALG_H
#define	_EE3_ITSALG_H

#define HEIGHT			544 //��ӦALG_ROW
#define WIDTH 			960 //ALG_COL
#define SEG_THR 		25
#define PER_THR 		50.0 //������ֵ
#define THRESHOLD1 		30
#define BAC_READY 		10.0 
#define BG_THR			15  //��ģ��ֵ
/************************************************************************************/
#define THRESHOLD1 		30    //�ж���ǰ��������������ֵҲҪ����ʹ�ã����˵�һ����ǰ��
#define THRESHOLD2 		0.08  //������������ռ���ʣ�Ϊ����ȥ�ǳ���
#define THRESHOLD3 		20    //�ж��׵���֮����ʹ��
#define THRESHOLD4 		10    //��������

//��˹������ģ
extern void gaussian(Uint8 *b_data, Uint8 *i_data, float *ww, float *mean, float *sd,int yMin, int yMax, int xMin, int xMax);
//��˹������ģ�㷨������ĳ�ʼ������
extern void setwwmeansd(Float *ww, Float *mean, Float *sd);
extern void GaussianPram(Float *nMat, int k);
//�������
extern int GrayscaleVD(Uint8 *in_data, float *ww, float *mean, float *sd, DpPoint *ptP,
		 Uint8 *ptn, Uint8 *carNum,	float *nMat, Uint8 *back_data,	Uint8 numofframe);
//�ӳ����
extern int GrayscaleQD(Uint8 *back_data,Uint8 *in_data,DpPoint *pt,Uint32 *firstip,Uint32 *lastip);


#endif
