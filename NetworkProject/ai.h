#pragma once

#include "protocol.h"
#include "network.h"

class Ai {
private:
	static protocol_team pivot;
public:
	static void setPivot(protocol_team t) { pivot = t; };
	static protocol_team getPivot() { return pivot; }
	static void aiInit(void);
	static void ai(protocol_data info);
	static void CharacterInit(int i, int x) {
		Network::setCharacterSelection(i % UNIT_PER_TEAM, (protocol_dep)x);
	};
	static void move(int i, protocol_direction x) {
		if (Network::getTeam() == pivot)
			Network::setCommand(i % UNIT_PER_TEAM, direction_to_movecommand(x));
		else
			Network::setCommand(i % UNIT_PER_TEAM, direction_to_movecommand(direction_mirror(x)));
	};
	static void attack(int i, protocol_direction x) {
		if (Network::getTeam() == pivot)
			Network::setCommand(i % UNIT_PER_TEAM, direction_to_attackcommand(x));
		else
			Network::setCommand(i % UNIT_PER_TEAM, direction_to_attackcommand(direction_mirror(x)));
	};
	static void skill(int i, protocol_direction x) {
		if (Network::getTeam() == pivot)
			Network::setCommand(i % UNIT_PER_TEAM, direction_to_skillcommand(x));
		else
			Network::setCommand(i % UNIT_PER_TEAM, direction_to_skillcommand(direction_mirror(x)));
	};
	static void spawn(int i, protocol_dep x) {
		Network::setCommand(i % UNIT_PER_TEAM, spawn_command(x));
	};
};