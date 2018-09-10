#include "Goal.h"
#include <Windows.h>
#include <iostream>


Goal::Goal()
{
}


Goal::~Goal()
{
}

void Goal::Draw()
{
	//커서위치를 플레이어 위치로 변경
	COORD c;
	c.X = X;
	c.Y = Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

	//콘솔 출력 색상 변경(Goal색깔)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	std::cout << "G";
	//콘솔 출력 색상 변경(기본 칼라로 변경)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
