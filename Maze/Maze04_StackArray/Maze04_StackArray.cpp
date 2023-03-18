// Maze04_StackArray.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

using namespace std;

#define MAX_POSITION_STACK_SIZE 50
#define MAX 10

enum EMAZETYPE { PATH, WALL, VISITED, BACK };



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
	Position(int InitX = -1, int InitY = -1)
	{
		x = InitX;
		y = InitY;
	}
	int x;
	int y;		
};

Position g_Start(1, 1);
Position g_Goal(8, 8);




struct PositionStack
{
	Position arrPosition[MAX_POSITION_STACK_SIZE];

	int TopIndex;

	PositionStack()
	{
		TopIndex=-1;
	}

	void Push(Position data)
	{
		if (IsFull())
		{
			std::cout << "Stack is full\n";
			return;
		}

		TopIndex++;
		arrPosition[TopIndex]= data;
	}

	Position Pop()
	{
		Position retValue;
		if (IsEmpty())
		{
			std::cout << "Stack is empty\n";
			return retValue;
		}
		
		retValue = arrPosition[TopIndex];
		TopIndex--;
		return retValue;
	}

	bool IsEmpty()
	{
		return TopIndex == -1;
	}

	bool IsFull()
	{
		return TopIndex + 1 >= MAX_POSITION_STACK_SIZE;
	}
};


void PrintMaze()
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			int type = Maze[y][x];
			if (type == WALL)
			{
				cout << "#";
			}
			else if (type == PATH)
			{
				cout << " ";
			}
			else if (type == VISITED)
			{
				cout << "V";
			}
			else if (type == BACK)
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

bool CheckMazeType(const Position& Target, int type)
{
	return Maze[Target.y][Target.x] == type;
}


bool FindNextPath(Position& Current)
{
	Position Next;
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
	if (CheckMazeType(Next, PATH))
	{
		Current = Next;
		return true;
	}
	return false;
}



int main()
{

	PositionStack BackPositions;
	Position Current = g_Start;
	Maze[Current.y][Current.x] = VISITED;
	BackPositions.Push(Current);
	

	while (!IsGoal(Current))
	{

		if (FindNextPath(Current))
		{
			Maze[Current.y][Current.x] = VISITED;
			BackPositions.Push(Current);
		}
		else
		{
			if (!BackPositions.IsEmpty())
			{
				Current = BackPositions.Pop();
				Maze[Current.y][Current.x] = BACK;

			}
			else
			{
				cout << "No way more.\n";
				break;
			}
		}
	}
	PrintMaze();
}

// 프로그램 실행: <Ctrl+F5> 또는 [디버그] > [디버깅하지 않고 시작] 메뉴
// 프로그램 디버그: <F5> 키 또는 [디버그] > [디버깅 시작] 메뉴

// 시작을 위한 팁: 
//   1. [솔루션 탐색기] 창을 사용하여 파일을 추가/관리합니다.
//   2. [팀 탐색기] 창을 사용하여 소스 제어에 연결합니다.
//   3. [출력] 창을 사용하여 빌드 출력 및 기타 메시지를 확인합니다.
//   4. [오류 목록] 창을 사용하여 오류를 봅니다.
//   5. [프로젝트] > [새 항목 추가]로 이동하여 새 코드 파일을 만들거나, [프로젝트] > [기존 항목 추가]로 이동하여 기존 코드 파일을 프로젝트에 추가합니다.
//   6. 나중에 이 프로젝트를 다시 열려면 [파일] > [열기] > [프로젝트]로 이동하고 .sln 파일을 선택합니다.
