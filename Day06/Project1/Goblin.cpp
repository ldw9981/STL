#include "Goblin.h"
#include <iostream>

using namespace std;

Goblin::Goblin()
{
	cout << name << "����" << endl;
}


Goblin::~Goblin()
{
	cout << name << "�ı�" << endl;
}

void Goblin::Move(Map * map, int KeyCode)
{
}

void Goblin::Draw()
{
	cout << name << "�׸���" << endl;
}

void Goblin::Init()
{
}

void Goblin::Tick()
{
}
