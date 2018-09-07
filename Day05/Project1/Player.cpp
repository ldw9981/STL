#include "Player.h"
#include <iostream>
#include <Windows.h>
#include "Map.h"



Player::Player()
{
	Init();
}


Player::~Player()
{
}


void Player::Draw()
{
	Character::Draw();

	//콘솔 출력 색상 변경(Player색깔)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	std::cout << "P";
	//콘솔 출력 색상 변경(기본 칼라로 변경)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void Player::Init()
{
	Character::Init();

	X = 1;
	Y = 1;
	job = EJob::Farmer;
}
