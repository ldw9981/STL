#include <iostream>
#include <conio.h>
#include <stack>
#include <list>
#include <queue>
using namespace std;

#define MAX 10
enum EDIR { UP=0,RIGHT,DOWN,LEFT} ;

int Maze[MAX][MAX] = {
	{0,0,1,1,0,0,0,1,1,0},
	{1,0,0,0,0,0,0,1,1,0},
	{1,0,1,1,0,0,0,1,1,0},
	{1,0,1,1,1,0,0,1,1,0},
	{1,0,0,0,0,0,0,1,1,0},
	{0,0,1,1,0,0,0,1,1,0},
	{1,0,0,0,0,0,0,1,1,0},
	{1,0,1,1,0,0,1,1,1,0},
	{1,0,1,1,1,0,0,0,0,0},
	{1,0,0,0,0,0,0,1,1,0}
};

#define PATH 0
#define WALL 1
#define VISITED 2
#define BACKTRACE 3

class Position
{
public:
	Position(int NewX=0, int NewY=0)
	{
		x = NewX;
		y = NewY;
	}
	int x;
	int y;
};

Position Dir[4];

void PrintMap()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << Maze[y][x] << " ";
		}
		cout << endl;

	}	
	_getch();
	system("cls");
}

void PrintEscape()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << Maze[y][x] << " ";
		}
		cout << endl;

	}
	_getch();
	system("cls");
}


bool CanMove(Position& Current)
{
	Position Next;
	//up
	Next.x = Current.x;
	Next.y = Current.y - 1;
	if (Next.y > -1 && Maze[Next.x][Next.y] == PATH)
	{
		Current = Next;
		return true;
	}

	//right
	Next.x = Current.x +1;
	Next.y = Current.y ;
	if (Next.x < MAX && Maze[Next.x][Next.y] == PATH)
	{
		Current = Next;
		return true;
	}

	//down
	Next.x = Current.x;
	Next.y = Current.y+1;
	if (Next.y < MAX && Maze[Next.x][Next.y] == PATH)
	{
		Current = Next;
		return true;
	}

	//left
	Next.x = Current.x -1;
	Next.y = Current.y;
	if (Next.x > -1 && Maze[Next.x][Next.y] == PATH)
	{
		Current = Next;
		return true;
	}
	return false;
}

bool CanMove2(Position& Current)
{
	Position Next ;
	for ( int dir = EDIR::UP;  dir <= EDIR::LEFT ; dir++)
	{
		Next.x = Current.x + Dir[dir].x;
		Next.y = Current.y + Dir[dir].y;

		if (Next.x < 0 || Next.y < 0 || Next.x >= MAX || Next.y >= MAX)
		{//이동불가
			continue;
		}

		if (Maze[Next.y][Next.x] == PATH)
		{
			Current = Next;
			return true;
		}
	}
	return false;
}

void Process1()
{
	//up
	Dir[EDIR::UP].x = 0;
	Dir[EDIR::UP].y = -1;
	//right
	Dir[EDIR::RIGHT].x = 1;
	Dir[EDIR::RIGHT].y = 0;
	//down
	Dir[EDIR::DOWN].x = 0;
	Dir[EDIR::DOWN].y = 1;
	//left
	Dir[EDIR::LEFT].x = -1;
	Dir[EDIR::LEFT].y = 0;

	Position Current;
	Current.x = 0;
	Current.y = 0;

	stack<Position> EscapeMap;
	while (true)
	{
		Maze[Current.y][Current.x] = VISITED;

		if (CanMove(Current))
		{
			EscapeMap.push(Current);
		}
		else
		{
			// 현재는 이동불가
			// 
			if (!EscapeMap.empty())	// 되돌아갈길 있나?
			{
				Position temp = EscapeMap.top();
				EscapeMap.pop();
				Maze[Current.y][Current.x] = BACKTRACE;
			}
			else
			{
				cout << "탈출 불가" << endl;
				break;
			}

			if (Current.x == (MAX - 1) && Current.y == (MAX - 1))
			{
				cout << "탈출 성공" << endl;
				break;
			}
		}
		PrintMap();
	}

	while (!EscapeMap.empty())
	{
		Position Temp = EscapeMap.top();

		cout << "(" << Temp.x << "," << Temp.y << ")" << endl;

		EscapeMap.pop();
	}
	_getch();
}

void MazeMove(Position& Current, list<Position>& outList)
{	
	Maze[Current.y][Current.x] = VISITED;
	if (Current.x == (MAX - 1) && Current.y == (MAX - 1))
	{
		cout << "탈출 성공" << endl;
		return;
	}

	Position Next;
	PrintMap();
	for (int dir = EDIR::UP; dir <= EDIR::LEFT; dir++)
	{
		Next.x = Current.x + Dir[dir].x;
		Next.y = Current.y + Dir[dir].y;

		if (Next.x < 0 || Next.y < 0 || Next.x >= MAX || Next.y >= MAX)
		{
			continue;
		}

		if (Maze[Next.y][Next.x] == PATH)
		{			
			MazeMove(Next, outList);
		}
	}
	Maze[Current.y][Current.x] = BACKTRACE;
}

bool MazeMove2(Position Current)
{
	if (Current.x < 0 || Current.y < 0 || Current.x >= MAX || Current.y >= MAX)
	{
		return false;
	}
	else if (Maze[Current.y][Current.x] != PATH)
	{
		return false;
	}
	else if (Current.x == (MAX - 1) && Current.y == (MAX - 1))
	{
		Maze[Current.y][Current.x] = VISITED;
		return true;
	}
	else 
	{
		Maze[Current.y][Current.x] = VISITED;
		if (MazeMove2(Position(Current.x, Current.y - 1)) ||
			MazeMove2(Position(Current.x+1, Current.y)) ||
			MazeMove2(Position(Current.x, Current.y + 1)) ||
			MazeMove2(Position(Current.x-1, Current.y)) )
		{
			return true;
		}
		Maze[Current.y][Current.x] = BACKTRACE;
		return false;
	}
}

void ProcessMaze(Position Current)
{	
	MazeMove2(Current);
}

void InitDir()
{
	//up
	Dir[EDIR::UP].x = 0;
	Dir[EDIR::UP].y = -1;
	//right
	Dir[EDIR::RIGHT].x = 1;
	Dir[EDIR::RIGHT].y = 0;
	//down
	Dir[EDIR::DOWN].x = 0;
	Dir[EDIR::DOWN].y = 1;
	//left
	Dir[EDIR::LEFT].x = -1;
	Dir[EDIR::LEFT].y = 0;
}

bool CanMoveQ(Position& Current, Position& NewPosition, int dir)
{
	Position Next;

	for (int i = UP; i <= LEFT; ++i)
	{
		Next.x = Current.x + Dir[i].x;
		Next.y = Current.y + Dir[i].y;

		if (Next.x < 0 || Next.y < 0 ||
			Next.x > MAX || Next.y > MAX) // 배열 영역 밖을 벗어나면 이동 불가
		{
			continue;
		}

		if (Maze[Next.y][Next.x] == PATH)
		{
			Current = Next;
			return true;
		}
	}

	return false;

}

int main()
{
	InitDir();

	Position Current;

	queue<Position> Q;
	InitDir();
	Position Current;


	Q.push(Current);
	Maze[Current.y][Current.x] = 10;
	bool found = false;
	while (!Q.empty())
	{
		Current = Q.front();
		Q.pop();
		int Level = Maze[Current.y][Current.x];
		for (int dir = UP; dir <= LEFT ; dir++)
		{

		}
	}



	return 0;
}