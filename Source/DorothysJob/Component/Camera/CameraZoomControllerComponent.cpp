// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraZoomControllerComponent.h"
#include "DorothysJob/Actor/Camera/BaseCamera.h"

// Sets default values for this component's properties
UCameraZoomControllerComponent::UCameraZoomControllerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

  m_fOrthoWidth = 1800.0f;
}

void UCameraZoomControllerComponent::AdjustCameraZoom(float DistanceToHit)
{
  // Interp speed: cuánto más alto, más rápido interpola
  float InterpSpeed = 5.0f;

  // Calcular el target ortho width basado en la distancia
  if (LastDistanceToDoor < 0.0f)
  {
    LastDistanceToDoor = DistanceToHit;
  }

  float MinZoom = 800.f;
  float MaxZoom = 1800.f;

  float Delta = LastDistanceToDoor - DistanceToHit;

  if (FMath::Abs(Delta) > KINDA_SMALL_NUMBER)
  {
    float TargetOrthoWidth = FMath::Clamp(m_fOrthoWidth - Delta * 10, MinZoom, MaxZoom);
    m_fOrthoWidth = FMath::FInterpTo(m_fOrthoWidth, TargetOrthoWidth, GetWorld()->GetDeltaSeconds(), InterpSpeed);
    LastDistanceToDoor = DistanceToHit;
  }
}

float UCameraZoomControllerComponent::GetOrthoWidth()
{
  return m_fOrthoWidth;
}

void UCameraZoomControllerComponent::SetOrthoWidth(float _OrthoWidth)
{
  m_fOrthoWidth = _OrthoWidth;
}

