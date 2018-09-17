#pragma once
template<typename T>
class Node
{
public:
	Node();
	~Node();
	T Data;
	Node<T>* Next;
	Node<T>* Prev;
};

template<typename T>
Node<T>::Node()
{
	Prev = nullptr;
	Next = nullptr;
}

template<typename T>
Node<T>::~Node()
{
}