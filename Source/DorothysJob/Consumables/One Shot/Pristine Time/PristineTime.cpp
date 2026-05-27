/************************************************************************
 * @description: Represents the Pristine Time consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "PristineTime.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"

 // Audio
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include <DorothysJob/Manager/BaseGameInstance.h>
#include <DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h>

bool UPristineTime::Use(ABasePlayer* _pPlayer) {
  // Check if the consumable can be used: it must have quantity, the player must be valid, and the player's health must not be full.
  if (m_uStackQuantity <= 0 || !IsValid(_pPlayer) || _pPlayer->GetMaxHealth() == _pPlayer->GetCurrentHealth()) return false;

  // Get the game instance and ensure it's valid.
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance) return false;

  // Achievement Logic.
  UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>();
  if (IsValid(pAchSubsystem)) {
    const bool bIsStinky = _pPlayer->GetIsPlayerStinky();
    const bool bHasB13 = _pPlayer->GetIsPlayerWithB13();

    if (bIsStinky && bHasB13) pAchSubsystem->UnlockAchievement("HowRefreshing");
    if (bIsStinky) pAchSubsystem->AddProgress("UpUpAndAwash", 1);

    pAchSubsystem->m_bEquipApron = true;
    pAchSubsystem->m_bApronUse = true;
  }

  // Heal the player to full health and trigger the Pristine Time visual effects.
  _pPlayer->Heal(_pPlayer->GetMaxHealth());
  _pPlayer->PristineTimeVFX();

  // Play audio through FMOD System.
  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/CombatTools/CleanStation/SFX_CLS_cleanStationInteract", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }

  return Super::Use(_pPlayer);
}