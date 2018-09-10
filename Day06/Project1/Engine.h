#pragma once
#include <vector>

class Engine
{
public:
	Engine();
	~Engine();

	void Run();

	class Map* map;
	class Player* player;
	class Goal* goal;
	class Monster* monster;
	class Goblin* goblin;
	class Slime* slime;
private:
	int Input();
	void Process(int KeyCode);
	void StopEngine();
	void IsFail();
	void IsQuit(int KeyCode);
	void IsComplete();
	void Render();
	void InitGoalPosition();
	void InitMonstersPosition();

	bool bIsRunnging;

	std::vector<class Character*> characters;
};

