/**
 * \file      	EE3_SendSDFSData.c
 * \author    	Tanggula Yang
 * \version   	1.08
 * \date      	2012.07.13
 * \brief     	SD���ڴ洢��ͼƬ���ݷ��ʡ�����ģ��
**/

#include "EE3_common.h"
#include "netmain.h"
#include "SDFS_filesys.h"

#define MAX_FILE_NAME_LEN	19
#define MAX_FILE_SIZE		(2 * 1024 * 1024)
volatile Uint8 	g_ui8SDFSCMD = 0;
char	g_szParam1[MAX_FILE_NAME_LEN];
char	g_szParam2[MAX_FILE_NAME_LEN];	

extern void Roseek_Reset();
extern int extHeap;	//����������cdb�ļ��ж���
extern SEM_Handle sem_SDFSInitReady;

static void Uint32ToBuf(Uint8 *pBuf, Uint32 ui32Value)
{
	*((Uint32*)pBuf) = ui32Value;
}

/**
 * \function 	SendSDFSDataPort
 * \brief    	ʹ��TCP/IPЭ��ʵ��SD���ڴ洢���ݵ�������ʡ�����
 * \			Ĭ����Ϊ�������ˣ�Port = 46000		
 * \note		ʹ�ô��߲��ȡ���ݵ�ǰ�����û����밴��SD FS�ĸ�ʽ����SD���ڵ�������д
**/	
void SendSDFSDataPort()
{
	SOCKET	sockSDFSListen;
	SOCKET	sockSDFSSvr;
	struct	sockaddr_in	addr;
	int		iSize;
	Uint8	ui8Ret;
	Uint8	*pui8DataBuf = NULL;
	Uint32	ui32DataBufSize = 0;
	Uint8 	aui8Temp[4];
	
	//TSK_sleep(10000);
	//��ʼSD���ļ�ϵͳģ��
	RS_FS_Initialize(50*1024, 512, 2048, 3);
	/*�ȴ��ļ�ϵͳ��ʼ���*/
	SEM_pend(sem_SDFSInitReady, SYS_FOREVER);
	SEM_post(sem_SDFSInitReady);
	
	ui32DataBufSize = MAX_FILE_SIZE;
	pui8DataBuf = MEM_alloc(extHeap, ui32DataBufSize, 256);
	
	fdOpenSession(TaskSelf());
	
	sockSDFSListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if ( sockSDFSListen == INVALID_SOCKET)
	{
		Roseek_Reset();
	}
	
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_len = sizeof(addr);
	addr.sin_port = htons(46000);
	
	if (bind(sockSDFSListen, (PSA)&addr, sizeof(addr)) < 0)
	{
		Roseek_Reset();
	}
	
	if (listen(sockSDFSListen, 1) < 0)
	{
		Roseek_Reset();
	}
	
	while (1)
	{
		iSize = sizeof(addr);
		sockSDFSSvr = accept(sockSDFSListen, (PSA)&addr, &iSize);
		if (sockSDFSSvr == INVALID_SOCKET)
		{
			fdClose(sockSDFSSvr);
			continue;
		}
		
		if (0 == g_ui8SDFSCMD)
		{
			fdClose(sockSDFSSvr);
			continue;
		}
		
		switch (g_ui8SDFSCMD)
		{
			/*��ȡ���������ڵ�����ͼƬ�ļ���*/
			case 1:
			{
				Uint32 ui32FileNum = 0;
				ui8Ret = RS_FS_Find(&ui32FileNum, g_szParam1, g_szParam2);
				if (0 == ui8Ret && ui32FileNum > 0)
				{
					Uint32 i = 0;
					char szFileName[MAX_FILE_NAME_LEN];
					Uint32ToBuf(aui8Temp, ui32FileNum);
					send(sockSDFSSvr, aui8Temp, 4, 0);
					Uint32ToBuf(aui8Temp, MAX_FILE_NAME_LEN);					
					for (; i < ui32FileNum; i++)
					{
						send(sockSDFSSvr, aui8Temp, 4, 0);
						RS_FS_GetNext(szFileName, MAX_FILE_NAME_LEN, i);
						send(sockSDFSSvr, szFileName, MAX_FILE_NAME_LEN, 0); 
						if (i%100 == 0)
						{
							TSK_sleep(100);
						}
					}
				}
			}
			break;
			/*��ȡ���������ڵ�����ͼƬ�ļ�����*/
			case 2:
			{
				Uint32 ui32FileNum = 0;
				ui8Ret = RS_FS_Find(&ui32FileNum, g_szParam1, g_szParam2);
				if (0 == ui8Ret && ui32FileNum > 0)
				{
					Uint32 i = 0;
					Uint32 ui32FileSize;
					char szFileName[MAX_FILE_NAME_LEN];
					Uint32ToBuf(aui8Temp, ui32FileNum);
					send(sockSDFSSvr, aui8Temp, 4, 0);					
					for (; i < ui32FileNum; i++)
					{
						RS_FS_GetNext(szFileName, MAX_FILE_NAME_LEN, i);
						RS_FS_GetFileSize(&ui32FileSize, szFileName);
						if (ui32FileSize > ui32DataBufSize)
						{
							ui32FileSize = ui32DataBufSize;
						}
						RS_FS_Read(pui8DataBuf, ui32DataBufSize, szFileName);
						Uint32ToBuf(aui8Temp, ui32FileSize + MAX_FILE_NAME_LEN);
						send(sockSDFSSvr, aui8Temp, 4, 0);
						send(sockSDFSSvr, szFileName, MAX_FILE_NAME_LEN, 0); 
						send(sockSDFSSvr, pui8DataBuf, ui32FileSize, 0);

						if (i%100 == 0)
						{
							TSK_sleep(100);
						}
					}
				}
			}
			break;
			/*ɾ�����������ڵ�����ͼƬ�ļ�*/
			case 3:
			{
				char szReturn[10];
				ui8Ret = RS_FS_DelByDate(g_szParam1, g_szParam2);
				if (0 == ui8Ret)
				{
					strcpy(szReturn, "success");
				}
				else
				{
					strcpy(szReturn, "fail");
				}
				Uint32ToBuf(aui8Temp, 1);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				Uint32ToBuf(aui8Temp, 10);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				send(sockSDFSSvr, szReturn, 10, 0);
			}
			break;
			/*��ȡĳ���ļ�����ͼƬ����*/
			case 4:
			{
				Uint32 ui32FileSize;
				RS_FS_GetFileSize(&ui32FileSize, g_szParam1);
				if (ui32FileSize > ui32DataBufSize)
				{
					ui32FileSize = ui32DataBufSize;
				}
				RS_FS_Read(pui8DataBuf, ui32DataBufSize, g_szParam1); 
				Uint32ToBuf(aui8Temp, 1);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				Uint32ToBuf(aui8Temp, ui32FileSize + MAX_FILE_NAME_LEN);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				send(sockSDFSSvr, g_szParam1, MAX_FILE_NAME_LEN, 0);
				send(sockSDFSSvr, pui8DataBuf, ui32FileSize, 0);				
			}
			break;			
			/*ɾ��ĳ��ͼƬ�ļ�*/
			case 5:
			{
				char szReturn[10];
				ui8Ret = RS_FS_Del(g_szParam1);
				if (0 == ui8Ret)
				{
					strcpy(szReturn, "success");
				}
				else
				{
					strcpy(szReturn, "fail");
				}
				Uint32ToBuf(aui8Temp, 1);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				Uint32ToBuf(aui8Temp, 10);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				send(sockSDFSSvr, szReturn, 10, 0);
			}
			break;
			/*��ȡ���������ڵ�����ͼƬ�ļ����ݣ���ɾ��*/
			case 6:
			{
				Uint32 ui32FileNum = 0;
				ui8Ret = RS_FS_Find(&ui32FileNum, g_szParam1, g_szParam2);
				if (0 == ui8Ret && ui32FileNum > 0)
				{
					Uint32 i = 0;
					Uint32 ui32FileSize;
					char szFileName[MAX_FILE_NAME_LEN];
					Uint32ToBuf(aui8Temp, ui32FileNum);
					send(sockSDFSSvr, aui8Temp, 4, 0);					
					for (; i < ui32FileNum; i++)
					{
						RS_FS_GetNext(szFileName, MAX_FILE_NAME_LEN, i);
						RS_FS_GetFileSize(&ui32FileSize, szFileName);
						if (ui32FileSize > ui32DataBufSize)
						{
							ui32FileSize = ui32DataBufSize;
						}
						RS_FS_Read(pui8DataBuf, ui32DataBufSize, szFileName);
						Uint32ToBuf(aui8Temp, ui32FileSize + MAX_FILE_NAME_LEN);
						send(sockSDFSSvr, aui8Temp, 4, 0);
						send(sockSDFSSvr, szFileName, MAX_FILE_NAME_LEN, 0); 
						send(sockSDFSSvr, pui8DataBuf, ui32FileSize, 0);
						RS_FS_Del(szFileName);

						if (i%100 == 0)
						{
							TSK_sleep(100);
						}
					}
				}
			}
			break;
			/*��ȡĳ��ͼƬ�ļ����ݣ���ɾ��*/
			case 7:
			{
				Uint32 ui32FileSize;
				RS_FS_GetFileSize(&ui32FileSize, g_szParam1);
				if (ui32FileSize > ui32DataBufSize)
				{
					ui32FileSize = ui32DataBufSize;
				}
				RS_FS_Read(pui8DataBuf, ui32DataBufSize, g_szParam1); 
				Uint32ToBuf(aui8Temp, 1);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				Uint32ToBuf(aui8Temp, ui32FileSize + MAX_FILE_NAME_LEN);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				send(sockSDFSSvr, g_szParam1, MAX_FILE_NAME_LEN, 0);
				send(sockSDFSSvr, pui8DataBuf, ui32FileSize, 0);
				RS_FS_Del(g_szParam1);
			}
			break;
			/*��ȡ����洢����*/
			case 8:
			{
				Uint32ToBuf(aui8Temp, 1);
				send(sockSDFSSvr, aui8Temp, 4, 0);
				Uint32ToBuf(aui8Temp, RS_FS_GetIdleBlocks());
				send(sockSDFSSvr, aui8Temp, 4, 0);
				Uint32ToBuf(aui8Temp, RS_FS_GetUsedBlocks());
				send(sockSDFSSvr, aui8Temp, 4, 0);
			}
			break;
			default:
			break;
		}
		fdClose(sockSDFSSvr);
		g_ui8SDFSCMD = 0;
	}
	
}

















