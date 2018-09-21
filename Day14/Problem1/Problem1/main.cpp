#include <iostream>

#include <vector>
#include <string>
#include <conio.h>
#include <sstream>

using namespace std;

void Parse(const string& InputLine, const string& delimiters, vector<string>& wordVector)
{
	stringstream stringStream(InputLine);
	std::string line;

	while (std::getline(stringStream, line))
	{
		std::size_t prev = 0, pos;
		while ((pos = line.find_first_of(delimiters.c_str(), prev)) != std::string::npos)
		{
			if (pos > prev)
				wordVector.push_back(line.substr(prev, pos - prev));
			prev = pos + 1;
		}
		if (prev < line.length())
			wordVector.push_back(line.substr(prev, std::string::npos));
	}
}

int main()
{
	string inputLine;
	vector<string> wordVector;
	
	cout << "입력" << endl;
	getline(cin, inputLine);
	Parse(inputLine, ",", wordVector);

	cout << "출력" << endl;
	int count = wordVector.size();	
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
				line.append(wordVector[bitShift]);
			}
		}

		if (!line.empty())
			cout << line << endl;
	}
	
	while(_getch()!=0)
	return 0;
}