#pragma comment(lib, "ws2_32.lib")

#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <string>
#include "server.h"
#include "protocol.h"
using namespace std;


/*==================================================
Error Message print
==================================================*/
void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void communicate(int cnt)
{
	printf("�Ϳ� ��������");
}


void makeServerSocket(SOCKET * hServSock, char * argv[], SOCKADDR_IN *servAddr)
{

	WSADATA wsaData;

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error!");
	}

	// ���� ���� ����
	*hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (*hServSock == INVALID_SOCKET)
	{
		ErrorHandling("socket() error");
	}

	memset(servAddr, 0, sizeof(*servAddr));
	servAddr->sin_family = AF_INET;
	servAddr->sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr->sin_port = htons(atoi(argv[1]));

	// ���Ͽ� �ּ� �Ҵ�
	if (bind(*hServSock, (SOCKADDR*)servAddr, sizeof(*servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("bind() error");
	}

	// ���� ��û ��� ����
	if (listen(*hServSock, 6) == SOCKET_ERROR)
	{
		ErrorHandling("listen() error");
	}
}

void acceptClient(SOCKET * hServSock, SOCKET  hClntSock[], SOCKADDR_IN clntAddr[], int szClntAddr[])
{
	// ���� ��û ����
	szClntAddr[0] = sizeof(clntAddr[0]);
	szClntAddr[1] = sizeof(clntAddr[1]);
	hClntSock[0] = accept(*hServSock, (SOCKADDR*)&clntAddr[0], &szClntAddr[0]);
	hClntSock[1] = accept(*hServSock, (SOCKADDR*)&clntAddr[1], &szClntAddr[1]);

	if ((hClntSock[0] == INVALID_SOCKET) || (hClntSock[1] == INVALID_SOCKET))
	{
		ErrorHandling("accept() error");
	}
}

void sendToClient(char messageToClient[], SOCKET  hClntSock[])
{
	send(hClntSock[0], messageToClient, sizeof(messageToClient), 0);
	send(hClntSock[1], messageToClient, sizeof(messageToClient), 0);
	send(hClntSock[2], messageToClient, sizeof(messageToClient), 0);
	send(hClntSock[3], messageToClient, sizeof(messageToClient), 0);
	send(hClntSock[4], messageToClient, sizeof(messageToClient), 0);
	send(hClntSock[5], messageToClient, sizeof(messageToClient), 0);
}

void recieveFromClient(SOCKET hClntSock[], char messageFromClient[][sizeof(protocol_data)])
{
	for (int i = 0; i < 6; i++)
	{
		int strLen = recv(hClntSock[i], messageFromClient[i], 1024, 0);
		messageFromClient[i][strLen] = '\0';
	}

}

void closeServerConnection(SOCKET  hClntSock[])
{
	for (int i = 0; i < 6; i++)
	{
		closesocket(hClntSock[i]);
	}
	WSACleanup();
}


/*==================================================
Entry Point
==================================================*/



/*
int main(int argc, char **argv)
{
WSADATA wsaData;
SOCKET hServSock;
SOCKET hClntSock;
SOCKET hClntSock2;
SOCKADDR_IN servAddr;
SOCKADDR_IN clntAddr;
SOCKADDR_IN clntAddr2;
int szClntAddr;
int szClntAddr2;

char message[] = "Hello World!\n";

if (argc != 2)
{
printf("Usage : %s <port> \n", argv[0]);
exit(1);
}

// Load Winsock 2.2 DLL
if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
{
ErrorHandling("WSAStartup() error!");
}

// ���� ���� ����
hServSock = socket(PF_INET, SOCK_STREAM, 0);
if (hServSock == INVALID_SOCKET)
{
ErrorHandling("socket() error");
}

memset(&servAddr, 0, sizeof(servAddr));
servAddr.sin_family = AF_INET;
servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
servAddr.sin_port = htons(atoi(argv[1]));

// ���Ͽ� �ּ� �Ҵ�
if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
{
ErrorHandling("bind() error");
}

// ���� ��û ��� ����
if (listen(hServSock, 5) == SOCKET_ERROR)
{
ErrorHandling("listen() error");
}

// ���� ��û ����
szClntAddr = sizeof(clntAddr);
szClntAddr2 = sizeof(clntAddr2);
hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
hClntSock2 = accept(hServSock, (SOCKADDR*)&clntAddr2, &szClntAddr2);

if ((hClntSock == INVALID_SOCKET) || hClntSock2 == INVALID_SOCKET)
{
ErrorHandling("accept() error");
}

char *from_client = new char[1024];
char *from_client2 = new char[1024];

// ������ ����
send(hClntSock, message, sizeof(message), 0);
send(hClntSock2, message, sizeof(message), 0);

int cnt = 5;
while (cnt--)
{
Sleep(500);

int strLen = recv(hClntSock, from_client, 1024, 0);
int strLen2 = recv(hClntSock2, from_client2, 1024, 0);

from_client[strLen] = '\0';
from_client2[strLen2] = '\0';
printf("this is %s from client0\n", from_client);
printf("this is %s from client1\n\n", from_client2);
}



// ���� ����
closesocket(hClntSock);
WSACleanup();

return 0;
}
*/

