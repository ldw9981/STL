// PrintStar.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <stdio.h>
#include <math.h>


int main()
{
    printf("Hello World!\n"); 

	
	int height,width;
	printf("height:");
	scanf("%d", &height);
	width = height + height - 1;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int printLeftIndex = width / 2 -y;
			int printRightIndex = width / 2 + y;

			if (x >= printLeftIndex && x <= printRightIndex)
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n");

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int printLeftIndex = y;
			int printRightIndex = (width-1) - y;

			if (x >= printLeftIndex && x <= printRightIndex)
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n");

	printf("width:");
	scanf("%d", &width);
	height = width + width - 1;
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{			
			int printLeftIndex =  abs( y - height / 2);
			int printRightIndex = width - 1;  

			if (x >= printLeftIndex && x <= printRightIndex)
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}

	printf("\n");

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int printLeftIndex = 0;
			int printRightIndex = (width-1) - abs(y - height / 2);

			if (x >= printLeftIndex && x <= printRightIndex)
			{
				printf("*");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}



}

