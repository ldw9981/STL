#include "Engine.h"
#include "Map.h"
#include <stdlib.h>
#include <conio.h>

#include "Player.h"
#include "Goal.h"
#include "Monster.h"

Engine::Engine()
{
	bIsRunnging = true;
	map = new Map();
	player = new Player();
	goal = new Goal();
	monster = new Monster();
	InitGoalPosition();
	InitMonstersPosition();
}


Engine::~Engine()
{
	delete monster;
	delete goal;
	delete player;
	delete map;
}

void Engine::Run()
{
	while (bIsRunnging)
	{
		int KeyCode = Input();
		Process(KeyCode);
		Render();
	}
}

int Engine::Input()
{
	return _getch();
}

void Engine::Process(int KeyCode)
{
	//ĳ���� �̵�
	player->Move(map, KeyCode);
	monster->Move(map);

	//���� ���� ����
	IsComplete();
	IsFail();
	IsQuit(KeyCode);

}

void Engine::StopEngine()
{
	bIsRunnging = false;
}

void Engine::IsFail()
{
	if (player->X == monster->X && player->X == monster->Y)
	{
		StopEngine();
	}
}

void Engine::IsQuit(int KeyCode)
{
	if (KeyCode == 'q' || KeyCode == 'Q')
	{
		StopEngine();
	}
}

void Engine::IsComplete()
{
	if (player->X == goal->X && player->X == goal->Y)
	{
		//�¸�ȭ�� ���
		StopEngine();
	}
}


void Engine::Render()
{
	system("cls");

	map->Draw();

	goal->Draw();

	player->Draw();

	monster->Draw();
}

void Engine::InitGoalPosition()
{
	for (int y = 0; y < map->sizeY; ++y)
	{
		for (int x = 0; x < map->sizeX; ++x)
		{
			if (map->map[y][x] == '3')
			{
				goal->X = x;
				goal->Y = y;
			}
		}
	}
}

void Engine::InitMonstersPosition()
{
	//�ʿ� ���� �Ѹ��� 
	for (int y = 0; y < map->sizeY; ++y)
	{
		for (int x = 0; x < map->sizeX; ++x)
		{
			if (map->map[y][x] == '4')
			{
				monster->X = x;
				monster->Y = y;
			}
		}
	}
}