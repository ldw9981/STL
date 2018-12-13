// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCameraShake.h"

UMyCameraShake::UMyCameraShake()
{
	//OscillationDuration = 5.0f; //����ũ Ÿ��
	//OscillationBlendInTime = 0.5f; //ī�޶� ���� ���� �Ҷ� ���� �ð�
	//OscillationBlendOutTime = 0.5f; //ī�޶� ���� �� �Ҷ� ���� �ð�

	//RotOscillation.Pitch.Amplitude = 50.0f;  // ũ�� 
	//RotOscillation.Pitch.Frequency = 20.0f; //Ƚ��
	//RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom; // ���� ��

	//RotOscillation.Yaw.Amplitude = 30.0f;  // ũ�� 
	//RotOscillation.Yaw.Frequency = 20.0f; //Ƚ��
	//RotOscillation.Yaw.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom; // ��

	//RotOscillation.Roll.Amplitude = 100.0f;  // ũ�� 
	//RotOscillation.Roll.Frequency = 10.0f; //Ƚ��
	//RotOscillation.Roll.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom; // ��


	OscillationDuration = 0.12f; //����ũ Ÿ��
	OscillationBlendInTime = 0.05f; //ī�޶� ���� ���� �Ҷ� ���� �ð�
	OscillationBlendOutTime = 0.05f; //ī�޶� ���� �� �Ҷ� ���� �ð�

	RotOscillation.Pitch.Amplitude = 10.0f;  // ũ�� 
	RotOscillation.Pitch.Frequency = 1.0f; //Ƚ��
	RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetZero; // ���� ��
}
