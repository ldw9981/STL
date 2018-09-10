#pragma once

#include "Character.h"

class Slime : public Character
{
public:
	Slime();
	~Slime();

	virtual void Move(class Map* map, int KeyCode) override;

	virtual void Draw() override;

	virtual void Init() override;
	virtual void Tick() override;
};

