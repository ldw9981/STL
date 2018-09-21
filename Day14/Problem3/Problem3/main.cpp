#include <iostream>
#include <string>
#include <conio.h>

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

// https://www.falstad.com/dotproduct/ 내적 시뮬레이션
bool CheckCollisionDetect( D3DXVECTOR2& PosP1, D3DXVECTOR2& PosP2, D3DXVECTOR2& PosC, const float& RadiusC)
{
	// 두점이 원 내부에 있는지 검사
	float RadiusCSQ = RadiusC * RadiusC;
	if (D3DXVecDistanceSq(&PosP1, &PosC) <= RadiusCSQ)
		return true;

	if (D3DXVecDistanceSq(&PosP2, &PosC) <= RadiusCSQ)
		return true;	
	
	D3DXVECTOR2 vecP1ToP2 = PosP2 - PosP1;
	float LineLength = D3DXVec2Length(&vecP1ToP2);
	D3DXVECTOR2 LineDirection;
	D3DXVec2Normalize(&LineDirection,&vecP1ToP2);


	// 라인의 방향벡터를 기준으로 90~270 도 즉 뒤쪽에 있는지 검사 
	D3DXVECTOR2 vecP1ToCircle = PosC - PosP1;
	float P1ToCircleLength = D3DXVec2Length(&vecP1ToCircle);
	D3DXVECTOR2 CircleDirection;
	D3DXVec2Normalize(&CircleDirection, &vecP1ToCircle);

	float Angle = D3DXVec2Dot(&CircleDirection, &LineDirection);	// DotResult = |a|*|b|* cos@ , 길이는 모두 1이므로 결과는 cos@ 
	if (Angle < 0)
		return false;

	// 수선의길이를 구하여 반지름보다 크면 충돌 아님.    
	float ShadowLength = P1ToCircleLength * Angle;					// |A| * cos@ = 그림자길이
	float result = sqrt(P1ToCircleLength*P1ToCircleLength - ShadowLength* ShadowLength);	
	if ( result > RadiusC) 
		return false;

	return true;
}




int main()
{
	D3DXVECTOR2 posP1(0.0f,0.0f);
	D3DXVECTOR2 posP2(10.0f,0.0f);

	D3DXVECTOR2 posC1(5.0f, -4.9f);
	float RadiusC1 = 5.0f;
	bool result;
	result = CheckCollisionDetect(posP1, posP2, posC1, RadiusC1);

	return 0;
}