#pragma once

struct Node
{
	int data;
	Node* pNext = nullptr;
	Node* pPrev = nullptr;
};

struct DoubleLinkedList
{
	Node* head = nullptr;
	Node* tail = nullptr;


	//create a new node and returns to caller
	Node* GetNewNode(int val);

	//Insert a node at front of the list. This node will be the new head
	void Insert_at_head(int value);

	//Insert a node after last node
	void Insert_at_tail(int value);


	// delete any node of list according to position
	void DeleteNode(int position);

	// print the list in forward order
	void PrintLinkedListForward();

	// print the list in reverse order
	void PrintLinkedListBackward();

	//This function search the value in the list
	bool IsValueAvailable(int value);
	Node* FindNodeByData(int data);
	// 첫번째 노드가 0이다
	Node* FindNodeByPosition(int position);
	void DeleteNode(Node* pNode);
	void InsertAfterNode(int data, Node* pNode);
	void InsertAfterPos(int data, int position);
	void DeleteNodeByData(int data);
};