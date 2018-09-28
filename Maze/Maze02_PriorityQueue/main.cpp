#include <iostream>
#include <string>
#include <conio.h>
#include <queue>
#include <functional>

#define MAX 10

enum EDIRECTION { START=0,LEFT_UP = START, RIGHT_UP, LEFT_DOWN, RIGHT_DOWN, LEFT, RIGHT, UP, DOWN , END = DOWN};
enum EMAZETYPE { PATH, WALL, VISITED, BACK, EXIT };



using namespace std;

int Maze[MAX][MAX] = {
	{0, 1, 1, 0, 0, 0, 1, 1, 1, 0},
	{0, 0, 0, 1, 0, 0, 1, 1, 0, 0},
	{1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
	{1, 0, 1, 0, 1, 1, 1, 0, 1, 0},
	{1, 0, 1, 0, 0, 0, 0, 1, 1, 0},
	{0, 0, 1, 0, 0, 1, 0, 0, 1, 0},
	{0, 0, 1, 0, 0, 1, 0, 1, 1, 0},
	{1, 0, 1, 0, 1, 1, 1, 1, 1, 0},
	{1, 0, 0, 0, 1, 1, 0, 0, 0, 0},
	{1, 0, 0, 0, 0, 0, 1, 1, 1, 0}
};
void pause()
{
	while (_getch() != 0);
}


class Position
{
public:
	Position(int InitX = 0, int InitY = 0,int InitData=0)
	{
		x = InitX;
		y = InitY;
		data = InitData;
	}

	int x;
	int y;

	int data;

	static bool Less(const Position& a,const Position& b) {
		return a.data < b.data;
	}
	static bool Greater(const Position& a, const Position& b) {
		return a.data > b.data;
	}

};

typedef priority_queue<Position, vector<Position>, std::function<bool(Position, Position)>> MazeQueue;

Position g_Start(0, 0,0);
Position g_Goal(MAX-1,MAX-1,0);
Position g_Result;



void PrintMaze(const Position& OutPosition)
{
	system("cls");
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << Maze[y][x] << '\t';
		}
		cout << endl;
	}
	cout << OutPosition.x << ',' << OutPosition.y << endl;

	pause();
}


bool IsValidPosition(const Position& Target)
{
	if (Target.x < 0 || Target.x >= MAX || Target.y < 0 || Target.y >= MAX)
		return false;

	return true;
}

bool IsGoal(const Position& Target)
{
	if (Target.x == g_Goal.x && Target.y == g_Goal.y)
		return true;

	return false;
}

void SetMazeValue(const Position& Target, int SetValue)
{

	Maze[Target.y][Target.x] = SetValue;
}
int GetMazeValue(const Position& Target)
{
	return Maze[Target.y][Target.x];
}

bool CheckMazeValue(const Position& Target, int CheckValue)
{
	if (Maze[Target.y][Target.x] == CheckValue)
		return true;

	return false;
}




Position GetDirectionPosition(const Position& Current,int Direction)
{
	Position Next = Current;
	if (Direction == LEFT)
	{
		Next.x -= 1;
	}
	else if (Direction == RIGHT)
	{
		Next.x += 1;
	}
	else if (Direction == UP)
	{
		Next.y -= 1;
	}
	else if (Direction == DOWN)
	{
		Next.y += 1;
	}
	else if (Direction == LEFT_UP)
	{
		Next.x -= 1;
		Next.y -= 1;
	}
	else if (Direction == LEFT_DOWN)
	{
		Next.x -= 1;
		Next.y += 1;
	}
	else if (Direction == RIGHT_UP)
	{
		Next.x += 1;
		Next.y -= 1;
	}
	else if (Direction == RIGHT_DOWN)
	{
		Next.x += 1;
		Next.y += 1;
	}
	return Next;
}



bool StepTo(MazeQueue& BFSPositions,Position& CurrentPosition)
{
	int CurrentLevel = abs(GetMazeValue(CurrentPosition));
	SetMazeValue(CurrentPosition, CurrentLevel);
	PrintMaze(CurrentPosition);
	if (IsGoal(CurrentPosition))
	{		
		cout << "Goal" << endl;
		pause();
		return true;
	}
	
	for (int dir = START; dir <= END; dir++)
	{
		Position DirectionPosition = GetDirectionPosition(CurrentPosition, dir);
		if (!IsValidPosition(DirectionPosition))
			continue;

		int setValue = 0;
		int mazeValue = GetMazeValue(DirectionPosition);
		if (CheckMazeValue(DirectionPosition, PATH))
		{
			int newLevel = abs(CurrentLevel);
			if ((dir == LEFT) || (dir == RIGHT) || (dir == UP) || (dir == DOWN))
			{
				newLevel += 10;
			}
			else if ((dir == LEFT_UP) || (dir == LEFT_DOWN) || (dir == RIGHT_UP) || (dir == RIGHT_DOWN))
			{
				newLevel += 14;
			}
			newLevel *= -1;

			SetMazeValue(DirectionPosition, newLevel);
			DirectionPosition.data = newLevel;
			BFSPositions.push(DirectionPosition);			
			PrintMaze(CurrentPosition);
		}
		else if (mazeValue < 0)	// 0���� ������ �̹� ���� �����̴�.
		{
			int newLevel = abs(CurrentLevel);
			if ((dir == LEFT) || (dir == RIGHT) || (dir == UP) || (dir == DOWN))
			{
				newLevel += 10;
			}
			else if ((dir == LEFT_UP) || (dir == LEFT_DOWN) || (dir == RIGHT_UP) || (dir == RIGHT_DOWN))
			{
				newLevel += 14;
			}
			newLevel *= -1;

			// �̹� ������ �ִ°��� ť�� �����Ƿ� �����ʰ� ���������� ������Ʈ�Ѵ�..
			if (abs(mazeValue) > abs(newLevel))
			{
				SetMazeValue(DirectionPosition, newLevel);
				
				PrintMaze(CurrentPosition);
			}
		}
		else
		{
			continue;
		}	
	}

	while (!BFSPositions.empty())
	{

		CurrentPosition = BFSPositions.top();
		BFSPositions.pop();
		
		if (StepTo(BFSPositions, CurrentPosition))
			return true;
	}
	return false;
}


bool SearchMazeQueue(Position CurrentPosition)
{
	MazeQueue BFSPositions(Position::Greater); // BreadthFirstSearch
	int Level = -10;
	SetMazeValue(CurrentPosition, Level);
	StepTo(BFSPositions, CurrentPosition);
	PrintMaze(CurrentPosition);
	return true;
}


int main()
{
	SearchMazeQueue(g_Start);		
	/*
	priority_queue<Position,vector<Position>, std::function<bool(Position, Position)>> a(Position::Greater);
	a.push(Position(0,0,30));
	a.push(Position(0, 0, 20));
	a.push(Position(0, 0, 10));
	a.push(Position(0, 0, -30));

	a.pop();
	int b = a.top().data;
	*/
	return 0;
}

