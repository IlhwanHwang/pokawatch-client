//
// CNH 2016.05.19
// timer module
//

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <cstdio>
#include <WinSock2.h>
#include <string>

#include "timer.h"
#include "client.h"
#include "server.h"
#include "client.h"
#include "protocol.h"
#include "key.h"

using namespace std;

int Timer::frameInterval;
int Timer::framePerTurn;

extern SOCKET hServSock;			// of server
extern SOCKET hClntSock[6];		// of server
extern SOCKET hSocket;				// of client
extern SOCKADDR_IN servAddr;		// of server/client
extern SOCKADDR_IN clntAddr[6];	// of server
extern int szClntAddr[6];			// of server
extern char messageToClient[3];		// of server
extern char messageFromClient[6][16]; // of server
extern char messageToServer[];		// of client
extern int mode;					//determine server(1) client(2) or nothing (0)
extern int characterSelection;		//character_selection_time;
extern char gameStart[2];				// start(1)




int command;


void Timer::init(int interval, int perturn) {
	frameInterval = interval;
	framePerTurn = perturn;

	glutTimerFunc(frameInterval, update, 0);
}

void Timer::turn() {
	//std::cout << "Turn!" << std::endl;

	// add any per turn action

	if (mode == 8 && gameStart[0] == 'G')
	{
		sendToServer(&hSocket, (char *)(to_string(command)).c_str());
		printf("command : %d was sent", command);
	}
	if (mode == 9 && gameStart[0] != 'G')
	{
		printf("GAME START!!!!!!!!!!!!!!!!");
		sendToClient(messageToClient, hClntSock);

		recieveFromClient(hClntSock, messageFromClient);	// recieve data;
		printf("%s", messageFromClient);
	}
}

void Timer::update(int count) {
	glutTimerFunc(frameInterval, update, count + 1);

	if (count % framePerTurn == 0) {
		turn();
	}

	//if (mode > 0) printf("mode CHANGED!");

	// add any per frame actions
	// such as update() and draw() for all object

	if (mode == 0 && Key::keyCheckPressed('9'))
	{
		mode = 9;
		printf("mode- server chosn\n");
		makeServerSocket(&hServSock, &servAddr);
		printf("socket made");
		acceptClient(&hServSock, hClntSock, clntAddr, szClntAddr);
		recieveFromClient(hClntSock, messageFromClient);	// recieve spawn 

		char acceptMessage[2];
		acceptMessage[0] = 'G';
		acceptMessage[1] = '\0';
		sendToClient(acceptMessage, hClntSock);				// accept clients

		gameStart[0] = 'G';									// game started
	}

	if (mode == 0 && Key::keyCheckPressed('8'))
	{
		mode = 8;
		printf("mode- client chose\n");
		makeClientSocket(&hSocket, &servAddr);
		connectToServer(&servAddr, &hSocket);
	}// if not 1 or 2 give error message

	//if (characterSelection == 0 && Key::keyCheckPressed('a') && Key::keyCheckOn('s')) printf("hihi"); // sa
	
	if (mode > 0)
	{
		if (mode==8 && characterSelection == 0 && Key::keyCheckPressed('1'))
		{
			characterSelection = COMMAND_SPAWN_CSE;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}
		if (mode == 8 && characterSelection == 0 && Key::keyCheckPressed('2'))
		{
			characterSelection = COMMAND_SPAWN_PHYS;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}
		if (mode == 8 && characterSelection == 0 && Key::keyCheckPressed('3'))
		{
			characterSelection = COMMAND_SPAWN_LIFE;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}

		if (mode == 8 && characterSelection == 0 && Key::keyCheckPressed('4'))
		{
			characterSelection = COMMAND_SPAWN_ME;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}

		if (mode == 8 && characterSelection == 0 && Key::keyCheckPressed('5'))
		{
			characterSelection = COMMAND_SPAWN_CHEM;
			sendToServer(&hSocket, (char *)(to_string(characterSelection)).c_str());
			printf("character chosen : %d", characterSelection);
			getProtocolDataFromServer(&hSocket, gameStart);
			printf("GameStart!");
		}

		if (mode == 8 && characterSelection > 0 && gameStart[0] == 'G')
		{
			if (Key::keyCheckPressed('d')) command = COMMAND_MOVE_RIGHT;
			if (Key::keyCheckPressed('w')) command = COMMAND_MOVE_UP;
			if (Key::keyCheckPressed('a'))
			{
				command = COMMAND_MOVE_LEFT;
		
			}
			if (Key::keyCheckPressed('s')) command = COMMAND_MOVE_DOWN;

			if (Key::keyCheckPressed('d') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_RIGHT;
			if (Key::keyCheckPressed('w') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_UP;
			if (Key::keyCheckPressed('a') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_LEFT;
			if (Key::keyCheckPressed('s') && Key::keyCheckOn('j')) command = COMMAND_ATTACK_DOWN;

			if (Key::keyCheckPressed('d') && Key::keyCheckOn('k')) command = COMMAND_SKILL_RIGHT;
			if (Key::keyCheckPressed('w') && Key::keyCheckOn('k')) command = COMMAND_SKILL_UP;
			if (Key::keyCheckPressed('a') && Key::keyCheckOn('k')) command = COMMAND_SKILL_LEFT;
			if (Key::keyCheckPressed('s') && Key::keyCheckOn('k')) command = COMMAND_SKILL_DOWN;

			if (Key::keyCheckPressed('l')) command = COMMAND_FLAG;
		}
	}

	Key::keyUpdate();
}