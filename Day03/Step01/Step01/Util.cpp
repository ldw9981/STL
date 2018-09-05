#pragma once
#include <stdio.h>


void DrawLine(int a)
{
	for (int i = 0; i < a; i++)
	{
		printf("*");
	}
	printf("\n");
}
