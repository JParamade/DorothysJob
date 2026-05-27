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

  // Bind to the Weapon's Delegates.
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

    // Get audio components from the weapon.
    m_pCleanAudioComponent = pOwnerWeapon->GetCleanAudioComponent();
    m_pSpecialAudioComponent = pOwnerWeapon->GetSpecialAudioComponent();
  }

  if (UWorld* pWorld = GetWorld()) {
    // Bind to the Level Manager's Pause and Unpause Events.
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      pLevelManager->OnGamePaused.AddUObject(this, &UBaseWeaponAudioComponent::PauseEvents);
      pLevelManager->OnGameUnpaused.AddUObject(this, &UBaseWeaponAudioComponent::UnpauseEvents);
    }
  }

  // Get the Audio Manager from the Game Instance.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
    m_pAudioManager = pGameInstance->GetSubsystem<UAudioManager>();
  }
}

void UBaseWeaponAudioComponent::AttackSound(EAttackType _eType) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  // Get the Data Asset from the Weapon.
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Sound.
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(GetWorld(), pDataAsset->m_pAttackSound);
}

void UBaseWeaponAudioComponent::ReleaseAttack(EAttackType _eType) {}

void UBaseWeaponAudioComponent::CleanSound() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  // Get the Data Asset from the Weapon.
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Sound.
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(GetWorld(), pDataAsset->m_pCleanSound);
}

void UBaseWeaponAudioComponent::ReleaseClean() {}

void UBaseWeaponAudioComponent::SwitchWeapon() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  // Get the Data Asset from the Weapon.
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Sound.
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(this, pDataAsset->m_pSwitchSound);
}

void UBaseWeaponAudioComponent::SpecialCharge() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  // Get the Data Asset from the Weapon.
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Sound.
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(this, pDataAsset->m_pSpecialChargeSound);
}

void UBaseWeaponAudioComponent::SpecialAttack() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  // Get the Data Asset from the Weapon.
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Sound.
  if (IsValid(m_pAudioManager)) m_pAudioManager->PlaySound2D(GetWorld(), pDataAsset->m_pSpecialAttackSound);
}

void UBaseWeaponAudioComponent::ReleaseSpecialAttack() {}

void UBaseWeaponAudioComponent::OnEnemyHit(float _fMitigation, bool _bThirdAttack) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  // Get the Data Asset from the Weapon.
  UBaseWeaponStatsDataAsset* pDataAsset = pOwnerWeapon->GetDataAsset().Get();
  if (!pDataAsset) return;

  // Play Sound with parameters depending on whether the hit was mitigated and if it was a third attack in a combo.
  if (IsValid(m_pAudioManager)) {
    m_pAudioManager->PlayEventInstanceWithParameters(
      pDataAsset->m_pImpactSound,
      {
        // If the hit was mitigated, set "isArmored" to 1, otherwise set it to 0.
        {"isArmored", _fMitigation > 0 ? 1.f : 0.f},
        // If it was a third attack in a combo, set "isPunch" to 1, otherwise set it to 0.
        {"isPunch", (float)_bThirdAttack}
      }
    );
  }
}

void UBaseWeaponAudioComponent::PauseEvents() {}
void UBaseWeaponAudioComponent::UnpauseEvents() {}