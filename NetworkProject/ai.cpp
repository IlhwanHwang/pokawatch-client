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

int Network::team;

void Ai::aiInit(void)
{
	Ai::CharacterInit(0,DEP_CSE);
	Ai::CharacterInit(1, DEP_CSE);
	Ai::CharacterInit(2, DEP_CSE);
}


void Ai::ai(void)
{

}

void Ai::CharacterInit(int i, int x)
{
	Network::setCharacterSelection(i,(protocol_dep)x);
}

void Ai::move(int i, char x)
{
	if (Network::getTeam() == TEAM_POSTECH)
	{
		if(x == 'r')	   Network::setCommand(i, COMMAND_MOVE_RIGHT);
		else if (x == 'l') Network::setCommand(i, COMMAND_MOVE_LEFT);
		else if (x == 'u') Network::setCommand(i, COMMAND_MOVE_UP);
		else if (x == 'd') Network::setCommand(i, COMMAND_MOVE_DOWN);
	}
	else
	{
		if (x == 'r')	   Network::setCommand(i + (UNIT_NUM_MAX)/2, COMMAND_MOVE_LEFT);
		else if (x == 'l') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_MOVE_RIGHT);
		else if (x == 'u') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_MOVE_UP);
		else if (x == 'd') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_MOVE_DOWN);
	}
}

void Ai::attack(int i, char x)
{
	if (Network::getTeam() == TEAM_POSTECH)
	{
		if (x == 'r')	   Network::setCommand(i, COMMAND_ATTACK_RIGHT);
		else if (x == 'l') Network::setCommand(i, COMMAND_ATTACK_LEFT);
		else if (x == 'u') Network::setCommand(i, COMMAND_ATTACK_UP);
		else if (x == 'd') Network::setCommand(i, COMMAND_ATTACK_DOWN);
	}
	else
	{
		if (x == 'r')	   Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_ATTACK_LEFT);
		else if (x == 'l') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_ATTACK_RIGHT);
		else if (x == 'u') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_ATTACK_UP);
		else if (x == 'd') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_ATTACK_DOWN);
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
		if (x == 'r')	   Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_SKILL_LEFT);
		else if (x == 'l') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_SKILL_RIGHT);
		else if (x == 'u') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_SKILL_UP);
		else if (x == 'd') Network::setCommand(i + (UNIT_NUM_MAX) / 2, COMMAND_SKILL_DOWN);
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