#pragma once
class BNode
{
public:
	BNode();
	~BNode();


	BNode(int Init = 0)
	{
		Data = Init;
		Parent = nullptr;
		LeftChild = nullptr;
		RightChild = nullptr;
	}
	BNode* Parent;
	int Data;
	BNode* LeftChild;
	BNode* RightChild;
};

