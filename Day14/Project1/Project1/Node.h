#pragma once
#include <list>

using namespace std;

class Node
{
public:
	Node(int InitData = 0)
	{
		Data = InitData;
		Parent = nullptr;
	}
	~Node()
	{

	}

	Node* Parent;
	int Data;
	list<class Node*> Children;

	void AddChild(class Node* Node);
	int GetChildCount();
};

