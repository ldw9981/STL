#include <iostream>
#include <string>
#include <conio.h>
#include <stack>
#include <windows.h>
#include <synchapi.h>

using namespace std;
#define MAX 10

enum EMAZETYPE { PATH,WALL,VISITED,BACK};

int Maze[MAX][MAX] = {
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 1, 0, 1, 1, 0, 1},
	{1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 1, 1, 1, 1, 1},
	{1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 1, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
};

struct Position
{
	Position(int InitX = 0, int InitY = 0)
	{
		x = InitX;
		y = InitY;
	}

	int x;
	int y;
}; 

Position g_Start(1, 1);
Position g_Goal(8,8);



void PrintMaze()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			if (Maze[y][x]== WALL)
			{
				cout << "#";
			}
			else if (Maze[y][x] == PATH)
			{
				cout << " ";
			}
			else if (Maze[y][x] == VISITED)
			{
				cout << "V";
			}
			else if (Maze[y][x] == BACK)
			{
				cout << "B";
			}
			
		}
		cout << endl;
	}
}



bool IsGoal(const Position& Target)
{
	if (Target.x == g_Goal.x && Target.y == g_Goal.y)
		return true;

	return false;
}

bool CheckMazeType(const Position& Target,int type)
{
	return Maze[Target.y][Target.x] == type;
}


bool FindNextPath(Position& Current)
{
	Position Next = Position(Current.x, Current.y);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}

	Next = Position(Current.x, Current.y - 1);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = Position(Current.x + 1, Current.y);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = Position(Current.x, Current.y + 1);
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	Next = Position(Current.x - 1, Current.y);
	if (CheckMazeType(Next,PATH))
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
		if (CheckMazeType(Current,PATH))
		{
			Maze[Current.y][Current.x] = VISITED;		
			//PrintMaze(Current);
			BackPositions.push(Current);
			if (IsGoal(Current))
				break;
		}
		else if (!FindNextPath(Current))
		{
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
	Position Current = g_Start;
	stack<Position> BackPositions;
	COORD cdPos={0,0};

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	while (!IsGoal(Current))
	{
		if (_kbhit())
		{
			_getch();
	
			if (FindNextPath(Current))
			{
				Maze[Current.y][Current.x] = VISITED;
				BackPositions.push(Current);
			}
			else
			{
				if (!BackPositions.empty())
				{
					Current = BackPositions.top();
					Maze[Current.y][Current.x] = BACK;
					BackPositions.pop();
				}
				else
				{
					break;
				}
			}
		}
		SetConsoleCursorPosition(hConsole, cdPos);
		PrintMaze();
		Sleep(10);
	} 

	Sleep(3000);
	return 0;
}

