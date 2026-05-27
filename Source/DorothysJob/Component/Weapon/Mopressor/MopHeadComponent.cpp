#include "MopHeadComponent.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Actor/Weapon/Mopressor/Mopressor.h"
#include "DorothysJob/Data/DataAssets/Weapon/MoppresorWeaponStatsDataAsset.h"
#include "DorothysJob/Component/Action/Attack/DamageCollider.h"
#include "Components/ShapeComponent.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"

UMopHeadComponent::UMopHeadComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
}

void UMopHeadComponent::Use()
{
  if (!m_pOwnerWeapon.IsValid() || !IsValid(m_pDataAsset) || !IsValid(m_pDrawingComponent))
  {
    return;
  }

  // Set speed to slowed value.
  if (!m_bIsCleaning)
  {
    m_bIsCleaning = true;
    m_pOwnerWeapon->SetPlayerVelocity(true, m_pDataAsset->m_fMovementCleaningMultiplier, 1.f);
    m_pOwnerWeapon->OnCleanStart.Broadcast();
  }

  float Cleaning = m_pDrawingComponent->GetNotCleaningPercentage();

  if (IsValid(m_pDataAsset->m_pCleanTexture))
  {
    m_pDrawingComponent->Clean(m_pDataAsset->m_pCleanTexture, m_pDataAsset->m_v2SCleaningSize, m_fAngle, m_pDataAsset->m_v2SCleaningPivot, CalculateCleaningLocation());
  }

  m_fAngle += 30.0f;
  m_pOwnerWeapon->NotifyCleaningLiquid(Cleaning);
  m_pOwnerWeapon->OnMopressorCleanUse.Broadcast();

  if (m_pDataAsset->m_pCleanEffect)
  {
    m_pOwnerWeapon->StartAttachedVFX(m_pDataAsset->m_pCleanEffect, FVector(100, 0, 0));
  }
}

void UMopHeadComponent::Release()
{
  if (!m_pOwnerWeapon.IsValid() || !IsValid(m_pDataAsset))
  {
    return;
  }

  // Return speed to normal value.
  if (m_bIsCleaning)
  {
    m_pOwnerWeapon->SetPlayerVelocity(false, m_pDataAsset->m_fMovementCleaningMultiplier, 1.f);
    m_pOwnerWeapon->OnCleanEnd.Broadcast();
  }

  m_bIsCleaning = false;
  if (m_bIsDashing)
  {
    ActivateCollider(false);
  }
  m_pOwnerWeapon->StopAttachedVFX(false);
}

void UMopHeadComponent::DashStatus(bool _bActivate)
{
  if (!m_bIsCleaning)
  {
    return;
  }

  ActivateCollider(_bActivate);
}

void UMopHeadComponent::SetCollider(ADamageCollider* Collider)
{
  m_pDashCollider = Collider;
}

void UMopHeadComponent::BeginPlay()
{
  Super::BeginPlay();

  if (AMopressor* pWeapon = Cast<AMopressor>(GetOwner()))
  {
    m_pOwnerWeapon = pWeapon;
  }

  if (!m_pOwnerWeapon.IsValid() || !m_pOwnerWeapon->GetDataAsset().IsValid())
  {
    return;
  }

  if (ABasePlayer* Player = Cast<ABasePlayer>(m_pOwnerWeapon->GetOwner()))
  {
    m_pOwnerPlayer = Player;
  }

  if (UMoppresorWeaponStatsDataAsset* pDataAsset = Cast<UMoppresorWeaponStatsDataAsset>(m_pOwnerWeapon->GetDataAsset().Get()))
  {
    m_pDataAsset = pDataAsset;
  }

  if (UDrawing* pDrawing = m_pOwnerWeapon->GetDrawingComponent())
  {
    m_pDrawingComponent = pDrawing;
  }
}

void UMopHeadComponent::OnDashColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!m_pOwnerWeapon.IsValid() || !IsValid(OtherActor))
  {
    return;
  }

  if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(OtherActor))
  {
    m_pOwnerWeapon->ApplyCleanStatusEffects(pCharacter);
  }
}

void UMopHeadComponent::ActivateCollider(bool _bActivate)
{
  if (!IsValid(m_pDashCollider))
  {
    return;
  }

  TArray<UShapeComponent*> lSphereColliders = m_pDashCollider->GetSphereColliders();
  for (UShapeComponent* pSphereCollider : lSphereColliders)
  {
    if (!IsValid(pSphereCollider))
    {
      continue;
    }

    _bActivate ? pSphereCollider->OnComponentBeginOverlap.AddDynamic(this, &UMopHeadComponent::OnDashColliderBeginOverlap) : pSphereCollider->OnComponentBeginOverlap.RemoveDynamic(this, &UMopHeadComponent::OnDashColliderBeginOverlap);
  }

  m_bIsDashing = _bActivate;
  m_pDashCollider->OnActive(_bActivate);
}

FVector UMopHeadComponent::CalculateCleaningLocation() const
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pOwnerPlayer->GetSkeletalMesh()))
  {
    return FVector::ZeroVector;
  }

  return m_pOwnerPlayer->GetActorLocation() + m_pOwnerPlayer->GetSkeletalMesh()->GetForwardVector() * 85.f;
}