#include <iostream>
#include <algorithm>    // std::swap

using namespace std;

bool compareHigh(int a, int b)
{
	return a > b;
}
bool compareLow(int a, int b)
{
	return a < b;
}


void BubbleSort(int *data, int n,bool(*fp)(int,int))
{
	for (int i = 0; i <n ; i++)
	{
		for (int j = i; j < n-1; j++)
		{
			if (data[j] > data[j+1])
			{
				swap(data[j], data[j+1]);
			}

			for (int index = 0; index < n; index++)
			{
				cout << data[index];
			}
			cout << "\n";
		}
	}
}

void SelectionSort(int *data, int n, bool(*fp)(int, int))
{
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			if (data[i] > data[j])
			{
				swap(data[i], data[j]);
			}

			for (int index = 0; index < n; index++)
			{
				cout << data[index];
			}
			cout << "\n";
		}
	}
}

int partition(int* data, int begin, int end)
{
	int pivot = data[end];
	int min = begin - 1;
	for (int max = begin; max <= end -1; max++)
	{

	}
}


void quicksort(int* data, int begin, int end)
{
	if (begin < end)
	{
		int pivot = partition(data, begin, end);
		quicksort(data, begin, pivot - 1);

	}
}

int main()
{
	int data[] = { 5,1,4,2,3 };
	//
	BubbleSort(data, 5,compareHigh);
	for (auto i : data)
	{
		cout << i << ", ";
	}

	//
	SelectionSort(data, 5, compareHigh);
	for (auto i : data)
	{
		cout << i << ", ";
	}


	quicksort(data, 5, compareHigh);
	for (auto i : data)
	{
		cout << i << ", ";
	}



	return 0;
}