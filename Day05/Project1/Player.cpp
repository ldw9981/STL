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

	//�ܼ� ��� ���� ����(Player����)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
	std::cout << "P";
	//�ܼ� ��� ���� ����(�⺻ Į��� ����)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void Player::Init()
{
	Character::Init();

	X = 1;
	Y = 1;
	job = EJob::Farmer;
}
