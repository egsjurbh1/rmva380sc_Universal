






#ifndef _EE3_IMGFUNC_H
#define _EE3_IMGFUNC_H
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
extern void PointDataConfig(DpPoint *pt, int *pointdata);
//��FLASH�ж�ȡ�����߿�����
extern void ReadPointDataFromFlash();
//��FLASH�ж�ȡ���������Ϣ
extern void ReadSetInfFormFlash(Bool isReadflash);
//�����߿���ͼ���ϱ��,�����ο�
extern void DrawPoint(Uint8 *in_data, int *PointData);
//�ֶ�������
extern void Test_PointDPlus();
//������������ʱ�亯��
extern void Count_RunTime(Uint32 *nRT);
//������������ݣ�����IO�����
extern void VD_SemOut_IO(Uint8 *isC);
//��Ȧ����
extern void ResetPointData();
#endif


