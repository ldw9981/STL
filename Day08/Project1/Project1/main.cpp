#include <iostream>
#include <vector>

#include "MyVector.h"

#include "MyT.h"

using namespace std;

int main()
{

	MyVector<int> IntVector;
	MyVector<float> FloatVector;

	for (int i = 0; i < 200; i++)
	{
		IntVector.PushBack(i);
	}

	for (int i = 0; i < 200; i++)
	{
		FloatVector.PushBack(i*1.1f);
	}


	IntVector.Erase(1);

	cout << IntVector[30];

	MyT<int> b;
	b.a = 30;

	MyT<char> c;
	c.a = 'c';
	
	return 0;
}