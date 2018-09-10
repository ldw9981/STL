#include "Map.h"
#include <iostream>
#include <fstream>

Map::Map()
{
	sizeX = 20;
	sizeY = 20;
	InitMap();
}


Map::~Map()
{
}


void Map::Draw()
{
	//for (int y = 0; y < sizeX; ++y)
	//{
	//	std::cout << map[y] << std::endl;
	//}

	
	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; ++x)
		{
			if (map[y][x] == '1')
			{
				std::cout << map[y][x];
			}
			else
			{
				std::cout << " ";
			}
		}
		std::cout << std::endl;
	}
}

bool Map::IsWall(int x, int y)
{
	if (x < 0 || y < 0 || x > sizeX-1 || y > sizeY-1)
	{
		return true;
	}
	else
	{
		return (map[y][x] == '1');
	}
}

void Map::InitMap()
{
	//FILE* fp;
	//fp = fopen("map.txt", "r");
	//if (fp)
	//{
	//	for (int i = 0; i < 20; ++i)
	//	{
	//		char Buffer[1024] = { 0 };
	//		fgets(Buffer, 1023, fp);
	//		map[i] = Buffer;
	//	}
	//}
	//
	//fclose(fp);

	std::ifstream dataFile("map.txt");

	for(int i  = 0; i < 20; ++i)
	{
		std::getline(dataFile, map[i]);
	}

	dataFile.close();
}
