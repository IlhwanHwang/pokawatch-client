//
// CNH 2016.04.30
// CSED353 Project
//

#include "network.h"

void main(int argc, char **argv)
{
	Network::init(argv[1]);
	Network::loop();
}