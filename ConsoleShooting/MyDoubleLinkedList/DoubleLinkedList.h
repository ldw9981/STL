#pragma once
#include <iostream>

template<typename T>
struct Node
{
	T data;
	Node* pNext = nullptr;
	Node* pPrev = nullptr;
};

template<typename T>
struct DoubleLinkedList
{
	Node<T>* pHead = nullptr;
	Node<T>* pTail = nullptr;
	int Count=0;
	void InsertAtHead(T value)
	{
		Node<T>* pNewNode = new Node<T>;  // ������ delete �޸��ּ�
		pNewNode->data = value;
		if (pHead == nullptr)
		{
			// �ƹ��͵� ���� ���� ó��
			pHead = pNewNode;
			pTail = pNewNode;
			Count++;
			return;
		}

		pHead->pPrev = pNewNode;
		pNewNode->pNext = pHead;
		pHead = pNewNode;
		Count++;
	}
	void PrintListAll()
	{
		Node<T>* pNode = pHead;
		while (pNode != nullptr)
		{
			std::cout << pNode->data << " ";
			pNode = pNode->pNext;
		}

		std::cout << std::endl;
	}

	// ������ ��� ã��
	Node<T>* FindNode(int value)
	{
		Node<T>* result = nullptr;
		//....

		return result;
	}

	// ��� �����ϱ�  
	void DeleteNode(Node<T>* pNode)
	{
		//�����Ϸ��� ��尡 Head,Tail,�߰� �� ��� ��� �׽�Ʈ �Ѵ�.
		if (pNode == nullptr)
		{
			printf("deleteNode(nullptr)\n");
			return;
		}

		if (pNode == pHead)
		{
			pHead = pNode->pNext;     // ������� ��尡 head�̸� �������� head����
		}
		else if (pNode == pTail)
		{
			pTail = pNode->pPrev; // ������� ��尡 tail�̸� ���� ���� tail����
		}

		// ������ ����
		if (pNode->pPrev != nullptr)
		{
			pNode->pPrev->pNext = pNode->pNext;
		}

		if (pNode->pNext != nullptr)
		{
			pNode->pNext->pPrev = pNode->pPrev;
		}

		//�Ҵ� ����
		delete pNode;
		Count--;
	}

	// �ѹ��� ��ȸ�� ���� ���� ��带 ����� �Լ�
	void Erase(T value)
	{

	}

	void Clear()
	{
		Node<T>* pNodeNext = nullptr;
		Node<T>* pNode = pHead;
		while (pNode != nullptr)
		{
			pNodeNext = pNode->pNext;
			DeleteNode(pNode);
			pNode = pNodeNext;
		}
	}
};

