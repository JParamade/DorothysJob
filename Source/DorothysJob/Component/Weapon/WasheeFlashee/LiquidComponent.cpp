#include "LiquidComponent.h"
#include "DorothysJob/Actor/Projectile/BaseProjectile.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/WasheeFlashee/WasheeFlashee.h"
#include "DorothysJob/Data/DataAssets/Weapon/WFWeaponStatsDataAsset.h"
#include "NiagaraFunctionLibrary.h"

ULiquidComponent::ULiquidComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
}

void ULiquidComponent::Use()
{
  Shoot();
}

void ULiquidComponent::BeginPlay()
{
  Super::BeginPlay();

  if (!ValidateShootData())
  {
    UE_LOG(LogTemp, Display, TEXT("[LiquidComponent] Could not initialize shoot data."));
    return;
  }
  m_oSpawnParams.Owner = m_pOwnerWeapon.Get();
  m_oSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

void ULiquidComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  if (CachedWorld)
  {
    CachedWorld->GetTimerManager().ClearTimer(BurstTimerHandle);
  }
  Super::EndPlay(EndPlayReason);
}

bool ULiquidComponent::ValidateShootData()
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

void ULiquidComponent::Shoot()
{
  m_iBurstShotsFired = 0;
  if (IsValid(CachedWorld) && IsValid(m_pWFDataAsset))
  {
    CachedWorld->GetTimerManager().SetTimer(
      BurstTimerHandle,
      this,
      &ULiquidComponent::FireSingleBullet,
      m_pWFDataAsset->m_fBurstInterval,
      true
    );
  }
}

void ULiquidComponent::FireSingleBullet()
{
  if (!m_pOwnerWeapon.IsValid() || !IsValid(m_pWFDataAsset) || !m_pOwnerPlayer.IsValid())
  {
    if (CachedWorld)
    {
      CachedWorld->GetTimerManager().ClearTimer(BurstTimerHandle);
    }
    return;
  }

  //Broadcast to the shoot
  m_pOwnerWeapon->OnAttackStart.Broadcast(EAttackType::BASE_ATTACK);
  m_pOwnerWeapon->SetCameraShake(m_pWFDataAsset->m_oDamageCameraShake);
  m_pOwnerWeapon->StartCameraShake();
  m_pOwnerWeapon->StartControllerRumble(
    m_pWFDataAsset->m_oDamageVibrationParams.m_fIntensity,
    m_pWFDataAsset->m_oDamageVibrationParams.m_fDuration,
    m_pWFDataAsset->m_oDamageVibrationParams.m_bAffectsLeftLarge,
    m_pWFDataAsset->m_oDamageVibrationParams.m_bAffectsLeftSmall,
    m_pWFDataAsset->m_oDamageVibrationParams.m_bAffectsRightLarge,
    m_pWFDataAsset->m_oDamageVibrationParams.m_bAffectsRightSmall
  );

  // Calculate Aim Assist
  m_pOwnerPlayer->CalculateAimAssist();

  // Adding dispersion to the shots
  USceneComponent* Muzzle = m_pOwnerPlayer->GetMuzzle();
  if (IsValid(Muzzle))
  {
    FRotator MuzzleRotation = Muzzle->GetComponentRotation();
    float RandomYaw = FMath::FRandRange(-m_pWFDataAsset->m_fBurstDispersion, m_pWFDataAsset->m_fBurstDispersion);
    MuzzleRotation.Yaw += RandomYaw;

    if (m_pWFDataAsset->m_pWFShotVFX)
    {
      UNiagaraFunctionLibrary::SpawnSystemAtLocation(CachedWorld, m_pWFDataAsset->m_pWFShotVFX, Muzzle->GetComponentLocation(), MuzzleRotation);
    }

    // Spawn Projectile
    CachedWorld->SpawnActor<ABaseProjectile>(
      m_pWFDataAsset->m_oProjectileClass,
      Muzzle->GetComponentLocation(),
      MuzzleRotation,
      m_oSpawnParams
    );

    ++m_iBurstShotsFired;

    if (m_iBurstShotsFired >= m_pWFDataAsset->m_iBurstCount)
    {
      CachedWorld->GetTimerManager().ClearTimer(BurstTimerHandle);
      m_pOwnerWeapon->OnBarrageShot.Broadcast();
    }
  }
}