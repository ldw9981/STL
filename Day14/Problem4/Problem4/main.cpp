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

int SortQuickPivot(vector<int>& Array,int BeginIndex,int EndIndex)
{
	int LeftIndex=BeginIndex;
	int RightIndex=EndIndex-1;	
	
	while(LeftIndex != RightIndex)
	{
		if (Array[LeftIndex] > Array[EndIndex])
		{
			swap(Array[LeftIndex], Array[RightIndex]);
			RightIndex--;
		}
		else
		{
			LeftIndex++;
		}	
	}

	if (Array[LeftIndex] > Array[EndIndex])
	{
		swap(Array[LeftIndex], Array[EndIndex]);
	}

	
	PrintArray(Array);
	for (size_t i = 0; i < Array.size(); i++)
	{
		if (i == LeftIndex)
		{
			cout << 'P';
		}
		else if (i == BeginIndex)
		{
			cout << 'B';
		}
		else if (i == EndIndex)
		{
			cout << 'E';
		}
		else
		{
			cout << '_';
		}
	}
	cout << endl;
	/*
	if (selectionIndex != endIndex)
	{
		swap(Array[endIndex], Array[selectionIndex]);
	}
	*/
	return LeftIndex;
}

void SortQuick(vector<int>& Array,int begin,int end )
{
	if (begin >= end)
		return;

	int PivotIndex = SortQuickPivot(Array, begin,end);
	// 여기까지 피봇위치를 중심으로 왼쪽은 피봇값보다 작은것들  오른쪽은 큰것들로 구분이 되어있어야한다.(완벽한정렬이 아님)
	SortQuick(Array, begin, PivotIndex - 1);
	SortQuick(Array, PivotIndex + 1, end);
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

	PrintArray(Data);
	SortQuick(Data, 0, Data.size() - 1);

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