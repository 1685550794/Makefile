#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include "text.h"

static int fd;
char FileName[128];
struct stat tStat;
static unsigned char *g_TextFileMem;
static unsigned char *g_TextFileMemEnd;


int OpenFile(void)
{
	strcpy(FileName,"data.txt");
	fd = open(FileName, O_RDONLY);
	if (fd < 0)
	{
		printf("open %s error \n", FileName);
	}

	//����ı�����Ϣ�������ı��ж��
	if(fstat(fd, &tStat))
	{
		printf("can't get fstat\n");
		return -1;
	}
	//ͨ��mmap,���ı�������ӳ�䵽һ���ڴ���
	g_TextFileMem = (unsigned char *)mmap(NULL , tStat.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (g_TextFileMem == (unsigned char *)-1)
	{
		printf("can't mmap for text file\n");
		return -1;
	}

	g_TextFileMemEnd = g_TextFileMem + tStat.st_size;
	return 0;

}

#if 1
int GetCodeFrmBuf(unsigned char *pdwCode)
{
	unsigned char *pucBuf = g_TextFileMem;
	unsigned char c = *pucBuf;
	int i;

	for (i = 0; i <  tStat.st_size; i++)
	{
		unsigned char c = *pucBuf++;
		/* ����ASCII�� */
		*pdwCode++ = (unsigned int)c;
		
	}
	*pdwCode = '\0';
	return 1;
}
int GetFileSize(void)
{
	return tStat.st_size;
}
#endif

#if 0
//��buf����������ļ�һ���ַ��ı��룬���ļ���ʱ�����ݻ�浽buf���˺�����buf�ж�����һ������ֵ���浽pdwCode��
static int GetCodeFrmBuf(unsigned char *pucBufStart, unsigned char *pucBufEnd, unsigned char *pdwCode)
{
	unsigned char *pucBuf = pucBufStart;
	unsigned char c = *pucBuf;

	//if ((pucBuf < pucBufEnd) && (c < (unsigned char)0x80))
	{
		/* ����ASCII�� */
		*pdwCode = (unsigned int)c;
		return 1;
	}
	//else
	{
		//return 0;
	}
}




int CopyCodeFromBuf(unsigned char *pucSendBuf)
{
	int i;
	int iRet;
	unsigned char *pucBuf = g_TextFileMem;
	unsigned char dwCode;
	
	for (i = 0; i < 10; i++)
	{
		iRet = GetCodeFrmBuf(pucBuf, g_TextFileMemEnd, &dwCode);
		if (!iRet)
		{
			return 0;
		}
		pucSendBuf = &dwCode;
		pucSendBuf++;
		pucBuf++;
		
	}
}
#endif


