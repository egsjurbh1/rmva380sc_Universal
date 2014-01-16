/**
 * \file      	EE3_Imgfunc.h
 * \author    	LQ
 * \version   	0.1.0
 * \date      	2014.1.16
 * \brief     	imgfunc������
 *              �����£�
**/

#ifndef _EE3_IMGFUNC_H
#define _EE3_IMGFUNC_H

#define CONNECT_CYCLE 	5	//���ӷ��������ڣ���λ����
/***********************************************/
//**ȫ�ֱ���
extern int     PointDataVD[16];
extern int     PointDataQD[16];
extern EE3_CfgStruct g_EE3Cur;//�������״̬
extern volatile     float  RunTimeOnce[NUM_RUMTIME];
extern CmdUData g_CmdUdata;
extern CarInfo g_carInfo;
/**********************************************/
//����
//�����߿�Point������ʼ������
void PointDataConfig(DpPoint *pt, int *pointdata);
//��FLASH�ж�ȡ�����߿�����
void ReadPointDataFromFlash();
//��FLASH�ж�ȡ���������Ϣ
void ReadSetInfFormFlash(Bool isReadflash);
//�����߿���ͼ���ϱ��,�����ο�
void DrawPoint(Uint8 *in_data, int *PointData);
//�ֶ�������
void Test_PointDPlus();
//������������ʱ�亯��
void Count_RunTime(Uint32 *nRT);
//������������ݣ�����IO�����
void VD_SemOut_IO(Uint8 *isC);
//��Ȧ����
void ResetPointData();
//�����Է����ź���
void SendSemCycle();

#endif


