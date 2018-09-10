#include "Character.h"
#include <Windows.h>
#include <iostream>
#include "Map.h"

Character::Character()
{
	std::cout << "Character::Character()";
}


Character::~Character()
{
	std::cout << "Character::~Character()";
}

void Character::Move(Map * map, int KeyCode)
{

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
	std::cout << "Character::Init()";
}

void Character::Tick()
{
	std::cout << "Character::Tick()";
}

void Character::SetName(std::string name)
{
}

std::string Character::GetName()
{
	return std::string();
}
