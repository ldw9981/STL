#pragma once

#include <string>

#define UP		0x48
#define DOWN	0x50
#define LEFT	0x4B
#define RIGHT	0x4D



class Character
{
public:
	Character();
	~Character();

	virtual void Draw();
	virtual void Tick();

	void SetName(std::string name);
	std::string GetName();
	int X;
	int Y;
protected:

	std::string name;
	virtual void Move(class Map* map, int KeyCode);
	virtual void Init();
};

