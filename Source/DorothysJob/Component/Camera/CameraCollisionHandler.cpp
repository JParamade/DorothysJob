// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraCollisionHandler.h"
#include "CameraLookAheadComponent.h"
#include "CameraZoomControllerComponent.h"
#include "CameraFogOfWarComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Actor/Camera/BaseCamera.h"

// Sets default values for this component's properties
UCameraCollisionHandler::UCameraCollisionHandler()
{
	PrimaryComponentTick.bCanEverTick = true;

  m_fMarginDistance = 1000.f;
  m_fDoorDistance = 300.f;
}

void UCameraCollisionHandler::BeginPlay()
{
  Super::BeginPlay();

  AActor* Owner = GetOwner();
  if (!Owner) return;

  m_pCameraOwner = Cast<ABaseCamera>(Owner);
  if (!m_pCameraOwner) return;
}

void UCameraCollisionHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCameraCollisionHandler::CalculateDoorCollision()
{
  m_vTargetPosition = m_pCameraOwner->GetTargetActor()->GetActorLocation();

  //The four directions of the line trace, to get if you are near 1 of teh four walls:
  TArray<FVector> vDirections = {
      FVector(-1, 0, 0), // Left
      FVector(1, 0, 0),  // Right
      FVector(0, -1, 0), // Back
      FVector(0, 1, 0)   // Forward
  };

  //For each direction do
  for (const FVector& vDir : vDirections)
  {
    //Get start and end of the line trace.
    //The line trace only detect the new channel call "Border"
    //Every door nedds to have the collision Border Block for this linetrace to detected.
    FVector Start = m_vTargetPosition;
    FVector End = Start + vDir * m_fMarginDistance;
    FHitResult Hit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_GameTraceChannel3);

    UPrimitiveComponent* HitComponent = Hit.GetComponent();

    if (HitComponent != nullptr)
    {
      if (HitComponent->ComponentHasTag("Door"))
      {
        
        HandleDoorHit(Hit, vDir);
        m_bIsDoorHit = true;
      }
    }
    else
    {
      m_bIsDoorHit = false;
    }
  }
}

void UCameraCollisionHandler::HandleDoorHit(const FHitResult& Hit, const FVector& Direction)
{
  // Calculate the zoom adjustment:
  float DistanceToHit = FVector::Dist(m_vTargetPosition, Hit.ImpactPoint);
  FVector DoorOrigin = Hit.GetActor()->GetActorLocation();
  FVector DoorForward = Hit.GetActor()->GetActorForwardVector();

  m_fCurrentDot = FVector::DotProduct(m_vTargetPosition - DoorOrigin, DoorForward);

  if (DistanceToHit <= m_fDoorDistance)
  {
    m_fDoorZoomDistance = DistanceToHit;
  }
}

float UCameraCollisionHandler::GetCurrentDot()
{
  return m_fCurrentDot;
}

float UCameraCollisionHandler::GetDoorZoomDistance()
{
  return m_fDoorZoomDistance;
}

bool UCameraCollisionHandler::GetDoorHit()
{
  CalculateDoorCollision();
  return m_bIsDoorHit;
}