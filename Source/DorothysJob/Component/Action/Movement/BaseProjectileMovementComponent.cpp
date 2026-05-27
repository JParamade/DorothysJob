
#include "BaseProjectileMovementComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "AIController.h"

UBaseProjectileMovementComponent::UBaseProjectileMovementComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
}

void UBaseProjectileMovementComponent::BeginPlay()
{
  Super::BeginPlay();
}

void UBaseProjectileMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  AActor* Owner = GetOwner();

  if (!Owner) return;

  FVector NewLocation = Owner->GetActorLocation() + m_vVelocity * DeltaTime;
  FHitResult Hit;
  Owner->SetActorLocation(NewLocation, true, &Hit); // enable sweep to detect collisions while moving

  if (m_bCanPush)
  {
    PushActor(Hit, DeltaTime);
  }

  //THIS NEEDS TO BE CHANGE
  m_fElapsedTime += DeltaTime;
  if (m_fLifeTime > 0 && m_fElapsedTime >= m_fLifeTime)
  {
    GetOwner()->Destroy();
  }
}

void UBaseProjectileMovementComponent::PushActor(FHitResult Hit, float DeltaTime)
{
  UPrimitiveComponent* HitComp = Hit.GetComponent();
  if (Hit.IsValidBlockingHit() && HitComp)
  {
    AActor* HitActor = Hit.GetActor();
    if (HitActor && HitActor->IsA<ABaseEnemy>())
    {
      if (AAIController* AI = Cast<AAIController>(HitActor->GetInstigatorController()))
      {
        AI->StopMovement();
      }

      UFloatingPawnMovement* MovementComp = HitActor->FindComponentByClass<UFloatingPawnMovement>();
      if (MovementComp)
      {
        FVector CurrentVelocity = MovementComp->Velocity;
        FVector DesiredVelocity = m_vVelocity.GetSafeNormal() * PushStrength;
        MovementComp->Velocity = FMath::VInterpTo(CurrentVelocity, DesiredVelocity, DeltaTime, 30.0f);
      }
    }
  }
}

FVector UBaseProjectileMovementComponent::GetVelocity()
{
  return FVector();
}

void UBaseProjectileMovementComponent::SetVelocity(FVector _vVelocity)
{
  m_vVelocity = _vVelocity;
}

void UBaseProjectileMovementComponent::SetCanPush(bool _bCanPush)
{
  m_bCanPush = _bCanPush;
}