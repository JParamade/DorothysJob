#include "TimeManager.h"
#include "FMODBlueprintStatics.h"
#include "Logging/StructuredLog.h"    // for logging

// Audio
#include "FMODBlueprintStatics.h"

// Gameplay
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

DEFINE_LOG_CATEGORY(TimerManager);

void UTimeManager::OnWorldBeginPlay(UWorld& InWorld)
{
  Super::OnWorldBeginPlay(InWorld);

  if (UWorld* pCachedWorld = GetWorld())
  {
    CachedWorld = pCachedWorld;
  }
}

bool UTimeManager::StartTimer(int _min, int _seg)
{
  if (_min < 0 || _seg < 0)
  {
    return false;
  }

  if (IsValid(CachedWorld) && !CachedWorld->GetTimerManager().TimerExists(m_countDownHandle))
  {
    m_iTotalSeconds = _min * 60 + _seg;
    m_iCurrentSeconds = m_iTotalSeconds;

    CachedWorld->GetTimerManager().SetTimer(m_countDownHandle, this, &UTimeManager::CountDown, 1.f, true);
    OnTimeChanged.Broadcast(m_iCurrentSeconds);

    return true;
  }
  return false;
}

bool UTimeManager::PauseTimer()
{
  if (IsValid(CachedWorld) && CachedWorld->GetTimerManager().IsTimerActive(m_countDownHandle))
  {
    CachedWorld->GetTimerManager().PauseTimer(m_countDownHandle);
    return true;
  }
  return false;
}

bool UTimeManager::PlayTimer()
{
  if (IsValid(CachedWorld) && CachedWorld->GetTimerManager().IsTimerPaused(m_countDownHandle))
  {
    CachedWorld->GetTimerManager().UnPauseTimer(m_countDownHandle);
    return true;
  }
  return false;
}

bool UTimeManager::ClearTimer()
{
  if (IsValid(CachedWorld) && CachedWorld->GetTimerManager().TimerExists(m_countDownHandle))
  {
    OnTimeChanged.Broadcast(m_iCurrentSeconds);
    CachedWorld->GetTimerManager().ClearTimer(m_countDownHandle);
    return true;
  }
  return false;
}

void UTimeManager::CountDown()
{
  if (m_iCurrentSeconds > 0)
  {
    m_iCurrentSeconds--;

    if (m_iCurrentSeconds == 5) {
      FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
      if (pStudioSystem)
      {
        FMOD::Studio::EventDescription* pEventDesc = nullptr;
        pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/UI/HUD/SFX_HUD_finalCountdown", &pEventDesc);

        if (pEventDesc) pEventDesc->createInstance(&pTimeEvent);
        if (pTimeEvent) pTimeEvent->start();
      }
    }
    else if (m_iCurrentSeconds > 5) {
      if (pTimeEvent) {
        FMOD_STUDIO_PLAYBACK_STATE pPlaybackState;
        pTimeEvent->getPlaybackState(&pPlaybackState);

        if (pPlaybackState == FMOD_STUDIO_PLAYBACK_PLAYING) {
          pTimeEvent->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
          pTimeEvent = nullptr;
        }
      }
    }

    if (m_iCurrentSeconds == m_iRunningOutOfTime) {
      FMOD::Studio::EventInstance* pEvent = nullptr;
      FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
      if (pStudioSystem)
      {
        FMOD::Studio::EventDescription* pEventDesc = nullptr;
        pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/UI/HUD/SFX_HUD_hurryTickTock", &pEventDesc);

        if (pEventDesc) pEventDesc->createInstance(&pEvent);
        if (pEvent) pEvent->start();
      }
    }

    // Change Target Interpolation Value.
    m_bStartAudioInterp = true;
    m_fTargetAudioValue = m_iCurrentSeconds <= m_iRunningOutOfTime ? 1.f : 0.f;

    OnTimeChanged.Broadcast(m_iCurrentSeconds);
  }
  else
  {
    if (pTimeEvent) {
      FMOD_STUDIO_PLAYBACK_STATE pPlaybackState;
      pTimeEvent->getPlaybackState(&pPlaybackState);

      if (pPlaybackState == FMOD_STUDIO_PLAYBACK_PLAYING) {
        pTimeEvent->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
        pTimeEvent = nullptr;
      }
    }

    ClearTimer();
    UE_LOG(TimerManager, Log, TEXT("Timer has been cleaned"));
  }
}

void UTimeManager::AddTime(int _addMin, int _addSeg)
{
  m_iCurrentSeconds += _addMin * 60 + _addSeg;
  //******
  //Reset timer when adding?
  //What's up if the added time exceed the total initial time?
  //******

  OnTimeChanged.Broadcast(m_iCurrentSeconds);
  UE_LOG(TimerManager, Log, TEXT("Seconds: %d"), m_iCurrentSeconds);
}

int UTimeManager::GetCurrentSeconds() const {
  return m_iCurrentSeconds;
}

int UTimeManager::GetRunningOutTime() const {
  return m_iRunningOutOfTime;
}