#pragma once
template<typename T>
class FixedQueue
{
public:
	FixedQueue(int InitCapacity)
	{
		Index = 0;
		Tail = -1;
		Head = -1;
		Capacity = InitCapacity;
		Data = new T[InitCapacity];
	}
	~FixedQueue()
	{
		delete[] Data;
	}

	void Enqueue(int Value)
	{
		if (CurrentSize == Capacity)
			return;

		if (Head == -1)
		{
			Head = 0;
		}

		Tail++;
		Data[Tail%Capacity] = Value;
		
		CurrentSize++;
	}
	T Dequeue()
	{
		if (CurrentSize == 0)
			return -1;

		T Temp = Data[Head];
		Head++;
		Head = Head % Capacity;
		CurrentSize++;
		return Temp;
	}
	static T Empty;
protected:
	T* Data;
	int Index;
	int Tail;
	int Head;
	int CurrentSize;
	int Capacity;
};
template<typename T>
T FixedQueue<T>::Empty;