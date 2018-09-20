#include "BTree.h"
#include "BNode.h"

void BTree::Insert(int Value)
{
	class BNode* Current = Root;
	if (Root == nullptr) // 루트가 없다, 처음 생성
	{
		Root = new BNode(Value);
	}
	else
	{
		while (Current != nullptr)
		{
			if (Value < Current->Data)
			{
				if (Current->LeftChild == nullptr) //현재 노드의 왼쪽은 비어 있다.
				{
					BNode* NewNode = new BNode(Value);
					Current->LeftChild = NewNode;
					NewNode->Parent = Current;
					Current = nullptr;
				}
				else //왼쪽에 있으면
				{
					Current = Current->LeftChild;
				}
			}
			else
			{
				if (Current->RightChild == nullptr) //현재 노드의 오른쪽 비어 있다.
				{
					BNode* NewNode = new BNode(Value);
					Current->RightChild = NewNode;
					NewNode->Parent = Current;
					Current = nullptr;
				}
				else //오른쪽 자식이 있다.
				{
					Current = Current->RightChild;
				}
			}
		}
	}
}

BNode * BTree::Find(int Value)
{
	BNode* Current = Root;
	while (Current != nullptr)
	{
		if (Current->Data == Value)
		{
			break;
		}
		else if (Current->Data > Value)
		{
			Current = Current->LeftChild;
		}
		else
		{
			Current = Current->RightChild;
		}
	}
	return Current;
}
