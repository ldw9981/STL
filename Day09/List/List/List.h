#pragma once

class Node
{
public:
	Node();
	~Node();
	int Data;
	Node* Next;
	Node* Prev;
};



class List
{
public:
	List();
	~List();

	void PushFront(int Value);
	void PushBack(int Value);

	void InsertBefore(Node* TargetNode,int Value);
	void InsertAfter(Node* TargetNode, int Value);
	Node* Remove(Node* TargetNode);
	Node* GetHead();
	Node* GetTail();

	Node* Find(int Value);
	Node* RFind(int Value);
	Node* Begin();
	Node* End();
	Node* RBegin();
	Node* REnd();


	void RemoveAll();
	void Print();
	void RPrint();
private:
	Node* Head;
	Node* Tail;
	int Size;
};

