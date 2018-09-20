#include "BTree.h"
#include "BNode.h"

void BTree::Insert(int Value)
{
	class BNode* Current = Root;
	if (Root == nullptr) // ��Ʈ�� ����, ó�� ����
	{
		Root = new BNode(Value);
	}
	else
	{
		while (Current != nullptr)
		{
			if (Value < Current->Data)
			{
				if (Current->LeftChild == nullptr) //���� ����� ������ ��� �ִ�.
				{
					BNode* NewNode = new BNode(Value);
					Current->LeftChild = NewNode;
					NewNode->Parent = Current;
					Current = nullptr;
				}
				else //���ʿ� ������
				{
					Current = Current->LeftChild;
				}
			}
			else
			{
				if (Current->RightChild == nullptr) //���� ����� ������ ��� �ִ�.
				{
					BNode* NewNode = new BNode(Value);
					Current->RightChild = NewNode;
					NewNode->Parent = Current;
					Current = nullptr;
				}
				else //������ �ڽ��� �ִ�.
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
