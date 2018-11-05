// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	OscillationDuration = 0.12f; // ����ũ Ÿ��
	OscillationBlendInTime = 0.05f; // ī�޶� ���� �����Ҷ� ���� �ð�.
	OscillationBlendOutTime = 0.05f; // ������ �����ð�

	RotOscillation.Pitch.Amplitude = 10.0f;	//ũ��
	RotOscillation.Pitch.Frequency = 1.0f;	//Ƚ��
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero;	// ���۰�
}
