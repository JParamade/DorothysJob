// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionTrigger.h"
#include "Components/ShapeComponent.h"
#include "Components/BoxComponent.h"

ACollisionTrigger::ACollisionTrigger()
  : m_bOnTriggerExit(false)
{
  m_pCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
  RootComponent = m_pCollision;
}

void ACollisionTrigger::BeginPlay()
{
  m_pCollision->OnComponentBeginOverlap.AddDynamic(this, &ACollisionTrigger::OnOverlapEnter);
  m_pCollision->OnComponentEndOverlap.AddDynamic(this, &ACollisionTrigger::OnOverlapExit);
}

void ACollisionTrigger::OnOverlapEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  /*GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("On trigger enter"));*/
  if (m_bOnTriggerExit)
  {
    return;
  }

  ActivateTrigger();
}

void ACollisionTrigger::OnOverlapExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  /*GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("On trigger exid"));*/
  if (!m_bOnTriggerExit)
  {
    return;
  }

  ActivateTrigger();
}
