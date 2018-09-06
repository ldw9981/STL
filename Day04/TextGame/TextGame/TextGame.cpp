// TextGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define KEY_UP 0x48
#define KEY_DOWN 0x50
#define KEY_LEFT 0x4B
#define KEY_RIGHT 0x4D

#define MAXSIZE_WIDTH 10
#define MAXSIZE_HEIGHT 10
bool bIsRunning = true;

typedef struct tagPosition
{
	int x;
	int y;
}Position;



int map[MAXSIZE_HEIGHT][MAXSIZE_HEIGHT] = {

	{1,1,1,1,1,1,1,1,1,1},
	{1,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,1,1,1,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,1,0,0,0,1},
	{1,0,0,0,0,0,0,0,0,1},
	{1,0,0,0,0,0,0,0,3,1},
	{1,1,1,1,1,1,1,1,1,1}
};

Position PlayerPosition;




int Input()
{
	return _getch();
}

void Rendering()
{
	system("cls");
	for (int y = 0; y < MAXSIZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAXSIZE_WIDTH; x++)
		{
			if ( map[y][x] == 1)
			{
				printf("1");
			}
			else if (x == PlayerPosition.x && y == PlayerPosition.y)
			{
				printf("P");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

bool IsWall(int x, int y)
{
	if (map[y][x] == 1)
		return true;

	return false;
}
bool IsEnd(int x, int y)
{
	if (map[y][x] == 3)
		return true;

	return false;
}

void Process(int key)
{
	if (key == 'q' || key == 'Q')
	{
		bIsRunning = false;
	}
	else if (key == KEY_UP)
	{
		if(!IsWall(PlayerPosition.x, PlayerPosition.y-1))
			PlayerPosition.y--;
	}
	else if (key == KEY_DOWN)
	{
		if (!IsWall(PlayerPosition.x, PlayerPosition.y+1))
			PlayerPosition.y++;
	}
	else if (key == KEY_LEFT)
	{
		if (!IsWall(PlayerPosition.x-1, PlayerPosition.y))
			PlayerPosition.x--;
	}
	else if (key == KEY_RIGHT)
	{
		if (!IsWall(PlayerPosition.x+1, PlayerPosition.y))
			PlayerPosition.x++;
	}

	if (IsEnd(PlayerPosition.x, PlayerPosition.y))
	{
		bIsRunning = false;
	}

}

int main()
{
	PlayerPosition.x = 1;
	PlayerPosition.y = 1;


	while (bIsRunning)
	{
		Rendering();
		int key = Input();
		Process(key);
		

	}
}

