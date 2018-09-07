#pragma once
#include "Character.h"

class Monster : public Character
{
public:
	Monster();
	~Monster();

	virtual void Move(class Map* map, int KeyCode = 0) override;
	virtual void Draw() override;
};

