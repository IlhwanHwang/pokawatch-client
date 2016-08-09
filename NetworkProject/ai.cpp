#include "ai.h"

protocol_team Ai::pivot = TEAM_POSTECH;

void Ai::aiInit(void)
{
	// Initializing sequence
	// Example on POSTECH side
	setPivot(TEAM_POSTECH);

	CharacterInit(0, DEP_ME);
	CharacterInit(1, DEP_PHYS);
	CharacterInit(2, DEP_CHEM);
}

void Ai::ai(protocol_data info)
{
	print_data(info);

	// Implement your ai below
	// Example random ai in POSTECH side
	for (int i = 0; i < UNIT_PER_TEAM; i++) {
		if (info.unit[i].state == STATE_DEAD) {
			spawn(i, random_dep());
		}
		else if (info.unit[i].x < MAP_WIDTH / 2 - 2) {
			move(i, DIRECTION_RIGHT);
		}
		else if (info.unit[i].x >= MAP_WIDTH / 2 + 2) {
			move(i, DIRECTION_LEFT);
		}
		else {
			switch (rand() % 2) {
			case 0: attack(i, random_direction()); break;
			case 1: move(i, random_direction()); break;
			}
			
		}
	}
}