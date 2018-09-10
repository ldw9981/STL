#include "Player.h"
#include <iostream>
#include <Windows.h>
#include "Map.h"



Player::Player()
{
	std::cout << "Player::Player()";
	Init();
}


Player::~Player()
{
	std::cout << "Player::~Player()";
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


void Player::Move(Map * map, int KeyCode)
{
	if (KeyCode == UP)
	{
		//���� ��ĭ ���� ���� ������ Ȯ��
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





void Player::Tick()
{
	std::cout << "Player::Tick()";
}

void Player::Skill1()
{
}

void Player::Input()
{
}
