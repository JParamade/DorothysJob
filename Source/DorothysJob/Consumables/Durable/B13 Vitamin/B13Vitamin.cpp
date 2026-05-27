/************************************************************************
 * @description: Represents the B13 Vitamin consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "B13Vitamin.h"

#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "DorothysJob/Actor/Weapon/BaseWeapon.h"

 // Game Manager
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

// FMOD
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

bool UB13Vitamin::Use(ABasePlayer* _pPlayer) {
  return Super::Use(_pPlayer);
}

void UB13Vitamin::OnEffectStart(ABasePlayer* _pPlayer) {
  Super::OnEffectStart(_pPlayer);

  // Achievement Logic.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_bUseB13 = true;
    }
  }

  // Bind Pause Events.
  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      m_oPauseHandle = pLevelManager->OnGamePaused.AddUObject(this, &UB13Vitamin::PauseFMODEvents);
      m_oUnpauseHandle = pLevelManager->OnGameUnpaused.AddUObject(this, &UB13Vitamin::UnpauseFMODEvents);
    }
  }

  // Play Swallow Sound.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this))) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/CombatTools/Consumables/B13Vitamin/SFX_CONS_B13_swallow.SFX_CONS_B13_swallow")));
    }
  }

  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pBuffEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/CombatTools/Consumables/B13Vitamin/SFX_CONS_B13_buff_loop", &pBuffEventDesc);

    // Play Audio Buff Event
    if (pBuffEventDesc) pBuffEventDesc->createInstance(&pBuffEvent);
    if (pBuffEvent) pBuffEvent->start();

    // Play Audio Breathe Event
    FMOD::Studio::EventDescription* pBreatheEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/CombatTools/Consumables/B13Vitamin/SFX_CONS_B13_buffSexyLoop", &pBreatheEventDesc);

    if (pBreatheEventDesc) pBreatheEventDesc->createInstance(&pBreatheEvent);
    if (pBreatheEvent) pBreatheEvent->start();
  }

  // Apply the consumable's effect to the player.
  if (IsValid(_pPlayer)) _pPlayer->OnConsumableEffectStart(m_fMovementSpeedIncrement, m_fAttackSpeedMultiplier);
}

void UB13Vitamin::OnEffectEnd(ABasePlayer* _pPlayer) {
  Super::OnEffectEnd(_pPlayer);

  // Achievement Logic.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_bUseB13 = false;
      pAchSubsystem->m_iEnemyKillsWith1B13 = 0;
    }
  }

  // Remove Pause Event Bindings.
  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      pLevelManager->OnGamePaused.Remove(m_oPauseHandle);
      pLevelManager->OnGameUnpaused.Remove(m_oUnpauseHandle);
    }
  }

  // Stop FMOD Events.
  if (pBuffEvent) {
    pBuffEvent->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    pBuffEvent = nullptr;
  }
  if (pBreatheEvent) {
    pBreatheEvent->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
    pBreatheEvent = nullptr;
  }

  // Remove the consumable's effect from the player and allow the consumable to be used again.
  if (IsValid(_pPlayer)) _pPlayer->OnConsumableEffectEnd();
}


void UB13Vitamin::PauseFMODEvents() {
  // Manage FMOD Events when the game is paused to ensure they are paused correctly.
  if (pBuffEvent) {
    FMOD_STUDIO_PLAYBACK_STATE oBuffState;
    pBuffEvent->getPlaybackState(&oBuffState);
    // Only pause the event if it is currently playing to avoid unnecessary state changes.
    if (oBuffState == FMOD_STUDIO_PLAYBACK_PLAYING) {
      pBuffEvent->setPaused(true);
      m_bBuffPaused = true;
    }
  }
  if (pBreatheEvent) {
    FMOD_STUDIO_PLAYBACK_STATE oBreatheState;
    pBreatheEvent->getPlaybackState(&oBreatheState);
    // Only pause the event if it is currently playing to avoid unnecessary state changes.
    if (oBreatheState == FMOD_STUDIO_PLAYBACK_PLAYING) {
      pBreatheEvent->setPaused(true);
      m_bBreathePaused = true;
    }
  }
}

void UB13Vitamin::UnpauseFMODEvents() {
  // Manage FMOD Events when the game is unpaused to ensure they are resumed correctly.
  if (pBuffEvent && m_bBuffPaused) {
    pBuffEvent->setPaused(false);
    m_bBuffPaused = false;
  }
  if (pBreatheEvent && m_bBreathePaused) {
    pBreatheEvent->setPaused(false);
    m_bBreathePaused = false;
  }
}