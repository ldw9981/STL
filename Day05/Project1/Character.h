#pragma once
#define UP		0x48
#define DOWN	0x50
#define LEFT	0x4B
#define RIGHT	0x4D

class Character
{
public:
	Character();
	~Character();

	int X;
	int Y;

	virtual void Move(class Map* map, int KeyCode);

	virtual void Draw();

	virtual void Init();
};

