#pragma once

class Node
{
public:
	Node();
	~Node();

	int Value;
	Node* Next;
};



class SingleLinkedList
{
public:
	SingleLinkedList();
	~SingleLinkedList();

	Node* Head;
	int Size;

	void InsertAfter(Node* TargetNode, int Value);
	void PushFront(int Value);
	Node* Erase(Node* TargetNode);
	Node* Find(int Value);
	Node* Begin();
	void RemoveAll();
};

