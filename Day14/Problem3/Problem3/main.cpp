#include <iostream>
#include <string>
#include <conio.h>

using namespace std;
float EPSILON = 0.001f;



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

	float DotResult = D3DXVec2Dot(&CircleDirection, &LineDirection);
	if (DotResult < 0)
		return false;

	// �����Ǳ��̸� ���Ͽ� ���������� ũ�� �浹 �ƴ�.     (*LenghSq�� ���ϸ� SQRT ���Ű���)
	float P1ToResultLength = LineLength * (DotResult*DotResult);
	float result = sqrt(P1ToCircleLength*P1ToCircleLength - P1ToResultLength* P1ToResultLength);	
	if (fabs(result - RadiusC) > EPSILON)
		return false;

	return true;
}

int intersection(float x, float y, float r, float a, float b, float c, float d, float xy[][2])
{
	float m, n;

	// A,B1,C ���� �������κ��� ������� 2���������� �����
	// D: �Ǻ���
	// X,Y: ������ ��ǥ
	float A, B1, C, D;
	float X, Y;

	// A,B1,C,D�Ի�
	if (c != a)
	{
		// m, n���
		m = (d - b) / (c - a);
		n = (b*c - a * d) / (c - a);

		A = m * m + 1;
		B1 = (m*n - m * y - x);
		C = (x*x + y * y - r * r + n * n - 2 * n*y);
		D = B1 * B1 - A * C;

		if (D < 0)
			return 0;
		else if (D == 0)
		{
			X = -B1 / A;
			Y = m * X + n;
			xy[0][0] = X;
			xy[0][1] = Y;
			return 1;
		}
		else
		{
			X = -(B1 + sqrt(D)) / A;
			Y = m * X + n;
			xy[0][0] = X;
			xy[0][1] = Y;

			X = -(B1 - sqrt(D)) / A;
			Y = m * X + n;
			xy[1][0] = X;
			xy[1][1] = Y;
			return 2;
		}
	}
	else
	{
		// a == c �� ���� �������̹Ƿ�
		// ���� �������� a >= (x-r) && a <=(x+r) )
		// (a-x)*(a-x)
		// 1. ���� ���� ���
		// a < (x-r) || a > (x+r)

		// ���� ����
		if (a < (x - r) || a >(x + r))
			return 0;
		// �ϳ��� �߱�
		else if (a == (x - r) || a == (x + r))
		{
			X = a;
			Y = y;
			xy[0][0] = X;
			xy[0][1] = Y;

			return 1;
		}
		// �ΰ��� ��
		else
		{
			// x = a�� �����Ͽ� Y�� ���Ͽ� Ǯ��
			X = a;
			Y = y + sqrt(r*r - (a - x)*(a - x));
			xy[0][0] = X;
			xy[0][1] = Y;

			Y = y - sqrt(r*r - (a - x)*(a - x));
			xy[1][0] = X;
			xy[1][1] = Y;

			return 2;
		}
	}
}




int main()
{

	float xy[2][2];
	D3DXVECTOR2 posP1(0.0f,0.0f);
	D3DXVECTOR2 posP2(10.0f,0.0f);

	D3DXVECTOR2 posC1(5.0f, 5.0f);
	float RadiusC1 = 5.0f;

	D3DXVECTOR2 posC2(20.0f, 20.0f);
	float RadiusC2 = 1.0f;


	bool result;
	
	result = CheckCollisionDetect(posP1, posP2, posC1, RadiusC1);
	result = CheckCollisionDetect(posP1, posP2, posC2, RadiusC2);
	

	int a;
	a = intersection(posC1.x, posC1.y, RadiusC1, posP1.x, posP1.y, posP2.x, posP2.y, xy);
	a = intersection(posC2.x, posC2.y, RadiusC2, posP1.x, posP1.y, posP2.x, posP2.y, xy);
	return 0;
}