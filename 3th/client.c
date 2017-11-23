#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#include "text.h"

/*涉及到的系统调用: 
 * 1. socket
 * 2. connect
 * 3. send/recv
 */

#define SERVER_PORT 8888	/*表示监测哪个ip的8888端口的数据*/

int main(int argc, char **argv)
{
	int iSocketClient;
	struct sockaddr_in tSocketServerAddr;
	
	int iRet;
	//unsigned char ucSendBuf[FileSize];
	int iSendLen;
	int FileSize;
       /*如果输入参数的个数不为2，打印用法*/
	if (argc != 2)
	{
		printf("Usage:\n");
		printf("%s <server_ip>\n", argv[0]);
		return -1;
	}
       //AF_INET:IPV4(不修改),SOCK_STREAM:表示TCP,0(不修改)
	iSocketClient = socket(AF_INET, SOCK_STREAM, 0);		/*1. 获得socket的句柄*/
       /*tSocketServerAddr的设置和服务器文件中相同*/
	tSocketServerAddr.sin_family      = AF_INET;/*不修改*/
	tSocketServerAddr.sin_port        = htons(SERVER_PORT);/*检测的端口,SERVER_PORT宏定义8888，即检测的端口为8888,只需修改宏SERVER_PORT*/
	                                                                                   /*htons为将端口号SERVER_PORT转化为网络字节序，端口号8888任写，只需和客户端相同即可*/
 	
 	//将字符串(输入的第2个参数argv[1]) 转化为网络字节序 并存入tSocketServerAddr.sin_addr，和server端的htons函数作用相反
 	if (0 == inet_aton(argv[1], &tSocketServerAddr.sin_addr))	/*输入第二个参数argv[1]为server的IP地址，即表示client想连接到哪一个server*/
 	{
		printf("invalid server_ip\n");
		return -1;
	}
	memset(tSocketServerAddr.sin_zero, 0, 8);

       //启动连接tSocketServerAddr
	iRet = connect(iSocketClient, (const struct sockaddr *)&tSocketServerAddr, sizeof(struct sockaddr));	/*2. 建立到server端的连接*/	
       /*连接不成功则打印连接失败*/
	if (-1 == iRet)	/*连接成功返回0，连接不成功返回-1*/
	{
		printf("connect error!\n");
		return -1;
	}
	/*若连接成功，则在while中不断地向server端发送数据*/
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
	       //连接成功后，从标准输入stdin中获得数据存入ucSendBuf数组，存入的最大长度为999
		//if (fgets(ucSendBuf, 999, stdin))		/*从标准输入获得数据，并保存到ucSendBuf数组*/

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
			

			
		
		       //通过socket将数组ucSendBuf的数据发送到服务器iSocketClient，发送长度为strlen(ucSendBuf)，flag=0
                     //返回值为发送的长度
			//iSendLen = send(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0);		/*3. 向server端发送数据*/
			   iSendLen = send(iSocketClient, ucSendBuf, strlen(ucSendBuf), 0);
                     //若返回的发送的长度为0，关闭socket
			if (iSendLen <= 0)
			{
				close(iSocketClient);
				return -1;
			}
			break;
	}
	
	return 0;
}

