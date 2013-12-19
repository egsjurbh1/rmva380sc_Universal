/**
 * \file      	EE3_pio.c
 * \author    	Leo Bai
 * \version   	1.08
 * \date      	2012.07.13
 * \brief     	�ɱ��IO�Ĵ���ģ��
**/

#include "EE3_common.h"

/***********************�ⲿ����*************************/
extern Uint8 ui8SPIO1Config[];
extern Uint8 ui8SPIO2Config[];
extern RoseekImgInfStruct g_ImgInfStruct;
extern RoseekTrgCtrlInfStruct g_TrgCtrlInfStruct;
/***********************ȫ�ֱ���*************************/
//PIO�ж�״̬��Ϣ�ṹ��
typedef struct SPIOIntInf{
	Uint32	ui32IntOccur;			//�Ƿ����жϣ�1->�����жϣ�0->û�з����ж�
	Uint32	ui32IntEdgePolarity;	//�������жϣ�1->�������жϣ�0->�½����ж�
	Uint32	ui32IntTime;			//�������жϣ��ж�ʱ�䣬��λΪus
}EagleEye2SPIOIntInf;
//PIO�ж���Ϣ�ṹ��
EagleEye2SPIOIntInf IntePIOIntInf[12] = {0};
//IO�жϼ�ʱ��,������Ȧ����ʱ��ʱ�䣬һ���������㳵��
Uint32	ui32IO1IntTime = 0;
Uint32	ui32IO2IntTime = 0;
//����ģʽ����ʱ������ʽ����(����������Ȧ������˫��Ȧ����)�������ж����������Ϣ
volatile Uint8	g_ui8TmpTrgMode = 0;//0->������1->����Ȧ������2->˫��Ȧ����
//����ģʽ����ʱ���ٻ��棬�����ɼ�����ж����������Ϣ
volatile float	g_fTmpSpeed = 0.0;
//����ģʽ����ʱץ��ʱ�仺�棬�����ɼ�����ж����������Ϣ
volatile Uint8	g_ui8TmpTrgTime[8];
//�Բ�ⴥ�����ڼ���
volatile Uint32 g_ui32ADLITrgCnt = 0;
/********************************************************/


/** 	
 * \function 	hwiExpSynIntFunc
 * \brief    	ͬ���ع��ж���Ӧ����(Ӳ���ж�)
 * \			ÿ����������ʼ�ع�ɼ�һ֡ͼ��������Ϣʱ�����ж���Ӧ�����ᱻϵͳͬʱ����		
 * \note		1	���ж���Ӧ���������õ�ǰ�����û���ϵͳ��ʼ��ģ���е���	Roseek_Enable_ExpSynInt();
 * \			2	�����û��ڸ��ж���Ӧ�����е���ϵͳʱ��RTC��ȡ
**/
void hwiExpSynIntFunc()
{
	int iTrgSur;
	//�������������ȡץ��ʱ��
	iTrgSur = Roseek_Get_TrgSurFlag();
	if( iTrgSur>=0 ){//ȷ��Ϊ����
		//��ȡץ��ʱ��
		Roseek_RTC_ReadTime( (Uint8*)g_ui8TmpTrgTime );
		//�Բ�ⴥ�����ڼ�������
		g_ui32ADLITrgCnt = 0;
		//��Ϊ��������ʾ��Ҫ��ʾ
		if( iTrgSur==TRGSUR_SOFT ){
			//�����³��ٱ�ʾΪ0
			g_fTmpSpeed = 0.0;
			//��ʾ����ͼ��Ϊ����
			g_ui8TmpTrgMode = 0;
		}
	}
}

/** 	
 * \function 	hwiReadPIOIntFunc
 * \brief    	I/O�ж���Ӧ����(Ӳ���ж�)
 * \			I/O�����ó����벢ʹ�������жϣ�ÿ���ⲿ�ӿڵĵ�·����0-5V�ĵ�ƽ�仯ʱ�����ж���Ӧ�����ᱻϵͳͬʱ����		
 * \note		1	12���ɱ��IO����Ӧ�¼�����һ���жϣ��û����ڸú����ڽ����ж�
 * \			2	���жϱ����õ�ǰ������ӦIO�����ó����룬��ʹ�������ж� �����û�ʹ���˸��жϵĴ���ģʽץ�������¼�֪ͨ����
**/
void hwiReadPIOIntFunc()
{
	//process interrupt there
	//1. ȷ���Ƿ�Ϊ�л�������ģʽ��Ĵ�������

	//2. ����ͨ��I/O�ж�
	Roseek_Get_PIOIntInf( (Uint32*)IntePIOIntInf );
	//����IO1�ж�
	if( !ui8SPIO1Config[3] ){
		//�½����ж�
		if( IntePIOIntInf[0].ui32IntOccur && !IntePIOIntInf[0].ui32IntEdgePolarity ){
			ui32IO1IntTime = IntePIOIntInf[0].ui32IntTime;
		}
	}
	else{
		//�������ж�
		if( IntePIOIntInf[0].ui32IntOccur && IntePIOIntInf[0].ui32IntEdgePolarity ){
			ui32IO1IntTime = IntePIOIntInf[0].ui32IntTime;
		}
	}
	//����IO2�ж�
	if( !ui8SPIO2Config[3] ){
		//�½����ж�
		if( IntePIOIntInf[1].ui32IntOccur && !IntePIOIntInf[1].ui32IntEdgePolarity ){
			ui32IO2IntTime = IntePIOIntInf[1].ui32IntTime;
			Roseek_RTC_ReadTime( g_ImgInfStruct.ui8TimeInf );
			SEM_post( &sem_CalSpeed );
		}
	}
	else{
		//�������ж�
		if( IntePIOIntInf[1].ui32IntOccur && IntePIOIntInf[1].ui32IntEdgePolarity ){
			ui32IO2IntTime = IntePIOIntInf[1].ui32IntTime;
			Roseek_RTC_ReadTime( g_ImgInfStruct.ui8TimeInf );
			SEM_post( &sem_CalSpeed );
		}
	}
}

/** 	
 * \function 	tskCalSpeedFunc
 * \brief    	��Ȧ����
 * \
**/
void tskCalSpeedFunc()
{
	Uint32 ui32TimeDef;

	while(1){
		SEM_pend( &sem_CalSpeed, SYS_FOREVER );
		//���ⴥ��ǰ�Բ��ץ���Ѿ����������б�ʾ�ӱ���������Ч
		if( g_TrgCtrlInfStruct.bADLIOccur ){
			g_TrgCtrlInfStruct.bExtTrgOccurWhenAutoDetcLig = TRUE;
		}
		else{
			g_TrgCtrlInfStruct.bExtTrgOccurWhenAutoDetcLig = FALSE;
		}
		//���㳵��
		//��ʾ����ͼ��ΪӲ����
		if( g_TrgCtrlInfStruct.ui8IsSingalCoin ){
			//���Ϊ����Ȧ���򲻼��㳵��
			g_ui8TmpTrgMode = 1;
			g_fTmpSpeed = 0.0;
			continue;
		}
		//˫��Ȧ����,���㳵��
		g_ui8TmpTrgMode = 2;
		if( ui32IO2IntTime < ui32IO1IntTime ){
			ui32TimeDef = ( 0xffffffff - ui32IO1IntTime ) + ui32IO2IntTime;
		}
		else{
			ui32TimeDef = ui32IO2IntTime - ui32IO1IntTime;
		}
		if( ui32TimeDef < 1 ){
			g_fTmpSpeed = 0.0;
		}
		else{
			g_fTmpSpeed = (g_TrgCtrlInfStruct.fInteval * 3600000) / ui32TimeDef;
		}
	}
}


