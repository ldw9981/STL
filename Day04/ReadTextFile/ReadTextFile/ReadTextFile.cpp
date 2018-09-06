// ReadTextFile.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned long long BigInt;
typedef long double BigDouble;

void calculate1()
{
	int count;
	scanf("%d", &count);

	int size = sizeof(BigInt);
	BigInt test = 0xFFFFFFFFFFFFFFFF;

	BigDouble average = 0.0;
	BigInt total = 0;
	BigInt temp = 0;
	for (int i = 0; i < count; i++)
	{		
		scanf("%llu", &temp);
		total += temp;
	}

	average = (BigDouble)total / (BigDouble)count;

	printf("Total:%llu , Average: %f", total, average);

}

int main()
{
	calculate1();
}

