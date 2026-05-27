// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraLookAheadComponent.h"

UCameraLookAheadComponent::UCameraLookAheadComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

  m_pRelativeMousePos = FVector::ZeroVector;
  m_fLookAheadMaxDistance = 0.f;
}


// Called when the game starts
void UCameraLookAheadComponent::BeginPlay()
{
	Super::BeginPlay();
}

//void UCameraLookAheadComponent::CalculateLookAheadOffset()
//{
//
//  //Get mouse direction and distance:
//  m_vMouseDirection = FVector2D(m_pRelativeMousePos.X, m_pRelativeMousePos.Y);
//  float fMouseDistance = m_vMouseDirection.Size();
//
//  float fDeadZone = 50.f;
//
//  // Si el rat�n est� dentro del "dead zone", la c�mara no se mueve
//  if (fMouseDistance < fDeadZone)
//  {
//    m_vLookAheadOffset = FVector::ZeroVector;
//    return;
//  }
//
//  // Clampa la distancia para que nunca supere el m�ximo
//  float ClampedDistance = FMath::Min(fMouseDistance, m_fLookAheadMaxDistance);
//
//  // Normaliza la direcci�n del rat�n (si no est� ya normalizada)
//  FVector2D vNormalizedDir = m_vMouseDirection.GetSafeNormal();
//
//  // Multiplica la direcci�n por la distancia clamped para obtener el offset
//  FVector2D vOffset2D = vNormalizedDir * ClampedDistance;
//
//  // Devuelve el offset con Z en 0 (solo plano XY)
//  m_vLookAheadOffset = FVector(vOffset2D.X, vOffset2D.Y, 0.0f);
//}

void UCameraLookAheadComponent::CalculateLookAheadOffset()
{
  //If activates for 4 cardinal direction:
  if (m_bLookIn4Directions)
  {
    FourDirectionsLookAt();
  }
  else
  {
    NormalLookAt();
  }
}

void UCameraLookAheadComponent::StopLookAheadIfAligned(const FVector& Direction)
{
  FVector2D Dir2D(Direction.X, Direction.Y);
  if (FVector2D::DotProduct(m_vMouseDirection, Dir2D) > 0.7f)
  {
    StopLookAhead();
  }
}

void UCameraLookAheadComponent::StopLookAhead()
{
  m_vLookAheadOffset = FVector::ZeroVector;
}

void UCameraLookAheadComponent::FourDirectionsLookAt()
{
  // If is the old look at:
  // Get mouse direction and distance:
  m_vMouseDirection = FVector2D(m_pRelativeMousePos.X, m_pRelativeMousePos.Y);
  float fMouseDistance = m_vMouseDirection.Size();

  float fDeadZone = 50.f;

  // Si est� dentro del dead zone, no hacer offset
  if (fMouseDistance < fDeadZone)
  {
    m_vLookAheadOffset = FVector::ZeroVector;
    return;
  }

  // Normaliza la direcci�n del rat�n
  FVector2D vDir = m_vMouseDirection.GetSafeNormal();

  // Cuantizar a la diagonal m�s cercana:
  FVector2D vQuantizedDir;
  if (vDir.X >= 0.f)
    vQuantizedDir.X = 1.f;
  else
    vQuantizedDir.X = -1.f;

  if (vDir.Y >= 0.f)
    vQuantizedDir.Y = 1.f;
  else
    vQuantizedDir.Y = -1.f;

  // Ahora la direcci�n es una de las 4: (1,1), (-1,1), (-1,-1), (1,-1)

  // Clampa distancia al m�ximo permitido
  float ClampedDistance = FMath::Min(fMouseDistance, m_fLookAheadMaxDistance);

  // Re-normaliza para mantener siempre misma longitud
  FVector2D vFinalDir = vQuantizedDir.GetSafeNormal();
  FVector2D vOffset2D = vFinalDir * ClampedDistance;

  // Offset en plano XY
  m_vLookAheadOffset = FVector(vOffset2D.X, vOffset2D.Y, 0.0f);
}

void UCameraLookAheadComponent::NormalLookAt()
{
  //Get mouse direction and distance:
  m_vMouseDirection = FVector2D(m_pRelativeMousePos.X, m_pRelativeMousePos.Y);
  float fMouseDistance = m_vMouseDirection.Size();

  float fDeadZone = 50.f;

  // Si est� dentro del dead zone, no hacer offset
  if (fMouseDistance < fDeadZone)
  {
    m_vLookAheadOffset = FVector::ZeroVector;
    return;
  }

  // Normaliza la direcci�n del rat�n
  FVector2D vDir = m_vMouseDirection.GetSafeNormal();

  // Cuantizar a la diagonal m�s cercana:
  FVector2D vQuantizedDir;
  if (vDir.X >= 0.f)
    vQuantizedDir.X = 1.f;
  else
    vQuantizedDir.X = -1.f;

  if (vDir.Y >= 0.f)
    vQuantizedDir.Y = 1.f;
  else
    vQuantizedDir.Y = -1.f;

  // Ahora la direcci�n es una de las 4: (1,1), (-1,1), (-1,-1), (1,-1)

  // Clampa distancia al m�ximo permitido
  float ClampedDistance = FMath::Min(fMouseDistance, m_fLookAheadMaxDistance);

  // Re-normaliza para mantener siempre misma longitud
  FVector2D vFinalDir = vQuantizedDir.GetSafeNormal();
  FVector2D vOffset2D = vFinalDir * ClampedDistance;

  // Offset en plano XY
  m_vLookAheadOffset = FVector(vOffset2D.X, vOffset2D.Y, 0.0f);
}

FVector UCameraLookAheadComponent::GetLookAheadOffset()
{
  return m_vLookAheadOffset;
}

void UCameraLookAheadComponent::SetTargetPosition(const FVector& RelativeMousePos)
{
  m_pRelativeMousePos = RelativeMousePos;
}
