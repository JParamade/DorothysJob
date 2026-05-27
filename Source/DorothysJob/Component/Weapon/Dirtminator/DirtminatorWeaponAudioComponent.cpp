// Fill out your copyright notice in the Description page of Project Settings.


#include "DirtminatorWeaponAudioComponent.h"

#include "DorothysJob/Data/DataAssets/Weapon/DirtminatorWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"

#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "FMODAudioComponent.h"

void UDirtminatorWeaponAudioComponent::AttackSound(EAttackType _eType) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDirtminatorWeaponStatsDataAsset* pDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) 
  {
    if (_eType != EAttackType::BASE_ATTACK)
    {
      // Play New Charge Audio
      m_pAudioManager->PlayEventInstanceWithParameters(
        pDataAsset->m_pAttackNewChargeSound,
        { {"shotTier", static_cast<float>(_eType)} }
      );
    }
    else
    {
      m_pAudioManager->PlaySound2D(this, pDataAsset->m_pAttackChargingSound);
    }
  }
}

void UDirtminatorWeaponAudioComponent::ReleaseAttack(EAttackType _eType) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDirtminatorWeaponStatsDataAsset* pDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Play Attack Audio
    m_pAudioManager->PlayEventInstanceWithParameters(
      pDataAsset->m_pAttackSound,
      { {"shotTier", _eType == EAttackType::MAX ? 2.f : static_cast<float>(_eType)} }
    );
  }
}

void UDirtminatorWeaponAudioComponent::CleanSound() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDirtminatorWeaponStatsDataAsset* pDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Play Clean Audio
    m_pAudioManager->PlayEventWithParameters(
      m_pCleanAudioComponent,
      pDataAsset->m_pCleanSound,
      { {"loopStop_dirtminatorCleanup", 0.f} }
    );
  }
}

void UDirtminatorWeaponAudioComponent::ReleaseClean() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDirtminatorWeaponStatsDataAsset* pDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Stop Clean Audio
    m_pAudioManager->SetComponentParameters(
      m_pCleanAudioComponent,
      pDataAsset->m_pCleanSound,
      { {"loopStop_dirtminatorCleanup", 1.f} }
    );
  }
}

void UDirtminatorWeaponAudioComponent::SpecialAttack() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDirtminatorWeaponStatsDataAsset* pDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Play Special Audio
    m_pAudioManager->PlayEventWithParameters(
      m_pSpecialAudioComponent,
      pDataAsset->m_pSpecialAttackSound,
      { {"loopStop_dirtminatorSpecial", 0.f} }
    );
  }
}

void UDirtminatorWeaponAudioComponent::ReleaseSpecialAttack() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UDirtminatorWeaponStatsDataAsset* pDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Stop Special Audio
    m_pAudioManager->SetComponentParameters(
      m_pSpecialAudioComponent,
      pDataAsset->m_pSpecialAttackSound,
      { {"loopStop_dirtminatorSpecial", 1.f} }
    );
  }
}

void UDirtminatorWeaponAudioComponent::PauseEvents() {
  Super::PauseEvents();

  if (IsValid(m_pSpecialAudioComponent) && m_pSpecialAudioComponent->IsPlaying()) {
    m_pSpecialAudioComponent->SetPaused(true);
    m_bSpecialPaused = true;
  }
}

void UDirtminatorWeaponAudioComponent::UnpauseEvents() {
  Super::UnpauseEvents();

  if (IsValid(m_pSpecialAudioComponent) && m_bSpecialPaused) {
    m_pSpecialAudioComponent->SetPaused(false);
    m_bSpecialPaused = false;
  }
}