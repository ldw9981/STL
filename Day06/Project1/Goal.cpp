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
	//Ŀ����ġ�� �÷��̾� ��ġ�� ����
	COORD c;
	c.X = X;
	c.Y = Y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);

	//�ܼ� ��� ���� ����(Goal����)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14);
	std::cout << "G";
	//�ܼ� ��� ���� ����(�⺻ Į��� ����)
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}
