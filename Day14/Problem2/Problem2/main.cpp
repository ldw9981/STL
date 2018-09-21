#include <iostream>
#include <string>
#include <conio.h>
#include <stack>

using namespace std;
#define MAX 5

enum EDIRECTION { LEFT,RIGHT,UP,DOWN};
enum EMAZETYPE { PATH,WALL,VISITED,BACK,EXIT};

int Maze[MAX][MAX] = {
	{0, 1, 1, 1, 0},
	{0, 0, 0, 0, 0},
	{1, 0, 1, 1, 1},
	{1, 0, 1, 1, 1},
	{1, 0, 0, 0, 0}
};
void pause()
{
	while (_getch() != 0);
}


class Position
{
public:
	Position(int InitX=0, int InitY=0)
	{
		x = InitX;
		y = InitY;
	}	

	int x;
	int y;
};
Position g_Start(0, 0);
Position g_Goal(MAX-1,MAX-1);
Position g_Result;



void PrintMaze(const Position& OutPosition)
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
	cout << OutPosition.x << ',' << OutPosition.y << endl;

	pause();
}


bool IsValidPosition(const Position& Target)
{
	if(Target.x < 0 || Target.x >= MAX || Target.y < 0 || Target.y >= MAX)
		return false;

	return true;
}

bool IsGoal(Position& Target)
{
	if (Target.x == g_Goal.x && Target.y == g_Goal.y)
		return true;

	return false;
}

bool CheckMazeType(const Position& Target,int type)
{
	if (Maze[Target.y][Target.x] == type)
		return true;

	return false;
}


bool MoveNextPath(Position& Current)
{
	Position Next = Position(Current.x, Current.y - 1);
	if (IsValidPosition(Next) && CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = Position(Current.x + 1, Current.y);
	if (IsValidPosition(Next) && CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = Position(Current.x, Current.y + 1);
	if (IsValidPosition(Next) && CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = Position(Current.x - 1, Current.y);
	if (IsValidPosition(Next) && CheckMazeType(Next,PATH))
	{
		Current = Next;
		return true;
	}
	return false;
}

bool SearchMazeStack(Position Start)
{	
	Position Current = Start;	
	stack<Position> BackPositions;
	while (true)
	{	
		if (IsValidPosition(Current) && CheckMazeType(Current,PATH))
		{
			Maze[Current.y][Current.x] = VISITED;		
			//PrintMaze(Current);
			BackPositions.push(Current);
			if (IsGoal(Current))
				break;
		}

		if (MoveNextPath(Current))
		{
			continue;
		}
		
		if (!BackPositions.empty())
		{
			Maze[Current.y][Current.x] = BACK;
			//PrintMaze(Current);
			Current = BackPositions.top();
			BackPositions.pop();
		}
		else
		{
			break;
		}			
	}

	if (!BackPositions.empty())
	{
		auto container = BackPositions._Get_container();
		for (auto iter = container.begin(); iter != container.end(); ++iter)
		{
			cout << "(" << (*iter).x << "," << (*iter).y << ")" << endl;
		}
		return true;
	}
	return false;
}


int main()
{	
	SearchMazeStack(g_Start);
	pause();
	return 0;
}

