#include "RocketLauncherComponent.h"
#include "DorothysJob/Actor/Projectile/BaseProjectile.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/Dirtminator/Dirtminator.h"
#include "DorothysJob/Data/DataAssets/Weapon/DirtminatorWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Camera/BaseCamera.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void URocketLauncherComponent::BeginPlay()
{
  Super::BeginPlay();

  // Get the owner and the data asset.
  ADirtminator* pOwner = Cast<ADirtminator>(GetOwner());
  if (!IsValid(pOwner))
  {
    UE_LOG(LogTemp, Error, TEXT("[Rocket Launcher Component] Owner is not setted properly."));
    return;
  }

  m_pOwnerWeapon = pOwner;

  if (ABasePlayer* Player = Cast<ABasePlayer>(m_pOwnerWeapon->GetOwner()))
  {
    m_pOwnerPlayer = Player;
  }

  if (m_pOwnerWeapon->GetDataAsset().IsValid())
  {
    UDirtminatorWeaponStatsDataAsset* oDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(m_pOwnerWeapon->GetDataAsset().Get());

    if (oDataAsset)
    {
      m_pDataAsset = oDataAsset;
    }
  }

  // Get skeletal mesh and the owner material:
  m_pOwnerSkeletalMesh = m_pOwnerWeapon->GetMeshComponent();
  if (IsValid(m_pOwnerSkeletalMesh))
  {
    m_pDefaultOwnerMaterial = m_pOwnerSkeletalMesh->GetMaterial(0);
  }
  {
    UE_LOG(LogTemp, Error, TEXT("[Rocket Launcher Component] Owner skeletal mesh not found."));
  }

  m_oSpawnParams.Owner = m_pOwnerWeapon.Get();
  m_oSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

}

void URocketLauncherComponent::Use()
{
  // Prevent multible shots. If has already shot and the release is not call, dónt do anything.
  if (m_bHasShot || !m_pOwnerWeapon.IsValid())
  {
    return;
  }

  // Init firts tier if it´s first use.
  if (IsFirstUse())
  {
    m_pOwnerWeapon->StopControllerRumble();
    m_iCurrentTier = 0;
    m_bIsSlow = true;
    ManageChargeTiers();
  }

  // Set time with actual time.
  UpdateChargeTime();

  // Check if it needs to change tiers.
  CheckTierChange();

  // Activate VFX.
  if (IsValid(m_pDataAsset) && IsValid(m_pDataAsset->m_pAimVFX))
  {
    m_pOwnerWeapon->SetActiveAimVFX(true, m_pDataAsset->m_pAimVFX);
  }



  //Set camera shake and gamepad vibration.
  if (m_pOwnerPlayer.IsValid())
  {
    if (ABaseCamera* Camera = m_pOwnerPlayer->GetCameraComponent())
    {
      Camera->PlayShakeComponent();
    }
  }

  //Check if the charge reach the max time. and shoot if needed.
  CheckMaxChargeAndShoot();
}

void URocketLauncherComponent::Release()
{
  if (!m_pOwnerWeapon.IsValid() || !IsValid(m_pDataAsset))
  {
    return;
  }

  //If it does not do the max charging, meaning it is in tier 0, 1 or 2, shoot and do the release sake.
  if (m_iCurrentTier >= 0)
  {

    if (!m_bHasShot)
    {
      if (m_pDataAsset->m_oTierThresholds.IsValidIndex(m_iCurrentTier))
      {
        Shoot(m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_pProjectilClass);
      }

      m_pOwnerWeapon->OnAttackEnd.Broadcast(static_cast<EAttackType>(m_iCurrentTier));
    }

    m_iCurrentTier = -1;
  }

  //If it's finaly it has shot, set the has shot variable to false.
  if (m_bHasShot)
  {
    m_bHasShot = false;
  }
}

void URocketLauncherComponent::SetCooldownMultiplier(float _fCooldownMultiplier)
{
  m_fCooldownMultiplier = _fCooldownMultiplier;
}

bool URocketLauncherComponent::IsFirstUse()
{
  if (m_dLastTime < 0.0f)
  {
    m_dLastTime = FPlatformTime::Seconds();
    m_fChargeTime = 0.0f;
    return true;
  }
  return false;
}

void URocketLauncherComponent::UpdateChargeTime()
{
  //Get the current time
  double Current = FPlatformTime::Seconds();
  double Delta = (float)(Current - m_dLastTime);

  m_dLastTime = Current;
  m_fChargeTime += Delta;
}

void URocketLauncherComponent::CheckTierChange()
{
  if (!IsValid(m_pDataAsset))
  {
    return;
  }

  const int32 ArraySize = m_pDataAsset->m_oTierThresholds.Num();

  if (ArraySize == 0 || ArraySize - 1 <= m_iCurrentTier)
  {
    return;
  }

  // Check if we are not out of the tier array range:
  if (m_iCurrentTier >= m_pDataAsset->m_oTierThresholds.Num())
  {
    m_iCurrentTier = m_pDataAsset->m_oTierThresholds.Num() - 1;
    return;
  }

  if (!m_pDataAsset->m_oTierThresholds.IsValidIndex(m_iCurrentTier + 1))
  {
    return;
  }

  // Take the next tier time:
  float fnextTierTime = m_pDataAsset->m_oTierThresholds[m_iCurrentTier + 1].m_fThresholdTime * m_fCooldownMultiplier;

  // If charge time surpass next tier time and there are nex tiers available, change tier.
  if (m_fChargeTime > fnextTierTime && m_iCurrentTier < m_pDataAsset->m_oTierThresholds.Num() - 1)
  {
    if (m_bIsSlow)
    {
      SetPlayerVelocity(false);
    }
    else
    {
      m_bIsSlow = true;
    }

    m_iCurrentTier++;

    //If the tier has change manage the changes.
    ManageChargeTiers();
  }
}

void URocketLauncherComponent::ManageChargeTiers()
{
  //Set Camera Shake and Controller Rumble
  if (!IsValid(m_pDataAsset) || !m_pOwnerWeapon.IsValid() ||
    !m_pDataAsset->m_oTierThresholds.IsValidIndex(m_iCurrentTier))
  {
    return;
  }

  FGamePadVibrationParams oVibrationParams = m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_oChargeVibrationParams;
  m_pOwnerWeapon->SetCameraShake(m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_oChargeShakeBP);

  SetPlayerVelocity(true);
  SetDirtminatorTierVisualEffect(m_iCurrentTier);

  if (m_iCurrentTier == 0)
  {
    m_pOwnerWeapon->StartControllerRumble(
      oVibrationParams.m_fIntensity,
      oVibrationParams.m_fDuration,
      oVibrationParams.m_bAffectsLeftLarge,
      oVibrationParams.m_bAffectsLeftSmall,
      oVibrationParams.m_bAffectsRightLarge,
      oVibrationParams.m_bAffectsRightSmall);
    m_pOwnerWeapon->OnAttackStart.Broadcast(EAttackType::BASE_ATTACK);
  }
  else
  {
    m_pOwnerWeapon->UpdateControllerRumble(
      oVibrationParams.m_fIntensity,
      oVibrationParams.m_fDuration,
      oVibrationParams.m_bAffectsLeftLarge,
      oVibrationParams.m_bAffectsLeftSmall,
      oVibrationParams.m_bAffectsRightLarge,
      oVibrationParams.m_bAffectsRightSmall);
    m_pOwnerWeapon->OnAttackStart.Broadcast(static_cast<EAttackType>(m_iCurrentTier));
  }
}

void URocketLauncherComponent::Shoot(TSubclassOf<ABaseProjectile> _ProjectileClass)
{
  if (!m_pOwnerWeapon.IsValid())
  {
    return;
  }

  //Reset timers.
  m_dLastTime = -1.0f;
  float CurrentTime = m_fChargeTime;
  m_fChargeTime = 0.0f;

  //See if it has a prejectile class.
  if (!IsValid(_ProjectileClass))
  {
    UE_LOG(LogTemp, Warning, TEXT("You are trying to shoot a weapon that doesn't have a projectile class"));
    return;
  }

  //Activate the has shot bolean so it wont charge in the use fuction.
  m_bHasShot = true;

  if (m_pOwnerPlayer.IsValid() && IsValid(m_pOwnerPlayer->GetMuzzle()) && GetWorld())
  {

    m_pOwnerPlayer->CalculateAimAssist();
    FTransform oSpawnTransform = m_pOwnerPlayer->GetMuzzle()->GetComponentTransform();
    FVector vOffset = m_pDataAsset->m_vAttackOffset;
    oSpawnTransform.SetLocation(oSpawnTransform.GetLocation() + vOffset);
    GetWorld()->SpawnActor<ABaseProjectile>(_ProjectileClass, oSpawnTransform, m_oSpawnParams);
  }

  if (IsValid(m_pDataAsset) && m_pDataAsset->m_oTierThresholds.IsValidIndex(m_iCurrentTier))
  {
    //Stop the shake of the charging and controller rumble.
    m_pOwnerWeapon->SetCameraShake(m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_oReleaseShakeBP);
    FGamePadVibrationParams oVibrationParams = m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_oReleaseVibrationParams;

    m_pOwnerWeapon->StopControllerRumble();

    m_pOwnerWeapon->StartControllerRumble(
      oVibrationParams.m_fIntensity,
      oVibrationParams.m_fDuration,
      oVibrationParams.m_bAffectsLeftLarge,
      oVibrationParams.m_bAffectsLeftSmall,
      oVibrationParams.m_bAffectsRightLarge,
      oVibrationParams.m_bAffectsRightSmall);

    //Desactivate the vfx.
    m_pOwnerWeapon->SetActiveAimVFX(false, m_pDataAsset->m_pAimVFX);
  }

  //Set the dirtminator vfx and material to its original state.
  SetOwnerMaterial(m_pDefaultOwnerMaterial);
  SetOwnerVFX(nullptr);

  if (m_pDataAsset->m_oTierThresholds.IsValidIndex(m_iCurrentTier))
  {
    UNiagaraSystem* pSystem = m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_pShotVFX;
    if (pSystem)
    {
      FTransform oSpawnTransform = m_pOwnerPlayer->GetMuzzle()->GetComponentTransform();
      FVector vOffset = m_pDataAsset->m_vAttackOffset;
      oSpawnTransform.SetLocation(oSpawnTransform.GetLocation() + vOffset);
      UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), pSystem, oSpawnTransform.GetLocation(), m_pOwnerPlayer->GetMuzzle()->GetComponentRotation());
    }
  }

  //Set the player velocity to its original state.
  if (m_bIsSlow)
  {
    SetPlayerVelocity(false);
    m_bIsSlow = false;
  }
}

void URocketLauncherComponent::CheckMaxChargeAndShoot()
{
  if (!IsValid(m_pDataAsset))
  {
    return;
  }

  // If it does not reach the max time, return.
  if (m_fChargeTime < m_pDataAsset->m_fMaxChargeTime)
  {
    return;
  }

  // If there are no tier return.
  const int32 iSize = m_pDataAsset->m_oTierThresholds.Num();
  if (iSize <= 0)
  {
    return;
  }

  // Else, the charging reach the dirtminator max charging time, so 
  // Reset the visual effects. 
  SetDirtminatorTierVisualEffect(0);

  //Shoot in the actual tier.
  if (m_pDataAsset->m_oTierThresholds.IsValidIndex(m_iCurrentTier))
  {
    Shoot(m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_pProjectilClass);
  }

  if (m_pOwnerWeapon.IsValid())
  {
    m_pOwnerWeapon->OnAttackEnd.Broadcast(static_cast<EAttackType>(m_iCurrentTier));
    m_pOwnerWeapon->HasAlreadyShot.Broadcast();
  }
}

void URocketLauncherComponent::SetPlayerVelocity(bool _bActivate)
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset) ||
    !m_pDataAsset->m_oTierThresholds.IsValidIndex(m_iCurrentTier))
  {
    return;
  }

  //Set the player velocity multiplier.
  float movementMulty = m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_fMovementMultiplier;
  float rotatorMulty = m_pDataAsset->m_oTierThresholds[m_iCurrentTier].m_fRotationMultiplier;

  movementMulty = _bActivate
    ? movementMulty
    : (FMath::IsNearlyZero(movementMulty) ? 1.f : 1.f / movementMulty);

  rotatorMulty = _bActivate
    ? rotatorMulty
    : (FMath::IsNearlyZero(rotatorMulty) ? 1.f : 1.f / rotatorMulty);

  UE_LOG(LogTemp, Log, TEXT("Tier %d | Activate: %s | MoveMult: %.2f | RotMult: %.2f"),
    m_iCurrentTier,
    _bActivate ? TEXT("true") : TEXT("false"),
    movementMulty,
    rotatorMulty);

  m_pOwnerPlayer->ChangeMovementVelocity(movementMulty, rotatorMulty);
}

#pragma region VFX & Material Function
void URocketLauncherComponent::SetDirtminatorTierVisualEffect(int32 _Tier)
{
  //If there is no data asset don't do a thing.
  if (!IsValid(m_pDataAsset) && !m_pDataAsset->m_oTierThresholds.IsValidIndex(_Tier))
  {
    return;
  }

  const FDirtminatorTier& TierData = m_pDataAsset->m_oTierThresholds[_Tier];


  //Set the dirtminator material.
  if (UMaterialInterface* pMaterial = TierData.m_pMaterial)
  {
    SetOwnerMaterial(pMaterial);
  }

  //Set the dirtminator VFX.
  if (UNiagaraSystem* pVFX = TierData.m_pVFX)
  {
    SetOwnerVFX(pVFX);
  }
}

void URocketLauncherComponent::SetOwnerVFX(UNiagaraSystem* _pVFX)
{
  //Set the charge VFX.
  if (m_pOwnerWeapon.IsValid())
  {
    m_pOwnerWeapon->SetChargeVFX(_pVFX);
  }
}

void URocketLauncherComponent::SetOwnerMaterial(UMaterialInterface* _pNewMaterial)
{
  if (!m_pOwnerSkeletalMesh)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't access to the Onwer Mesh"));
    return;
  }

  if (!_pNewMaterial)
  {
    UE_LOG(LogTemp, Error, TEXT("Can't access to the New Material"));
    return;
  }

  m_pOwnerSkeletalMesh->SetOverlayMaterial(_pNewMaterial);
}
#pragma endregion