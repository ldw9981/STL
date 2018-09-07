#include "Character.h"
#include <Windows.h>
#include <iostream>
#include "Map.h"

Character::Character()
{
}


Character::~Character()
{
}

void Character::Move(Map * map, int KeyCode)
{
	if (KeyCode == UP)
	{
		//위로 한칸 먼저 가서 벽인지 확인
		if (!map->IsWall(X, Y - 1))
		{
			Y--;
		}
	}
	else if (KeyCode == DOWN)
	{
		if (!map->IsWall(X, Y + 1))
		{
			Y++;
		}
	}
	else if (KeyCode == LEFT)
	{
		if (!map->IsWall(X - 1, Y))
		{
			X--;
		}
	}
	else if (KeyCode == RIGHT)
	{
		if (!map->IsWall(X + 1, Y))
		{
			X++;
		}
	}
}

void Character::Draw()
{
	//커서위치를 플레이어 위치로 변경
	COORD c;
	c.X = X;
	c.Y = Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void Character::Init()
{
}