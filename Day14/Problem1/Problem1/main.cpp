#include <iostream>

#include <vector>
#include <string>
#include <conio.h>

using namespace std;
int main()
{
	vector<int> numbers;
	int count;
	
	cin >> count;
	for (int i = 0; i < count; i++)
	{
		int value;
		cin >> value;
		numbers.push_back(value);
	}

	int countPrint = 0;	
	for (int i = 0; i < count; i++)
	{
		countPrint |= 0x01 << i;
	}
	
	for (int i = 0; i <= countPrint; i++)
	{
		string line;
		for (int bitShift = 0; bitShift < count; bitShift++)
		{
			int mask = 0x01 << bitShift;
			int value = i & mask;
			
			if (value)
			{	
				if (!line.empty())
				{
					line.append(",");
				}
				line.append(to_string(numbers[bitShift]));
			}
		}

		if (!line.empty())
			cout << line << endl;
	}
	
	while(_getch()!=0)
	return 0;
}