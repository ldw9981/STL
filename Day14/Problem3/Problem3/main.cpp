#include <iostream>
#include <string>
#include <conio.h>
#include <sstream>
#include <vector>

using namespace std;


class D3DXVECTOR2
{
public:
	float x, y;
	D3DXVECTOR2()
	{
	}
	D3DXVECTOR2(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
	D3DXVECTOR2 operator -(const D3DXVECTOR2 &v)
	{
		return D3DXVECTOR2(x - v.x, y - v.y);
	}

	void operator -=(const D3DXVECTOR2 &v)
	{
		x -= v.x;
		y -= v.y;
	}
	D3DXVECTOR2 operator +(const D3DXVECTOR2 &v)
	{
		return D3DXVECTOR2(x + v.x, y + v.y);
	}
	void operator +=(const D3DXVECTOR2 &v)
	{
		x += v.x;
		y += v.y;
	}
	D3DXVECTOR2 operator *(const D3DXVECTOR2 &v)
	{
		return D3DXVECTOR2(x * v.x, y * v.y);
	}
	D3DXVECTOR2 operator *(const float &a)
	{
		return D3DXVECTOR2(x * a, y * a);
	}
	D3DXVECTOR2 operator *=(const float &a)
	{
		return D3DXVECTOR2(x * a, y * a);
	}
	D3DXVECTOR2 operator /(const float &a)
	{
		if (x != 0.0f) {
			return D3DXVECTOR2(x / a, y / a);
		}
		else {
			return D3DXVECTOR2(x, y);
		}
	}
	void operator /=(const D3DXVECTOR2 &v)
	{
		x /= v.x;
		y /= v.y;
	}
	void operator /=(const float &a)
	{
		x /= a;
		y /= a;
	}
	bool operator ==(const D3DXVECTOR2 &v)
	{
		return (v.x == x) && (v.y == y);
	}
};

float D3DXVecDistanceSq(const D3DXVECTOR2 *a, const D3DXVECTOR2 *b)
{
	D3DXVECTOR2 c;
	c.x = a->x - b->x;
	c.y = a->y - b->y;
	return c.x * c.x + c.y * c.y;
}

/**
 * @brief D3DXVec2Length
 * @param pV
 * @return
 */
float D3DXVec2Length(const D3DXVECTOR2 *pV)
{
	return	sqrtf(pV->x * pV->x +
		pV->y * pV->y);
}
float D3DXVec2LengthSq(const D3DXVECTOR2 *pV)
{
	return	pV->x * pV->x +
		pV->y * pV->y;
}

float D3DXVec2Dot(const D3DXVECTOR2 *a, const D3DXVECTOR2 *b)
{
	return (a->x * b->x +
		a->y * b->y);
}

D3DXVECTOR2 *D3DXVec2Normalize(D3DXVECTOR2 *pOut, const D3DXVECTOR2 *pV)
{

	float length = (pV->x * pV->x + pV->y * pV->y);

	if (length > 0.0f) {
		length = sqrtf(length);
		pOut->x = pV->x / length;
		pOut->y = pV->y / length;
		return pOut;
	}
	else {
		return pOut;
	}
}

D3DXVECTOR2 *D3DXVec2Normalize(D3DXVECTOR2 *pOut)
{
	float length = (pOut->x * pOut->x + pOut->y * pOut->y);

	if (length > 0.0f) {
		length = sqrtf(length);
		pOut->x /= length;
		pOut->y /= length;
		return pOut;
	}
	else {
		return pOut;
	}
}

// https://www.falstad.com/dotproduct/ ���� �ùķ��̼�
bool CheckCollisionDetect( D3DXVECTOR2& PosP1, D3DXVECTOR2& PosP2, D3DXVECTOR2& PosC, const float& RadiusC)
{
	// ������ �� ���ο� �ִ��� �˻�
	float RadiusCSQ = RadiusC * RadiusC;
	if (D3DXVecDistanceSq(&PosP1, &PosC) <= RadiusCSQ)
		return true;

	if (D3DXVecDistanceSq(&PosP2, &PosC) <= RadiusCSQ)
		return true;	
	
	D3DXVECTOR2 vecP1ToP2 = PosP2 - PosP1;
	float LineLength = D3DXVec2Length(&vecP1ToP2);
	D3DXVECTOR2 LineDirection;
	D3DXVec2Normalize(&LineDirection,&vecP1ToP2);


	// ������ ���⺤�͸� �������� 90~270 �� �� ���ʿ� �ִ��� �˻� 
	D3DXVECTOR2 vecP1ToCircle = PosC - PosP1;
	float P1ToCircleLength = D3DXVec2Length(&vecP1ToCircle);
	D3DXVECTOR2 CircleDirection;
	D3DXVec2Normalize(&CircleDirection, &vecP1ToCircle);

	float Angle = D3DXVec2Dot(&CircleDirection, &LineDirection);	// DotResult = |a|*|b|* cos@ , ���̴� ��� 1�̹Ƿ� ����� cos@ 
	if (Angle < 0)
		return false;

	// �����Ǳ��̸� ���Ͽ� ���������� ũ�� �浹 �ƴ�.    
	float ShadowLength = P1ToCircleLength * Angle;					// |A| * cos@ = �׸��ڱ���
	float resultHeightLength = sqrt(P1ToCircleLength*P1ToCircleLength - ShadowLength* ShadowLength);	
	if ( resultHeightLength > RadiusC) 
		return false;

	// ����� ����.
	float resultPiercedLength = sqrt(RadiusCSQ - resultHeightLength* resultHeightLength);
	if (LineLength < (ShadowLength - resultPiercedLength))   // ������ ���̰� �� �������� �ʴ´�.
		return false;

	return true;
}


void Parse(const string& InputLine,const string& delimiters,vector<string>& wordVector)
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

	D3DXVECTOR2 posP1(0.0f, 0.0f);
	D3DXVECTOR2 posP2(10.0f, 10.0f);
	D3DXVECTOR2 posC1(5.0f, 5.0f);
	float RadiusC1 = 5.0f;
	vector<string> wordVector;
	string inputLine;
	bool result;
	for (size_t i = 0; i < 2; i++)
	{
		cout << endl << "�Է�" << endl;

		wordVector.clear();
		getline(cin, inputLine);
		Parse(inputLine, " ,[]", wordVector);
		posP1.x = (float)atoi(wordVector[0].c_str());
		posP1.y = (float)atoi(wordVector[1].c_str());

		wordVector.clear();
		getline(cin, inputLine);
		Parse(inputLine, " ,[]", wordVector);
		posP2.x = (float)atoi(wordVector[0].c_str());
		posP2.y = (float)atoi(wordVector[1].c_str());

		wordVector.clear();
		getline(cin, inputLine);
		Parse(inputLine, " ,[]", wordVector);
		posC1.x = (float)atoi(wordVector[0].c_str());
		posC1.y = (float)atoi(wordVector[1].c_str());
		RadiusC1 = (float)atoi(wordVector[2].c_str());

		cout << endl << "���" << endl;
		result = CheckCollisionDetect(posP1, posP2, posC1, RadiusC1);
		if (result)
		{
			cout << "�浹" << endl;
		}
		else
		{
			cout << "�浹x" << endl;
		}
	}


	while (_getch() != 0);
	return 0;
}