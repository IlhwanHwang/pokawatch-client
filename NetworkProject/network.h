//
// CNH 2016.05.19
// network module
//

#pragma once

#include <WinSock2.h>
#include "protocol.h"
#include <string>

class Network {																// EVERY THING RELATED TO NETWORK OPERATION IS IN HERE
private:
	static SOCKET hSocket;													// Client socket variable of client side
	static SOCKADDR_IN servAddr;											// Server address variable of server/client side
	static char messageToClient[MESSAGE_T0_CLIENT_SIZE];					// Message buffer of server side
	static int mode;														// determine server/ client/ nothing
	static int characterSelection[UNIT_NUM_MAX];											// Information of selection of charactor(dep)
	static char gameStart[3];												// game started? [0] : not(N) start(G), [1] : team_information
	static protocol_command command[(UNIT_NUM_MAX)/2];														// selected command of client side
	static char *serverIpArg;
	static protocol_team team;														// 0 : TEAM_NULL, 1: TEAM_POSTECH, 2: TEAM_KAIST

public:
	static void init(char * argv, char * port);								// initiate network state
	static void loop();
	static void ErrorHandling(char *message);		// Error handling
	static void makeClientSocket(char * port);					// Client socket making routine
	static void getProtocolDataFromServer();		// Client message recieving routine
	static void recieveGameStart();					// Client message recieving for game start
	static void sendToServer(char message[]);		// Client message sending routine
	
	static int getCommand(int i) { return command[i]; }
	static int getCharacterSelection(int i) { return characterSelection[i]; }
	static char *getGameStart() { return gameStart; }
	static protocol_team getTeam() { return team; }

	static void setMode(int x) { mode = x; }
	static void setCommand(int i, protocol_command x) { command[i] = x; }
	static void setCommandbyIndex(int i, int x) { command[i] = (protocol_command)x; }
	static void setCharacterSelection(int i, int x) { characterSelection[i] = x; }
	static void setTeam(protocol_team x) { team = x; }
	static void setTeambyIndex(int x) { switch (x) { case 0: team = TEAM_POSTECH; break; case 1: team = TEAM_KAIST; break; default: team = TEAM_NULL; } }
	
};