#pragma once


class BTree
{
public:
	BTree()
	{
	}


	~BTree()
	{
	}



	class BNode* Root;

	void Insert(int Value);

	class BNode* Find(int Value);

	// ��   �������۾��� �����ϰ� ��� �湮�� ����,������ �Ѵ�.
	// ��   ��� �湮�� ���� �ϰ� �������۾��� ,������ �Ѵ�.
	// ��   ��� �湮�� ����,������ �� ������ �������۾��Ѵ�.
};

