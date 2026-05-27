 // Fill out your copyright notice in the Description page of Project Settings.


#include "EventManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Dialogs/DialogsManager.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Actor/Tutorial/Trigger.h"
#include "DorothysJob/Data/DataAssets/Tutorial/EventTutorialDataAsset.h"
#include "DorothysJob/Actor/Tutorial/TutorialArrow.h"
#include "Engine/Texture2DArray.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include <DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h>
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

#pragma region | Main Functions
void UEventManager::Init(UDialogsManager* _pDialogsManager, UEventTutorialDataAsset* _pEventWidgetDataAsset, TObjectPtr<UEventListDataAsset> _pGeneralEvents)
{
  m_pDialogsManager = _pDialogsManager;
  m_pDialogsManager->OnDialogFinish.AddUObject(this, &UEventManager::OnDialogueFinish);

  m_pEventWidgetDataAsset = _pEventWidgetDataAsset;

  m_pGeneralEventDataAsset = _pGeneralEvents;

  m_oHubLevelTutNum = { 0 ,1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0 };

  // If a trigger activates start the event manager and all the events logic.
  ATrigger::OnTriggerEnter.AddDynamic(this, &UEventManager::TriggerEvent);

  m_oActualStatus = EBaseGameState::PLAYING;

  // Get Save Game Manager.
  if (UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance()))
  {
    m_pSaveGameManager = pBaseGameInstance->GetSubsystem<USaveGameManager>();
  }
}

void UEventManager::OnTriggerEventsStart(FName _sTrigerId, const FEventDataArray& _oEventDataArray)
{
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance)
  {
    return;
  }

  // Actualizar progresión por nivel si aplica
  FString TriggerIdStr = _sTrigerId.ToString();
  if (TriggerIdStr.StartsWith(TEXT("LV_")))
  {
    FString AfterPrefix = TriggerIdStr.RightChop(3);

    TArray<FString> Parts;
    AfterPrefix.ParseIntoArray(Parts, TEXT("_"), true);

    int32 LevelNum = (Parts.Num() > 0) ? FCString::Atoi(*Parts[0]) : 0;

    if (!pBaseGameInstance->m_oHubEventTriggered.Contains(LevelNum))
    {
      pBaseGameInstance->m_oHubEventTriggered.Add(LevelNum, 0);
    }

    int32& CurrentValue = pBaseGameInstance->m_oHubEventTriggered[LevelNum];

    if (LevelNum < m_oHubLevelTutNum.Num() && CurrentValue < m_oHubLevelTutNum[LevelNum])
    {
      UE_LOG(LogTemp, Warning, TEXT("Current Value Before: %d"), CurrentValue);
      CurrentValue++;
      UE_LOG(LogTemp, Warning, TEXT("Current Value After: %d"), CurrentValue);
    }
  }

  // Lanzar triggers del evento
  OnEventsStart(_oEventDataArray.m_aTutorialData);
}

void UEventManager::TriggerEvent(FName _sEventName)
{
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance)
  {
    return;
  }

  m_sCurrentEventName = _sEventName;

  if (!m_pGeneralEventDataAsset)
  {
    UE_LOG(LogTemp, Warning, TEXT("TriggerEvent: DataAsset general no está asignado"));
    return;
  }

  FEventDataArray* pDataArray = m_pGeneralEventDataAsset->m_oAllEventData.Find(_sEventName);

  // Evento inexistente o ya ejecutado terminar
  if (!pDataArray || pDataArray->m_bHasAlreadyAppear)
  {
    HandleEventFinish();
    return;
  }

  // Marcar como ya aparecido
  pDataArray->m_bHasAlreadyAppear = true;

  if (_sEventName == FName("INIT_LEVEL_0"))
  {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->UnlockAchievement("TwinSqueezeShooters");
    }
  }

  if (_sEventName == FName("LV_2_WEAPON_CHEST"))
  {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->UnlockAchievement("RememberSwishAndClean");
    }
  }

  if (_sEventName == FName("LV_4_WEAPON_CHEST"))
  {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->UnlockAchievement("TidyTirant");
    }
  }

  if (_sEventName == FName("LV_6_WEAPON_CHEST"))
  {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->UnlockAchievement("ComeWithMeIfYouWantToClean");
    }
  }

  m_bHasSkipEvent = false;

  // Arrancar triggers de este evento
  OnTriggerEventsStart(_sEventName, *pDataArray);
}

void UEventManager::OnEventsStart(TArray<FTriggerData> _oEventDataArray)
{
  m_iEventArrayIndex = 0;
  m_oEventArray = _oEventDataArray;

  if (m_oEventArray.IsEmpty())
  {
    UE_LOG(LogTemp, Warning, TEXT("OnEventsStart: No events in the array."));
    return;
  }

  // Empezar el primer evento
  StartEvent(m_oEventArray[m_iEventArrayIndex]);
}

void UEventManager::HandleEventFinish()
{
  OnAllEventFinish.Broadcast();

  const FString& EventNameStr = m_sCurrentEventName.ToString();

  if (EventNameStr.StartsWith(TEXT("ENTER_HUB_")))
  {
    OnEnterHubFinish.Broadcast();
  }
}
#pragma endregion

#pragma region | Event Functions
void UEventManager::StartEvent(FTriggerData _oEventData)
{
  // Save the actual event type.
  m_pEventType = _oEventData.m_eTutorialType;

  // Depending of the type of the event:
  switch (m_pEventType)
  {
  case ETutorialType::DIALOGUE:
    DoDialogEvent(_oEventData);
    break;
  case ETutorialType::PERMANENT_WIDGET:
    DoPermanentEvent(_oEventData);
    break;
  case ETutorialType::TEMPORAL_WIDGET:
    DoTemporalEvent(_oEventData);
    break;
  case ETutorialType::ARROW:
    DoArrowEvent(_oEventData);
    break;
  case ETutorialType::HIDEARROW:
    DoArrowHideEvent(_oEventData);
    break;
  case ETutorialType::REVEAL:
    DoRevealEvent(_oEventData);
    break;
  default:
    UE_LOG(LogTemp, Warning, TEXT("OnEventsStart: Tutorial type don't found (%d)."), (int32)m_pEventType);
    break;
  }
}

void UEventManager::OnEventContinue()
{
  // If the level is actually in a dialog, go to the dialog next line
  // This is simple, if it's the last line in the dialog the dialog manager will exit the dialog state so it will go to the on event finish
  // if all event finish or will pass to the next event in line.
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (pLevelManager->GetCurrentGameState() == EBaseGameState::DIALOGUE)
  {
    m_pDialogsManager->OnNextDialogLine();
    return;
  }

  PlaySilentKillerEvent();

  // If the dialog ends or if its other type of event:
  // Escalate the index and check if its valid.
  int32 eventIndex = m_iEventArrayIndex + 1;
  int32 Count = m_oEventArray.Num();
  if (eventIndex >= Count)
  {
    //If There are no more events in the array, finish:
    OnEventFinish();
    return;
  }

  //If there is another event in the array:
  m_iEventArrayIndex = eventIndex;
  FTriggerData oEventData = m_oEventArray[m_iEventArrayIndex];
  StartEvent(oEventData);
}

void UEventManager::OnSkipEvent()
{
  m_bHasSkipEvent = true;

  // If there is a dialog that is going to be skip:
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (pLevelManager->GetCurrentGameState() == EBaseGameState::DIALOGUE)
  {
    m_pDialogsManager->OnSkipDialog();
  }

  // If there are no events to skip return. This is for when the dialog don't work like an event.
  if (m_oEventArray.Num() <= 0)
  {
    return;
  }

  for (FTriggerData& Event : m_oEventArray)
  {
    if (Event.m_eTutorialType == ETutorialType::REVEAL)
    {
      DoRevealEvent(Event);
    }
  }

  OnEventFinish();
}
#pragma endregion

#pragma region | PreLevel Functions
bool UEventManager::CanStartLevel(int level)
{
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance || !m_oHubLevelTutNum.IsValidIndex(level))
  {
    return false;
  }

  int32& CurrentValue = pBaseGameInstance->m_oHubEventTriggered.FindOrAdd(level);
  UE_LOG(LogTemp, Warning, TEXT("Current Value: %d, Minimum Value: %d"), CurrentValue, m_oHubLevelTutNum[level]);
  return CurrentValue >= m_oHubLevelTutNum[level];
}

void UEventManager::SetIsEventActive(FName _sEventName, bool _bEventIsActive)
{
  FEventDataArray* pDataArray = m_pGeneralEventDataAsset->m_oAllEventData.Find(_sEventName);
  
  if (!pDataArray)
  {
    return;
  }

  pDataArray->m_bHasAlreadyAppear = _bEventIsActive;
}
#pragma endregion

#pragma region | On weapon and consumable screen functions
void UEventManager::SetCurrentStatus(EBaseGameState _status)
{
  m_oActualStatus = _status;
}
#pragma endregion

#pragma region | Each event functions
void UEventManager::DoDialogEvent(FTriggerData _oEventData)
{
  if (m_bHasSkipEvent)
  {
    OnDialogueFinish();
    return;
  }

  // If there is no level manager, return.
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!pLevelManager)
  {
    UE_LOG(LogTemp, Error, TEXT("OnEventsStart: LevelManager no encontrado."));
    return;
  }

  // And then if the id is a valid id, is not empty or something like that, set the current game state and init the dialog sequence.
  if (_oEventData.m_iId.IsValid())
  {
    pLevelManager->SetCurrentGameState(EBaseGameState::DIALOGUE);
    m_pDialogsManager->OnInitSequence(_oEventData.m_iId);
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("OnEventsStart: EventData.m_sId is not a valid id"));
  }
}

void UEventManager::DoPermanentEvent(FTriggerData _oEventData)
{
  if (m_bHasSkipEvent)
  {
    OnSkipPermanentWidget();
    return;
  }

  // If there is no level manager, return.
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!pLevelManager)
  {
    UE_LOG(LogTemp, Error, TEXT("OnEventsStart: LevelManager no encontrado."));
    return;
  }

  // If there is no permanent data asset get out.
  if (!m_pEventWidgetDataAsset)
  {
    UE_LOG(LogTemp, Error, TEXT("OnEventsStart: PermanentWidgetDataAsset is nullptr."));
    return;
  }

  //If there is a widget, show the widget:
  if (pLevelManager->GetCurrentGameState() != EBaseGameState::WIDGET)
  {
    pLevelManager->SetCurrentGameState(EBaseGameState::WIDGET);
  }// else, dont change the game state because is currently ok.

  FName Id = _oEventData.m_iId;
  OnPermanentEventShow.Broadcast(Id);
}

void UEventManager::DoTemporalEvent(FTriggerData _oEventData)
{
  // If there is no level manager, return.
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!pLevelManager)
  {
    UE_LOG(LogTemp, Error, TEXT("OnEventsStart: LevelManager no encontrado."));
    return;
  }

  // If the temporal widget data asset is not empty.
  if (!m_pEventWidgetDataAsset)
  {
    UE_LOG(LogTemp, Error, TEXT("OnEventsStart: TemporalWidgetDataAsset is nullptr."));
    return;
  }

  // Find temporal widget by id:
  FEventData* EventData = m_pEventWidgetDataAsset->m_lTemporalEventData.Find(_oEventData.m_iId);

  //If there is no temporal widget whit that id get out.
  if (!EventData)
  {
    UE_LOG(LogTemp, Warning, TEXT("OnEventsStart: Didn't find a event data with id %s in TemporalWidgetDataAsset."), *_oEventData.m_iId.ToString());
    OnEventContinue();
    return;
  }

  if (EventData->m_sText.IsEmpty())
  {
    UE_LOG(LogTemp, Warning, TEXT("OnEventsStart: Temporal Widget text is empty"));
  }

  m_sText = EventData->m_sText;
  OnTemporalEventShow.Broadcast(m_sText);

  // Start temportal widget timer.
  if (EventData->m_fDuration > 0.f)
  {
    m_fDuration = EventData->m_fDuration;
    GetWorld()->GetTimerManager().SetTimer(
      TemporalWidgetTimerHandle,
      this,
      &UEventManager::OnTemporalWidgetFinish,
      m_fDuration,
      false
    );
  }
  else
  {
    UE_LOG(LogTemp, Warning, TEXT("OnEventsStart: Temporal Widget duration is <= 0."));
  }
}

void UEventManager::DoArrowEvent(FTriggerData _oEventData)
{
  TArray<AActor*> aOutActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATutorialArrow::StaticClass(), aOutActors);

  for (AActor* Actor : aOutActors)
  {
    if (ATutorialArrow* Arrow = Cast<ATutorialArrow>(Actor))
    {
      if (Arrow->GetArrowId() == _oEventData.m_iId)
      {
        FEventDataArray* pDataArray = m_pGeneralEventDataAsset->m_oAllEventData.Find(m_sCurrentEventName);
        pDataArray->m_bHasAlreadyAppear = false;

        ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
        pLevelManager->m_aArrows.Add(m_sCurrentEventName);

        Arrow->Active();
      }
    }
  }

  OnEventContinue();
}

void UEventManager::DoArrowHideEvent(FTriggerData _oEventData)
{
  TArray<AActor*> aOutActors;
  UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATutorialArrow::StaticClass(), aOutActors);

  for (AActor* Actor : aOutActors)
  {
    if (ATutorialArrow* Arrow = Cast<ATutorialArrow>(Actor))
    {
      if (Arrow->GetArrowId() == _oEventData.m_iId)
      {
        ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
        for (size_t i = 0; i < pLevelManager->m_aArrows.Num(); i++)
        {
          FEventDataArray* pDataArray = m_pGeneralEventDataAsset->m_oAllEventData.Find(pLevelManager->m_aArrows[i]);
          pDataArray->m_bHasAlreadyAppear = true;
        }

        Arrow->DesActive();
      }
    }
  }

  OnEventContinue();
}

void UEventManager::DoRevealEvent(FTriggerData _oEventData)
{
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance)
  {
    return;
  }

  if (UEnum* EnumPtr = StaticEnum<EHUDSection>())
  {
    int64 Value = EnumPtr->GetValueByName(_oEventData.m_iId);
    if (Value != INDEX_NONE) // Si encontró el valor
    {
      EHUDSection Section = static_cast<EHUDSection>(Value);
      OnRevealHUD.Broadcast(Section);
      if (pBaseGameInstance->m_oHUDData.Contains(Section))
      {
        pBaseGameInstance->m_oHUDData[Section] = true;
      }
      UE_LOG(LogTemp, Log, TEXT("Convert to enum: %d"), static_cast<int32>(Section));
    }
    else
    {
      UE_LOG(LogTemp, Warning, TEXT("FName '%s' it's not a EHUDSection"), *_oEventData.m_iId.ToString());
    }
  }

  if (UWorld* World = GetWorld())
  {
    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(
      TimerHandle,
      this,
      &UEventManager::OnEventContinue,
      1.2f,
      false
    );
  }
}
#pragma endregion

#pragma region | Private General Event
void UEventManager::OnEventFinish()
{
  UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pBaseGameInstance)
  {
    return;
  }

  if (m_sCurrentEventName == FName("LV_3_CONSUMABLES_CHEST"))
  {
    if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->UnlockAchievement("Free2Wash");
    }
  }

  // Validate index.
  if (!m_oEventArray.IsValidIndex(m_iEventArrayIndex))
  {
    UE_LOG(LogTemp, Warning, TEXT("Invalid index in m_oEventArray: %d"), m_iEventArrayIndex);
    return;
  }

  // Restore the game status only if the status is widget, because de dialog already do it in dialog manager.
  const FTriggerData& oEventData = m_oEventArray[m_iEventArrayIndex];
  if (oEventData.m_eTutorialType == ETutorialType::PERMANENT_WIDGET &&
    m_oActualStatus != EBaseGameState::SELECTWEAPON &&
    m_oActualStatus != EBaseGameState::SELECTCONSUMABLE &&
    m_oActualStatus != EBaseGameState::PRELEVEL &&
    m_oActualStatus != EBaseGameState::REPLAY)
  {
    if (ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>())
    {
      pLevelManager->SetCurrentGameState(m_oActualStatus);
    }
  }

  // Save event progress
  if (IsValid(m_pSaveGameManager))
  {
    m_pSaveGameManager->SaveEventsTriggers();
  }

  // Reset manager state
  ResetEventState();

  // General nofitication
  OnAllEventFinish.Broadcast();

  // Especial case: when finish hub event.
  const FString TriggerIdStr = m_sCurrentEventName.ToString();
  if (TriggerIdStr.StartsWith(TEXT("ENTER_HUB_")))
  {
    OnEnterHubFinish.Broadcast();
  }
}

void UEventManager::ResetEventState()
{
  m_sCurrentEventName = "";
  m_pIsDialogueFinish = false;
  m_iEventArrayIndex = -1;
  m_pImage = nullptr;
  m_sText = FText::GetEmpty();
  m_fDuration = 0.f;
  m_oEventArray.Empty();
}
#pragma endregion

#pragma region | Private dialog function	
void UEventManager::OnDialogueFinish()
{
  // On dialog finish, continue with next event.
  // From here, the game state will be on playing so it will pass the first if from OnEventContinue().
  m_pIsDialogueFinish = true;
  if (m_oEventArray.Num() > 0)
  {
    OnEventContinue();
  }
}
#pragma endregion

#pragma region | Private permanent widget function
void UEventManager::OnSkipPermanentWidget()
{
  // Set the current gam estate to playing again.
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  pLevelManager->SetCurrentGameState(EBaseGameState::PLAYING);

  // Go to next event.
  int32 CurrentIndex = m_iEventArrayIndex;
  int32 ArraySize = m_oEventArray.Num();

  while (CurrentIndex < ArraySize && m_oEventArray[CurrentIndex].m_eTutorialType == ETutorialType::PERMANENT_WIDGET)
  {
    CurrentIndex++;
  }

  m_iEventArrayIndex = CurrentIndex;

  if (m_iEventArrayIndex >= m_oEventArray.Num())
  {
    // We are at the end of the event array so we finish all actions
    m_iEventArrayIndex = -1;
    m_pImage = nullptr;
    m_sText = FText::GetEmpty();
    m_fDuration = 0.f;
  }
  else
  {
    // There are more elements like dialogues:
    FTriggerData oEventData = m_oEventArray[m_iEventArrayIndex];
    StartEvent(oEventData);
  }
}
#pragma endregion

#pragma region | Private tempotal widget function
void UEventManager::OnTemporalWidgetFinish()
{
  // Clear Timer:
  GetWorld()->GetTimerManager().ClearTimer(TemporalWidgetTimerHandle);

  // Close the window:
  OnTemporalEventClose.Broadcast();

  // Comtimue with next event
  OnEventContinue();
}
void UEventManager::ActiveTriggersFromLevel(int _iLevel)
{
  if (!m_pGeneralEventDataAsset) 
  {
    UE_LOG(LogTemp, Warning, TEXT("ActiveTriggersFromLevel: DataAsset no está asignado"));
    return;
  }

  for (const TPair<FName, FEventDataArray>& Pair : m_pGeneralEventDataAsset->m_oAllEventData)
  {
    const FName& EventName = Pair.Key;
    const FEventDataArray& EventData = Pair.Value;

    if (EventData.m_iEventLevel == _iLevel)
    {
      for (const FTriggerData& Trigger : EventData.m_aTutorialData)
      {
        if (Trigger.m_eTutorialType == ETutorialType::REVEAL)
        {
          DoRevealEvent(Trigger);
        }
      }
      FEventDataArray& MutableEventData = m_pGeneralEventDataAsset->m_oAllEventData[EventName];
      MutableEventData.m_bHasAlreadyAppear = true;
    }
  }
}
#pragma endregion

#pragma region | Audio
void UEventManager::PlaySilentKillerEvent() {
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this))) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, pGameInstance->m_pBabyKillerEvent);
    }
  }
}
#pragma endregion