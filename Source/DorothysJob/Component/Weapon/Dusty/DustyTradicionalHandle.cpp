#include "DustyTradicionalHandle.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/Dusty/Dusty.h"
#include "DorothysJob/Interface/Damageable.h"
#include "DorothysJob/Component/Action/Attack/DamageCollider.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/DustyWeaponStatsDataAsset.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "FMODEvent.h"
#include "NiagaraFunctionLibrary.h"

void UDustyTradicionalHandle::BeginPlay()
{
  Super::BeginPlay();

  m_pOwner = Cast<ADusty>(GetOwner());
  InitDustyComponents();
}

void UDustyTradicionalHandle::DashStatus(bool _bActivate)
{
  if (!_bActivate && m_pPlayer.IsValid())
  {
    m_pPlayer->OnDashModeChange.RemoveAll(this);
    m_pPlayer->ChangeMovementVelocity(1.0f, 1 / 0.002f);
  }
}

void UDustyTradicionalHandle::Use()
{
  if (!m_pPlayer.IsValid() || !m_pOwner.IsValid() || !IsValid(m_pStatsDataAsset))
  {
    UE_LOG(LogTemp, Error, TEXT("PLAYER NOT FOUND IN DustyTraditionalHandle.cpp"));
    return;
  }

  // Broadcast Sound (parameter doesn't affect which sound is played).
  m_pOwner->OnAttackStart.Broadcast(EAttackType::BASE_ATTACK);

  //If its a normal combo:
  if (m_pOwner->CurrentComboHit() != m_pStatsDataAsset->m_oDamage.Num() - 1)
  {
    //Activate mini collider:
    if (IsValid(m_pMiniHitDamageCollider))
    {
      m_pMiniHitDamageCollider->OnActive(true);
      m_pMiniHitDamageCollider->DrawDebug(FColor::Red);
    }

    //Show VFX:
    if (m_pPlayer.IsValid() && IsValid(m_pPlayer->GetMuzzle()) && IsValid(m_pStatsDataAsset->m_pAttackComboInitialEffect))
    {
      UNiagaraFunctionLibrary::SpawnSystemAttached(
        m_pStatsDataAsset->m_pAttackComboInitialEffect,
        m_pPlayer->GetMuzzle(),
        NAME_None,
        m_pStatsDataAsset->m_vBaseHitOffset,
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        true);
    }
  }
  else
  {
    //Activate mega collider:
    if (IsValid(m_pMegaHitDamageCollider))
    {
      m_pMegaHitDamageCollider->OnActive(true);
      m_pMegaHitDamageCollider->DrawDebug(FColor::Blue);
    }

    //Show VFX:
    if (IsValid(m_pStatsDataAsset->m_pAttackComboEndEffect) && IsValid(m_pPlayer->GetMuzzle()))
    {
      UNiagaraFunctionLibrary::SpawnSystemAttached(
        m_pStatsDataAsset->m_pAttackComboEndEffect,
        m_pPlayer->GetMuzzle(),
        NAME_None,
        m_pStatsDataAsset->m_vPunchHitOffset,
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        true);
    }

    //Perform Dash:
    m_pPlayer->ChangeMovementVelocity(1.0f, 0.002f);
    if (IsValid(m_pPlayer->GetSkeletalMesh()))
    {
      m_pPlayer->PerformDash(m_pPlayer->GetSkeletalMesh()->GetForwardVector(), m_pStatsDataAsset->m_fFistDashDistance, m_pStatsDataAsset->m_fFistDashTime);
    }
    m_pPlayer->OnDashModeChange.AddUObject(this, &UDustyTradicionalHandle::DashStatus);

    // Start Camera Shake:
    if (IsValid(m_pStatsDataAsset->m_oFinalAttackCameraShake))
    {
      m_pOwner->SetCameraShake(m_pStatsDataAsset->m_oFinalAttackCameraShake);
      m_pOwner->StartCameraShake();
      m_pOwner->StopControllerRumble();
      m_pOwner->StartControllerRumble(
        m_pStatsDataAsset->m_oFinalAttackVibration.m_fIntensity,
        m_pStatsDataAsset->m_oFinalAttackVibration.m_fDuration,
        m_pStatsDataAsset->m_oFinalAttackVibration.m_bAffectsLeftLarge,
        m_pStatsDataAsset->m_oFinalAttackVibration.m_bAffectsLeftSmall,
        m_pStatsDataAsset->m_oFinalAttackVibration.m_bAffectsRightLarge,
        m_pStatsDataAsset->m_oFinalAttackVibration.m_bAffectsRightSmall);
    }
  }
}

void UDustyTradicionalHandle::MakeDash()
{
  if (!m_pPlayer.IsValid() || !IsValid(m_pPlayer->GetMovementComponent()) || !IsValid(m_pStatsDataAsset))
  {
    return;
  }

  //DASH
  if (UFloatingPawnMovement* pFloatingMovement = Cast<UFloatingPawnMovement>(m_pPlayer->GetMovementComponent()))
  {
    m_pPlayer->PerformDash(pFloatingMovement->Velocity.GetSafeNormal(), m_pStatsDataAsset->m_fDistanceComboDash, m_pStatsDataAsset->m_fDurationComboDash);
  }
}

void UDustyTradicionalHandle::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
  Super::EndPlay(EndPlayReason);
}

void UDustyTradicionalHandle::InitDustyComponents()
{
  //Get data asset:
  m_pStatsDataAsset = Cast<UDustyWeaponStatsDataAsset>(m_pOwner->GetDataAsset().Get());

  //Get Player:
  m_pPlayer = Cast<ABasePlayer>(m_pOwner->GetOwner());

  // Check if the player has a valid Muzzle component:
  USceneComponent* Muzzle = m_pPlayer->GetMuzzle();

  if (!IsValid(Muzzle))
  {
    UE_LOG(LogTemp, Error, TEXT("Player has no valid Muzzle component in DustyTradicionalHandle.cpp"));
    return;
  }

  // Generate the colliders:
  if (m_pOwner.IsValid())
  {
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = m_pOwner.Get();

    FVector Location = m_pOwner->GetActorLocation();
    FRotator Rotator = m_pOwner->GetActorRotation();

    // Spawn the collider:
    if(UWorld* CachedWorld = GetWorld())
    {
      m_pMiniHitDamageCollider = CachedWorld->SpawnActor<ADamageCollider>(m_pMiniDustyDamageColliderClass, Location, Rotator, SpawnParams);
      m_pMegaHitDamageCollider = CachedWorld->SpawnActor<ADamageCollider>(m_pMegaDustyDamageColliderClass, Location, Rotator, SpawnParams);
    }
  }

  // If there is no collider, don't do a thing, just return.
  if (!m_pMiniHitDamageCollider || !m_pMegaHitDamageCollider)
  {
    UE_LOG(LogTemp, Error, TEXT("There are no colliders, soy the can't be set in DustyTradicionalHandle.cpp"));
    return;
  }

  float fCollidersPosition = m_pStatsDataAsset->m_vMiniAttackColliderSize.X / 2.f + 50;
  m_pMiniHitDamageCollider->Init(Muzzle);
  m_pMiniHitDamageCollider->UpdateLocation(FVector(fCollidersPosition, 0.0f, 0.0f));
  m_pMiniHitDamageCollider->m_lColliders[0].m_pCollider->OnComponentBeginOverlap.AddDynamic(this, &UDustyTradicionalHandle::OnBeginOverlap);
  m_pMiniHitDamageCollider->OnActive(false);

  fCollidersPosition = m_pStatsDataAsset->m_vMegaAttackColliderSize.X / 2.f;
  m_pMegaHitDamageCollider->Init(Muzzle);
  m_pMegaHitDamageCollider->UpdateLocation(FVector(fCollidersPosition, 0.0f, 0.0f));
  m_pMegaHitDamageCollider->m_lColliders[0].m_pCollider->OnComponentBeginOverlap.AddDynamic(this, &UDustyTradicionalHandle::OnBeginOverlap);
  m_pMegaHitDamageCollider->OnActive(false);
}

void UDustyTradicionalHandle::AttackApplyDamage(int _iDamage, AActor* _DamagedActor, FVector _ImpactPoint)
{
  if (!IsValid(_DamagedActor))
  {
    return;
  }

  if (!m_lEnemiesToDamage.Contains(_DamagedActor) && m_pOwner.IsValid() && IsValid(m_pStatsDataAsset))
  {
    IDamageable* DamageableActor = Cast<IDamageable>(_DamagedActor);
    if (DamageableActor)
    {
      m_pOwner->ApplyDamage(DamageableActor, _iDamage, m_pStatsDataAsset->m_eWeaponDirtType, true, _ImpactPoint);

      if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(_DamagedActor)) {
        m_pOwner->OnMeleeHit.Broadcast(pCharacter->GetMitigation(), !(m_pOwner->CurrentComboHit() < 2));
      }
    }
  }
}

void UDustyTradicionalHandle::AttackFinish()
{
  if (!m_pOwner.IsValid() || !IsValid(m_pStatsDataAsset) ||
    m_pStatsDataAsset->m_oDamage.IsEmpty() || !IsValid(m_pMiniHitDamageCollider))
  {
    return;
  }

  // Deactivate colliders and camera shake
  if (m_pOwner->CurrentComboHit() != m_pStatsDataAsset->m_oDamage.Num() - 1)
  {
    MakeDash();
    m_pMiniHitDamageCollider->OnActive(false);
  }
  else
  {
    m_pMegaHitDamageCollider->OnActive(false);
  }
  m_lEnemiesToDamage.Empty();
}


void UDustyTradicionalHandle::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!IsValid(OtherActor) || !m_pPlayer.IsValid())
  {
    return;
  }

  if (m_pOwner->CurrentComboHit()<2)
  {
    m_pOwner->SetCameraShake(m_pStatsDataAsset->m_oDamageCameraShake);
    m_pOwner->StartCameraShake();
    m_pOwner->StopControllerRumble();
    m_pOwner->StartControllerRumble(
      m_pStatsDataAsset->m_oDamageVibrationParams.m_fIntensity,
      m_pStatsDataAsset->m_oDamageVibrationParams.m_fDuration,
      m_pStatsDataAsset->m_oDamageVibrationParams.m_bAffectsLeftLarge,
      m_pStatsDataAsset->m_oDamageVibrationParams.m_bAffectsLeftSmall,
      m_pStatsDataAsset->m_oDamageVibrationParams.m_bAffectsRightLarge,
      m_pStatsDataAsset->m_oDamageVibrationParams.m_bAffectsRightSmall);
  }

  //Get the current combo damage:
  int32 iDamage = 0;
  if (IsValid(m_pStatsDataAsset))
  {
    int32 iCurrentCombo = m_pOwner->CurrentComboHit();
    iDamage = (m_pOwner->IsInCombo() && m_pStatsDataAsset->m_oDamage.IsValidIndex(iCurrentCombo)) ? m_pStatsDataAsset->m_oDamage[iCurrentCombo] : 0;
  }

  //Apply damage:
  FVector vDirection = m_pPlayer->GetActorLocation() - OtherActor->GetActorLocation();
  vDirection.Z = 0;
  vDirection = vDirection.GetSafeNormal() * 80;
  vDirection.Z = FMath::RandRange(50, 120);

  AttackApplyDamage(iDamage, OtherActor, OtherActor->GetActorLocation() + vDirection);
  m_lEnemiesToDamage.AddUnique(OtherActor);
}


