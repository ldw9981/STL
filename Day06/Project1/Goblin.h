#pragma once
#include "Monster.h"

class Goblin : public Monster
{
public:
	Goblin();
	~Goblin();

	virtual void Move(class Map* map, int KeyCode) override;

	virtual void Draw() override;

	virtual void Init() override;
	virtual void Tick() override;
};

