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

	//콘솔 출력 색상 변경(Player색깔)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
	std::cout << "M";
	//콘솔 출력 색상 변경(기본 칼라로 변경)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
