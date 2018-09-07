#pragma once
#include "Job.h"
#include  "Character.h"

class Player : public Character
{
public:
	Player();
	~Player();

	EJob job;

	virtual void Draw() override;
	virtual void Init() override;
};

