#include "Slime.h"
#include <iostream>
#include "Character.h"

using namespace std;

Slime::Slime()
{
}


Slime::~Slime()
{
}


void Slime::Move(Map * map, int KeyCode)
{
	
}

void Slime::Draw()
{
	Character::Draw();
}

void Slime::Init()
{
	std::cout << "Slime::Init()";
}

void Slime::Tick()
{
	std::cout << "Slime::Tick()";
}