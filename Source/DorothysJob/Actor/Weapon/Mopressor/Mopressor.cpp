#include "Mopressor.h"
// COMPONENTS
#include "DorothysJob/Component/Weapon/Mopressor/MopHeadComponent.h"
#include "DorothysJob/Component/Weapon/Mopressor/MopHandleComponent.h"
#include "DorothysJob/Component/Weapon/Mopressor/MopSpecialComponent.h"
// ACTOR
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Animations/Player/PlayerAnimInstance.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/MoppresorWeaponStatsDataAsset.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DorothysJob/Component/Action/Attack/DamageCollider.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Component/Weapon/Mopressor/MoppressorWeaponAudioComponent.h"
#include "FMODEvent.h"
#include "FMODAudioComponent.h"

AMopressor::AMopressor()
{
  // Components
  m_pMopHead = CreateDefaultSubobject<UMopHeadComponent>(TEXT("MopHead"));
  m_pHandle = CreateDefaultSubobject<UMopHandleComponent>(TEXT("MopHandle"));
  m_pSpecial = CreateDefaultSubobject<UMopSpecialComponent>(TEXT("MopSpecial"));
  m_pWeaponAudioComponent = CreateDefaultSubobject<UMoppressorWeaponAudioComponent>(TEXT("Weapon audio component"));
  m_pDrawing = CreateDefaultSubobject<UDrawing>(TEXT("Drawing component"));
  m_pBasicAttackPivot = CreateDefaultSubobject<USceneComponent>(TEXT("BasicAttackPivot"));
}

void AMopressor::BeginPlay()
{
  Super::BeginPlay();

  m_pMopressorDataAsset = Cast<UMoppresorWeaponStatsDataAsset>(m_pWeaponDataAsset);
  AttachToPlayer();
}

void AMopressor::AttachToPlayer()
{
  m_pOwnerPlayer = Cast<ABasePlayer>(GetOwner());

  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDamageColliderClass) || !IsValid(m_pLastHitColliderClass) ||
    !IsValid(m_pSpecialDamageColliderClass) || !IsValid(m_pCleanDashColliderClass) || !IsValid(m_pBasicAttackPivot))
  {
    return;
  }

  if (USceneComponent* Muzzle = m_pOwnerPlayer->GetMuzzle())
  {
    m_pBasicAttackPivot->AttachToComponent(Muzzle, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  }

  if (UWorld* pCachedWorld = GetWorld())
  {
    FActorSpawnParameters Params;
    Params.Owner = this;

    FVector Location = GetActorLocation();
    FRotator Rotator = GetActorRotation();

    m_pDamageCollider = pCachedWorld->SpawnActor<ADamageCollider>(m_pDamageColliderClass, Location, Rotator, Params);
    m_pLastHitCollider = pCachedWorld->SpawnActor<ADamageCollider>(m_pLastHitColliderClass, Location, Rotator, Params);
    m_pSpecialDamageCollider = pCachedWorld->SpawnActor<ADamageCollider>(m_pSpecialDamageColliderClass, Location, Rotator, Params);
    m_pCleanDashCollider = pCachedWorld->SpawnActor<ADamageCollider>(m_pCleanDashColliderClass, Location, Rotator, Params);

    InitColliderComponent(m_pDamageCollider, m_pBasicAttackPivot);
    InitColliderComponent(m_pLastHitCollider, m_pOwnerPlayer->GetRootComponent());
    InitColliderComponent(m_pSpecialDamageCollider, m_pOwnerPlayer->GetRootComponent());
    InitColliderComponent(m_pCleanDashCollider, m_pOwnerPlayer->GetRootComponent());

    if (IsValid(m_pHandle))
    {
      m_pHandle->SetDamageCollider(m_pDamageCollider, m_pLastHitCollider);
      m_pHandle->SetAttackPivot(m_pBasicAttackPivot);
    }

    if (IsValid(m_pMopHead) && IsValid(m_pCleanDashCollider))
    {
      m_pMopHead->SetCollider(m_pCleanDashCollider);
      m_pOwnerPlayer->OnDashModeChange.AddUObject(m_pMopHead, &UMopHeadComponent::DashStatus);
    }

    if (IsValid(m_pSpecial))
    {
      m_pSpecial->SetDamageCollider(m_pSpecialDamageCollider);
    }
  }
}

void AMopressor::InitColliderComponent(ADamageCollider* _DamageColliderComponent, USceneComponent* _RootComponent)
{
  if (!IsValid(_DamageColliderComponent) || !IsValid(_RootComponent))
  {
    return;
  }

  _DamageColliderComponent->Init(_RootComponent);
  _DamageColliderComponent->OnActive(false);
}

#pragma region | Getters
ADamageCollider* AMopressor::GetDamageCollider() const
{
  return m_pDamageCollider;
}

ADamageCollider* AMopressor::GetSpecialDamageCollider() const
{
  return m_pSpecialDamageCollider;
}

UDrawing* AMopressor::GetDrawingComponent() const
{
  return m_pDrawing;
}

UMoppresorWeaponStatsDataAsset* AMopressor::GetMoppressorDataAsset() const
{
  return m_pMopressorDataAsset;
}

ADamageCollider* AMopressor::GetCleanDashCollider() const
{
  return m_pCleanDashCollider;
}
#pragma endregion

#pragma region | Actions
void AMopressor::ReleaseAttack()
{
  OnMopressorAttackRelease.Broadcast();

  if (!IsValid(m_pHandle) || !IsValid(m_pMopressorDataAsset))
  {
    return;
  }

  m_pHandle->ChangedWeapon();

  if (m_pHandle->HasReductionMovility())
  {
    m_pHandle->SetReductionMovility(false);
    SetPlayerVelocity(m_pHandle->HasReductionMovility(), m_pMopressorDataAsset->m_fMovementAttackMultiplier);
  }

  if (m_pHandle->HasReductionRotation())
  {
    m_pHandle->SetReductionRotation(false);
    SetPlayerVelocity(m_pHandle->HasReductionRotation(), 1.0f, m_pMopressorDataAsset->m_fRotationAttackMultiplier);
  }
}

void AMopressor::MeleeAttackHitStart(int32 _iCurrentCombo)
{
  Super::MeleeAttackHitStart(_iCurrentCombo);

  if (!IsValid(m_pHandle) || !IsValid(m_pSpecial) || !IsValid(m_pMopressorDataAsset))
  {
    return;
  }

  if (!m_bSpecialOngoing)
  {
    if (!m_pHandle->HasReductionRotation())
    {
      m_pHandle->SetReductionRotation(true);
      SetPlayerVelocity(m_pHandle->HasReductionRotation(), 1.f, m_pMopressorDataAsset->m_fRotationAttackMultiplier);
    }

    if (!m_pHandle->HasReductionMovility())
    {
      m_pHandle->SetReductionMovility(true);
      SetPlayerVelocity(m_pHandle->HasReductionMovility(), m_pMopressorDataAsset->m_fMovementAttackMultiplier);
    }

    m_pHandle->Use();
  }
  else
  {
    m_pSpecial->SpecialExplosion();
  }
}

void AMopressor::MeleeAttackHitEnd()
{
  Super::MeleeAttackHitEnd();

  if (!IsValid(m_pHandle) || !IsValid(m_pSpecial) || !IsValid(m_pMopressorDataAsset))
  {
    return;
  }

  if (!m_bSpecialOngoing)
  {
    m_pHandle->DeactivateCollider();
    if (m_pHandle->HasReductionRotation() && IsValid(m_pMopressorDataAsset))
    {
      m_pHandle->SetReductionRotation(false);
      SetPlayerVelocity(m_pHandle->HasReductionRotation(), 1.f, m_pMopressorDataAsset->m_fRotationAttackMultiplier);
    }
  }
  else
  {
    m_bSpecialOngoing = false;
    m_pSpecial->SetHasShoot(false);
  }
}

void AMopressor::Clean()
{
  if (IsValid(m_pMopHead))
  {
    m_pMopHead->Use();
  }
}

void AMopressor::ReleaseClean()
{
  if (IsValid(m_pMopHead))
  {
    m_pMopHead->Release();
  }
}

void AMopressor::SpecialAttack()
{
  if (IsValid(m_pSpecial) && m_bCanUseSpecial)
  {
    if (!m_bReleasSpecialAttack)
    {
      SetReleaseSpecial(true);
      OnSpecialActive.Broadcast(true);
      m_bSpecialOngoing = true;
    }

    m_bCanUseSpecial = false;
    m_pSpecial->Use();

    if (UWorld* pCachedWorld = GetWorld())
    {
      if (!IsValid(m_pMopressorDataAsset))
      {
        return;
      }

      FTimerHandle SpecialTimerHandle;
      pCachedWorld->GetTimerManager().SetTimer(
        SpecialTimerHandle,
        [WeakThis = TWeakObjectPtr<AMopressor>(this)]()
        {
          if (WeakThis.IsValid())
          {
            WeakThis->m_bCanUseSpecial = true;
          }
        },
        m_pMopressorDataAsset->m_fSpecialAttackGrowthRate,
        false
      );
    }
  }
}

void AMopressor::ReleaseSpecialAttack()
{
  StopAttachedVFX(true);
}

#pragma endregion


#pragma region | VFX

void AMopressor::CreateWaveVFX(float _damageRadius, float _cleanRadius, float fAlphaLerp)
{
  if (!m_pMopressorDataAsset || !IsValid(m_pMopressorDataAsset->m_pWaveEffect) || !m_pOwnerPlayer.IsValid())
  {
    return;
  }

  FVector vPlayerLocation = m_pOwnerPlayer->GetActorLocation();

  float fSpriteSize = FMath::Lerp(700.f, 1700.f, fAlphaLerp);
  float fSpriteSizeSmall = FMath::Lerp(100.f, 1100.f, fAlphaLerp);
  float fLifeTime = FMath::Lerp(0.3f, 0.5f, fAlphaLerp);
  float fBubelIntSizeMin = FMath::Lerp(25.f, 75.f, fAlphaLerp);
  float fBubelIntSizeMax = FMath::Lerp(100.f, 150.f, fAlphaLerp);
  float fBubelExtSizeMin = FMath::Lerp(50.f, 150.f, fAlphaLerp);
  float fBubelExtSizeMax = FMath::Lerp(150.f, 300.f, fAlphaLerp);

  //@review ::--> this will auto destroy after its lifetime completes
  if (UWorld* pCachedWorld = GetWorld())
  {
    UNiagaraComponent* WaveSpecial = UNiagaraFunctionLibrary::SpawnSystemAtLocation(pCachedWorld, m_pMopressorDataAsset->m_pWaveEffect, vPlayerLocation - FVector(0.f, 0.f, 100.f), FRotator::ZeroRotator, FVector::OneVector, true, true);
    if (WaveSpecial)
    {
      WaveSpecial->SetFloatParameter(FName("Sprite Size Small"), fSpriteSizeSmall);
      WaveSpecial->SetFloatParameter(FName("Sprite Size"), fSpriteSize);
      WaveSpecial->SetFloatParameter(FName("LifeTimeWater"), fLifeTime);
      WaveSpecial->SetFloatParameter(FName("LifeTimeLead"), fLifeTime);
      WaveSpecial->SetFloatParameter(FName("LifeTimeBubel"), fLifeTime);
      WaveSpecial->SetFloatParameter(FName("Bubel Int Size Max"), fBubelIntSizeMax);
      WaveSpecial->SetFloatParameter(FName("Bubel Int Size Min"), fBubelIntSizeMin);
      WaveSpecial->SetFloatParameter(FName("Bubel Ext Size Max"), fBubelExtSizeMax);
      WaveSpecial->SetFloatParameter(FName("Bubel Ext Size Min"), fBubelExtSizeMin);
    }
#if WITH_EDITOR
    DrawDebugSphere(pCachedWorld, vPlayerLocation, _damageRadius, 16, FColor::Red, false, 1.5f, 0.0f, 2.0f);
    DrawDebugSphere(pCachedWorld, vPlayerLocation, _cleanRadius, 16, FColor::Yellow, false, 1.5f, 0.0f, 2.0f);
#endif
  }
}

void AMopressor::ComboStateChanged(bool _bStarts)
{
  Super::ComboStateChanged(_bStarts);

  if (!IsValid(m_pHandle))
  {
    return;
  }

  if (_bStarts)
  {
    if (m_pOwnerPlayer.IsValid())
    {
      m_pHandle->SetAttackSpeedModifier(m_pOwnerPlayer->GetAttackSpeedMultiplier());
    }
  }
  else
  {
    m_pHandle->SetAttackSpeedModifier(1.0f);
    if (m_pHandle->HasReductionMovility() && IsValid(m_pMopressorDataAsset))
    {
      m_pHandle->SetReductionMovility(false);
      SetPlayerVelocity(m_pHandle->HasReductionMovility(), m_pMopressorDataAsset->m_fMovementAttackMultiplier);
    }
  }
}

void AMopressor::StartAttachedVFX(UNiagaraSystem* _pNiagaraEffect, FVector _offset)
{
  if (!IsValid(m_pVFXNiagaraComponent))
  {
    if (m_pOwnerPlayer.IsValid() && IsValid(m_pOwnerPlayer->GetSkeletalMesh()) && IsValid(_pNiagaraEffect))
    {
      m_pVFXNiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(_pNiagaraEffect, m_pOwnerPlayer->GetSkeletalMesh(),
        NAME_None, _offset, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, false, false);
    }
  }

  if (IsValid(m_pVFXNiagaraComponent) && !m_pVFXNiagaraComponent->IsActive())
  {
    m_pVFXNiagaraComponent->ReinitializeSystem();
  }
}

void AMopressor::StopAttachedVFX(bool _immediate)
{
  if (IsValid(m_pVFXNiagaraComponent))
  {
    _immediate ? m_pVFXNiagaraComponent->DeactivateImmediate() : m_pVFXNiagaraComponent->Deactivate();
    m_pVFXNiagaraComponent = nullptr;
  }
}

TObjectPtr<UNiagaraComponent> AMopressor::CreateBasicAttackParticle(int32 _index)
{
  if (!IsValid(m_pMopressorDataAsset) || !m_pOwnerPlayer.IsValid() || !m_pMopressorDataAsset->m_lComboHitsParameters.IsValidIndex(_index)
    || !IsValid(m_pMopressorDataAsset->m_lComboHitsParameters[_index].m_lBasicVFXAttackEffect) || !IsValid(m_pOwnerPlayer->GetMuzzle()))
  {
    return nullptr;
  }

  return UNiagaraFunctionLibrary::SpawnSystemAttached(
    m_pMopressorDataAsset->m_lComboHitsParameters[_index].m_lBasicVFXAttackEffect, 
    m_pOwnerPlayer->GetMuzzle(),
    NAME_None, 
    m_pMopressorDataAsset->m_lComboHitsParameters[_index].m_fBasicVFXSpawnDistance, 
    FRotator(0.0f, -90.f, 0.0f),
    EAttachLocation::KeepRelativeOffset, 
    false, 
    false);
}
#pragma endregion

void AMopressor::SetPlayerVelocity(bool _bActivate, float _fMovementMultiplier, float _fRotationMultiplier) 
{
  if (!m_pOwnerPlayer.IsValid()) 
  {
    return;
  }

  if (FMath::IsNearlyZero(_fMovementMultiplier) || FMath::IsNearlyZero(_fRotationMultiplier))
  {
    UE_LOG(LogTemp, Error, TEXT("Moppresor SetPlayerVelocity: _fMovementMultiplier or _fRotationMultiplier is/are zero"));
    return;
  }

  float fMovementMultiplier = _bActivate ? _fMovementMultiplier : 1.f / _fMovementMultiplier;
  float fRotationMultiplier = _bActivate ? _fRotationMultiplier : 1.f / _fRotationMultiplier;

  m_pOwnerPlayer->ChangeMovementVelocity(fMovementMultiplier, fRotationMultiplier);
}