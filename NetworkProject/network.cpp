#pragma comment(lib, "ws2_32.lib")

#include <stdlib.h>
#include <winsock2.h>
#include <windows.h>
#include <cstdio>
#include <string>
#include "network.h"
#include "protocol.h"
#include "ai.h"

using namespace std;

SOCKET Network::hSocket;														// Client socket variable of client side
SOCKADDR_IN Network::servAddr;													// Server address variable of server/client side
char Network::messageToClient[MESSAGE_T0_CLIENT_SIZE];							// Message buffer of server side
int Network::mode;																// determine server/ client/ nothing
int Network::characterSelection[UNIT_NUM_MAX];												// Information of selection of charactor(dep)
char Network::gameStart[3];														// game started? not(N) start(G)
protocol_command Network::command[(UNIT_NUM_MAX)/2];															// selected command of client side
char * Network::serverIpArg;
protocol_team Network::team;

void Network::ErrorHandling(char *message)  //Error handling routine
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
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
		ErrorHandling("read() error");
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
	int strLen;
	strLen = recv(hSocket, gameStart, sizeof(gameStart) - 1, 0); // data recieving
	if (strLen == -1)
	{
		ErrorHandling("read() error");
	}
	gameStart[strLen] = '\0';
}

void Network::sendToServer(char message[]) // message sending routine
{
	send(hSocket, message, sizeof(message), 0);
}

void Network::init(char * argv) {
	setMode(MODE_CLIENT);
	serverIpArg = argv;

	makeClientSocket();

	int strLen;																			// ∆¿ ¡§∫∏ ¿˙¿Â
	char teamInfo[8];
	strLen = recv(hSocket, teamInfo, sizeof(teamInfo) - 1, 0); // data recieving

	if (strLen == -1)
	{
		ErrorHandling("read() error");
	}
	teamInfo[strLen] = '\0';
	setTeambyIndex(atoi(teamInfo));
	printf("team : %d", team);
	Ai::aiInit();
	char characterInfo[((UNIT_NUM_MAX) / 2) + 1];
	for (int i = 0; i<(UNIT_NUM_MAX) / 2; i++)
	{
		characterInfo[i] = getCharacterSelection(i) + '0';
	}
	sendToServer(characterInfo);
	printf("character chosen\n");
	recieveGameStart();
	printf("GameStart!");
}

void Network::loop() {
	while (1) {
		getProtocolDataFromServer();												// get protocol data of that time

		if (team == Ai::getPivot()) Ai::ai(*((protocol_data*)messageToClient));
		else Ai::ai(mirror_data(*((protocol_data*)messageToClient)));

		char toSend[MESSAGE_TO_SERVER_SIZE];
		for (int i = 0; i < (UNIT_NUM_MAX) / 2; i++)
			toSend[i] = getCommand(i) + '0';

		sendToServer(toSend);														// send command
	}
}