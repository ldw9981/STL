#pragma once

#include <algorithm>
class Stack
{
public:
	Stack(int StackSize = 10)
	{
		Data = new int[StackSize];
		Capacity = StackSize;
		Size = 0;
	}
	~Stack()
	{
		delete[] Data;
	}

	bool IsEmpty()
	{
		return (Size == 0);
	}
	bool IsFull()
	{
		return (Size == Capacity);
	}

	void Push(int Value)
	{
		if (Size == Capacity)
			return;

		Data[Size] = Value;
		Size++;
	}

	int Pop()
	{
		Size--;
		if (Size < 0)
			return -1;

		return Data[Size];
	}

	bool Resize(int NewSize)
	{		
		if (NewSize <= 0 || NewSize == Capacity)
			return false;

		int* NewData = new int[NewSize];			
		if (NewData == nullptr)
			return false;

		Capacity = NewSize;
		Size = Size < NewSize ? NewSize : Size;
		for (int i = 0; i < Size; i++)
		{
			NewData[i] = Data[i];
		}		
		delete[] Data;
		Data = NewData;	

		return true;
	}

private:
	int Capacity;
	int* Data;
	int Size;
};

