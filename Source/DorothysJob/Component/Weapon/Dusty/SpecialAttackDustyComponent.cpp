// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialAttackDustyComponent.h"

#include "Components/SphereComponent.h"
#include "DorothysJob/Interface/Damageable.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Weapon/Dusty/Dusty.h"
#include "DorothysJob/Actor/Dust/DustPoop.h"
#include "DorothysJob/Animations/Player/PlayerAnimInstance.h"

#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/DustyWeaponStatsDataAsset.h"
#include "DorothysJob/Status/BaseStatus.h"


// Gameplay
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

// FMOD
#include "FMODEvent.h"

// Component
#include "FMODAudioComponent.h"

#include "Components/ShapeComponent.h"
#include "NiagaraFunctionLibrary.h"


USpecialAttackDustyComponent::USpecialAttackDustyComponent()
{

  /// TEMP
  m_pCollider = CreateDefaultSubobject<USphereComponent>(TEXT("DustyCollider"));
  m_pCollider->SetCollisionProfileName(TEXT("Weapon"));

  m_pCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void USpecialAttackDustyComponent::BeginPlay()
{
  Super::BeginPlay();

  ADusty* pOwner = Cast<ADusty>(GetOwner());
  if (pOwner)
  {
    m_pOwner = pOwner;
    UDustyWeaponStatsDataAsset* oDataAsset = Cast<UDustyWeaponStatsDataAsset>(m_pOwner->GetDataAsset().Get());
    if (oDataAsset)
    {
      m_oStatsDataAsset = oDataAsset;
    }
  }
  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(Cast<ADusty>(GetOwner())->GetDataAsset().Get());

  pWeapon = Cast<ABaseWeapon>(GetOwner());
  pPlayer = Cast<ABasePlayer>(pWeapon->GetOwner());

  m_pCollider->InitSphereRadius(pDataAsset->m_fWaveDamageRange);
  m_pCollider->AttachToComponent(
    pPlayer->GetRootComponent(),
    FAttachmentTransformRules::KeepRelativeTransform
  );
  m_pCollider->SetRelativeLocation(FVector(0.f, 0.f, -90.f));
}

void USpecialAttackDustyComponent::Use()
{
  UE_LOG(LogTemp, Log, TEXT("DUSTY ESPECIAL"));

  // play animation

  // Activate Invincibility Status Effect
  if (pPlayer)
  {
    if (IsValid(m_oStatsDataAsset->m_pSpecialInvincibilityEffect)) pPlayer->ApplyStatus(m_oStatsDataAsset->m_pSpecialInvincibilityEffect);
  }

  CreateWaveVFX();
  m_pOwner->SetCameraShake(m_oStatsDataAsset->m_oSpecialAttackCameraShake);
  m_pOwner->StartCameraShake();
  m_pOwner->StopControllerRumble();
  m_pOwner->StartControllerRumble(
    m_oStatsDataAsset->m_oSpecialAttackVibrationParams.m_fIntensity,
    m_oStatsDataAsset->m_oSpecialAttackVibrationParams.m_fDuration,
    m_oStatsDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftLarge,
    m_oStatsDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftSmall,
    m_oStatsDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightLarge,
    m_oStatsDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightSmall);

  SetPlayerVelocity(true);

  GetWorld()->GetTimerManager().SetTimer(
    DamageTickHandle,
    this,
    &USpecialAttackDustyComponent::ApplySpecialAttackDamage,
    m_oStatsDataAsset->m_fCooldownSpecialAttack,
    true
  );

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_bDustySpecialActive = true;
    }
  }

  FTimerHandle StopHandle;

  GetWorld()->GetTimerManager().SetTimer(
    StopHandle,
    this,
    &USpecialAttackDustyComponent::ReleaseSpecialAttack,
    m_oStatsDataAsset->m_fSpecialAttackDuration,
    false
  );

  if (IsValid(m_pOwner)) m_pOwner->OnSpecialStart.Broadcast();
}

void USpecialAttackDustyComponent::ApplySpecialAttackDamage()
{
  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(Cast<ADusty>(GetOwner())->GetDataAsset().Get());
  if (IsValid(m_pCollider))
  {


    const FVector Center = m_pCollider->GetComponentLocation();;
    //const float Radius = pDataAsset->m_fWaveDamageRange;

    // Pinta la esfera en el mundo durante 2 segundos
    DrawDebugSphere(
      GetWorld(),
      Center,
      pDataAsset->m_fWaveDamageRange,
      16,
      FColor::Red,
      false,
      0,
      2.0f
    );

    UE_LOG(LogTemp, Log, TEXT("El Collider es valido"));

    TArray<AActor*> lEnemiesInRange;

    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));

    bool hit = UKismetSystemLibrary::SphereOverlapActors(
      GetWorld(),
      Center,
      pDataAsset->m_fWaveDamageRange,
      ObjectTypes,
      nullptr,
      {},
      lEnemiesInRange
    );

    if (hit)
    {

      for (AActor* enemy : lEnemiesInRange)
      {
        ADustPoop* pDustPoop = Cast<ADustPoop>(enemy);
        IDamageable* pDamageable = Cast<IDamageable>(enemy);
        int32 iDamage = IsValid(pDustPoop) ? pDataAsset->m_fSpecialAttackDastPoopDamage : pDataAsset->m_fSpecialAttackEnemyDamage;
        pWeapon->ApplyDamage(pDamageable, iDamage, pDataAsset->m_eWeaponDirtType, false);

        if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(enemy)) {
          m_pOwner->OnMeleeHit.Broadcast(pCharacter->GetMitigation(), !(m_pOwner->CurrentComboHit() < 2));
        }
      }
    }
  }
}

void USpecialAttackDustyComponent::CreateWaveVFX()
{
  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(Cast<ADusty>(GetOwner())->GetDataAsset().Get());

  if (IsValid(pDataAsset->m_pWaveEffect))
  {
    UE_LOG(LogTemp, Log, TEXT("Spawning the VFX!"));
    FVector PlayerLocation = FVector::ZeroVector;
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
      APawn* PlayerPawn = PlayerController->GetPawn();
      if (PlayerPawn && PlayerPawn->GetRootComponent())
      {
        
        UNiagaraFunctionLibrary::SpawnSystemAttached(
          pDataAsset->m_pWaveEffect,
          PlayerPawn->GetRootComponent(),   
          NAME_None,                       
          FVector::ZeroVector,             
          FRotator::ZeroRotator,           
          EAttachLocation::KeepRelativeOffset, 
          true                            
        );
      }
    }

  }
}
void USpecialAttackDustyComponent::SetPlayerVelocity(bool _bActivate) {
  APawn* pPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
  pPlayer = Cast<ABasePlayer>(pPawn);
  if (!pPlayer) return;

  if (UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(Cast<ADusty>(GetOwner())->GetDataAsset().Get()))
  {
    float fMovementMultiplier = pDataAsset->m_fMovementMultiplier;

    fMovementMultiplier = _bActivate ? fMovementMultiplier : 1.f / fMovementMultiplier;

    pPlayer->ChangeMovementVelocity(fMovementMultiplier, 1.f);
  }
}

void USpecialAttackDustyComponent::ReleaseSpecialAttack()
{
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_bDustySpecialActive = false;
      pAchSubsystem->m_iDustPoopsClean = 0;
    }
  }

  if (IsValid(m_pOwner)) {
    m_pOwner->OnSpecialEnd.Broadcast();
    m_pOwner->StopCameraShake();
    m_pOwner->OnSpecialActive.Broadcast(false);
  }
  
  SetPlayerVelocity(false);
  GetWorld()->GetTimerManager().ClearTimer(DamageTickHandle);
}