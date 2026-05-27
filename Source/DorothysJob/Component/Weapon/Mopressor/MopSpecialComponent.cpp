#include "MopSpecialComponent.h"
#include "DorothysJob/Data/DataAssets/Weapon/MoppresorWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Weapon/Mopressor/Mopressor.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Component/Action/Attack/DamageCollider.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"
#include "DorothysJob/Manager/BaseGameInstance.h"

void UMopSpecialComponent::Use()
{
  if (m_bHasShoot || !IsValid(m_pDataAsset))
  {
    return;
  }

  if (m_pPlayerOwner.IsValid() && !(m_pPlayerOwner->GetMitigation() > 0.f) && !m_pPlayerOwner->IsInvulnerable())
  {
    m_pPlayerOwner->SetIsInvulnerable(true);
  }

  m_fCurrentWhirlCharge += m_pDataAsset->m_fWhirlChargeRate;

  if (m_pWeaponOwner.IsValid())
  {
    if (m_pDataAsset->m_pSpecialChargeEffect)
    {
      m_pWeaponOwner->StartAttachedVFX(m_pDataAsset->m_pSpecialChargeEffect, FVector::ZeroVector);
    }

    if (m_fCurrentWhirlCharge >= m_pDataAsset->m_fWhirlCharge)
    {
      m_pWeaponOwner->StopAttachedVFX(true);
      m_pWeaponOwner->OnHasSpecialAlreadyShot.Broadcast();
      m_bHasShoot = true;
    }
  }
}

void UMopSpecialComponent::SpecialExplosion()
{
  if (!m_pPlayerOwner.IsValid() || !IsValid(m_pDamageCollider) || !IsValid(m_pDataAsset) ||
    FMath::IsNearlyZero(m_pDataAsset->m_fWhirlCharge) || !m_pWeaponOwner.IsValid() || !IsValid(m_pDrawingComponent))
  {
    return;
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->SetMopSAAchievement();
    }
  }

  m_pWeaponOwner->SetCameraShake(m_pDataAsset->m_oSpecialAttackCameraShake);
  m_pWeaponOwner->StartCameraShake();
  m_pWeaponOwner->StopControllerRumble();
  m_pWeaponOwner->StartControllerRumble(
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_fIntensity,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_fDuration,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftLarge,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftSmall,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightLarge,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightSmall);

  m_pPlayerOwner->SetIsInvulnerable(false);
  m_pDamageCollider->OnActive(true);

  m_fCurrentWhirlCharge = FMath::Clamp(m_fCurrentWhirlCharge, 0, m_pDataAsset->m_fWhirlCharge);
  float fAlphaLerp = m_fCurrentWhirlCharge / m_pDataAsset->m_fWhirlCharge;
  float fDamageRadius = FMath::Lerp(m_pDataAsset->m_fWaveDamageRangeMin, m_pDataAsset->m_fWaveDamageRangeMax, fAlphaLerp);
  float fCleanRadius = FMath::Lerp(m_pDataAsset->m_fWaveCleanRangeMin, m_pDataAsset->m_fWaveCleanRangeMax, fAlphaLerp);

  m_pWeaponOwner->CreateWaveVFX(fDamageRadius, fCleanRadius, fAlphaLerp);

  if (IsValid(m_pDataAsset->m_pCleanTexture))
  {
    m_pDrawingComponent->Clean(m_pDataAsset->m_pCleanTexture, FVector2D(fCleanRadius * 2.f), 0.f, FVector2D(0.5f), m_pPlayerOwner->GetActorLocation());
  }

  TArray<AActor*> lEnemiesInRange;
  m_pDamageCollider->SetSphereRadius(fDamageRadius);
  m_pDamageCollider->OnOverlap(lEnemiesInRange);
  for (AActor* Enemy : lEnemiesInRange)
  {
    if (!IsValid(Enemy))
    {
      continue;
    }

    IDamageable* DamageableActor = Cast<IDamageable>(Enemy);
    if (DamageableActor)
    {
      m_pWeaponOwner->ApplyDamage(DamageableActor, m_pDataAsset->m_fSpecialAttackDamage, m_pDataAsset->m_eWeaponDirtType, false);
    }
    if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(Enemy))
    {
      m_pWeaponOwner->ApplySpecialStatusEffects(pCharacter);
    }
  }

  m_fCurrentWhirlCharge = 0.0f;
  m_pDamageCollider->OnActive(false);
  m_pWeaponOwner->OnSpecialActive.Broadcast(false);
  m_pWeaponOwner->OnSpecialStart.Broadcast();
}

void UMopSpecialComponent::SetHasShoot(bool _shot)
{
  m_bHasShoot = _shot;
}

void UMopSpecialComponent::SetDamageCollider(ADamageCollider* _DamageCollider)
{
  m_pDamageCollider = _DamageCollider;
}

void UMopSpecialComponent::BeginPlay()
{
  Super::BeginPlay();

  if (AMopressor* Mopressor = Cast<AMopressor>(GetOwner()))
  {
    m_pWeaponOwner = Mopressor;
    m_pDrawingComponent = m_pWeaponOwner->GetDrawingComponent();
    m_pPlayerOwner = Cast<ABasePlayer>(m_pWeaponOwner->GetOwner());

    if (UMoppresorWeaponStatsDataAsset* DataAsset = Cast<UMoppresorWeaponStatsDataAsset>(m_pWeaponOwner->GetDataAsset().Get()))
    {
      m_pDataAsset = DataAsset;
    }
  }
}