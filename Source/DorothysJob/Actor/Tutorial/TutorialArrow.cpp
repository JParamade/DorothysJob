// Fill out your copyright notice in the Description page of Project Settings.


#include "TutorialArrow.h"

ATutorialArrow::ATutorialArrow()
{
  // Crear el componente de malla
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

  // Crear la malla como hija
  m_pMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  m_pMeshComponent->SetupAttachment(RootComponent);
}

void ATutorialArrow::BeginPlay()
{
  Super::BeginPlay();

  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetVisibility(false);
  }
}

void ATutorialArrow::Active()
{
  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetVisibility(true);
  }
}

void ATutorialArrow::DesActive()
{
  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetVisibility(false);
  }
}

FName ATutorialArrow::GetArrowId()
{
  return m_sId;
}
