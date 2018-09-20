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

	// 전   데이터작업을 먼저하고 노드 방문을 왼쪽,오른쪽 한다.
	// 중   노드 방문을 왼쪽 하고 데이터작업을 ,오른쪽 한다.
	// 후   노드 방문을 왼쪽,오른쪽 이 끝나면 데이터작업한다.
};

