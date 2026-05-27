// Fill out your copyright notice in the Description page of Project Settings.


#include "DustyWeaponAudioComponent.h"

#include "DorothysJob/Data/DataAssets/Weapon/DustyWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Actor/Weapon/Dusty/Dusty.h"

#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "FMODAudioComponent.h"

void UDustyWeaponAudioComponent::AttackSound(EAttackType _eType) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Play Attack Audio
    if (ADusty* pDusty = Cast<ADusty>(pOwnerWeapon)) {
      if (pDusty->CurrentComboHit() < 2) m_pAudioManager->PlayEventInstance(pDataAsset->m_pAttackSound);
      else m_pAudioManager->PlayEventInstance(pDataAsset->m_pFinalAttackEvent);
    }
  }
}

void UDustyWeaponAudioComponent::CleanSound() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Play Clean Audio
    m_pAudioManager->PlayEventOnComponent(m_pCleanAudioComponent, pDataAsset->m_pCleanSound);
  }
}

void UDustyWeaponAudioComponent::ReleaseClean() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  // Stop Clean Audio
  if (IsValid(m_pCleanAudioComponent)) {
    m_pCleanAudioComponent->SetEvent(pDataAsset->m_pCleanSound);
    m_pCleanAudioComponent->Stop();
  }
}

void UDustyWeaponAudioComponent::SpecialAttack() {
  if (!m_bCanPlaySpecialSound) return;
  
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  
  if (IsValid(m_pAudioManager)) {
    // Play Special Audio
    m_pAudioManager->PlayEventWithParameters(
      m_pSpecialAudioComponent,
      pDataAsset->m_pSpecialAttackSound,
      { {"loopStop_dustySpecial", 0.f} }
    );
  }

  m_bCanPlaySpecialSound = false;
}

void UDustyWeaponAudioComponent::ReleaseSpecialAttack() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDustyWeaponStatsDataAsset* pDataAsset = Cast<UDustyWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Stop Special Audio
    m_pAudioManager->SetComponentParameters(
      m_pSpecialAudioComponent,
      pDataAsset->m_pSpecialAttackSound,
      { {"loopStop_dustySpecial", 1.f} }
    );
  }

  m_bCanPlaySpecialSound = true;
}

void UDustyWeaponAudioComponent::PauseEvents() {
  Super::PauseEvents();

  if (IsValid(m_pSpecialAudioComponent) && m_pSpecialAudioComponent->IsPlaying()) {
    m_pSpecialAudioComponent->SetPaused(true);
    m_bSpecialPaused = true;
  }
}

void UDustyWeaponAudioComponent::UnpauseEvents() {
  Super::UnpauseEvents();

  if (IsValid(m_pSpecialAudioComponent) && m_bSpecialPaused) {
    m_pSpecialAudioComponent->SetPaused(false);
    m_bSpecialPaused = false;
  }
}