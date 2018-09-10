#include "Goblin.h"
#include <iostream>

using namespace std;

Goblin::Goblin()
{
	cout << name << "생성" << endl;
}


Goblin::~Goblin()
{
	cout << name << "파괴" << endl;
}

void Goblin::Move(Map * map, int KeyCode)
{
}

void Goblin::Draw()
{
	cout << name << "그리기" << endl;
}

void Goblin::Init()
{
}

void Goblin::Tick()
{
}
