#include "List.h"
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

Node* DoubleLinkedList::GetNewNode(int val)
{
	Node* temp_node = new Node;
	temp_node->data = val;
	return temp_node;
}

//Insert a node at front of the list. This node will be the new head

void DoubleLinkedList::Insert_at_head(int value)
{
	Node* newNode = GetNewNode(value);

	if (head == nullptr) //For the 1st element
	{
		//For now, newNode is the only one node of list
		//So it it is head and also tail
		head = newNode;
		tail = newNode;
		return;
	}

	//newNode will be the NEW HEAD of list.
	//So it'll point to head as 'next node'
	newNode->pNext = head;
	head->pPrev = newNode; //before, the previous node of head was NULL. but now newNode

	head = newNode; //update the global node 'head' by newNode
}

//Insert a node after last node

void DoubleLinkedList::Insert_at_tail(int value)
{
	Node* newNode = GetNewNode(value);

	if (head == nullptr) //For the 1st element
	{
		head = newNode;
		tail = newNode;
		return;
	}

	//'tail' is a global node. 'newNode' will be the next node of tail.
	//finally newNode will be the 'tail' node of the list
	tail->pNext = newNode;
	newNode->pPrev = tail; //'newNode' point 'tail' node as previous node

	tail = newNode; // update the global node 'tail' by newNode.
}

// 첫번째 노드가 0이다
Node* DoubleLinkedList::FindNodeByPosition(int position)
{
	int positionCount = 0;
	Node* pNode = head;
	while (pNode != nullptr)
	{
		if (position == positionCount)
		{
			return pNode;
		}
		else
		{
			pNode = pNode->pNext;
			positionCount++;
		}
	}
	return nullptr;
}

void DoubleLinkedList::InsertAfterPos(int data, int position)
{
	Node* pFindNode = FindNodeByPosition(position);
	if (pFindNode == nullptr)
	{
		printf("insertAfterPos()  pFindNode==nullptr\n");
		return;
	}
	InsertAfterNode(data, pFindNode);
}

void DoubleLinkedList::InsertAfterNode(int data, Node* pNode)
{
	if (pNode == nullptr)
	{
		printf("insertNodeAfter(nullptr)\n");
		return;
	}

	Node* pNewNode = GetNewNode(data);
	pNewNode->data = data;
	pNewNode->pPrev = pNode;
	pNewNode->pNext = pNode->pNext;

	pNode->pNext = pNewNode;

	if (pNode == tail)
	{
		tail = pNewNode;
	}
}




// delete any node of list according to position

void DoubleLinkedList::DeleteNode(int position)
{
	Node* pFindNode = FindNodeByPosition(position);
	DeleteNode(pFindNode);
}

// print the list in forward order

void DoubleLinkedList::PrintLinkedListForward()
{
	printf("\nYour updated linked list in FORWARD ORDER:\n");

	Node* myList;
	myList = head;

	while (1)
	{
		if (head == NULL || tail == NULL) break;

		printf("%d ", myList->data);

		if (myList == tail) break;

		myList = myList->pNext;
	}
	puts("\n");
}

// print the list in reverse order

void DoubleLinkedList::PrintLinkedListBackward()
{
	printf("\nYour full linked list in REVERSE ORDER:\n");

	Node* myList;
	myList = tail;

	while (1)
	{
		if (head == NULL || tail == NULL) break;

		printf("%d ", myList->data);

		if (myList->pPrev == NULL) break;

		myList = myList->pPrev;
	}
	puts("\n");
}

//This function search the value in the list

bool DoubleLinkedList::IsValueAvailable(int value)
{
	Node* p = head;
	while (p != NULL)
	{
		if (p->data == value)
		{
			return true;
		}
		else
		{
			p = p->pNext;
		}
	}
	return 0;
}

Node* DoubleLinkedList::FindNodeByData(int data)
{
	Node* pNode = head;
	while (pNode != nullptr)
	{
		if (pNode->data == data)
		{
			return pNode;
		}
		else
		{
			pNode = pNode->pNext;
		}
	}
	return nullptr;
}




void DoubleLinkedList::DeleteNode(Node* pNode)
{
	if (pNode == nullptr)
	{
		printf("deleteNode(nullptr)\n");
		return;
	}

	if (pNode == head)
	{
		head = pNode->pNext;     // 지우려는 노드가 head이면 다음노드로 head설정
	}
	else if (pNode == tail)
	{
		tail = pNode->pPrev; // 지우려는 노드가 tail이면 이전 노드로 tail설정
	}

	// 포인터 정리
	if (pNode->pPrev != nullptr)
	{
		pNode->pPrev->pNext = pNode->pNext;
	}

	if (pNode->pNext != nullptr)
	{
		pNode->pNext->pPrev = pNode->pPrev;
	}

	//할당 해제
	delete pNode;
}



void DoubleLinkedList::DeleteNodeByData(int data)
{
	Node* pFindNode = FindNodeByData(data);
	if (pFindNode == nullptr)
	{
		printf("no data: %d\n", data);
		return;
	}
	DeleteNode(pFindNode);
}
