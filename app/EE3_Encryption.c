/**
 * \file      	EE3_Encryption.c
 * \author    	Oliver Wang
 * \version   	1.00
 * \date      	2011.07.13
 * \brief     	����ļ���оƬ�����Լ�����API��
**/

#include "EE3_Encryption.h"
/**************************************************************************/
Bool bTestFlag=FALSE;
static volatile Bool g_bUserEnpOpLock = FALSE;

//���������ݻ�����
AT88SC_Config AT88SC25616C_ConfigZone = {
	0x3b,0xb3,0x11,0x00,0x00,0x00,0x02,0x56,0x56,0x80,0xff,0xff,0xff,0xff,0xff,0xff,//00
	0x34,0x3e,0x72,0xa8,0x50,0x54,0x56,0x80,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//10
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//20
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//30
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//40
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//50
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//60
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//70
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//80
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//90
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//a0
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//b0
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//c0
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,//d0
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x17,0xc3,0x3a,0xff,0xff,0xff,0xff,//e0
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff //f0
	};
/**************************************************************************/
//////////////////////////////////////////////////////////////////////
// function:	Encryption_Set_SDA
// 
// Descript:	����SDAΪ�ߵ�ƽ
// Note:		D1=>SDA: 0=HZ, 1=Low
//////////////////////////////////////////////////////////////////////
static void Encryption_Set_SDA()
{
	g_ui32FpgaI2cWrReg &= 0xfffffffd;	//SDA=1
	*(volatile Uint32*)FPGA_PUBLIC_I2C_WR_REG = g_ui32FpgaI2cWrReg;
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_Clear_SDA
// 
// Descript:	����SDAΪ�͵�ƽ
// Note:		D1=>SDA: 0=HZ, 1=Low
//////////////////////////////////////////////////////////////////////
static void Encryption_Clear_SDA()
{
	g_ui32FpgaI2cWrReg |= 0x00000002;	//SDA=0
	*(volatile Uint32*)FPGA_PUBLIC_I2C_WR_REG = g_ui32FpgaI2cWrReg;
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_Clear_SDA
// 
// Descript:	��ȡSDA�߼�״̬
// Note:		D0<=SDA
//////////////////////////////////////////////////////////////////////
static Uint32 Encryption_Read_SDA()
{
	Uint32 temp;

	temp = (*(volatile Uint32*)FPGA_PUBLIC_I2C_RD_REG)&0x00000001;

	return temp;
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_Set_SCL
// 
// Descript:	����SCLΪ�ߵ�ƽ
// Note:		D0=>SCL: 0=HZ, 1=Low
//////////////////////////////////////////////////////////////////////
static void Encryption_Set_SCL()
{
	g_ui32FpgaI2cWrReg &= 0xfffffffe;	//SCL=1
	*(volatile Uint32*)FPGA_PUBLIC_I2C_WR_REG = g_ui32FpgaI2cWrReg;
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_Clear_SCL
// 
// Descript:	����SCLΪ�͵�ƽ
// Note:		D0=>SCL: 0=HZ, 1=Low
//////////////////////////////////////////////////////////////////////
static void Encryption_Clear_SCL()
{
	g_ui32FpgaI2cWrReg |= 0x00000001;	//SCL=0
	*(volatile Uint32*)FPGA_PUBLIC_I2C_WR_REG = g_ui32FpgaI2cWrReg;
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_SMSTART
// 
// Descript:	����I2C
// Note:		��һ���I2CоƬ��ͬ������AT88SCXXC֮ǰ�������ȷ���8��CLK
//////////////////////////////////////////////////////////////////////
static void Encryption_SMSTART()
{
	Uint32 i;

	Encryption_Set_SDA();
	Roseek_Waitusec(1);

	for( i=0; i<8; i++)
	{
		//�ȴ��8��CLK
		Encryption_Set_SCL();
		Roseek_Waitusec(1);
		Encryption_Clear_SCL();
		Roseek_Waitusec(1);
	}

	Encryption_Set_SDA();
	Roseek_Waitusec(3);
	Encryption_Set_SCL();
	Roseek_Waitusec(3);
	Encryption_Clear_SDA();
	Roseek_Waitusec(3);
	Encryption_Clear_SCL();
	Roseek_Waitusec(3);

	Roseek_Waitusec(1000);
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_SMSTOP
// 
// Descript:	ֹͣI2C
// Note:		
//////////////////////////////////////////////////////////////////////
static void Encryption_SMSTOP()
{
	Encryption_Clear_SCL();
	Roseek_Waitusec(3);
	Encryption_Clear_SDA();
	Roseek_Waitusec(3);
	Encryption_Set_SCL();
	Roseek_Waitusec(3);
	Encryption_Set_SDA();
	Roseek_Waitusec(3);
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_AckPolling
// 
// Descript:	��������͵������Ƿ��ѱ�ִ��
// Note:		ֻ�е�AckPolling_returnΪ1ʱ���ó�����˳�
//////////////////////////////////////////////////////////////////////
static void Encryption_AckPolling()
{
	Uint8 AckPolling_return;
	Uint8 temp;
	Uint8 bitcounter;

	Encryption_SMSTART();
	temp=0xb6;
	do	//cmd send
	{
		for(bitcounter=0;bitcounter<8;bitcounter++)
		{
			if ((temp&0x80)==0x80)
				Encryption_Set_SDA();
			else
				{Encryption_Clear_SDA();}

				Roseek_Waitusec(3);
				Encryption_Set_SCL();
				Roseek_Waitusec(2);
				Encryption_Clear_SCL();
				Roseek_Waitusec(2);
				temp=temp<<1;
		}
		Encryption_Set_SDA();
		Roseek_Waitusec(2);
		Encryption_Set_SCL();
		Roseek_Waitusec(2);
		if(Encryption_Read_SDA()==0) {Encryption_Clear_SCL();AckPolling_return=1;}
		else {Encryption_Clear_SCL();Encryption_SMSTART();temp=0xb6;AckPolling_return=0;}
	}while(AckPolling_return==0);

	Encryption_SMSTOP();
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_Read
// 
// Descript:	��at88scxx����������
// Note:		����������Encryption_Read(rwdata)����ʽ����
//				���:	rwdata����
//				����:	rwdata����
//				rd���鳤�ȿɸ�
//				��һ���I2CоƬ��ͬ������AT88SCXXC֮ǰ�������ȷ���8��CLK
//				cmd_send_counter�����������,����8��,�������˳�,����ΪI2C��·��������
//////////////////////////////////////////////////////////////////////
static void Encryption_Read(Uint8 rd[0x20])
{
	Uint8 bitcounter;
	Uint8 temp;
	Uint8 cmd_send_counter;
	Uint8 bytecounter;

	temp=rd[0];
	bytecounter=0;
	cmd_send_counter=0;
	Encryption_SMSTART();

	do	//cmd send
	{
		for(bitcounter=0;bitcounter<8;bitcounter++)
		{
			if ((temp&0x80)==0x80)	Encryption_Set_SDA();
			else {Encryption_Clear_SDA();}

			Roseek_Waitusec(2);
			Encryption_Set_SCL();
			Roseek_Waitusec(2);
			Encryption_Clear_SCL();

			temp=temp<<1;
		}

		Encryption_Set_SDA();
		Roseek_Waitusec(3);
		Encryption_Set_SCL();
		Roseek_Waitusec(3);

		if(Encryption_Read_SDA()==0)
		{Encryption_Clear_SCL();bytecounter++;temp=rd[bytecounter];}
		else
		{Encryption_Clear_SCL();Encryption_SMSTART();temp=rd[0];bytecounter=0;cmd_send_counter++;}

		if(cmd_send_counter>8){bytecounter=4;}

	}while(bytecounter<4);

	Encryption_Set_SDA();
	for(bytecounter=0;bytecounter<rd[3];bytecounter++)
	{
		for(bitcounter=0;bitcounter<8;bitcounter++)
		{
			Encryption_Set_SCL();
			Roseek_Waitusec(2);

			if (Encryption_Read_SDA()==1) {temp=temp|0x01;}
			else{temp=(temp&0xfe);}
			if(bitcounter<7){temp=temp<<1;}
			else{Roseek_Waitusec(1);}

			Encryption_Clear_SCL();
			Roseek_Waitusec(2);
		}

		Encryption_Clear_SDA();
		Roseek_Waitusec(2);
		Encryption_Set_SCL();
		Roseek_Waitusec(2);
		Encryption_Clear_SCL();
		Roseek_Waitusec(2);
		Encryption_Set_SDA();
		Roseek_Waitusec(1);

		rd[bytecounter+4]=temp;
	}

	Encryption_SMSTOP();
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_Write
// 
// Descript:	��at88scxxд��������
// Note:		����������Encryption_Write(rwdata)����ʽ����
//				���:	rwdata����
//				����:	rwdata����
//				rd���鳤�ȿɸ�
//				��һ��I2CоƬ��ͬ������AT88SCXXC֮ǰ�������ȷ���8��CLK
//				cmd_send_counter�����������,����8��,�������˳�,����ΪI2C��·��������
//////////////////////////////////////////////////////////////////////
static void Encryption_Write(Uint8 SDATA[0x14])
{
	Uint8 bitcounter;
	Uint8 temp;
	Uint8 bytecounter;
	Uint8 cmd_send_counter;

	temp=SDATA[0];
	bytecounter=0;
	cmd_send_counter=0;
	Encryption_SMSTART();

	do	//cmd send
	{
		for(bitcounter=0;bitcounter<8;bitcounter++)
		{
			if ((temp&0x80)==0x80)
			Encryption_Set_SDA();
			else
			{Encryption_Clear_SDA();}

			Roseek_Waitusec(3);
			Encryption_Set_SCL();
			Roseek_Waitusec(2);
			Encryption_Clear_SCL();
			Roseek_Waitusec(2);

			temp=temp<<1;
		}

		Encryption_Set_SDA();
		Roseek_Waitusec(2);
		Encryption_Set_SCL();
		Roseek_Waitusec(2);

		if(Encryption_Read_SDA()==0) {Encryption_Clear_SCL();bytecounter++;temp=SDATA[bytecounter];}
		else {Encryption_Clear_SCL();Encryption_SMSTART();temp=SDATA[0];bytecounter=0;cmd_send_counter++;}

		if(cmd_send_counter>8) {bytecounter=SDATA[3]+4;}

	}while(bytecounter<SDATA[3]+4);

	Encryption_SMSTOP();
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_GPA_CLOCK
// 
// Descript:	GPA����
// Note:		���:	data_in���ϴμ���󱣴���GPA[]�е�����
//				����:	GPA[]�е�����
//				����:	times:GPA����ѭ������Ĵ���
//				�˺�����DES�����㷨��һ�����֣����㷨�ǿ���ģ��ʼȿ�����
//				���㷨ʵ�ּ��ܹ��̣�Ҳ����ʵ�ֽ��ܹ��̡�
//////////////////////////////////////////////////////////////////////
static void Encryption_GPA_CLOCK(Uint8 Datain,Uint8 times)
{
	Uint8 t;
	Uint8 d;
	Uint8 din_gpa;	//GPA[2]
	Uint8 Ri;		//GPA[3]
	Uint8 R_Sum;	//GPA[13]
	Uint8 Si;		//GPA[4]
	Uint8 S_Sum;	//GPA[21]
	Uint8 Ti;		//GPA[5]
	Uint8 T_Sum;	//GPA[27]

	for(t=0x00;t<times;t++)
	{
		asm("NOP");
		din_gpa=Datain ^ GPA[0];
		Ri= din_gpa&0x1f;
		Si= ((din_gpa&0x0f)<<3)+((din_gpa&0xe0)>>5);
		Ti=(din_gpa&0xf8)>>3;

		//rԪ��
		if(((GPA[4])+((GPA[1]&0x0f)<<1)+((GPA[1]&0x10)>>4))>31) {R_Sum=((GPA[4])+((GPA[1]&0x0f)<<1)+((GPA[1]&0x10)>>4))-31;}
		else {R_Sum=((GPA[4])+((GPA[1]&0x0f)<<1)+((GPA[1]&0x10)>>4));}
		GPA[1]=GPA[2];
		GPA[2]=GPA[3];
		GPA[3]=GPA[4];
		GPA[4]=GPA[5]^Ri;
		GPA[5]=GPA[6];
		GPA[6]=GPA[7];
		GPA[7]=R_Sum;

		//sԪ��
		if ((GPA[9]+((GPA[8]&0x3f)<<1)+((GPA[8]&0x40)>>6) )>127) {S_Sum=( (GPA[9]) + ((GPA[8]&0x3f)<<1)+((GPA[8]&0x40)>>6) )-127;}
		else {S_Sum= (GPA[9]) + ((GPA[8]&0x3f)<<1)+((GPA[8]&0x40)>>6) ;}
		GPA[8]=GPA[9];
		GPA[9]=Si^GPA[10];
		GPA[10]=GPA[11];
		GPA[11]=GPA[12];
		GPA[12]=GPA[13];
		GPA[13]=GPA[14];
		GPA[14]=S_Sum;

		//tԪ��
		if ((GPA[15]+GPA[17])> 31) {T_Sum=GPA[15]+GPA[17]-31;}
		else {T_Sum=GPA[15]+GPA[17];}
		GPA[15]=GPA[16];
		GPA[16]=GPA[17];
		GPA[17]=GPA[18]^Ti;
		GPA[18]=GPA[19];
		GPA[19]=T_Sum;

		if((GPA[14]&0x01)==0) {d=((GPA[7]^GPA[3])&0x01);}
		else {d=((GPA[19]^GPA[16])&0x01);}

		if((GPA[14]&0x02)==0) {d=d+((GPA[7]^GPA[3])&0x02);}
		else {d=d+((GPA[19]^GPA[16])&0x02);}

		if((GPA[14]&0x04)==0) {d=d+((GPA[7]^GPA[3])&0x04);}
		else {d=d+((GPA[19]^GPA[16])&0x04);}

		if((GPA[14]&0x08)==0) {d=d+((GPA[7]^GPA[3])&0x08);}
		else {d=d+((GPA[19]^GPA[16])&0x08);}

		GPA[0]= ( (((GPA[0])&0x0f)<<4) +d);	//GPA����
	}
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_AUTHENTICATION
// 
// Descript:	˫�������֤����
// Note:		���̣�
//				1.����������Ci,���������Q0,������Կ;�����Q1,��Q0��Q1���͸�����,
//				  ������µ�Ci,SK
//				2.�ü������SK���Ci���ظ�1���̣�ʵ�ּ�����֤
//				AAC����֤���������
//				AAC!=0xff��ʾ˫�������֤δͨ����������
//////////////////////////////////////////////////////////////////////
static Uint8 Encryption_AUTHENTICATION(Uint8 GC_select)
{
	Uint8 SK[8];			//�Ự��Կ
	Uint8 Q_CH[0x14];		//����14H�ֽ�����
	Uint8 j;

	//GPA��Ԫ����
	for(j=0;j<=19;j++)
	{GPA[j]=0x00;}

	//����������Ci
	Q_CH[0]=0xb6;
	Q_CH[1]=0x00;
	Q_CH[2]=GC_select<<4;
	Q_CH[2]=Q_CH[2]+0x50;
	Q_CH[3]=0x08;
	Encryption_Read(Q_CH);

	//����Ci��Q_CH[12~19]��
	//��ֵ�����Q0=0xaaaaaaaaaaaaaaaa��Q_CH[4~12]��
	for(j=0;j<8;j++)
	{
		Q_CH[j+12]=Q_CH[j+4];
		Q_CH[j+4]=0xAA;
	}

	//����Q1����һ�μ���
	for(j=0;j<4;j++)
	{
		Encryption_GPA_CLOCK(Q_CH[12+2*j],0x03);
		Encryption_GPA_CLOCK(Q_CH[12+2*j+1],0x03);
		Encryption_GPA_CLOCK(Q_CH[4+j],0x01);
	}

	//ȡ����Gc��Q_CH[12~19]��
	if ((GC_select&0x0f)==0x00)
		for(j=0;j<8;j++)
		{Q_CH[12+j]=GC0[j];}
	else if ((GC_select&0x0f)==0x01)
		for(j=0;j<8;j++)
		{Q_CH[12+j]=GC1[j];}
	else if ((GC_select&0x0f)==0x02)
		for(j=0;j<8;j++)
		{Q_CH[12+j]=GC2[j];}
	else
		for(j=0;j<8;j++)
		{Q_CH[12+j]=GC3[j];}

	//����Q1���ڶ��μ��㣬���Q1��Q_CH[12~19]��
	for(j=0;j<4;j++)
	{
		Encryption_GPA_CLOCK(Q_CH[12+2*j],0x03);
		Encryption_GPA_CLOCK(Q_CH[12+2*j+1],0x03);
		Encryption_GPA_CLOCK(Q_CH[8+j],0x01);
	}
	Encryption_GPA_CLOCK(0x00,0x06);
	Q_CH[12]=GPA[0];
	for(j=1;j<8;j++)
	{
		Encryption_GPA_CLOCK(0x00,0x07);
		Q_CH[12+j]=GPA[0];
	}

	//��Q0��Q1���͸�������Q0=Q_CH[4~11], Q1=Q_CH[12~19]
	Q_CH[0]=0xb8;
	Q_CH[2]=0x00;
	Q_CH[3]=0x10;		//Q0,Q1��8�ֽ�
	Q_CH[1]=GC_select;	//����Gc����ţ���Ҫ����֤
	Encryption_Write(Q_CH);
	Encryption_AckPolling();

	//ģ��оƬ�ڵļ�����̣��������оƬ����ͬ�ĻỰ��ԿSK���Ա���ͬ��
	Q_CH[12]=0xff;		//���ֽ���AAC����֤�������������������0xff
	for(j=1;j<8;j++)	//���7�ֽ���Ci
	{
		Encryption_GPA_CLOCK(0x00,0x02);
		Q_CH[12+j]=GPA[0];
	}
	for(j=0;j<8;j++)
	{
		Encryption_GPA_CLOCK(0x00,0x02);
		SK[j]=GPA[0];
	}

	Encryption_GPA_CLOCK(0x00,0x03);

	////////�ԻỰ��Կ����֤һ�Σ�ʵ�ּ�����֤////////
	////////�������ü������SK���Ci,�ظ�������֤����
	////////��ǰ��Q_CH[4~11]����Q0
	////////      Q_CH[12~19]�����µ�Ci
	////////      SK[0~8]�����µ�SK

	//GPA��Ԫ����
	for(j=0;j<=19;j++)
	{GPA[j]=0x00;}

	//����Q1����һ�μ���
	for(j=0;j<4;j++)
	{
		Encryption_GPA_CLOCK(Q_CH[12+2*j],0x03);
		Encryption_GPA_CLOCK(Q_CH[12+2*j+1],0x03);
		Encryption_GPA_CLOCK(Q_CH[4+j],0x01);
	}

	//����Q1���ڶ��μ��㣬���Q1��Q_CH[12~19]��
	for(j=0;j<4;j++)
	{
		Encryption_GPA_CLOCK(SK[2*j],0x03);
		Encryption_GPA_CLOCK(SK[2*j+1],0x03);
		Encryption_GPA_CLOCK(Q_CH[8+j],0x01);
	}
	Encryption_GPA_CLOCK(0x00,0x06);
	Q_CH[12]=GPA[0];
	for(j=1;j<8;j++)
	{
		Encryption_GPA_CLOCK(0x00,0x07);
		Q_CH[12+j]=GPA[0];
	}

	//�ٴν�Q0��Q1���͸�������Q0=Q_CH[4~11], Q1=Q_CH[12~19]
	Q_CH[0]=0xb8;
	Q_CH[1]=GC_select+0x10;	//����Gc����ţ���Ҫ�������֤
	Q_CH[2]=0x00;
	Q_CH[3]=0x10;			//Q0,Q1��8�ֽ�
	Encryption_Write(Q_CH);
	Encryption_AckPolling();

	//ģ��оƬ�ڵļ�����̣��������оƬ����ͬ�ĻỰ��ԿSK���Ա���ͬ��
	Q_CH[12]=0xFF;		//���ֽ���AAC����֤�������������������0xff
	for(j=1;j<8;j++)	//���7�ֽ���CIx
	{
		Encryption_GPA_CLOCK(0x00,0x02);
		Q_CH[12+j]=GPA[0];
	}
	for(j=0;j<8;j++)
	{
		Encryption_GPA_CLOCK(0x00,0x02);
		SK[j]=GPA[0];
	}

	Encryption_GPA_CLOCK(0x00,0x03);

	//��ȡ����������Ӧ���AAC,�鿴��֤�Ƿ���ȷ
	//оƬ�ڲ���֤�����AAC�Ͳ���0xff
	Q_CH[0]=0xb6;
	Q_CH[1]=0x00;
	Q_CH[2]=GC_select<<4;	//ͨ��Gc����ţ������ACC��ַ
	Q_CH[2]=Q_CH[2]+0x50;
	Q_CH[3]=0x01;
	Encryption_Read(Q_CH);
	Encryption_AckPolling();

	//GPA�����ݽ�һ�����������������ڴ�������ʱ�ļӽ��ܹ���
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(Q_CH[2],0x01);
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(0x01,0x01);
	Encryption_GPA_CLOCK(0xff,0x01);
	Encryption_GPA_CLOCK(0x00,0x05);

	return Q_CH[4];
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_verify_write_password
// 
// Descript:	У��д����������������ȷУ����д����󿪷Ŷ�д
// Note:		pw_select����������ѡ��
//				PAC:����У����������.PAC!=0xff��ʾ��֤δͨ����������
//////////////////////////////////////////////////////////////////////
static Uint8 Encryption_verify_write_password(Uint8 pw_select)
{
	Uint8 j;
	Uint8 pw[7];

	//����ָ����PW
	if ((pw_select&0x0f)==0x00)
		{for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE0[j];}}
	else if ((pw_select&0x0f)==0x01)
		for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE1[j];}
	else if ((pw_select&0x0f)==0x02)
		for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE2[j];}
	else if ((pw_select&0x0f)==0x03)
		for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE3[j];}
	else if ((pw_select&0x0f)==0x04)
		for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE4[j];}
	else if ((pw_select&0x0f)==0x05)
		for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE5[j];}
	else if ((pw_select&0x0f)==0x06)
		for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE6[j];}
	else
		for(j=0;j<3;j++)
		{pw[4+j]=PW_WRITE7[j];}

	//�������ĵ�PW
	for(j=0;j<3;j++)
	{
		Encryption_GPA_CLOCK(pw[4+j],0x05);
		pw[4+j]=GPA[0];
	}

	//��������PW��оƬ����Ҫ��˶�
	pw[0]=0xba;
	pw[1]=pw_select&0x0f;	//����PW�����
	pw[2]=0x00;
	pw[3]=0x03;
	Encryption_Write(pw);
	Encryption_AckPolling();

	Roseek_Waitusec(5000);

	//��ȡ�������е�PAC,�鿴��֤�Ƿ���ȷ
	pw[0]=0xb6;
	pw[1]=0x00;
	pw[2]=pw_select&0x0f;
	pw[2]=pw[2]*0x08;
	pw[2]=pw[2]+0xb0;
	pw[3]=0x01;
	Encryption_Read(pw);
	Encryption_AckPolling();

	//���ܶ�����PAC����
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(pw[2],0x01);
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(0x01,0x01);
	pw[4]=pw[4]^GPA[0];
	Encryption_GPA_CLOCK(pw[4],0x01);
	Encryption_GPA_CLOCK(0x00,0x05);

	return pw[4];
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_set_user_zone
// 
// Descript:	ѡ���û���
// Note:		zone���û������
//////////////////////////////////////////////////////////////////////
static void Encryption_set_user_zone(Uint8 zone)
{
	Uint8 zon[4];

	Encryption_GPA_CLOCK(zone,0x01);	zon[0]=0xb4;
	zon[1]=0x03;
	zon[2]=zone;
	zon[3]=0x00;
	Encryption_Write(zon);
	Encryption_AckPolling();
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_encrypto_data
// 
// Descript:	���������ݽ��м���
// Note:		encryptodatanumber�������ݵĸ���
//				����ܵ��������ݴ���� rwdata[4]��rwdata[0x13]
//////////////////////////////////////////////////////////////////////
static void Encryption_encrypto_data(Uint8 encryptodatanumber)
{
	Uint8 j;
	Uint8 temp;

	for(j=0;j<encryptodatanumber;j++)
	{
		Encryption_GPA_CLOCK(0x00,0x05);
		temp=rwdata[4+j];
		rwdata[4+j]=rwdata[4+j]^GPA[0];
		Encryption_GPA_CLOCK(temp,0x01);
	 }
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_send_checksum
// 
// Descript:	����У��͵�����(��������ʽд�����ݺ�,�跢��У���)
// Note:		
//////////////////////////////////////////////////////////////////////
static void Encryption_send_checksum()
{
	Uint8 dat6[6];
	dat6[0]=0xb4;
	dat6[1]=0x02;
	dat6[2]=0x00;
	dat6[3]=0x02;

	Encryption_GPA_CLOCK(0x00,0x0f);
	dat6[4]=GPA[0];
	Encryption_GPA_CLOCK(0x00,0x05);
	dat6[5]=GPA[0];

	Encryption_Write(dat6);
	Encryption_AckPolling();
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_read_user_zone
// 
// Descript:	�����û����������ݣ��������������
// Note:		rd_high_addr �û������ֽڵ�ַ
//				rd_low_addr  �û������ֽڵ�ַ
//				rd_number    ��ȡ�������ݼ�����������ݵ��ֽ�����0<rd_number<=128
//				���ܳ����������ݴ����rwdata[0x04]��ʼ�Ĵ洢��
//////////////////////////////////////////////////////////////////////
static void Encryption_read_user_zone(Uint8 rd_high_addr,Uint8 rd_low_addr,Uint16 rd_number)
{
	Uint16 j;

	rwdata[0]=0xb2;
	rwdata[1]=rd_high_addr;
	rwdata[2]=rd_low_addr;
	rwdata[3]=rd_number;
	Encryption_Read(rwdata);
	Encryption_AckPolling();

	//��ʼ������
	Encryption_GPA_CLOCK(0x00,0x05);		//��ʹ��AT88SC6416/12816/25616ʱ���;
	Encryption_GPA_CLOCK(rd_high_addr,0x01);//��ʹ��AT88SC6416/12816/25616ʱ���;
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(rd_low_addr,0x01);
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(rd_number,0x01);

	for(j=0;j<rd_number;j++)
	{
		rwdata[4+j]=rwdata[4+j]^GPA[0];
		Encryption_GPA_CLOCK(rwdata[4+j],0x01);
		Encryption_GPA_CLOCK(0x00,0x05);
	}
}

//////////////////////////////////////////////////////////////////////
// function:	Encryption_write_user_zone
// 
// Descript:	�������Ĳ�д������
// Note:		wr_high_addr	�û������ֽڵ�ַ
//				wr_low_addr		�û������ֽڵ�ַ
//				wr_number		д�����ݵĸ�����0<wr_number<=128
//				������� rwdata[4]��ʼ����������д������
//				ÿ��д�벻�ܳ���128�ֽ�
//////////////////////////////////////////////////////////////////////
static void Encryption_write_user_zone(Uint8 wr_high_addr,Uint8 wr_low_addr,Uint8 wr_number)
{
	//��ʼ������
	Encryption_GPA_CLOCK(0x00,0x05);		//��ʹ��AT88SC6416/12816/25616ʱ���;
	Encryption_GPA_CLOCK(wr_high_addr,0x01);//��ʹ��AT88SC6416/12816/25616ʱ���;
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(wr_low_addr,0x01);
	Encryption_GPA_CLOCK(0x00,0x05);
	Encryption_GPA_CLOCK(wr_number,0x01);

	rwdata[0]=0xb0;
	rwdata[1]=wr_high_addr;
	rwdata[2]=wr_low_addr;
	rwdata[3]=wr_number;
	Encryption_encrypto_data(wr_number);

	Encryption_Write(rwdata);
	Roseek_Waitusec(5000);					//AT88SCXXд��ʱ�����5mS

	Encryption_send_checksum();
	Encryption_AckPolling();
}

//////////////////////////////////////////////////////////////////////
// function:	Roseek_Encryption_Read_Configuration
// 
// Descript:	Read configuration zone
// Note:		
//////////////////////////////////////////////////////////////////////
Bool Roseek_Encryption_Read_Configuration( Uint8 ui8SecureCode1of3, 
											  Uint8 ui8SecureCode2of3, 
											  Uint8 ui8SecureCode3of3
											)
{
	Uint8 i;

	if( g_bUserEnpOpLock ){
		return FALSE;
	}
	g_bUserEnpOpLock = TRUE;

	//������д������֤��У��д���룬ʹ��оƬ�˳�����ͨѶ��ʽ
	//�Ա��������ķ�ʽ��д������
	if( Encryption_AAC==0xff)
		Encryption_AAC=Encryption_AUTHENTICATION(0);

	if( Encryption_PAC==0xff)
		Encryption_PAC=Encryption_verify_write_password(0);

	//////UNLOCK CONFIGURATION ZONE//////
	rwdata[0]=0xba;
	rwdata[1]=0x07;
	rwdata[2]=0x00;
	rwdata[3]=0x03;
	rwdata[4]=ui8SecureCode1of3;
	rwdata[5]=ui8SecureCode2of3;
	rwdata[6]=ui8SecureCode3of3;
	Encryption_Write(rwdata);

	//read entire configuration zone
	rwdata[0]=0xb6;
	rwdata[1]=0x00;
	rwdata[2]=0x00;
	rwdata[3]=0xf0;	//N=240
	Encryption_Read(rwdata);

	//copy data
	for( i=0; i<240; i++)
		*(((Uint8*)&AT88SC25616C_ConfigZone)+i) = rwdata[i+4];

	g_bUserEnpOpLock = FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// function:	Roseek_Encryption_Initialization
// 
// Descript:	��ʼ��оƬ
// Note:		�����µ�AT88SCXXоƬ����Ҫ��ʼ��֮�����Ӧ��
//////////////////////////////////////////////////////////////////////
Bool Roseek_Encryption_Initialization( Uint8 ui8SecureCode1of3, 
										  Uint8 ui8SecureCode2of3, 
										  Uint8 ui8SecureCode3of3
										)
{
	Uint8 i,j,k;
	Uint8 * pnt = (Uint8*)&AT88SC25616C_ConfigZone;
	Uint8 tmp[256];

	if( g_bUserEnpOpLock ){
		return FALSE;
	}
	g_bUserEnpOpLock = TRUE;

	rwdata[0]=0xb6;
	rwdata[1]=0x01;
	rwdata[2]=0x00;
	rwdata[3]=0x01;
	Encryption_Read(rwdata);

	//������ֻ����������˿δ�۶ϵ�оƬ��fuse=0x07��
	if( (rwdata[4]&0x0f) != 0x07){
		g_bUserEnpOpLock = FALSE;
		return FALSE;
	}

	//������д������֤��У��д���룬ʹ��оƬ�˳�����ͨѶ��ʽ
	//�Ա��������ķ�ʽ��д������
	if( Encryption_AAC==0xff)
		Encryption_AAC=Encryption_AUTHENTICATION(0);
	if( Encryption_PAC==0xff)
		Encryption_PAC=Encryption_verify_write_password(0);

	//�ָ����д��������
	*(pnt+0x50)=0xff;		//ACC0
	*(pnt+0x60)=0xff;		//ACC1
	*(pnt+0x70)=0xff;		//ACC2
	*(pnt+0x80)=0xff;		//ACC3
	*(pnt+0xb0)=0xff;		//PAC_Write0
	*(pnt+0xb8)=0xff;		//PAC_Write1
	*(pnt+0xc0)=0xff;		//PAC_Write2
	*(pnt+0xc8)=0xff;		//PAC_Write3
	*(pnt+0xd0)=0xff;		//PAC_Write4
	*(pnt+0xd8)=0xff;		//PAC_Write5
	*(pnt+0xe0)=0xff;		//PAC_Write6
	*(pnt+0xe8)=0xff;		//PAC_Write7
	*(pnt+0xb4)=0xff;		//PAC_Read0
	*(pnt+0xbc)=0xff;		//PAC_Read1
	*(pnt+0xc4)=0xff;		//PAC_Read2
	*(pnt+0xcc)=0xff;		//PAC_Read3
	*(pnt+0xd4)=0xff;		//PAC_Read4
	*(pnt+0xdc)=0xff;		//PAC_Read5
	*(pnt+0xe4)=0xff;		//PAC_Read6
	*(pnt+0xec)=0xff;		//PAC_Read7

	//////UNLOCK CONFIGURATION ZONE//////
	rwdata[0]=0xba;
	rwdata[1]=0x07;
	rwdata[2]=0x00;
	rwdata[3]=0x03;
	rwdata[4]=ui8SecureCode1of3;
	rwdata[5]=ui8SecureCode2of3;
	rwdata[6]=ui8SecureCode3of3;
	Encryption_Write(rwdata);
	Roseek_Waitusec(5000);	//EEPROM Write Cycle Time Max 5 mS

	//////PERFORM WRITING//////
	j=0x19;
	pnt += 0x19;

	//address from 0x19 to 0x1f
	rwdata[0]=0xb4;
	rwdata[1]=0x00;
	rwdata[2]=j;
	rwdata[3]=0x07;
	for( k=0;k<0x07;k++)
	{
		rwdata[4+k]=*pnt;
		pnt++;
	}
	Encryption_Write(rwdata);
	Roseek_Waitusec(5000);		//EEPROM Write Cycle Time Max 5 mS
	j+=0x07;

	//address from 0x20 to 0xef
	for(i=0;i<13;i++)
	{
		rwdata[0]=0xb4;
		rwdata[1]=0x00;
		rwdata[2]=j;
		rwdata[3]=0x10;		//16 bytes per page for MIN device: AT88SC0104C
		for( k=0;k<16;k++)
		{
			rwdata[4+k]=*pnt;
			pnt++;
		}
		Encryption_Write(rwdata);
		Roseek_Waitusec(5000);	//EEPROM Write Cycle Time Max 5 mS
		j+=0x10;					//the next page
	}

	//��ȡд������ݲ��˶�

	memcpy( tmp, &AT88SC25616C_ConfigZone, 256);

	pnt = (Uint8*)&AT88SC25616C_ConfigZone;
	pnt += 0xe9;
	ui8SecureCode1of3 = *pnt;
	pnt++;
	ui8SecureCode2of3 = *pnt;
	pnt++;
	ui8SecureCode3of3 = *pnt;
	pnt++;

	pnt = (Uint8*)&AT88SC25616C_ConfigZone;
	pnt += 0x19;

	if( Roseek_Encryption_Read_Configuration( ui8SecureCode1of3, ui8SecureCode2of3, ui8SecureCode3of3))
	{
		//address from 0x19 to 0xef
		for( i=0; i<215; i++)
		{
			if( *(pnt+i) != tmp[0x19+i]){
				g_bUserEnpOpLock = FALSE;
				return FALSE;
			}
		}
	}
	else{
		g_bUserEnpOpLock = FALSE;
		return FALSE;
	}

	g_bUserEnpOpLock = FALSE;
	return TRUE;
}

//////////////////////////////////////////////////////////////////////
// function:	Roseek_Encryption_Read
// 
// Descript:	This function performs read operation from AT88SC25616C.
// Input:		
// Output:		None
// Note:		1 <= i2c_byte_number_read  <= 32768
//				if failure, return FALSE
//////////////////////////////////////////////////////////////////////
Bool Roseek_Encryption_Read(	Uint16 ui16SubAddress,
							Uint8 *puiDataReadBuff,
							Uint16 ui16ReadByteNumber
							)
{
	Uint8 current_zone,rd_high_addr,rd_low_addr;
	Uint16 zone_addr,current_len,rd_number,zone_rd_number,zone_rd_cnt;
	Uint16 j,k,l;

	if( g_bUserEnpOpLock ){
		return FALSE;
	}
	g_bUserEnpOpLock = TRUE;

	if( ui16ReadByteNumber < 1 ){
		g_bUserEnpOpLock = FALSE;
		return TRUE;
	}

	if( ui16SubAddress>32767 || ui16ReadByteNumber>32768 ){
		g_bUserEnpOpLock = FALSE;
		return FALSE;
	}

	if( Encryption_AAC!=0xff)									//ֻ�ܽ���һ����֤
		{Encryption_AAC=Encryption_AUTHENTICATION(KEY_SET);}//ʹ����Ӧ����Կ������֤

	if( Encryption_PAC!=0xff)												//ֻ�ܽ���һ��д����У��
		{Encryption_PAC=Encryption_verify_write_password(PASSWORD_SET);}	//ʹ����Ӧ���������д����У��

	if( Encryption_AAC!=0xff || Encryption_PAC!=0xff){
		g_bUserEnpOpLock = FALSE;
		return( FALSE );			//��֤����д�����У��û��ͨ��
	}

	current_zone = (Uint8)(ui16SubAddress/2048);
	zone_addr = ui16SubAddress%2048;
	current_len = ui16ReadByteNumber;

	while( current_len > 0)
	{
		Encryption_set_user_zone( current_zone);
		current_zone++;

		rd_high_addr = (Uint8)(zone_addr>>8);
		rd_low_addr = (Uint8)(zone_addr & 0x00ff);

		if( (zone_addr+current_len) >= 2048)
			zone_rd_number = 2048 - zone_addr;
		else
			zone_rd_number = current_len;

		if( zone_rd_number%128 == 0)
			zone_rd_cnt = zone_rd_number/128;
		else
			zone_rd_cnt = zone_rd_number/128+1;

		current_len -= zone_rd_number;

		//��ɱ��������ݵĶ�ȡ
		for( j=0; j<zone_rd_cnt; j++)
		{
			if( zone_rd_number >= 128)
				rd_number = 128;
			else
				rd_number = (Uint8)(zone_rd_number%128);

			//��ȡ���������ݣ����Ĵ����rwdata[0x04]��ʼ��λ��
			Encryption_read_user_zone(rd_high_addr,rd_low_addr,rd_number);

			for( k=0; k<rd_number; k++){
				*puiDataReadBuff++ = rwdata[k+4];
			}

			l = rd_high_addr*256 + rd_low_addr;
			l += rd_number;
			rd_high_addr = (Uint8)(l>>8);
			rd_low_addr = (Uint8)(l & 0x00ff);

			zone_rd_number -= rd_number;
		}

		zone_addr = 0x0000;
	}

	g_bUserEnpOpLock = FALSE;
	return( TRUE );
}

//////////////////////////////////////////////////////////////////////
// function:	Roseek_Encryption_Write
// 
// Descript:	This function performs write operation for AT88SC25616C.
// Input:		
// Output:		None
// Note:		if failure, return FALSE;
//////////////////////////////////////////////////////////////////////
Bool Roseek_Encryption_Write( 	Uint16 ui16SubAddress,
								Uint8 *pui8DataWriteBuff,
								Uint16 ui16WriteByteNumber
							)
{
	Uint8 current_zone,wr_high_addr,wr_low_addr,wr_number;
	Uint16 zone_addr,current_len,zone_wr_number,zone_wr_cnt;
	Uint16 j,k,l;

	if( g_bUserEnpOpLock ){
		return FALSE;
	}
	g_bUserEnpOpLock = TRUE;

	if( ui16SubAddress>32767 || ui16WriteByteNumber>32768 ){
		g_bUserEnpOpLock = FALSE;
		return FALSE;
	}

	if( Encryption_AAC!=0xff)									//ֻ�ܽ���һ����֤
		{Encryption_AAC=Encryption_AUTHENTICATION(KEY_SET);}	//ʹ����Ӧ����Կ������֤

	if( Encryption_PAC!=0xff)												//ֻ�ܽ���һ��д����У��
		{Encryption_PAC=Encryption_verify_write_password(PASSWORD_SET);} //ʹ����Ӧ���������д����У��

	if( Encryption_AAC!=0xff || Encryption_PAC!=0xff){
		g_bUserEnpOpLock = FALSE;
		return( FALSE );			//��֤����д�����У��û��ͨ��
	}

	current_zone = (Uint8)(ui16SubAddress/2048);
	zone_addr = ui16SubAddress%2048;
	current_len = ui16WriteByteNumber;

	while( current_len > 0)
	{
		//�л��û���
		Encryption_set_user_zone( current_zone);

		wr_high_addr = (Uint8)(zone_addr>>8);
		wr_low_addr = (Uint8)(zone_addr & 0x00ff);

		if( (zone_addr+current_len) >= 2048)
			zone_wr_number = 2048 - zone_addr;
		else
			zone_wr_number = current_len;

		if( zone_wr_number%128 == 0)
			zone_wr_cnt = zone_wr_number/128;
		else
			zone_wr_cnt = zone_wr_number/128+1;

		zone_addr = 0x0000;
		current_len -= zone_wr_number;

		//��ɱ��������ݵ�д��
		for( j=0; j<zone_wr_cnt; j++)
		{
			if( zone_wr_number >= 128)
				wr_number = 128;
			else
				wr_number = (Uint8)(zone_wr_number%128);

			for( k=0; k<wr_number; k++){
				rwdata[k+4] = *pui8DataWriteBuff++;
			}

			//���ܲ�д�����ݵ�AT88SCXX�����Ĵ����rwdata[0x04]��ʼ��λ��
			Encryption_write_user_zone(wr_high_addr,wr_low_addr,wr_number);

			//ִ����������ָ���Ժ󣬱�����ִ��һ��ָ�����ָ����ԣ�����������
			//���ݴ�AT88SCXXC�Ļ�������д��EEPROM�У�ʹ�õ�������ݲ���ʧ��
			//�ڴ�ִ���л��û�������ָ�
			Encryption_set_user_zone( current_zone);
			Encryption_AckPolling();

			l = wr_high_addr*256 + wr_low_addr;
			l += wr_number;
			wr_high_addr = (Uint8)(l>>8);
			wr_low_addr = (Uint8)(l & 0x00ff);

			zone_wr_number -= wr_number;
		}
		current_zone++;
	}

	g_bUserEnpOpLock = FALSE;
	return( TRUE );
}


