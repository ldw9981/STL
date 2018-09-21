#include <iostream>


#include <conio.h>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>


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

class Command
{
public:
	Command(int InitStart = 0, int InitEnd = 0, int InitSearchTarget = 0)
	{
		Start= InitStart;
		End= InitEnd;
		SearchTarget= InitSearchTarget;
	}
	~Command()
	{
	}
	
	int Start;
	int End;
	int SearchTarget;
};

void PrintArray(vector<int>& Array)
{
	for (size_t i = 0; i < Array.size(); i++)
	{
		cout << Array[i];
	}
	cout << endl;
	while (_getch() != 0);
}
void Sort(vector<int>& Array)
{
	for (size_t i = 0; i < Array.size(); i++)
	{		
		int endIndex = Array.size() - i -1;
		int selectionIndex = endIndex;
		for (int j = 0; j < endIndex; j++)
		{
			if (Array[j] > Array[selectionIndex])
			{
				selectionIndex = j;
			}
		}

		if (selectionIndex != endIndex)
		{
			swap(Array[endIndex], Array[selectionIndex]);
		}
		//PrintArray(Array);
	}
}


vector<int> solution(vector<int> array,const vector<Command>& commands)
{
	vector<int> answer;	
	for (size_t i = 0; i < commands.size(); i++)
	{
		int StartIndex = commands[i].Start - 1;
		int ReserveSize = commands[i].End - commands[i].Start + 1;		
		vector<int> SortArray(ReserveSize);
		copy(array.begin()+ StartIndex, array.begin()+ StartIndex + ReserveSize, SortArray.begin());
		Sort(SortArray);

		answer.push_back(SortArray[commands[i].SearchTarget - 1]);
	}
	return answer;
}

int main()
{
	vector<int> Data;
	vector<Command> Commands;

	string inputLine;
	vector<string> wordVector;

	// DAta
	wordVector.clear();
	getline(cin, inputLine);
	Parse(inputLine, " ,[]", wordVector);
	for (size_t i = 0; i < wordVector.size(); i++)
	{
		Data.push_back(atoi(wordVector[i].c_str()));
	}

	//Command
	wordVector.clear();
	getline(cin, inputLine);
	Parse(inputLine, " ,[]", wordVector);

	size_t count = wordVector.size() / 3;
	for (size_t i = 0; i < count; i++)
	{
		Command temp;
		temp.Start = atoi(wordVector[i*3 + 0].c_str());
		temp.End = atoi(wordVector[i*3 + 1].c_str());
		temp.SearchTarget = atoi(wordVector[i*3 + 2].c_str());

		Commands.push_back(temp);
	}

	vector<int> result = solution(Data, Commands);



	string resultText;
	

	for (size_t i = 0; i < result.size(); i++)
	{
		if (!resultText.empty())
		{
			resultText.append(string(" ,"));
		}
		string temp = to_string(result[i]);
		resultText.append(temp);
	}
	
	cout << "[" << resultText << "]" << endl;
 
	while (_getch() != 0);
	return 0;
}