#pragma comment(lib, "ws2_32.lib")

#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <string>
#include "network.h"
#include "protocol.h"
#include "game.h"
#include "key.h"
#include "ai.h"

#include "draw.h"
#include "resource.h"

using namespace std;

SOCKET Network::hServSock;														// Server socket variable of server/client side
SOCKET Network::hClntSock[CLIENT_NUM_MAX];										// Client socket variable of server side
SOCKET Network::hSocket;														// Client socket variable of client side
SOCKADDR_IN Network::servAddr;													// Server address variable of server/client side
SOCKADDR_IN Network::clntAddr[CLIENT_NUM_MAX];									// Client address variable of server side
int Network::szClntAddr[CLIENT_NUM_MAX];											// Client address size variable of server side
char Network::messageToClient[MESSAGE_T0_CLIENT_SIZE];							// Message buffer of server side
char Network::messageFromClient[CLIENT_NUM_MAX][MESSAGE_TO_SERVER_SIZE*3];			// Message buffer of server side
char Network::messageToServer[MESSAGE_TO_SERVER_SIZE*3];							// Message buffer of client side
int Network::mode;																// determine server/ client/ nothing
int Network::characterSelection[UNIT_NUM_MAX];												// Information of selection of charactor(dep)
char Network::gameStart[3];														// game started? not(N) start(G)
int Network::command[(UNIT_NUM_MAX)/2];															// selected command of client side
char * Network::serverIpArg;

void Network::ErrorHandling(char *message)  //Error handling routine
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void Network::makeServerSocket() // Server Socket making routine
{
	string portString = PORT_STRING;
	char * port = (char*)portString.c_str();

	WSADATA wsaData;

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) ErrorHandling("WSAStartup() error!");

	// Create Server socket
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(port));

	// Allocate address of server
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) ErrorHandling("bind() error");

	// Listeing for client
	if (listen(hServSock, CLIENT_NUM_MAX) == SOCKET_ERROR) ErrorHandling("listen() error");

	printf("Server Listen\n");
}

void Network::acceptClient()
{
	//accept client's request
	for (int i = 0; i < CLIENT_NUM_MAX; i++) szClntAddr[i] = sizeof(clntAddr[i]);
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		hClntSock[i] = accept(hServSock, (SOCKADDR*)&clntAddr[i], &szClntAddr[i]);
		if (hClntSock[i] == INVALID_SOCKET) ErrorHandling("accept() error");
	}
	// give their team
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		char teamInfo[8];
		teamInfo[0] = i + '0';
		teamInfo[1] = '\0';

		int WhatDo = send(hClntSock[i], teamInfo, sizeof(teamInfo) - 1, 0);
	}
}

void Network::sendToClient(char *messageToClient) // Message sending routine of client side
{
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		int WhatDo = send(hClntSock[i], messageToClient, MESSAGE_T0_CLIENT_SIZE - 1, 0);
	}
}

void Network::recieveFromClient() // Message recieving routine of server side
{
	for (int i = 0; i < CLIENT_NUM_MAX; i++)
	{
		int strLen = recv(hClntSock[i], messageFromClient[i], MESSAGE_TO_SERVER_SIZE - 1, 0);
		messageFromClient[i][strLen] = '\0';
	}
}

void Network::closeServerConnection() //server cosing routine
{
	for (int i = 0; i < UNIT_NUM_MAX; i++)
	{
		closesocket(hClntSock[i]);
	}
	WSACleanup();
}

void Network::makeClientSocket() // Client socket making routine
{
	string servIpString = serverIpArg;
	string portNumString = PORT_STRING;
	char * servIp = (char*)servIpString.c_str();
	char * portNum = (char*)portNumString.c_str();

	WSADATA wsaData;

	// Load WinSocket 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup(), error");
	}

	// Create client socket 
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
		ErrorHandling("hSocketet(), error");
	}

	printf("Socket made\n");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(servIp);
	servAddr.sin_port = htons(atoi(portNum));

	// Request for connetion
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		ErrorHandling("Connect() error");
	}
}


void Network::getProtocolDataFromServer() // Message receving from server
{
	int strLen;
	//recieving data
	strLen = recv(hSocket, messageToClient, sizeof(messageToClient) - 1, 0);
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	messageToClient[strLen] = '\0';

	/* For Client the only information given is protocol data below
	* because this game is made for the purpose of AI game
	* every AI should make decision based on this protocol data */

	// for checking recieving data appropriately protocol data is printed

	protocol_data *newData = (protocol_data*)messageToClient;
}

void Network::recieveGameStart()
{
	Draw::naivefill(Rspr::infoClient2);
	glutSwapBuffers();

	int strLen;
	strLen = recv(hSocket, gameStart, sizeof(gameStart) - 1, 0); // data recieving
	if (strLen == -1)
	{
		Network::ErrorHandling("read() error");
	}
	gameStart[strLen] = '\0';
}

void Network::closeClientConnection()
{
	//connection closing
	closesocket(hSocket);
	WSACleanup();
}

void Network::sendToServer(char message[]) // message sending routine
{
	send(hSocket, message, sizeof(message), 0);
}

void Network::turn() // turn routine
{
	// in timer each turn this function is called
	if (Network::getMode() == MODE_CLIENT && Network::getGameStart()[0] == GAME_START_CHAR && !Game::isEnded() ) // for client when game started 
	{
		Network::getProtocolDataFromServer();												// get protocol data of that time

		if(team==TEAM_POSTECH) Ai::ai(*((protocol_data*)messageToClient));
		else Ai::ai(mirror_data(*((protocol_data*)messageToClient)));

		char toSend[MESSAGE_TO_SERVER_SIZE];
		for (int i = 0; i < (UNIT_NUM_MAX) / 2; i++)		
			toSend[i] = getCommand(i) + '0';

		Network::sendToServer(toSend);														// send command
	}
}

void Network::update() // frame turn routine
{
	if (Network::getMode() == MODE_NOTHING)		// mode selection (SERVER)
	{
		char gameStartMessage[2];
		gameStartMessage[0] = GAME_START_CHAR;
		gameStartMessage[1] = '\0';

		Draw::naivefill(Rspr::infoServer);
		glutSwapBuffers();

		Network::setMode(MODE_SERVER);														// set as server
		printf("mode - server chosn\n");
		Network::makeServerSocket();														// server socket made
		printf("socket made \n");
		Network::acceptClient();															// accepting client
		printf("accepted\n");
		Network::recieveFromClient();														// recieve spawn information

		for (int i = 0; i < UNIT_NUM_MAX / 2; i++)												// spawn units
		{
			for (int j = 0; j < CLIENT_NUM_MAX; j++)
			{
				Game::getUnit(((UNIT_NUM_MAX) / 2) * j + i).spawn((protocol_dep)(messageFromClient[j][i] - '0'));
				cout << messageFromClient << "was came" << endl;
				cout << j << "����" << i << "���� ĳ���Ϳ� �ش��ϴ�" << messageFromClient[j][i] - '0' << endl;
			}
		}

		Game::release();
		sendToClient(gameStartMessage);
		setGameStart(0, GAME_START_CHAR);
	}
}
