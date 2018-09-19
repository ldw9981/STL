#include <iostream>
#include <conio.h>
#include <stack>
#include <list>
#include <queue>
#include <Windows.h>


using namespace std;

#define MAX 10
enum EDIRECTION { UP=0,RIGHT,DOWN,LEFT} ;
enum EMAZE { PATH=0,WALL,VISITED,BACKTRACE};


void PutPixel(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
	printf("#");
}

int ArrayMaze[MAX][MAX] = {
	{0,0,1,1,0,0,0,1,1,0},
	{1,0,0,1,0,0,0,1,1,0},
	{1,0,1,1,0,0,0,1,1,0},
	{1,0,1,1,1,0,0,1,1,0},
	{1,0,0,0,0,0,0,1,1,0},
	{0,0,1,1,0,0,0,1,1,0},
	{1,0,0,0,0,0,0,1,1,0},
	{1,0,1,1,0,0,1,1,1,0},
	{1,0,1,1,1,0,0,0,0,0},
	{1,0,0,0,0,0,0,1,1,0}
};


int ArrayMazeCount[MAX][MAX] = {
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,1,0,0,0,0,0},
	{0,0,0,0,1,1,1,0,0,0},
	{0,0,0,0,1,1,1,1,0,0},
	{0,0,0,0,1,1,1,1,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,1,1,0,0,0,0,0,0,0},
	{0,1,1,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0}
};

int ArrayMazeFill[MAX][MAX] = {
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,1,1,1,0,0,0,0,0},
	{0,0,0,0,1,0,0,0,0,0},
	{1,1,1,0,1,1,1,1,1,1},
	{0,0,1,0,0,0,0,0,0,1},
	{0,0,1,1,1,1,1,1,0,1},
	{0,0,0,0,0,0,0,1,0,1},
	{0,1,1,0,0,0,0,1,0,1},
	{0,1,1,0,0,0,0,1,0,1},
	{0,0,0,0,0,0,0,1,1,1}

};



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

void pause()
{
	while (_getch() != 0);
}

void PrintMap(int (&arr)[MAX][MAX],const Position& Current)
{
	for (int y = 0; y < MAX; y++)
	{
		for (int x = 0; x < MAX; x++)
		{
			cout << arr[y][x] << "\t";
		}
		cout << endl;

	}	
	cout << Current.x << "," << Current.y << endl;
	pause();
	system("cls");
}

void PrintPath() //스택용
{
	for (int y = 0; y < MAX; ++y)
	{
		for (int x = 0; x < MAX; ++x)
		{
			if (ArrayMaze[y][x] == VISITED)
			{
				cout << "(" << x << ", " << y << ")" << endl;
			}
		}
	}
}

bool IsValidPosition(const Position& Current)
{
	// 인덱스 검사
	if (Current.x < 0 || Current.y < 0 || Current.x >= MAX || Current.y >= MAX)
	{
		return false;
	}
	return true;
}

Position GetNextPosition(const Position& Current, int dir)
{
	return Position(Current.x+ Dir[dir].x, Current.y + Dir[dir].y);
}

void Trace()
{
	Position Current(MAX - 1, MAX - 1);
	stack<Position> Trace;
	Trace.push(Current);

	bool found = false;
	while (Current.y != 0 || Current.x != 0)
	{
		int Level = ArrayMaze[Current.y][Current.x];

		for (int dir = UP; dir <= LEFT; ++dir)
		{
			Position Next(Current.x + Dir[dir].x, Current.y + Dir[dir].y);
			if (Next.x < 0 || Next.y < 0 ||
				Next.x >= MAX || Next.y >= MAX) //배열을 벗어남
			{
				continue;
			}
			if (ArrayMaze[Next.y][Next.x] == Level - 1)
			{
				Trace.push(Next);
				Current = Next;
				break;
			}
		}
	}

	while (!Trace.empty())
	{
		Position Temp = Trace.top();
		Trace.pop();
		cout << "(" << Temp.x << ", " << Temp.y << ")" << endl;
	}
}

bool CanMove(Position& Current)
{
	Position Next;
	//up
	Next.x = Current.x;
	Next.y = Current.y - 1;
	if (Next.y > -1 && ArrayMaze[Next.y][Next.x] == PATH)
	{
		cout << Current.x << "," << Current.y << " Can Move up" << endl;
		Current = Next;
		return true;
	}

	//right
	Next.x = Current.x +1;
	Next.y = Current.y ;
	if (Next.x < MAX && ArrayMaze[Next.y][Next.x] == PATH)
	{
		cout << Current.x << "," << Current.y << " Can Move right" << endl;
		Current = Next;		
		return true;
	}

	//down
	Next.x = Current.x;
	Next.y = Current.y+1;
	if (Next.y < MAX && ArrayMaze[Next.y][Next.x] == PATH)
	{
		cout << Current.x << "," << Current.y << " Can Move down" << endl;
		Current = Next;
		return true;
	}

	//left
	Next.x = Current.x -1;
	Next.y = Current.y;
	if (Next.x > -1 && ArrayMaze[Next.y][Next.x] == PATH)
	{
		cout << Current.x << "," << Current.y << " Can Move left" << endl;
		Current = Next;		
		return true;
	}
	cout << Current.x  << "," << Current.y << " -> any = Cant" << endl;
	return false;
}

bool CanMove2(Position& Current)
{
	Position Next ;
	for ( int dir = UP;  dir <= LEFT ; dir++)
	{
		Next.x = Current.x + Dir[dir].x;
		Next.y = Current.y + Dir[dir].y;

		if (Next.x < 0 || Next.y < 0 || Next.x >= MAX || Next.y >= MAX)
		{//이동불가
			continue;
		}

		if (ArrayMaze[Next.y][Next.x] == PATH)
		{
			Current = Next;
			return true;
		}
	}
	return false;
}

// 탈출 성공인지 리턴
bool MazeMove(Position Current)
{	
	ArrayMaze[Current.y][Current.x] = VISITED;
	PrintMap(ArrayMaze,Current);

	if (Current.x == (MAX - 1) && Current.y == (MAX - 1))
	{		
		cout << "탈출 성공" << endl;
		PrintMap(ArrayMaze, Current);
		return true;
	}

	Position Next;
	for (int dir = EDIRECTION::UP; dir <= EDIRECTION::LEFT; dir++)
	{
		Next.x = Current.x + Dir[dir].x;
		Next.y = Current.y + Dir[dir].y;

		if (Next.x < 0 || Next.y < 0 || Next.x >= MAX || Next.y >= MAX)
			continue;		

		if (ArrayMaze[Next.y][Next.x] == PATH)
		{
			if (MazeMove(Next))
				return true;
		}	
	}
	
	ArrayMaze[Current.y][Current.x] = BACKTRACE;
	PrintMap(ArrayMaze, Current);
	return false;
}
void Process1()
{
	Position Current;
	bool Exit=false;
	MazeMove(Current);

	return;
	stack<Position> EscapeMap;
	while (true)
	{
		// 현재위치 방문표시
		ArrayMaze[Current.y][Current.x] = VISITED;
		
		if (Current.x == (MAX - 1) && Current.y == (MAX - 1))
		{
			cout << "탈출 성공" << endl;
			break;
		}

		// 이동?
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
				//cout << Current.x << "," << Current.y << "되돌아감 -> " << temp.x << "," << temp.y << endl;
				EscapeMap.pop();
				ArrayMaze[Current.y][Current.x] = BACKTRACE;
				Current = temp;
			}
			else
			{
				cout << "탈출 불가" << endl;
				break;
			}


		}
		PrintMap(ArrayMaze, Current);
	}



	while (!EscapeMap.empty())
	{
		Position Temp = EscapeMap.top();

		cout << "(" << Temp.x << "," << Temp.y << ")" << endl;

		EscapeMap.pop();
	}
	while (_getch() != 0);
}

// 이동가능하면 이동하여 VISITED를 남기고 스택을 풀어 이전 위치로 돌아간다.
bool MazeMoveRecursive(Position Current)
{
	// 인덱스 검사
	if (Current.x < 0 || Current.y < 0 || Current.x >= MAX || Current.y >= MAX)
	{
		return false;
	}
	else if (ArrayMaze[Current.y][Current.x] != PATH)  // 길인지 검사
	{
		return false;
	}
	else if (Current.x == (MAX - 1) && Current.y == (MAX - 1)) // 목적지인지검사
	{
		ArrayMaze[Current.y][Current.x] = VISITED;
		PrintMap(ArrayMaze, Current);
		return true;
	}
	else 
	{
		ArrayMaze[Current.y][Current.x] = VISITED;
		PrintMap(ArrayMaze, Current);
		if (MazeMoveRecursive(Position(Current.x, Current.y - 1)) ||
			MazeMoveRecursive(Position(Current.x+1, Current.y)) ||
			MazeMoveRecursive(Position(Current.x, Current.y + 1)) ||
			MazeMoveRecursive(Position(Current.x-1, Current.y)) )
		{
			return true;
		}
		ArrayMaze[Current.y][Current.x] = BACKTRACE;
		PrintMap(ArrayMaze, Current);
		return false;
	}
}

void ProcessMazeWithRecursive(Position Current)
{	
	MazeMoveRecursive(Current);
}

void InitDir()
{
	//up
	Dir[UP].x = 0;
	Dir[UP].y = -1;
	//right
	Dir[RIGHT].x = 1;
	Dir[RIGHT].y = 0;
	//down
	Dir[DOWN].x = 0;
	Dir[DOWN].y = 1;
	//left
	Dir[LEFT].x = -1;
	Dir[LEFT].y = 0;
}

bool CanMoveQ(Position& Current, Position& NewPosition, int dir)
{
	Position Next;
	Next.x = Current.x + Dir[dir].x;
	Next.y = Current.y + Dir[dir].y;

	if (Next.x < 0 || Next.y < 0 ||
		Next.x > MAX || Next.y > MAX) //배열을 벗어남
	{
		return false;
	}

	if (ArrayMaze[Next.y][Next.x] == PATH) //길이 맞으면 true
	{
		NewPosition = Next;
		return true;
	}

	return false; //길이 아니면

}

void ProcessMazeWithQueue()
{
	queue<Position> Q;
	InitDir();
	Position Current;

	Q.push(Current);
	ArrayMaze[Current.y][Current.x] = 10;
	PrintMap(ArrayMaze, Current);
	bool found = false;
	while (!Q.empty() && !found)
	{
		Current = Q.front();
		Q.pop();
		int Level = ArrayMaze[Current.y][Current.x];
		for (int dir = UP; dir <= LEFT; ++dir)
		{
			Position NewPosition;
			if (CanMoveQ(Current, NewPosition, dir))
			{
				ArrayMaze[NewPosition.y][NewPosition.x] = Level + 1;
				PrintMap(ArrayMaze, NewPosition);
				if (NewPosition.x == (MAX - 1) && NewPosition.y == (MAX - 1))
				{
					cout << "탈출" << endl;
					found = true;
					break;
				}

				Q.push(NewPosition);
			}
		}
	}

	if (!found)
	{
		cout << "탈출 불가" << endl;
	}

	PrintMap(ArrayMaze, Current);
	if (found)
	{
		Trace();
	}
	pause();
}


int MazeCount(Position Current)
{
	if (!IsValidPosition(Current))
	{
		return 0;
	}
	else if(ArrayMazeCount[Current.y][Current.x] != 1)
	{
		return 0;
	}
	
	ArrayMazeCount[Current.y][Current.x] = VISITED;
	int count = 1;
	for (int i = UP; i <= LEFT; i++)
	{
		count += MazeCount(GetNextPosition(Current, i));
	}
	return count;
}

void MazeFill(Position Current)
{
	if (!IsValidPosition(Current))
	{
		return;
	}
	else if (ArrayMazeFill[Current.y][Current.x] != 0)
	{
		return;
	}

	ArrayMazeFill[Current.y][Current.x] = 1;
	
	for (int i = UP; i <= LEFT; i++)
	{
		MazeFill(GetNextPosition(Current,i));
	}
	return;
}



void DrawLine(Position& pos1, Position& pos2)
{
	int absX = abs(pos1.x - pos2.x)/2;
	int absY = abs(pos1.y - pos2.y)/2;

	if (absX <= 0 && absY <= 0)
		return;

	Position Center(pos1.x + absX, pos1.y + absY);
	Position a;
	Position b;
	
	
	
	PutPixel(pos1.x, pos1.y);
	a.x = pos1.x ;
	a.y = pos1.y ;
	b.x = pos1.x + absX;
	b.y = pos1.y + absY;
	DrawLine( a,b);

	PutPixel(Center.x-1 , Center.y-1);

	a.x = pos2.x - absX;
	a.y = pos2.y - absY;
	b.x = pos2.x;
	b.y = pos2.y;
	DrawLine(a, b);

	PutPixel(pos2.x, pos2.y);
}

int main()
{
	InitDir();

	Position Current(1,8);	
	//cout << MazeCount(Current);
	

	Current = Position(5, 4);
	MazeFill(Current);
	PrintMap(ArrayMazeFill,Current);

	Position B(40, 25);

	DrawLine(Current,B);
	
	return 0;
}