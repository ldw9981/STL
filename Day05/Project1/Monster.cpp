#include "Monster.h"
#include <stdlib.h>
#include "Map.h"
#include <Windows.h>
#include <iostream>



Monster::Monster()
{
}


Monster::~Monster()
{
}

void Monster::Move(class Map* map, int KeyCode)
{
	int dir = rand() % 4;
	switch (dir)
	{
		case 0:
			KeyCode = UP;
			break;
		case 1:
			KeyCode = DOWN;
			break;
		case 2:
			KeyCode = LEFT;
			break;
		case 3:
			KeyCode = RIGHT;
			break;
	}

	Character::Move(map, KeyCode);
}

void Monster::Draw()
{
	Character::Draw();

	//�ܼ� ��� ���� ����(Player����)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	std::cout << "M";
	//�ܼ� ��� ���� ����(�⺻ Į��� ����)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
