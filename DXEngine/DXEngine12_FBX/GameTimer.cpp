#include "GameTimer.h"
#include <Windows.h>


GameTimer::GameTimer()
{
}


GameTimer::~GameTimer()
{
}

void GameTimer::StartTime()
{
	LARGE_INTEGER frequencyCount;	
	QueryPerformanceFrequency(&frequencyCount);			// �ϵ���� ������ ����	
	countsPerSecond = (double)frequencyCount.QuadPart;	// �ʴ� ������ ����
	// ƽ  ( tick = frame )
	QueryPerformanceCounter(&frequencyCount);
	countStart = frequencyCount.QuadPart;
}

double GameTimer::GetTime() const
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	double time = (double)((currentTime.QuadPart - countStart) / countsPerSecond);
	return time;
}

// ƽź�� ���� �ð� ���ϱ�
double GameTimer::DeltaTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	__int64 tickCount = currentTime.QuadPart - prevFrameTime;	
	prevFrameTime = currentTime.QuadPart; //���� �����ӽð� ����
	//����

	if (tickCount < 0)
		tickCount = 0;

	// �����ӽð��� ��(����)�ð����� ��ȯ.
	double deltaTime = (double)(tickCount / countsPerSecond);
	if (deltaTime < 0.0)
		deltaTime = 0;

	return deltaTime;
}

void GameTimer::UpdateTimer()
{	
	++frameCount;
	if (GetTime() > 1.0f)
		Reset();
}

void GameTimer::Reset()
{
	fps = frameCount;
	frameCount = 0;
	StartTime();
}

int GameTimer::GetFPS()
{
	return frameCount;
}
