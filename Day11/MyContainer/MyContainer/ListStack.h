#pragma once
#include  "List.h"

template<typename T>
class ListStack
{
public:
	ListStack()
	{

	}
	~ListStack()
	{

	}
	void Push(T Value)
	{
		Data.PushBack(Value);
	}
	T Pop()
	{
		auto iter = Data.rbegin();
		T Value = *iter;
		Data.Remove(iter);
		return Value;
	}

protected:
	List<T> Data;
};

