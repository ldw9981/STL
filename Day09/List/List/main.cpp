#include <iostream>
#include "List.h"

using namespace std;

int main()
{
	List MyList;
	
	MyList.PushFront(1);
	MyList.PushFront(2);
	MyList.PushFront(3);
	MyList.PushFront(4);
	MyList.PushFront(5);
	MyList.PushFront(6);
	MyList.PushFront(7);

	MyList.Print();

	MyList.Remove(MyList.Find(3));
	MyList.Remove(MyList.Find(1));
	MyList.Remove(MyList.Find(7));

	MyList.Print();

	MyList.RemoveAll();


	MyList.Print();

	return 0;
}