#include <iostream>
#include "List.h"
#include "Stack.h"
#include "ListStack.h"

using namespace std;



int main()
{
	List<int> MyList;
	
	MyList.PushFront(1);
	MyList.PushFront(2);
	MyList.PushFront(3);
	MyList.PushFront(4);
	MyList.PushFront(5);
	MyList.PushFront(6);
	MyList.PushFront(7);

	cout << "print" << endl;

	for (List<int>::Iterator Cursor = MyList.begin(); Cursor != MyList.end() ; ++Cursor)
	{
		cout << *Cursor << endl;
	}

	cout << "print" << endl;
	/*
	Cursor = MyList.REnd();
	for (Cursor = MyList.RBegin(); Cursor != MyList.REnd(); Cursor = Cursor->Prev)
	{
		cout << Cursor->Data << endl;
	}
	*/

	cout << "print" << endl;
	for (List<int>::Iterator iter = MyList.begin(); iter != MyList.end(); ++iter)
	{
		cout << *iter << endl;
	}

	

	MyList.Remove(MyList.Find(3));
	MyList.Remove(MyList.Find(1));
	MyList.Remove(MyList.Find(7));

	cout << "print" << endl;
	for (List<int>::Iterator iter = MyList.begin(); iter != MyList.end(); ++iter)
	{
		cout << *iter << endl;
	}
	

	
	MyList.RemoveAll();
	
	cout << "print" << endl;
	for (List<int>::Iterator iter = MyList.begin(); iter != MyList.end(); ++iter)
	{
		cout << *iter << endl;
	}
	

	Stack MyStack(20);

	int i = 1;
	while (!MyStack.IsFull())
	{
		MyStack.Push(i);		
		i++;
	}


	MyStack.Resize(30);
	
	
	cout << MyStack.Pop() << endl;
	cout << MyStack.Pop() << endl;
	cout << MyStack.Pop() << endl;
	cout << MyStack.Pop() << endl;
	
	
	ListStack<int> MyListStack;

	for (int i = 1; i < 10; i++)
	{
		MyListStack.Push(i);
	}
	

	cout << MyListStack.Pop() << endl;
	cout << MyListStack.Pop() << endl;
	cout << MyListStack.Pop() << endl;
	
	return 0;
}