#include <iostream>

#include "SingleLinkedList.h"

using namespace std;

int main()
{
	SingleLinkedList SList;

	SList.PushFront(1);
	SList.PushFront(2);
	SList.PushFront(3);


	for (Node* i = SList.Begin(); i != nullptr; i = i->Next)
	{
		cout << i->Value << endl;
	}


	Node* FindNode = SList.Find(3);
	if (FindNode != nullptr)
	{
		cout << FindNode->Value << endl;
	}

	return 0;
}