#pragma once
#include <memory.h>

template<typename T>
class MyVector
{
public:
	MyVector();
	~MyVector();

	static T Empty;

	void PushBack(T NewValue);
	int GetSize();
	void Erase(int EraseIndex);
	T& GetValue(int GetIndex);
	void IncreaseStorage();
	T operator[](int GetIndex);
protected:
	int Size;
	int Capacity;
	T* Value;
};

template<typename T>
T MyVector<T>::Empty;

template<typename T>
inline MyVector<T>::MyVector()
{
	Capacity = 10;
	Value = new T[Capacity];
	Size = 0;

}

template<typename T>
inline MyVector<T>::~MyVector()
{
	if (Value != nullptr)
	{
		delete[] Value;
	}

}

template<typename T>
inline void MyVector<T>::PushBack(T NewValue)
{
	if (Capacity <= Size)
	{
		IncreaseStorage();
	}
	Size++;
	Value[Size - 1] = NewValue;

}

template<typename T>
inline int MyVector<T>::GetSize()
{
	return Size;
}

template<typename T>
inline void MyVector<T>::Erase(int EraseIndex)
{
	//memcpy(Value + EraseIndex, Value + (EraseIndex + 1), sizeof(T)*(Size - EraseIndex - 1));

	for (int i = EraseIndex; i < Size-1; i++)
	{
		Value[i] = Value[i+1];
	}
	Size--;
}

template<typename T>
inline T& MyVector<T>::GetValue(int GetIndex)
{
	return Value[GetIndex];
}

template<typename T>
inline void MyVector<T>::IncreaseStorage()
{
	int Before = Capacity;
	T* Temp = Value;
	Capacity = Capacity * 2;
	Value = new T[Capacity];

	if (Before > 0)
	{
		for (int i = 0; i < Size; i++)
		{
			Value[i] = Temp[i];
		}		
		delete[] Temp;
	}
}

template<typename T>
inline T MyVector<T>::operator[](int GetIndex)
{
	return Value[GetIndex];
}
