#include <iostream>
#include <string>
#include <conio.h>


using namespace std;
#define MAX 20

enum EDIRECTION { LEFT,RIGHT,UP,DOWN};
enum EMAZETYPE { PATH,WALL,VISITED,BACK};

int Maze[MAX][MAX] = {
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
	{0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
	{0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0},
	{0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
	{1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0},
};
void pause()
{
	while (_getch() != 0);
}


class Position
{
public:
	Position(int InitX = 0, int InitY = 0)
	{
		x = InitX;
		y = InitY;
	}	

	int x;
	int y;
};


void PrintMaze(Position& Current)
{
	system("cls");
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << Maze[y][x] ;
		}
		cout << endl;
	}
	cout << Current.x << ',' << Current.y << endl;
	pause();
}


bool IsValidPosition(Position& Target)
{
	if(Target.x < 0 || Target.x >= MAX || Target.y < 0 || Target.y >= MAX)
		return false;

	return true;
}

bool IsGoal(Position& Target)
{
	if (Target.x == (MAX-1) && Target.y == (MAX-1))
		return true;

	return false;
}



bool SearchMazeByStack(Position Current)
{
	if (!IsValidPosition(Current))
	{
		return false;
	}
	else if( Maze[Current.y][Current.x] != PATH )
	{
		return false;
	}	
	Maze[Current.y][Current.x] = VISITED;
	//PrintMaze(Current);
	if (IsGoal(Current))
	{
		return true;
	}
	
	//
	if (SearchMazeByStack(Position(Current.x, Current.y - 1)) ||
		SearchMazeByStack(Position(Current.x + 1, Current.y)) ||
		SearchMazeByStack(Position(Current.x, Current.y + 1)) ||
		SearchMazeByStack(Position(Current.x - 1, Current.y)))
	{
		return true;	// Goal이면 골이다
	}
	// 갈데가없으면
	Maze[Current.y][Current.x] = BACK;
	//PrintMaze(Current);
	return SearchMazeByStack(Current);
}

bool PrintExitByStack(Position Current)
{
	if (!IsValidPosition(Current))
	{
		return false;
	}
	else if (Maze[Current.y][Current.x] != VISITED)
	{
		return false;
	}
	
	cout << Current.x << ',' << Current.y << endl;
	if(IsGoal(Current))
		return true;

	if (!PrintExitByStack(Position(Current.x, Current.y - 1)))
	{
		if (!PrintExitByStack(Position(Current.x + 1, Current.y)))
		{
			if (!PrintExitByStack(Position(Current.x, Current.y + 1)))
			{
				PrintExitByStack(Position(Current.x - 1, Current.y));
			}
		}
	}
	return false;
}



int main()
{
	Position Start(0,0);
	PrintMaze(Start);

	bool resultGoal = SearchMazeByStack(Start);
	PrintMaze(Start);
	if (resultGoal)
	{
		cout << "탈출 성공" << endl;
	}
	else
	{
		cout << "탈출 실패" << endl;
	}
	PrintExitByStack(Start);
	pause();

	return 0;
}

