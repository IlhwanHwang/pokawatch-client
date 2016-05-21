#pragma comment(lib, "ws2_32.lib")

#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <string>
#include "network.h"
#include "protocol.h"

using namespace std;

SOCKET Network::hServSock;												// of server
SOCKET Network::hClntSock[UNIT_NUM_MAX];									// of server
SOCKET Network::hSocket;													// of client
SOCKADDR_IN Network::servAddr;											// of server/client
SOCKADDR_IN Network::clntAddr[UNIT_NUM_MAX];								// of server
int Network::szClntAddr[UNIT_NUM_MAX];									// of server
char Network::messageToClient[MESSAGE_T0_CLIENT_SIZE];					// of server
char Network::messageFromClient[UNIT_NUM_MAX][MESSAGE_TO_SERVER_SIZE];	// of server
char Network::messageToServer[MESSAGE_TO_SERVER_SIZE];					// of client
int Network::mode;														//determine server/ client/ nothing
int Network::characterSelection;
char Network::gameStart[2];												// not(N) start(G)
int Network::command;

void Network::ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void Network::communicate(int cnt)
{
	printf("�Ϳ� ��������");
}

void Network::makeServerSocket()
{
	string portString = PORT_STRING;
	char * port = (char*)portString.c_str();

	WSADATA wsaData;

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");

	// ���� ���� ����
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(port));

	// ���Ͽ� �ּ� �Ҵ�
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("bind() error");

	// ���� ��û ��� ����
	if (listen(hServSock, UNIT_NUM_MAX) == SOCKET_ERROR) ErrorHandling("listen() error");

	printf("listen");
}

void Network::acceptClient()
{
	// ���� ��û ����
	for (int i = 0; i < UNIT_NUM_MAX; i++) szClntAddr[i] = sizeof(clntAddr[i]);
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAddr[i], &szClntAddr[i]);
		if (hClntSock[i] == INVALID_SOCKET) ErrorHandling("accept() error");
	}
}

void Network::sendToClient(char messageToClient[])
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)	send(hClntSock[i], messageToClient, sizeof(messageToClient), 0);
}

void Network::recieveFromClient()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		int strLen = recv(hClntSock[i], messageFromClient[i], MESSAGE_TO_SERVER_SIZE-1, 0);
		messageFromClient[i][strLen] = '\0';
	}
}

void Network::closeServerConnection()
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		closesocket(hClntSock[i]);
	}
	WSACleanup();
}

void Network::makeClientSocket()
{
	string servIpString = SERV_IP_STRING;
	string portNumString = PORT_STRING;
	char * servIp = (char*)servIpString.c_str();
	char * portNum = (char*)portNumString.c_str();

	WSADATA wsaData;

	// Load WinSocket 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup(), error");
	}

	// ���� ������ ���� ���� ����
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("hSocketet(), error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIp);
	servAddr.sin_port = htons(atoi(portNum));

	// ������ ���� ��û
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("Connect() error");
	}

}


void Network::getProtocolDataFromServer()
{
	int strLen;
	// ������ ����
	strLen = recv(hSocket, messageToClient, sizeof(messageToClient) - 1, 0);
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	messageToClient[strLen] = '\0';
}

void Network::recieveGameStart()
{
	int strLen;
	// ������ ����
	strLen = recv(hSocket, gameStart, sizeof(gameStart) - 1, 0);
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	gameStart[strLen] = '\0';
}

void Network::closeClientConnection()
{
	// ���� ����
	closesocket(hSocket);
	WSACleanup();
}

void Network::sendToServer(char message[])
{
	send(hSocket, message, sizeof(message), 0);
}
