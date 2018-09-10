#include <iostream>
#include "Engine.h"
#include "Map.h"

using namespace std;

int main()
{
	Engine* e = new Engine();

	e->Run();

	delete e;
	e = nullptr;
	return 0;
}