// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseWeaponAudioComponent.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "FMODEvent.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"

void UBaseWeaponAudioComponent::BeginPlay() {
  Super::BeginPlay();

  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (pOwnerWeapon)
  {
    pOwnerWeapon->OnAttackStart.AddDynamic(this, &UBaseWeaponAudioComponent::AttackSound);
    pOwnerWeapon->OnAttackEnd.AddDynamic(this, &UBaseWeaponAudioComponent::ReleaseAttack);

    pOwnerWeapon->OnCleanStart.AddDynamic(this, &UBaseWeaponAudioComponent::CleanSound);
    pOwnerWeapon->OnCleanEnd.AddDynamic(this, &UBaseWeaponAudioComponent::ReleaseClean);

    pOwnerWeapon->OnSpecialStart.AddDynamic(this, &UBaseWeaponAudioComponent::SpecialAttack);
    pOwnerWeapon->OnSpecialEnd.AddDynamic(this, &UBaseWeaponAudioComponent::ReleaseSpecialAttack);

    pOwnerWeapon->OnMeleeHit.AddDynamic(this, &UBaseWeaponAudioComponent::OnEnemyHit);

    pOwnerWeapon->OnSpecialCharge.AddDynamic(this, &UBaseWeaponAudioComponent::SpecialCharge);
    pOwnerWeapon->OnWeaponSwicht.AddDynamic(this, &UBaseWeaponAudioComponent::SwitchWeapon);

    m_pCleanAudioComponent = pOwnerWeapon->GetCleanAudioComponent();
    m_pSpecialAudioComponent = pOwnerWeapon->GetSpecialAudioComponent();
  }

  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      pLevelManager->OnGamePaused.AddUObject(this, &UBaseWeaponAudioComponent::PauseEvents);
      pLevelManager->OnGameUnpaused.AddUObject(this, &UBaseWeaponAudioComponent::UnpauseEvents);
    }
  }

  // Init Variables
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
    m_pAudioManager = pGameInstance->GetSubsystem<UAudioManager>();
  }
}

void UBaseWeaponAudioComponent::AttackSound(EAttackType _eType) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Attack Sound
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(GetWorld(), pDataAsset->m_pAttackSound);
}

void UBaseWeaponAudioComponent::ReleaseAttack(EAttackType _eType) {  }

void UBaseWeaponAudioComponent::CleanSound() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;
  
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Clean Sound
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(GetWorld(), pDataAsset->m_pCleanSound);
}

void UBaseWeaponAudioComponent::ReleaseClean() {  }

void UBaseWeaponAudioComponent::SwitchWeapon() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Special Charge Ready Audio
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(this, pDataAsset->m_pSwitchSound);
}

void UBaseWeaponAudioComponent::SpecialCharge() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;
  
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;
  
  // Play Special Charge Ready Audio
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(this, pDataAsset->m_pSpecialChargeSound);
}

void UBaseWeaponAudioComponent::SpecialAttack() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;
  
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;
  
  // Play Special Sound
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(GetWorld(), pDataAsset->m_pSpecialAttackSound);
}

void UBaseWeaponAudioComponent::ReleaseSpecialAttack() {  }

void UBaseWeaponAudioComponent::OnEnemyHit(float _fMitigation, bool _bThirdAttack) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Hit Impact Sound
  if (IsValid(m_pAudioManager)) {
    m_pAudioManager->PlayEventInstanceWithParameters(
      pDataAsset->m_pImpactSound,
      {
        {"isArmored", _fMitigation > 0 ? 1.f : 0.f},
        {"isPunch", (float)_bThirdAttack}
      }
    );
  }
}

void UBaseWeaponAudioComponent::PauseEvents() {}
void UBaseWeaponAudioComponent::UnpauseEvents() {}