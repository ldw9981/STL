#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <bitset>
using namespace std;

void Print(unsigned int n,int shift)
{
	if (shift < 0)
	{
		return;
	}

	unsigned int bit = 0x00000001 << shift;
	unsigned int value = n & bit;

	if (value)
	{
		cout << "#";
	}
	else
	{
		cout << " ";
	}

	Print(n, shift - 1);
}

void Process1()
{
	unsigned int count = 0;
	string lineCount;
	string line[2];
	getline(cin, lineCount);
	getline(cin, line[0]);
	getline(cin, line[1]);

	line[0] = line[0].substr(1, line[0].length() - 2);
	line[1] = line[1].substr(1, line[1].length() - 2);

	count = atoi(lineCount.c_str());
	vector<int> ArrayNum[2];
	string s;
	for (size_t i = 0; i < 2; i++)
	{
		istringstream f(line[i]);
		while (getline(f, s, ',')) {

			int value = atoi(s.c_str());
			ArrayNum[i].push_back(value);
		}
	}
	for (size_t i = 0; i < count; i++)
	{
		unsigned int value = ArrayNum[0][i] | ArrayNum[1][i];
		cout << "[";
		Print(value, count-1);
		cout << "]\n";
	}
}



string* findmap(int n, int* arr1, int* arr2)
{
	int* arr3 = new int[n];
	string* result = new string[n];
	for (int i = 0; i < n; i++)
	{
		arr3[i] = arr1[i] | arr2[i];

		result[i] = bitset<16>(arr3[i]).to_string();
		result[i] = result[i].substr(16 - n, n);

		for (int len = 0; len < result[i].length() ; len++)
		{
			result[i][len] = result[i][len] == '1' ? '#' : ' ';
		}
	}

	delete[] arr3;
	return nullptr;
}

int main()
{	
	int n = 5;
	int arr1[] = { 9,20,28,18,11 };
	int arr2[] = { 30,1,2,17,28 };

	return 0;
}