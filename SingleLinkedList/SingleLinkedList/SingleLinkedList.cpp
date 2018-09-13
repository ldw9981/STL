#include "SingleLinkedList.h"

Node::Node()
{
	Next = nullptr;
}

Node::~Node()
{
}

SingleLinkedList::SingleLinkedList()
{
	Head = new Node();	
	Size = 0;
}


SingleLinkedList::~SingleLinkedList()
{
	RemoveAll();	
}

void SingleLinkedList::InsertAfter(Node * TargetNode, int Value)
{	
	Node* NewNode = new Node();
	NewNode->Value = Value;

	Node* OldTargetNext = TargetNode->Next;
	TargetNode->Next = NewNode;
	NewNode->Next = OldTargetNext;
	Size++;
}

void SingleLinkedList::PushFront(int Value)
{
	InsertAfter(Head, Value);
}

Node* SingleLinkedList::Erase(Node * TargetNode)
{
	Node* Prev = Head;
	Node* Cursor = Head->Next;
	while (Cursor!=nullptr)
	{
		if (TargetNode == Cursor)
			break;

		Prev = Cursor;
		Cursor = Cursor->Next;				 
	}
	

	Node* TempCursorNext = Cursor->Next;
	Prev->Next = TempCursorNext;
	delete Cursor;
	Size--;
	return Prev;
}

Node * SingleLinkedList::Find(int Value)
{
	Node* Cursor = Head->Next;
	while (Cursor!=nullptr)
	{
		if (Cursor->Value == Value)
			break;

		Cursor = Cursor->Next;		
	}
	return Cursor;
}

Node * SingleLinkedList::Begin()
{
	return Head->Next;
}

void SingleLinkedList::RemoveAll()
{
	Node* Cursor = Head->Next;
	while (Cursor != nullptr)
	{
		Cursor = Erase(Cursor);
		Cursor = Cursor->Next;
	}
}
