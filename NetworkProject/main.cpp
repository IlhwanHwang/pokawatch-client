//
// CNH 2016.04.30
// CSED353 Project
//

#include "network.h"
#include <iostream>

using namespace std;

void main(int argc, char **argv)
{
	if (argc == 1)
		cout << "no argment, please write server ip address and port number"<<endl;
	else if(argc == 2)
		Network::init(argv[1], "2222");
	else if(argc == 3)
		Network::init(argv[1], argv[2]);
	else
		cout << "wrong argument" << endl;

	Network::loop();
}