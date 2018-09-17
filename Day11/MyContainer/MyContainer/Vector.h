#pragma once
#include <memory.h>

template<typename T>
class Vector
{
public:
	class Iterator
	{
		friend class Vector;
	public:
		Iterator() { }
		Iterator(T* InitData,int InitIndex)
		{
			Current = InitData;
			Index = InitIndex;
		}
		Iterator& operator++()
		{
			Index++;
			return *this;
		}
		bool operator!=(const Iterator& rhs)
		{
			return (Index != rhs.Index || Current != rhs.Current);
		}

		bool operator==(const Iterator& rhs)
		{
			return (Index == rhs.Index && Current == rhs.Current);
		}

		T& operator*()
		{
			return *(Current+Index);
		}
	protected:
		int Index;
		T* Current;
	};
	class ReverseIterator
	{
		friend class Vector;
	public:
		ReverseIterator() { }
		ReverseIterator(T* InitData, int InitIndex)
		{
			Data = InitData;
			Index = InitIndex;
		}
		ReverseIterator& operator++()
		{
			Index--;
			return *this;
		}
		bool operator!=(const ReverseIterator& rhs)
		{
			return (Index != rhs.Index );
		}

		bool operator==(const ReverseIterator& rhs)
		{
			return (Index == rhs.Index );
		}

		T& operator*()
		{
			return *(Data + Index);
		}


	protected:
		int Index;
		T* Data;
	};


public:
	Vector();
	~Vector();



	static T Empty;

	void PushBack(T NewValue);
	int GetSize();
	void Erase(int EraseIndex);
	Iterator Erase(Iterator EraseIter)
	{
		for (int i = EraseIter.Index; i < Size - 1; i++)
		{
			Value[i] = Value[i + 1];
		}
		Size--;
	}
	ReverseIterator Erase(ReverseIterator iter);

	T& GetValue(int GetIndex);
	void IncreaseStorage();
	T operator[](int GetIndex);

	Iterator begin()
	{
		return Iterator(Value,0);
	}
	Iterator end()
	{
		return Iterator(Value, Size);
	}
	ReverseIterator rbegin() { return ReverseIterator(Value,Size-1); }
	ReverseIterator rend() { return ReverseIterator(Value,-1); }

protected:
	int Size;
	int Capacity;
	T* Value;
};

template<typename T>
T Vector<T>::Empty;

template<typename T>
inline Vector<T>::Vector()
{
	Capacity = 10;
	Value = new T[Capacity];
	Size = 0;

}

template<typename T>
inline Vector<T>::~Vector()
{
	if (Value != nullptr)
	{
		delete[] Value;
	}

}

template<typename T>
inline void Vector<T>::PushBack(T NewValue)
{
	if (Capacity <= Size)
	{
		IncreaseStorage();
	}
	Size++;
	Value[Size - 1] = NewValue;

}

template<typename T>
inline int Vector<T>::GetSize()
{
	return Size;
}

template<typename T>
inline void Vector<T>::Erase(int EraseIndex)
{
	//memcpy(Value + EraseIndex, Value + (EraseIndex + 1), sizeof(T)*(Size - EraseIndex - 1));

	for (int i = EraseIndex; i < Size-1; i++)
	{
		Value[i] = Value[i+1];
	}
	Size--;
}


template<typename T>
inline T& Vector<T>::GetValue(int GetIndex)
{
	return Value[GetIndex];
}

template<typename T>
inline void Vector<T>::IncreaseStorage()
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
inline T Vector<T>::operator[](int GetIndex)
{
	return Value[GetIndex];
}
