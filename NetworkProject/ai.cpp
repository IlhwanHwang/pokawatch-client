// ai.cpp ������ //
//���� �� character ����,
//������ unit�鿡 ���� command �Է� �����ϵ���,
//���� character ���ÿ� ���� �Է��� ���� ������ �Ʊ�� ���� character�� ���õǵ��� �ۼ�
//�ؼ� �ᱹ������ ai.cpp�� ���� ����� �˰��� �Է��ϸ� �� �� �ֵ��� �ۼ�
//���� ai�� random�ϰ� �ۼ��� �� �ֵ��� �ۼ�
//random character ����, random command�� �����̵��� �ۼ�

#include <string.h>
#include "ai.h"
#include "network.h"
#include <cstdlib>
#include <time.h>

int Network::team;

void Ai::aiInit(void)
{
	srand(time(NULL));
	Ai::CharacterInit(0,DEP_PHYS);
	Ai::CharacterInit(1, DEP_CHEM);
	Ai::CharacterInit(2, DEP_CSE);
}


void Ai::ai(void)
{
	switch (rand() % 3)
	{
	case 0:
		attack(0, rand() % 4);
		move(1, rand() % 4);
		attack(2, rand() % 4);

		break;

	case 1:
		move(0, rand() % 4);
		attack(1, rand() % 4);
		attack(2, rand() % 4);

		break;

	case 2:
		attack(0, rand() % 4);
		attack(1, rand() % 4);
		move(2, rand() % 4);

		break;

	}


}

void Ai::CharacterInit(int i, int x)
{
	Network::setCharacterSelection(i,(protocol_dep)x);
}

void Ai::move(int i, int x)
{
	if (Network::getTeam() == TEAM_POSTECH)
	{
		if(x == 0)	   Network::setCommand(i, COMMAND_MOVE_RIGHT);
		else if (x == 1) Network::setCommand(i, COMMAND_MOVE_LEFT);
		else if (x == 2) Network::setCommand(i, COMMAND_MOVE_UP);
		else if (x == 3) Network::setCommand(i, COMMAND_MOVE_DOWN);
	}
	else
	{
		if (x == 0)	   Network::setCommand(i, COMMAND_MOVE_LEFT);
		else if (x == 1) Network::setCommand(i, COMMAND_MOVE_RIGHT);
		else if (x == 2) Network::setCommand(i, COMMAND_MOVE_UP);
		else if (x == 3) Network::setCommand(i, COMMAND_MOVE_DOWN);
	}
}

void Ai::attack(int i, int x)
{
	if (Network::getTeam() == TEAM_POSTECH)
	{
		if (x == 0)	   Network::setCommand(i, COMMAND_ATTACK_RIGHT);
		else if (x == 1) Network::setCommand(i, COMMAND_ATTACK_LEFT);
		else if (x == 2) Network::setCommand(i, COMMAND_ATTACK_UP);
		else if (x == 3) Network::setCommand(i, COMMAND_ATTACK_DOWN);
	}
	else
	{
		if (x == 0)	   Network::setCommand(i , COMMAND_ATTACK_LEFT);
		else if (x == 1) Network::setCommand(i , COMMAND_ATTACK_RIGHT);
		else if (x == 2) Network::setCommand(i , COMMAND_ATTACK_UP);
		else if (x == 3) Network::setCommand(i , COMMAND_ATTACK_DOWN);
	}
}

void Ai::skill(int i, char x)
{
	if (Network::getTeam() == TEAM_POSTECH)
	{
		if (x == 'r')	   Network::setCommand(i, COMMAND_SKILL_RIGHT);
		else if (x == 'l') Network::setCommand(i, COMMAND_SKILL_LEFT);
		else if (x == 'u') Network::setCommand(i, COMMAND_SKILL_UP);
		else if (x == 'd') Network::setCommand(i, COMMAND_SKILL_DOWN);
	}
	else
	{
		if (x == 'r')	   Network::setCommand(i , COMMAND_SKILL_LEFT);
		else if (x == 'l') Network::setCommand(i , COMMAND_SKILL_RIGHT);
		else if (x == 'u') Network::setCommand(i , COMMAND_SKILL_UP);
		else if (x == 'd') Network::setCommand(i , COMMAND_SKILL_DOWN);
	}
}

void Ai::flag(int i)
{
	if (Network::getTeam() == TEAM_POSTECH)
	{
		Network::setCommand(i, COMMAND_FLAG);
	}
	else
	{
		Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_FLAG);
	}
}