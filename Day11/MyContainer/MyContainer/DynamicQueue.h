#pragma once
#include "List.h"

template<typename T>
class DynamicQueue : public List<T>
{
public:
	DynamicQueue()
	{

	}
	virtual ~DynamicQueue()
	{

	}

	void Enqueue(T Value)
	{
		
		List<T>::PushBack(Value);
	}

	T Dequeue()
	{
		auto iter = List<T>::begin();
		T temp = *iter;
		Remove(iter);
		return temp;
	}
};

