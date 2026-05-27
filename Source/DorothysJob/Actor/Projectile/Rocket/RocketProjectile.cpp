#include "RocketProjectile.h"
#include "DorothysJob/Interface/Damageable.h"
#include "Engine/OverlapResult.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Actor/Weapon/Dirtminator/Dirtminator.h"
#include "DorothysJob/Component/Weapon/Dirtminator/RocketLauncherComponent.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DorothysJob/Data/DataAssets/Weapon/DirtminatorWeaponStatsDataAsset.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Actor/Camera/MainLegacyCameraShake.h"
#include "FMODAudioComponent.h"
#include <DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h>

ARocketProjectile::ARocketProjectile() :
  m_fExplosionRadius(300.0f),
  m_iExplosionDamage(10)
{
  m_pExplosionRadiusPreview = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionRadiusPreview"));
  if (IsValid(m_pExplosionRadiusPreview))
  {
    m_pExplosionRadiusPreview->SetupAttachment(RootComponent);
    m_pExplosionRadiusPreview->InitSphereRadius(m_fExplosionRadius);
    m_pExplosionRadiusPreview->SetSphereRadius(m_fExplosionRadius);
    m_pExplosionRadiusPreview->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_pExplosionRadiusPreview->SetVisibility(true);
    m_pExplosionRadiusPreview->SetHiddenInGame(true);
    m_pExplosionRadiusPreview->SetGenerateOverlapEvents(false);
    m_pExplosionRadiusPreview->bIsEditorOnly = true;
    m_pExplosionRadiusPreview->ShapeColor = FColor::Red;
  }

  PrimaryActorTick.bCanEverTick = false;
}

void ARocketProjectile::BeginPlay()
{
  Super::BeginPlay();

  ActorsToIgnore.Add(this);

  if (ADirtminator* OwnerWeapon = Cast<ADirtminator>(m_pWeaponOwner))
  {
    m_pDirtminatorOwner = OwnerWeapon;

    if (UDirtminatorWeaponStatsDataAsset* DirtminatorDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(m_pDirtminatorOwner->GetDataAsset().Get()))
    {
      m_pDirtminatorDataAsset = DirtminatorDataAsset;

      if (m_pDirtminatorDataAsset->m_oExplosionDamage.IsValidIndex(m_iDamageIndex - 1))
      {
        m_iExplosionDamage = DirtminatorDataAsset->m_oExplosionDamage[m_iDamageIndex - 1];
      }
    }
  }

  if (m_iBulletTier == 2)
  {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        pAchSubsystem->m_bIsMaxChargeShot = true;
      }
    }
  }
}

void ARocketProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  Super::OnComponentOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

  if (!IsValid(CachedWorld))
  {
    return;
  }

  OnRocketExplode.Broadcast(m_fExplosionRadius);
  Explode(OtherActor);

  // VFX, ver si queda bien aqui o despues dentro de la funcion explode
  if (m_oExplosionVFX)
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(CachedWorld, m_oExplosionVFX, GetActorLocation(), SweepResult.ImpactNormal.Rotation());
  }

  if (OtherActor)
  {
    if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(OtherActor)) PlayFMODSound(m_pImpactFMODEvent, pCharacter->GetMitigation());
    else PlayFMODSound(m_pImpactFMODEvent, 0.f);
  }
}

void ARocketProjectile::Explode(AActor* _OtherActor)
{
  if (!m_pDirtminatorOwner.IsValid() || !m_pDirtminatorDataAsset.IsValid())
  {
    return;
  }

  if (m_iBulletTier == 1 && IsValid(m_pDirtminatorDataAsset->m_oExplosionT2ShakeBP))
  {
    m_pDirtminatorOwner->SetCameraShake(m_pDirtminatorDataAsset->m_oExplosionT2ShakeBP);
    FGamePadVibrationParams oVibrationParams = m_pDirtminatorDataAsset->m_oExplosionT2VibrationParams;
    m_pDirtminatorOwner->StopControllerRumble();
    m_pDirtminatorOwner->StartControllerRumble(
      oVibrationParams.m_fIntensity,
      oVibrationParams.m_fDuration,
      oVibrationParams.m_bAffectsLeftLarge,
      oVibrationParams.m_bAffectsLeftSmall,
      oVibrationParams.m_bAffectsRightLarge,
      oVibrationParams.m_bAffectsRightSmall);
  }
  else if (m_iBulletTier == 2 && IsValid(m_pDirtminatorDataAsset->m_oExplosionT3ShakeBP))
  {
    m_pDirtminatorOwner->SetCameraShake(m_pDirtminatorDataAsset->m_oExplosionT3ShakeBP);
    FGamePadVibrationParams oVibrationParams = m_pDirtminatorDataAsset->m_oExplosionT3VibrationParams;
    m_pDirtminatorOwner->StopControllerRumble();
    m_pDirtminatorOwner->StartControllerRumble(
      oVibrationParams.m_fIntensity,
      oVibrationParams.m_fDuration,
      oVibrationParams.m_bAffectsLeftLarge,
      oVibrationParams.m_bAffectsLeftSmall,
      oVibrationParams.m_bAffectsRightLarge,
      oVibrationParams.m_bAffectsRightSmall);
  }

  m_pDirtminatorOwner->StartCameraShake();

  TArray<AActor*> Overlaps;
  FCollisionShape ExplosionShape = FCollisionShape::MakeSphere(m_fExplosionRadius);

  UKismetSystemLibrary::SphereOverlapActors(
    CachedWorld,
    GetActorLocation(),
    m_fExplosionRadius,
    m_oChannels,
    nullptr,
    ActorsToIgnore,
    Overlaps
  );

  for (AActor* Result : Overlaps)
  {
    AActor* Actor = Result;
    if (IsValid(Actor) && Actor->Implements<UDamageable>())
    {
      if (IDamageable* pDamageable = Cast<IDamageable>(Actor))
      {
        m_pDirtminatorOwner->ApplyDamage(pDamageable, m_iExplosionDamage, EDirtType::Dust, true, Actor->GetActorLocation());
      }
    }
  }

  if (m_iBulletTier == 2)
  {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        pAchSubsystem->m_bIsMaxChargeShot = false;
        pAchSubsystem->m_iMaxChargeDeaths = 0;
      }
    }
  }
}

#if WITH_EDITOR
void ARocketProjectile::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
  Super::PostEditChangeProperty(PropertyChangedEvent);

  FName PropertyName = (PropertyChangedEvent.Property != nullptr)
    ? PropertyChangedEvent.Property->GetFName()
    : NAME_None;

  if (PropertyName == GET_MEMBER_NAME_CHECKED(ARocketProjectile, m_fExplosionRadius))
  {
    if (m_pExplosionRadiusPreview)
    {
      m_pExplosionRadiusPreview->SetSphereRadius(m_fExplosionRadius);
    }
  }
}
#endif

void ARocketProjectile::PlayFMODSound(UFMODEvent* _pEvent, float _fMitigation)
{
  if (!IsValid(m_pAudioManager) || !_pEvent || !IsValid(m_pAudioComponent))
  {
    return;
  }

  float iCurrentTier = m_iDamageIndex - 1;

  m_pAudioManager->SetComponentParameters(
    m_pAudioComponent,
    _pEvent,
    {
      {"shotTier", iCurrentTier}
    }
  );

  Super::PlayFMODSound(_pEvent, _fMitigation);
}