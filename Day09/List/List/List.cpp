#include "List.h"
#include <iostream>

using namespace std;

Node::Node()
{
	Prev = nullptr;
	Next = nullptr;
	
}

Node::~Node()
{
}

List::List()
{
	Head = new Node;	
	Tail = new Node;

	Head->Prev = nullptr;
	Head->Next = Tail;
	
	Tail->Prev = Head;
	Tail->Next = nullptr;
	Size = 0;
}

List::~List()
{


	delete Head;
	delete Tail;
	Size = 0;
}

void List::PushFront(int Value)
{
	InsertAfter(Head, Value);
	Size++;
}

void List::PushBack(int Value)
{
	InsertBefore(Tail, Value);
	Size++;
}

void List::InsertBefore(Node * TargetNode, int Value)
{
	if (TargetNode != nullptr || TargetNode == Head)
		return;

	Node* NewNode = new Node;
	NewNode->Data = Value;

	Node* Temp = TargetNode->Prev;
	TargetNode->Prev = NewNode;
	Temp->Next = NewNode;
}

void List::InsertAfter(Node * TargetNode, int Value)
{
	if (TargetNode != nullptr || TargetNode == Tail)
		return;

	Node* NewNode = new Node;
	NewNode->Data = Value;

	Node* Temp = TargetNode->Next;
	TargetNode->Next = NewNode;
	Temp->Prev = NewNode;
}

Node* List::Remove(Node * TargetNode)
{
	if (TargetNode == Head || TargetNode == Tail || TargetNode == nullptr)
	{
		return nullptr;
	}
		

	Node* PrevNode = TargetNode->Prev;

	TargetNode->Prev->Next = TargetNode->Next;
	TargetNode->Next->Prev = TargetNode->Prev;
	
	delete TargetNode;

	Size--;
	return PrevNode;
}

Node * List::GetHead()
{
	return Head;
}

Node * List::GetTail()
{
	return Tail;
}

Node * List::Find(int Value)
{
	Node* Current = Head->Next;
	while (Current != Tail)
	{
		if (Current->Data == Value)
			break;

		Current = Current->Next;
	}
	return Current;
}

Node * List::RFind(int Value)
{
	Node* Current = Tail->Prev;
	while (Current != Head)
	{
		if (Current->Data == Value)
			break;

		Current = Current->Prev;
	}
	return Current;
}

Node * List::Begin()
{
	return Head->Next;
}

Node * List::End()
{
	return Tail;
}

Node * List::RBegin()
{
	return Tail->Prev;
}

Node * List::REnd()
{
	return Head;
}

void List::RemoveAll()
{
	for (Node* Cursor = Begin(); Cursor != End(); Cursor = Cursor->Next)
	{
		Cursor = Remove(Cursor);
	}
	Size = 0;
}

void List::Print()
{
	cout << "List::Print()" << endl;
	Node* Current = Head->Next;
	while (Current!=Tail)
	{
		cout << Current->Data << endl;
		Current = Current->Next;
	}
}

void List::RPrint()
{
}


