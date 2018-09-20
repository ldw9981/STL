#include <iostream>

#include "Node.h"
#include "Tree.h"
#include "BTree.h"
#include "BNode.h"

int main()
{
	Node* node = new Node(1);
	Node* nodeChild = new Node(2);

	

	BTree B;
	B.Insert(10);
	B.Insert(2);
	B.Insert(1);
	B.Insert(9);
	B.Insert(4);
	B.Insert(5);
	B.Insert(7);

	
	BNode* find = B.Find(5);

	if (find)
	{
		cout << find->Data << endl;
	}

}