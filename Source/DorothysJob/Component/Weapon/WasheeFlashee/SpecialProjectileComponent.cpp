// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialProjectileComponent.h"
#include "DorothysJob/Actor/Projectile/BaseProjectile.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/WasheeFlashee/WasheeFlashee.h"
#include "DorothysJob/Data/DataAssets/Weapon/WFWeaponStatsDataAsset.h"
#include "DorothysJob/Component/Action/Movement/BaseProjectileMovementComponent.h"
#include "DorothysJob/Actor/Camera/MainLegacyCameraShake.h"
USpecialProjectileComponent::USpecialProjectileComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
}

void USpecialProjectileComponent::Use()
{

  if (AWasheeFlashee* pOwner = Cast<AWasheeFlashee>(GetOwner()))
  {
    pOwner->OnSpecialActive.Broadcast(true);
  }
  Shoot();
}

void USpecialProjectileComponent::BeginPlay()
{
  Super::BeginPlay();

  if (!ValidateShootData())
  {
    return;
  }

  m_oSpawnParams.Owner = GetOwner();
  m_oSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

bool USpecialProjectileComponent::ValidateShootData()
{
  if (!IsValid(m_pWFDataAsset) || !IsValid(m_pWFDataAsset->m_oProjectileClass) || !m_pOwnerPlayer.IsValid() || !IsValid(CachedWorld))
  {
    if (AWasheeFlashee* OwnerWeapon = Cast<AWasheeFlashee>(GetOwner()))
    {
      m_pOwnerWeapon = OwnerWeapon;
      m_pWFDataAsset = OwnerWeapon->GetDataAsset();
      m_pOwnerPlayer = Cast<ABasePlayer>(OwnerWeapon->GetOwner());
      CachedWorld = GetWorld();
    }
    return IsValid(m_pWFDataAsset) && m_pOwnerPlayer.IsValid() && IsValid(CachedWorld);
  }

  return true;
}

void USpecialProjectileComponent::Shoot() const
{
  if (!IsValid(CachedWorld) || !m_pOwnerPlayer.IsValid() || !IsValid(m_pWFDataAsset) ||
    !IsValid(m_pWFDataAsset->m_oSpecialProjectileClass) || !m_pOwnerWeapon.IsValid())
  {
    return;
  }

  // Calculate Aim Assist
  m_pOwnerPlayer->CalculateAimAssist();

  // Spawn Projectile
  if (USceneComponent* Muzzle = m_pOwnerPlayer->GetMuzzle())
  {
    ABaseProjectile* Projectile = CachedWorld->SpawnActor<ABaseProjectile>(
      m_pWFDataAsset->m_oSpecialProjectileClass,
      Muzzle->GetComponentLocation() + Muzzle->GetForwardVector().GetSafeNormal() * 125.0f,
      Muzzle->GetComponentRotation(),
      m_oSpawnParams
    );

    if (IsValid(Projectile))
    {
      Projectile->OnProjectileFinished.BindDynamic(this, &USpecialProjectileComponent::StopSpecialShake);
    }
  }

  if (IsValid(m_pWFDataAsset->m_oSpecialAttackCameraShake))
  {
    m_pOwnerWeapon->SetCameraShake(m_pWFDataAsset->m_oSpecialAttackCameraShake);
    m_pOwnerWeapon->StartCameraShake();
    m_pOwnerWeapon->StopControllerRumble();
    m_pOwnerWeapon->StartControllerRumble(
      m_pWFDataAsset->m_oSpecialAttackVibrationParams.m_fIntensity,
      m_pWFDataAsset->m_oSpecialAttackVibrationParams.m_fDuration,
      m_pWFDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftLarge,
      m_pWFDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftSmall,
      m_pWFDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightLarge,
      m_pWFDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightSmall);
    m_pOwnerWeapon->OnSpecialActive.Broadcast(false);
  }
}

void USpecialProjectileComponent::StopSpecialShake()
{
  if (m_pOwnerWeapon.IsValid())
  {
    m_pOwnerWeapon->StopCameraShake();
  }
}
