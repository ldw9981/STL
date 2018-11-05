// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.12f; // 셰이크 타임
	OscillationBlendInTime = 0.05f; // 카메라 흔들기 시작할때 블렌딩 시간.
	OscillationBlendOutTime = 0.05f; // 끝날떄 블렌딩시간

	RotOscillation.Pitch.Amplitude = 10.0f;	//크기
	RotOscillation.Pitch.Frequency = 1.0f;	//횟수
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;	// 시작값
}
