#include "pch.h"
#include <stdio.h>

#include <string.h>
#include <stdlib.h>

void test()
{
	printf("Hello World!\n");

	FILE* fp = fopen("C:/Data/data.txt", "r");
	FILE* fpOut = fopen("C:/Data/output.txt", "w");
	char buffer[2048] = { 0, };

	int total = 0;
	fgets(buffer, 80, fp);

	while (strlen(buffer) > 0)
	{
		int temp = atoi(buffer);
		total += temp;
		printf("%s\n", buffer);
		buffer[0] = 0;

		fgets(buffer, 80, fp);


	}

	fprintf(fpOut, "%d\n", total);
	fclose(fp);
	fclose(fpOut);
}

int main()
{
	char buffer[1024] = { 0, };

	int count;
	int data[3][3];
	scanf("%d", &count);

	for (size_t i = 0; i < count; i++)
	{
		scanf("%d %d %d", &data[i][0], &data[i][1], &data[i][2]);
	}
	
	for (size_t i = 0; i < count; i++)
	{
		printf("%d %d %d\n", data[i][0], data[i][1], data[i][2]);
	}



}

