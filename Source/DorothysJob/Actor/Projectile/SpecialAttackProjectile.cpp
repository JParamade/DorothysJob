// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialAttackProjectile.h"
#include "Components/SphereComponent.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DorothysJob/Actor/Weapon/WasheeFlashee/WasheeFlashee.h"
#include "DorothysJob/Component/Action/Movement/BaseProjectileMovementComponent.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Data/DataAssets/Weapon/WFWeaponStatsDataAsset.h"
#include "DorothysJob/Interface/Damageable.h"
#include "NiagaraFunctionLibrary.h"

// FMOD
#include "FMODAudioComponent.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include <DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h>
#include <Kismet/GameplayStatics.h>

ASpecialAttackProjectile::ASpecialAttackProjectile()
{
  m_pDrawing = CreateDefaultSubobject<UDrawing>("Drawing");

  // Collision Component
  m_pPoppingCollision = CreateDefaultSubobject<USphereComponent>(TEXT("PoppingCollisionComponent"));
  m_pPoppingCollision->SetupAttachment(RootComponent);
  m_pPoppingCollision->SetCollisionProfileName(TEXT("BlockAllDynamic"));
  m_pPoppingCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpecialAttackProjectile::OnComponentPopingOverlap);
}

void ASpecialAttackProjectile::BeginPlay()
{
  Super::BeginPlay();

  m_fHalfSpeed = m_fSpeed * 0.5f;
  ValidateShootData();

  // Bind Pause Events.
  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      m_oPauseHandle = pLevelManager->OnGamePaused.AddUObject(this, &ASpecialAttackProjectile::PauseFMODEvents);
      m_oUnpauseHandle = pLevelManager->OnGameUnpaused.AddUObject(this, &ASpecialAttackProjectile::UnpauseFMODEvents);
    }
  }

  // Play Special Loop.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlayEventOnComponent(m_pAudioComponent, m_pSpecialLoopFMODEvent);
    }
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_fWFSpecialStart = UGameplayStatics::GetTimeSeconds(GetWorld());
    }
  }
}

void ASpecialAttackProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor == this || !IsValid(OtherActor))
  {
    return;
  }

  ValidateShootData();

  if (IDamageable* Damageable = Cast<IDamageable>(OtherActor))
  {
    if (ABaseWeapon* OwnerWeapon = Cast<ABaseWeapon>(GetOwner()))
    {
      OwnerWeapon->ApplyDamage(Damageable, m_pWFDataAsset->m_oDamage[1], m_pWFDataAsset->m_eWeaponDirtType, false, GetActorLocation(), GetActorRotation());
      if (m_pImpactVFX)
      {
        UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pImpactVFX, OtherActor->GetActorLocation(), OtherActor->GetActorRotation(), FVector(1, 1, 1));

      }
      if (ABaseCharacter* Character = Cast<ABaseCharacter>(OtherActor))
      {
        OwnerWeapon->ApplySpecialStatusEffects(Character);
      }
    }
  }
}

void ASpecialAttackProjectile::OnComponentPopingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor == this || !IsValid(OtherComp))
  {
    return;
  }

  ECollisionChannel Channel = OtherComp->GetCollisionObjectType();

  if (Channel == ECC_WorldStatic)
  {
    if (IsValid(m_pDestroyVFX))
    {
      UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pDestroyVFX, GetActorLocation(), GetActorRotation(), FVector(1, 1, 1));
    }
    OnProjectileFinished.ExecuteIfBound();

    // Bind Pause Events.
    if (UWorld* pWorld = GetWorld()) {
      if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
        pLevelManager->OnGamePaused.Remove(m_oPauseHandle);
        pLevelManager->OnGameUnpaused.Remove(m_oUnpauseHandle);
      }
    }

    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        pAchSubsystem->SetWFSpecialAttackAchievement();
      }
    }
    Destroy();
  }
  else if (Channel == ECC_WorldDynamic)
  {
    m_fSpeed = m_fHalfSpeed;
    InitVelocity(GetActorForwardVector());
  }
}

void ASpecialAttackProjectile::UpdateProjectile()
{
  m_v2CurrentCleaningSize.X *= (m_v2CurrentCleaningSize.X < m_pWFDataAsset->m_v2SMaxCleaningSize.X) ? m_pWFDataAsset->m_fGrowthRate : 1.0f;
  m_v2CurrentCleaningSize.Y *= (m_v2CurrentCleaningSize.Y < m_pWFDataAsset->m_v2SMaxCleaningSize.Y) ? m_pWFDataAsset->m_fGrowthRate : 1.0f;
  if (IsValid(m_pDrawing) && IsValid(m_pWFDataAsset->m_pCleaningTexture))
  {
    m_pDrawing->Clean(m_pWFDataAsset->m_pCleaningTexture, m_v2CurrentCleaningSize, 90.0f, FVector2D(0.5, 0.5));
  }
}

void ASpecialAttackProjectile::ValidateShootData()
{
  if (!IsValid(m_pWFDataAsset) && IsValid(GetOwner()))
  {
    if (AWasheeFlashee* OwnerWeapon = Cast<AWasheeFlashee>(GetOwner()))
    {
      GetWorldTimerManager().ClearTimer(m_oInitCheckTimer);
      m_pWFDataAsset = OwnerWeapon->GetDataAsset();
      GetWorldTimerManager().SetTimer(m_oUpdateProjectileTimer, this, &ASpecialAttackProjectile::UpdateProjectile, m_pWFDataAsset->m_fUpdateRate, true);
      m_v2CurrentCleaningSize = m_pWFDataAsset->m_v2StartingCleaningSize;
    }
  }
}

void ASpecialAttackProjectile::PauseFMODEvents() {
  if (IsValid(m_pAudioComponent) && m_pAudioComponent->IsPlaying()) {
    m_pAudioComponent->SetPaused(true);
    m_bSpecialPaused = true;
  }
}

void ASpecialAttackProjectile::UnpauseFMODEvents() {
  if (IsValid(m_pAudioComponent) && m_bSpecialPaused) {
    m_pAudioComponent->SetPaused(false);
    m_bSpecialPaused = false;
  }
}