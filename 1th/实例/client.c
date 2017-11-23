#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#include "text.h"

/*�漰����ϵͳ����: 
 * 1. socket
 * 2. connect
 * 3. send/recv
 */

#define SERVER_PORT 8888	/*��ʾ����ĸ�ip��8888�˿ڵ�����*/

int main(int argc, char **argv)
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	
	int iRet;
	//unsigned char ucSendBuf[FileSize];
	int iSendLen;
	int FileSize;
       /*�����������ĸ�����Ϊ2����ӡ�÷�*/
	if (argc != 2)
	{
		printf("Usage:\n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	}
       //AF_INET:IPV4(���޸�),SOCK_STREAM:��ʾTCP,0(���޸�)
	iSocketClient = socket(AF_INET, SOCK_STREAM, 0);		/*1. ���socket�ľ��*/
       /*tSocketServerAddr�����úͷ������ļ�����ͬ*/
	tSocketServerAddr.sin_family      = AF_INET;/*���޸�*/
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);/*���Ķ˿�,SERVER_PORT�궨��8888�������Ķ˿�Ϊ8888,ֻ���޸ĺ�SERVER_PORT*/
	                                                                                   /*htonsΪ���˿ں�SERVER_PORTת��Ϊ�����ֽ��򣬶˿ں�8888��д��ֻ��Ϳͻ�����ͬ����*/
 	
 	//���ַ���(����ĵ�2������argv[1]) ת��Ϊ�����ֽ��� ������tSocketServerAddr.sin_addr����server�˵�htons���������෴
 	if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))	/*����ڶ�������argv[1]Ϊserver��IP��ַ������ʾclient�����ӵ���һ��server*/
 	{
		printf("invalid server_ip\n");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);

       //��������tSocketServerAddr
	iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));	/*2. ������server�˵�����*/	
       /*���Ӳ��ɹ����ӡ����ʧ��*/
	if (-1 == iRet)	/*���ӳɹ�����0�����Ӳ��ɹ�����-1*/
	{
		printf("connect error!\n");
		return -1;
	}
	/*�����ӳɹ�������while�в��ϵ���server�˷�������*/
	iRet = OpenFile();
	FileSize = GetFileSize();
	printf("%d\n", FileSize);
	unsigned char ucSendBuf[FileSize];
	if (iRet == -1)
	{
		printf("OpenFile error!\n");
		return -1;
	}
	while (1)
	{
	       //���ӳɹ��󣬴ӱ�׼����stdin�л�����ݴ���ucSendBuf���飬�������󳤶�Ϊ999
		//if (fgets(ucSendBuf, 999, stdin))		/*�ӱ�׼���������ݣ������浽ucSendBuf����*/

			//iRet = CopyCodeFromBuf(ucSendBuf);
		   	iRet = GetCodeFrmBuf(ucSendBuf);
		   	//ucSendBuf[1] = '\0';
		   	//ucSendBuf[999] = '\0';
			if (iRet == -1)
			{
				printf("CopyCodeFromBuf error!\n");
				return -1;
			}
			//ucSendBuf[0] = '1';
			//ucSendBuf[999] = '\0';
			printf("%s\n", ucSendBuf);
			

			
		
		       //ͨ��socket������ucSendBuf�����ݷ��͵�������iSocketClient�����ͳ���Ϊstrlen(ucSendBuf)��flag=0
                     //����ֵΪ���͵ĳ���
			//iSendLen = send(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0);		/*3. ��server�˷�������*/
			   iSendLen = send(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0);
                     //�����صķ��͵ĳ���Ϊ0���ر�socket
			if (iSendLen <= 0)
			{
				close(iSocketClient);
				return -1;
			}
			break;
	}
	
	return 0;
}

