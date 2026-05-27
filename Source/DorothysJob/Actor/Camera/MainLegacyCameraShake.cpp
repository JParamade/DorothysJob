// Fill out your copyright notice in the Description page of Project Settings.


#include "MainLegacyCameraShake.h"
#include "Shakes/PerlinNoiseCameraShakePattern.h"

UMainLegacyCameraShake::UMainLegacyCameraShake()
{
  //OscillationDuration = 0.5f;
  //OscillationBlendInTime = 0.1f;
  //OscillationBlendOutTime = 0.2f;

  //RotOscillation.Pitch.Amplitude = 3.0f;
  //RotOscillation.Pitch.Frequency = 3.0f;
  //RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
  //RotOscillation.Pitch.Waveform = EOscillatorWaveform::PerlinNoise;

  //LocOscillation.Y.Amplitude = 3.0f;
  //LocOscillation.Y.Frequency = 3.0f;
  //LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
  //LocOscillation.Y.Waveform = EOscillatorWaveform::PerlinNoise;

  //LocOscillation.Z.Amplitude = 3.0f;
  //LocOscillation.Z.Frequency = 3.0f;
  //LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
  //LocOscillation.Z.Waveform = EOscillatorWaveform::PerlinNoise;
}

void UMainLegacyCameraShake::ApplySettingsFromDataAsset()
{
  OscillationDuration = 0.5f;
  OscillationBlendInTime = 0.1f;
  OscillationBlendOutTime = 0.2f;

  RotOscillation.Pitch.Amplitude = 3.0f;
  RotOscillation.Pitch.Frequency = 3.0f;
  RotOscillation.Pitch.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
  RotOscillation.Pitch.Waveform = EOscillatorWaveform::PerlinNoise;

  LocOscillation.Y.Amplitude = 3.0f;
  LocOscillation.Y.Frequency = 3.0f;
  LocOscillation.Y.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
  LocOscillation.Y.Waveform = EOscillatorWaveform::PerlinNoise;

  LocOscillation.Z.Amplitude = 3.0f;
  LocOscillation.Z.Frequency = 3.0f;
  LocOscillation.Z.InitialOffset = EInitialOscillatorOffset::EOO_OffsetRandom;
  LocOscillation.Z.Waveform = EOscillatorWaveform::PerlinNoise;
}
