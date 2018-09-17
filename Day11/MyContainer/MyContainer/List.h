#pragma once
#include "Node.h"


template<typename T>
class List
{
	
public:
	List();
	virtual ~List();


	class Iterator
	{
		friend class List;
	public:
		Iterator() { }
		Iterator(Node<T>* Value)
		{
			Current = Value;
		}
		Iterator& operator++()
		{
			Current = Current->Next;
			return *this;
		}
		bool operator!=(const Iterator& rhs)
		{
			return (Current != rhs.Current);
		}

		bool operator==(const Iterator& rhs)
		{
			return (Current == rhs.Current);
		}

		T& operator*()
		{
			return Current->Data;
		}
	protected:
		Node<T>* Current;
	};
	class ReverseIterator
	{
		friend class List;
	public:
		ReverseIterator() { }
		ReverseIterator(Node<T>* Value)
		{
			Current = Value;
		}
		ReverseIterator& operator++()
		{
			Current = Current->Prev;
			return *this;
		}
		bool operator!=(const ReverseIterator& rhs)
		{
			return (Current != rhs.Current);
		}

		bool operator==(const ReverseIterator& rhs)
		{
			return (Current == rhs.Current);
		}

		T& operator*()
		{
			return Current->Data;
		}

		
	protected:
		Node<T>* Current;
	};


	void PushFront(T Value);
	void PushBack(T Value);
	void InsertBefore(Node<T>* TargetNode,const T& Value);
	void InsertAfter(Node<T>* TargetNode, const T& Value);
	Iterator Remove(Iterator TargetNode)
	{
		if (TargetNode == Head || TargetNode == Tail || TargetNode == nullptr)
		{
			return nullptr;
		}

		Node<T>* NextNode = TargetNode.Current->Next;
		TargetNode.Current->Prev->Next = TargetNode.Current->Next;
		TargetNode.Current->Next->Prev = TargetNode.Current->Prev;

		delete TargetNode.Current;

		Size--;
		return NextNode;
	}
	ReverseIterator Remove(ReverseIterator TargetNode)
	{
		if (TargetNode == Head || TargetNode == Tail || TargetNode == nullptr)
		{
			return nullptr;
		}

		Node<T>* PrevNode = TargetNode.Current->Prev;
		TargetNode.Current->Prev->Next = TargetNode.Current->Next;
		TargetNode.Current->Next->Prev = TargetNode.Current->Prev;

		delete TargetNode.Current;

		Size--;
		return PrevNode;
	}

	Iterator GetHead()
	{
		return Head;
	}
	Iterator GetTail()
	{
		return Tail;
	}

	Iterator Find(T Value)
	{
		Node<T>* Current = Head->Next;
		while (Current != Tail)
		{
			if (Current->Data == Value)
				break;

			Current = Current->Next;
		}
		return Current;
	}
	ReverseIterator RFind(T Value)
	{
		Node<T>* Current = Tail->Prev;
		while (Current != Head)
		{
			if (Current->Data == Value)
				break;

			Current = Current->Prev;
		}
		return Current;
	}

	Iterator Begin()
	{
		return Head->Next;
	}

	Iterator End()
	{
		return Tail;
	}

	ReverseIterator RBegin()
	{
		return Tail->Prev;
	}

	ReverseIterator REnd()
	{
		return Head;
	}


	Iterator begin() { return Iterator(Head->Next); }
	Iterator end() { return Iterator(Tail); }

	ReverseIterator rbegin() { return ReverseIterator(Tail->Prev); }
	ReverseIterator rend() { return ReverseIterator(Head); }


	void RemoveAll();

private:
	Node<T>* Head;
	Node<T>* Tail;
	int Size;
};

template<typename T>
List<T>::List()
{
	Head = new Node<T>;
	Tail = new Node<T>;

	Head->Prev = nullptr;
	Head->Next = Tail;

	Tail->Prev = Head;
	Tail->Next = nullptr;
	Size = 0;
}
template<typename T>
List<T>::~List()
{
	RemoveAll();
	delete Head;
	delete Tail;
	Size = 0;
}
template<typename T>
void List<T>::PushFront(T Value)
{
	InsertAfter(Head, Value); // Head -> New
	Size++;
}
template<typename T>
void List<T>::PushBack(T Value)
{
	InsertBefore(Tail, Value); // New <- Tail
	Size++;
}

template<typename T>
void List<T>::InsertBefore(Node<T> * TargetNode, const T& Value)
{
	if (TargetNode == nullptr || TargetNode == Head)
		return;

	Node<T>* NewNode = new Node<T>;
	NewNode->Data = Value;

	Node<T>* OldTargetPrev = TargetNode->Prev;
	
	TargetNode->Prev = NewNode;
	NewNode->Prev = OldTargetPrev;
	NewNode->Next = TargetNode;
	
	OldTargetPrev->Next = NewNode;
}

template<typename T>
void List<T>::InsertAfter(Node<T> * TargetNode, const T& Value)
{
	if (TargetNode == nullptr || TargetNode == Tail)
		return;

	Node<T>* NewNode = new Node<T>;
	NewNode->Data = Value;

	Node<T>* OldTargetNext = TargetNode->Next;

	TargetNode->Next = NewNode;
	NewNode->Next = OldTargetNext;
	NewNode->Prev = TargetNode;

	OldTargetNext->Prev = NewNode;
}



template<typename T>
void List<T>::RemoveAll()
{
	for (Iterator Cursor = begin(); Cursor != end(); ++Cursor)
	{
		Cursor = Remove(Cursor);
	}
	Size = 0;
}
