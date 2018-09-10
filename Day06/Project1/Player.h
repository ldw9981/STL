#pragma once
#include "Job.h"
#include  "Character.h"

class Player : public Character
{
public:
	Player();
	~Player();

	EJob job;

	virtual void Move(class Map* map, int KeyCode) override;

	virtual void Draw() override;

	virtual void Init() override;
	virtual void Tick() override;


	void Skill1();

	void Input();
};

