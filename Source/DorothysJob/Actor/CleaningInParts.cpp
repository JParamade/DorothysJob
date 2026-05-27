// Fill out your copyright notice in the Description page of Project Settings.


#include "CleaningInParts.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"

ACleaningInParts::ACleaningInParts()
  :m_fDuration(0),
  m_fRotation(0),
  m_vPivot(FVector2D::ZeroVector),
  m_vSize(FVector2D::ZeroVector),
  m_iIterador(0)
{
  m_pSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
  SetRootComponent(m_pSceneComponent);
  m_pDrawing = CreateDefaultSubobject<UDrawing>("Drawing component");
}

void ACleaningInParts::Init(TArray<UTexture*> _textures, float _duration, float _rotation, FVector2D _size, FVector2D _pivot)
{
  m_oTexture = _textures;
  m_fDuration = _duration;
  m_fRotation = _rotation;

  m_vPivot = _pivot;
  m_vSize = _size;

  CleanPart();
}

void ACleaningInParts::CleanPart()
{
  if (!IsValid(m_pDrawing))
  {
    Destroy();
    return;
  }

  if (m_oTexture.Num() <= m_iIterador)
  {
    m_iIterador = 0;
    Destroy();
    return;
  }
  if (IsValid(m_oTexture[m_iIterador]))
  {
    m_pDrawing->Clean(m_oTexture[m_iIterador], m_vSize, m_fRotation, m_vPivot);
  }

  m_iIterador++;
  float time = m_fDuration / static_cast<float>(m_oTexture.Num());
  FTimerHandle handle;
  GetWorld()->GetTimerManager().SetTimer(handle, this, &ACleaningInParts::CleanPart, time);
}
