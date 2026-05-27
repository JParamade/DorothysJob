/************************************************************************
 * @description: Represents the Moppressor's weapon Audio Component.
 * @author: Jaime Paramo
 * @date: 31/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "MoppressorWeaponAudioComponent.h"

#include "DorothysJob/Data/DataAssets/Weapon/MoppresorWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Actor/Weapon/Mopressor/Mopressor.h"

#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "FMODAudioComponent.h"

void UMoppressorWeaponAudioComponent::AttackSound(EAttackType _eType) {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UMoppresorWeaponStatsDataAsset* pDataAsset = Cast<UMoppresorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Play Attack Audio
    if (AMopressor* pMoppressor = Cast<AMopressor>(pOwnerWeapon)) {
      if (pMoppressor->CurrentComboHit() < 2) m_pAudioManager->PlayEventInstance(pDataAsset->m_pAttackSound);
      else m_pAudioManager->PlayEventInstance(pDataAsset->m_pFinalAttackEvent);
    }
  }
}

void UMoppressorWeaponAudioComponent::CleanSound() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UMoppresorWeaponStatsDataAsset* pDataAsset = Cast<UMoppresorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  if (IsValid(m_pAudioManager)) {
    // Play Clean Audio
    m_pAudioManager->PlayEventOnComponent(m_pCleanAudioComponent, pDataAsset->m_pCleanSound);
  }
}

void UMoppressorWeaponAudioComponent::ReleaseClean() {
  ABaseWeapon* pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());
  if (!pOwnerWeapon) return;

  UMoppresorWeaponStatsDataAsset* pDataAsset = Cast<UMoppresorWeaponStatsDataAsset>(pOwnerWeapon->GetDataAsset().Get());
  if (!pDataAsset) return;

  // Stop Clean Audio
  if (IsValid(m_pCleanAudioComponent)) {
    m_pCleanAudioComponent->SetEvent(pDataAsset->m_pCleanSound);
    m_pCleanAudioComponent->Stop();
  }
}