#include <iostream>
#include "List.h"
#include "Stack.h"
#include "ListStack.h"
#include "Vector.h"
#include "FixedQueue.h"
#include "DynamicQueue.h"
#include <bitset>

#include <algorithm>
using namespace std;


void TestList()
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

	for (List<int>::Iterator Cursor = MyList.begin(); Cursor != MyList.end(); ++Cursor)
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


}
void TestVector()
{
	Vector<int> MyVector;

	

	MyVector.PushBack(1);
	MyVector.PushBack(2);
	MyVector.PushBack(3);
	MyVector.PushBack(4);
	MyVector.PushBack(5);
	MyVector.PushBack(6);
	MyVector.PushBack(7);

	cout << "print" << endl;

	for (auto Cursor = MyVector.begin(); Cursor != MyVector.end(); ++Cursor)
	{
		cout << *Cursor << endl;
	}

	cout << *find(MyVector.begin(), MyVector.end(), 5) << endl;
}

void TestQueue()
{
	FixedQueue<int> MyQueue(2);

	for (int i = 0;  i < 3;  i++)
	{
		MyQueue.Enqueue(i);
	}

	cout << MyQueue.Dequeue() << endl;
	cout << MyQueue.Dequeue() << endl;

	MyQueue.Enqueue(5);
	MyQueue.Enqueue(6);
	cout << MyQueue.Dequeue() << endl;


}

int Sum(int n)
{
	if (n == 1)
		return n;

	return n + Sum(n - 1);
}

void ReversePrint(string msg)
{
	if (msg.length() == 0)
		return;
	/*
	string stringPrint = msg.substr(msg.length() - 1, 1);
	cout << stringPrint.c_str();
	return ReversePrint(msg.substr(0, msg.length() - 1));
	*/

	ReversePrint(msg.substr(1));

	cout << msg[0];
}

int ArraySum(int* num, int size)
{
	if (size == 0)
		return 0;


}

void TestReverseString()
{
	string msg = "HelloWorld";
	ReversePrint(msg);
}

int RArraySum(int* data, int size)
{
	return 0;
}

int RStringLength(char* str)
{
	// for <-> Recursive 가능하다.   R함수는 끝낼조건 생각하자
	if (str[0] == '\0')
		return 0;

	return 1 + RStringLength(str + 1);
}


void ToBinaryUINT(unsigned int n,int i)
{
	unsigned int data = (unsigned int)n;
	unsigned int bit = 0x00000001 << i;
	if (i < 0)
		return;

	if (data & bit)
		cout << "1";	
	else
		cout << "0";

	ToBinaryUINT(n, i - 1);
}


void RToBinary(int n)
{
	if (n <= 1)
	{
		cout << n;
	}
	else
	{
		cout << n % 2;
		RToBinary(n / 2);		
	}
}

int Factorial(int n)
{
	int sum = 1;
	for (int i = n; i >= 1; i--)
	{
		sum *= i;
	}
	return sum;
}

int RFactorial(int n)
{
	if (n == 1)
		return 1;

	return n * RFactorial(n - 1);
}

int BSearch(int* Data, int begin, int end, int value)
{
	if (begin > end)
	{
		return -1;
	}
	int middle = (begin + end) / 2;
	if (Data[middle] == value)
	{
		return middle;
	}
	else if (Data[middle] > value)
	{
		return BSearch(Data, begin, middle - 1, value);
	}
	return BSearch(Data, middle + 1, end, value);
}


void TestSearch()
{
	int Data[] = { 1,2,3,4,5,6,7,8,9,10 };
	cout << BSearch(Data,0,9,3) << endl;
}


int main()
{
//	ToBinaryUINT((unsigned int)-9,32);
//	cout << RFactorial(3);
	TestList();
	TestVector();
	TestReverseString();
	TestQueue();
	TestSearch();
	
	return 0;
}