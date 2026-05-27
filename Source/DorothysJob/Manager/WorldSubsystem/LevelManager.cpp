/************************************************************************
 * @description: The LevelManager implementation
 * @author: Josephine Esposito
 * @date: 24/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#include "LevelManager.h"
#include "TimeManager.h"              // for the timer delegate
#include "Logging/StructuredLog.h"    // for logging
#include "Kismet/GameplayStatics.h"   // to pause the game level
#include "LevelInstance/LevelInstanceActor.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Actor/Room/Tile/BaseTile.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Controller/BasePlayerController.h"

#include "DorothysJob/Data/DataTables/LevelData.h"
#include "DorothysJob/Data/DataTables/ScorePointsData.h"
#include "DorothysJob/Data/DataAssets/ScoreDataAsset.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/ConsumableDataAsset.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Data/DataAssets/Tutorial/EventListDataAsset.h"

#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Manager/ProceduralGeneration.h"
#include "DorothysJob/Manager/Dialogs/DialogsManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SettingsManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"

#include "DorothysJob/Settings/SettingTypes.h"
#include "DorothysJob/Settings/GeneralSettingsManager.h"
 // FMOD
#include "FMODBlueprintStatics.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

// Achievements
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

DEFINE_LOG_CATEGORY(LevelManager);

#pragma region | Base Virtual Methods
void ULevelManager::Initialize(FSubsystemCollectionBase& Collection)
{
  Super::Initialize(Collection);

  UE_LOG(LevelManager, Log, TEXT("ULevelManager::Initialize World '%s'"), *GetWorld()->GetMapName());
}

void ULevelManager::PostInitialize()
{
  Super::PostInitialize();

  UTimeManager* pTimeManager = GetWorld()->GetSubsystem<UTimeManager>();
  if (pTimeManager)
  {
    pTimeManager->OnTimeChanged.AddUObject(this, &ULevelManager::HandleTimerExpired);
  }

  UE_LOG(LevelManager, Log, TEXT("ULevelManager::PostInitialize World '%s'"), *GetWorld()->GetMapName());
}

void ULevelManager::OnWorldBeginPlay(UWorld& InWorld)
{
  Super::OnWorldBeginPlay(InWorld);

  if (IsValid(GetWorld()))
  {
    pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
    if (IsValid(pGameInstance) && IsValid(pGameInstance->m_pGeneralEventsDataAsset))
    {
      pListDataAssets = pGameInstance->m_pGeneralEventsDataAsset;
    }
  }

  CurrentGameState = EBaseGameState::PLAYING;
  OnGameStateChanged.Broadcast(CurrentGameState);

  ProceduralGeneration = NewObject<UProceduralGeneration>(this);

  UE_LOG(LevelManager, Log, TEXT("ULevelManager::OnWorldBeginPlay World '%s'"), *GetWorld()->GetMapName());

  UBaseRoom::OnFloorCleaned.AddUObject(this, &ULevelManager::OnRoomCleaned);
  OnAllRoomsCreated.AddDynamic(this, &ULevelManager::AllRoomsCreated);

  // Score
  m_fExtraTime = 0.0f;
  m_iStinkyTimes = 0;
  m_iConsumableUses = 0;
  m_bHasDorothyBeenHit = false;
  m_bHasTimerExceeded = false;
}

void ULevelManager::Deinitialize()
{
  Super::Deinitialize();

  UTimeManager* pTimeManager = GetWorld()->GetSubsystem<UTimeManager>();
  if (pTimeManager)
  {
    pTimeManager->OnTimeChanged.RemoveAll(this);
  }

  // Score
  m_fExtraTime = 0.0f;
  m_iStinkyTimes = 0;
  m_iConsumableUses = 0;
  m_bHasDorothyBeenHit = false;
  m_bHasTimerExceeded = false;

  UE_LOG(LevelManager, Log, TEXT("ULevelManager::Deinitialize World '%s'"), *GetWorld()->GetMapName());
}
#pragma endregion

const TArray<TObjectPtr<UBaseRoom>>& ULevelManager::GetRooms() const
{
  return m_lRooms;
}

void ULevelManager::AddRoom(UBaseRoom* RoomToAdd)
{
  m_lRooms.Add(RoomToAdd);
}

void ULevelManager::OnLevel0DialogInit()
{
  FString SequenceNameText = FString::Printf(TEXT("INIT_LEVEL_0"));
  FName SequenceName(*SequenceNameText);

  ABasePlayer* pPlayer = Cast<ABasePlayer>(GetWorld()->GetPlayerControllerIterator()->Get()->GetPawn());
  if (IsValid(pPlayer))
  {
    int32 damage = 95;
    pPlayer->SetCurrentHealth(damage);
  }

  UEventManager* pEventManager = pGameInstance->GetEventManager();
  pEventManager->TriggerEvent(SequenceName);

  if (UWorld* World = GetWorld())
  {
    // Llama a OnLevelInit después de 1.5 segundos
    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(
      TimerHandle,
      this,
      &ULevelManager::OnLevelInit,
      1.5f,   // delay en segundos
      false   // no repetir
    );
  }
}

#pragma region | Private Functions
void ULevelManager::AllRoomsCreated()
{
  m_bAreAllRoomsCreated = true;

  UWorld* World = GetWorld();
  if (!World)
  {
    return;
  }

  if (pGameInstance->m_iCurrentLevel == 0)
  {
    FTimerHandle TimerHandle2;
    World->GetTimerManager().SetTimer(
      TimerHandle2,
      this,
      &ULevelManager::OnLevel0DialogInit,
      0.5f,   // delay en segundos
      false   // no repetir
    );
  }
  else
  {
    // Llama a OnLevelInit después de 2 segundos
    FTimerHandle TimerHandle;
    World->GetTimerManager().SetTimer(
      TimerHandle,
      this,
      &ULevelManager::OnLevelInit,
      2.0f,   // delay en segundos
      false   // no repetir
    );
  }
}

void ULevelManager::HandleTimerExpired(int _seconds)
{ // delegate handler
  if (_seconds <= 0)
  {
    m_fExtraTime = 0.0f;
    m_bHasTimerExceeded = false;
    OnGameOver();
  }
}

void ULevelManager::SetPauseMode(bool _status)
{
  UWorld* pWorld = GetWorld();
  if (!pWorld)
  {
    UE_LOG(LevelManager, Error, TEXT("ULevelManager::SetPauseMode Unable to get current World"));
    return;
  }

  // we get the player controller

  ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  if (!pPlayerController)
  {
    UE_LOG(LevelManager, Error, TEXT("ULevelManager::SetPauseMode Unable to get current Player"));
    return;
  }

  // we pause the game and the timer will also be paused and unpaused here
  UGameplayStatics::SetGamePaused(GetWorld(), _status);

  if (_status) OnGamePaused.Broadcast();
  else OnGameUnpaused.Broadcast();

  if (_status && CurrentGameState != EBaseGameState::DIALOGUE && CurrentGameState != EBaseGameState::WIDGET) pPlayerController->SwitchInputMode(EPlayerInputMode::UI);
  else if (CurrentGameState == EBaseGameState::DIALOGUE || CurrentGameState == EBaseGameState::WIDGET) pPlayerController->SwitchInputMode(EPlayerInputMode::EVENT);
  else pPlayerController->SwitchInputMode(EPlayerInputMode::GAMEPLAY);
}

void ULevelManager::OnLevelInit()
{
  if (!pGameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameInstance not found"));
    return;
  }

  // Init level time for achievements.
  if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
    pAchSubsystem->m_fLevelStartTime = UGameplayStatics::GetTimeSeconds(GetWorld());
    pGameInstance->m_lSelectedConsumables.ContainsByPredicate([](const UConsumableDataAsset* pCons){ return pCons && pCons->m_eConsumableClass == EConsumableClass::PRISTINETIME;}) ? pAchSubsystem->m_bEquipApron = true : pAchSubsystem->m_bEquipApron = false;
  }

  OnGameStateChanged.Broadcast(CurrentGameState);
}

void ULevelManager::OnLevelFinish()
{
  if (!pGameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameInstance not found"));
    return;
  }

  // Get achievement subsystem.
  UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>();
  if (pAchSubsystem)
  {
    pAchSubsystem->SetNotUseApronAchievement();
    
    // Reset level variables for achievements.
    pAchSubsystem->ResetTemporalLevelVariables();

    // If pass tutorial achievement.
    if (pGameInstance->m_iCurrentLevel == 0)
    {
      pAchSubsystem->UnlockAchievement("ShesNoIntern");
    }
    else if (pGameInstance->m_iCurrentLevel == 10) // If pass game (level 10) achievement.
    {
      pAchSubsystem->UnlockAchievement("DorothysJob");
      pAchSubsystem->SetGGSqueakyCleanAchievement();
      pAchSubsystem->Set2hTimeAchievement();
    }

    if (pGameInstance->m_iHighestLevelAchieved >= 10)
    {
      pAchSubsystem->CheckSqueakyMeanAchievement();
    }
  }

  // Init final level dialogue.
  const FString SequenceNameText = FString::Printf(TEXT("Lev_%d_End"), pGameInstance->m_iCurrentLevel);
  const FName SequenceName(*SequenceNameText);

  OnGameDialogue(true, SequenceName);

  // Play Killer Sound Event.
  if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
    pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
  }

  // Set nex level after show dialog
  TArray<FLevelData*> m_lLevelData;
  pGameInstance->m_pLevelData->GetAllRows(TEXT("Rooms"), m_lLevelData);
  if (m_lLevelData.IsValidIndex(pGameInstance->m_iCurrentLevel))
  {
    if (pGameInstance->m_iCurrentLevel >= pGameInstance->m_iHighestLevelAchieved)
    {
      pGameInstance->m_iCurrentLevel = FMath::Clamp(pGameInstance->m_iCurrentLevel + 1, 0, m_lLevelData.Num() - 1);
      pGameInstance->m_iHighestLevelAchieved = pGameInstance->m_iCurrentLevel;
      pGameInstance->UpdateAvailableWeaponsAndConsumables(pGameInstance->m_iCurrentLevel);

      USaveGameManager* pSaveGameManager = pGameInstance->GetSubsystem<USaveGameManager>();
      if (pSaveGameManager)
      {
        pSaveGameManager->SaveCurrentLevel(pGameInstance->m_iCurrentLevel, pGameInstance->m_iHighestLevelAchieved);
      }
    }
    else
    {
      pGameInstance->m_iCurrentLevel = pGameInstance->m_iHighestLevelAchieved;
      pGameInstance->UpdateAvailableWeaponsAndConsumables(pGameInstance->m_iCurrentLevel);
      pGameInstance->UpdateSelectedWeapons(); //Must be called after UpdateAvailableWeaponsAndConsumables
    }

    //CONSUMABLES
    if (!pGameInstance->m_lConsumables.IsEmpty() && pGameInstance->m_lSelectedConsumables.IsEmpty())
    {
      pGameInstance->UpdateSelectedConsumables();
    }
  }
}

void ULevelManager::StartEvent()
{
  EBaseGameState eGameState = EBaseGameState::DIALOGUE;
  SetCurrentGameState(eGameState);
}
#pragma endregion

#pragma region | Default Get and Set Game Status
EBaseGameState ULevelManager::GetCurrentGameState() const
{
  return CurrentGameState;
}

void ULevelManager::SetCurrentGameState(EBaseGameState _status)
{
  UE_LOG(LevelManager, Log, TEXT("ULevelManager::SetCurrentGameState > state is %d"), static_cast<uint8>(_status));
  if (CurrentGameState == _status)
  {
    return;
  }

  switch (_status)
  {
  case EBaseGameState::PLAYING:
    CurrentGameState = _status;
    OnGameStateChanged.Broadcast(CurrentGameState);
    SetPauseMode(false);
    break;
  case EBaseGameState::GAMEOVER:
    OnGameOver();
    break;
  case EBaseGameState::WIN:
    if (UWorld* CachedWorld = GetWorld())
    {
      // Play Killer Sound Event.
      if (IsValid(pGameInstance)) {
        if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
          pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
        }
      }

      FTimerDelegate TimerDel;
      TimerDel.BindWeakLambda(this, [this]()
        {
          OnGameWin();
        });

      CachedWorld->GetTimerManager().SetTimer(m_oAudioHandle, TimerDel, 1.0f, false);
    }
    break;
  case EBaseGameState::PAUSED:
    OnGamePause(true);
    break;
  case EBaseGameState::DIALOGUE:
    CurrentGameState = _status;
    OnGameStateChanged.Broadcast(CurrentGameState);
    SetPauseMode(true);
    break;
  case EBaseGameState::WIDGET:
    OnGameEvent(true);
    break;
  case EBaseGameState::SELECTCONSUMABLE:
  case EBaseGameState::SELECTWEAPON:
  case EBaseGameState::PRELEVEL:
  case EBaseGameState::REPLAY:
    CurrentGameState = _status;
    OnGameStateChanged.Broadcast(CurrentGameState);
    break;
  default:
    return;
  }
}

void ULevelManager::HandleEnemiesBuffed(UBaseRoom* Room)
{
  OnEnemiesBuffed.Broadcast();
}

void ULevelManager::HandleEnemiesDebuffed(UBaseRoom* Room)
{
  OnEnemiesDebuffed.Broadcast();
}
#pragma endregion

#pragma region Getters
UBaseRoom* ULevelManager::GetCurrentRoom() const
{
  return m_pCurrentRoom;
}

FFinalScore* ULevelManager::GetFinalScore() const
{ //@review this method needs some refactoring asap
  /// creating the objects for the score
  FFinalScore* pFinalScore = new FFinalScore();
  int32 iTotalScore = 0;
  int32 iTotalConsumablesNumber = 0;
  int32 iCurrentConsumablesNumber = 0;
  int32 iStinkyTimes = 0;
  float fExtraTime = 0.0f;
  float fConsumablesPercentage = 0.0f;

  /// calculating the total score
  // -> we obtain this from the data asset
  if (!IsValid(pGameInstance))
  {
    UE_LOG(LevelManager, Error, TEXT("[ULevelManager::GetFinalScore] The game instance is invalid?"));
    return nullptr;
  }

  /// extra time
  if (pGameInstance->m_iCurrentLevel == 0)
  {
    // adding the max for the time
    pFinalScore->m_iTimePoints = -1;
    fExtraTime = 200;
  }
  else
  {
    UTimeManager* pTimeManager = GetWorld()->GetSubsystem<UTimeManager>();
    if (pTimeManager)
    {
      fExtraTime = pTimeManager->GetCurrentSeconds();
    }
  }
  for (FBaseScore pScore : pGameInstance->m_pScores->m_lExtraTime)
  {
    if (pScore.m_iMinLimit <= fExtraTime && fExtraTime <= pScore.m_iMaxLimit)
    {
      iTotalScore += pScore.m_iAddedPoints;
      if (pFinalScore->m_iTimePoints != -1)
      {
        pFinalScore->m_iTimePoints = pScore.m_iAddedPoints * 100;
      }
    }
  }

  /// consumables used
  if (!pGameInstance->m_lSelectedConsumables.IsEmpty())
  {
    iTotalConsumablesNumber = pGameInstance->m_lSelectedConsumables[0]->m_iStackQuantity + pGameInstance->m_lSelectedConsumables[1]->m_iStackQuantity;
  }
  if (pGameInstance->m_iCurrentLevel < 3)
  {
    // adding the max for the used consumables
    pFinalScore->m_iConsumablesPoints = -1;
    fConsumablesPercentage = 100.0f;
  }
  else
  {
    // > calculating the percentage
    fConsumablesPercentage = ((m_iConsumableUses >= iTotalConsumablesNumber) ? 1 : ((m_iConsumableUses * 1.f) / iTotalConsumablesNumber)) * 100.f;
  }
  for (FBaseScore pScore : pGameInstance->m_pScores->m_lUsedConsumables)
  {
    if (pScore.m_iMinLimit <= fConsumablesPercentage && fConsumablesPercentage <= pScore.m_iMaxLimit)
    {
      iTotalScore += pScore.m_iAddedPoints;
      if (pFinalScore->m_iConsumablesPoints != -1)
      {
        pFinalScore->m_iConsumablesPoints = pScore.m_iAddedPoints * 100;
      }
    }
  }

  // Stinky times
  iStinkyTimes = m_iStinkyTimes;
  if (pGameInstance->m_iCurrentLevel == 0)
  {
    // adding the max for the cleaning stations
    pFinalScore->m_iCleaningStationPoints = -1;
    iStinkyTimes = 0;
  }
  for (FBaseScore pScore : pGameInstance->m_pScores->m_lCleaningStationUses)
  {
    if (pScore.m_iMinLimit <= iStinkyTimes && iStinkyTimes <= pScore.m_iMaxLimit)
    {
      iTotalScore += pScore.m_iAddedPoints;
      if (pFinalScore->m_iCleaningStationPoints != -1)
      {
        pFinalScore->m_iCleaningStationPoints = pScore.m_iAddedPoints * 100;
      }
    }
  }

  /// calculating the two starts
  pFinalScore->m_bHasBeenHit = m_bHasDorothyBeenHit;
  TArray<FLevelData*> lLevelData;
  pGameInstance->m_pLevelData->GetAllRows(TEXT("LevelData"), lLevelData);
  if (lLevelData.IsEmpty())
  {
    pFinalScore->m_bIsTimerExtra = false;
  }
  else
  {
    float fCurrentTime = lLevelData[pGameInstance->m_iCurrentLevel]->Minutes * 60 + lLevelData[pGameInstance->m_iCurrentLevel]->Seconds;
    pFinalScore->m_bIsTimerExtra = false;
    if (fExtraTime > fCurrentTime)
    {
      pFinalScore->m_bIsTimerExtra = true;
    }
  }

  /// setting the final score
  pFinalScore->m_iTotalScore = iTotalScore;
  if (fExtraTime <= 0.0f)
  {
    pFinalScore->m_iTotalScore = 0;
  }

  /// returning the final score
  return pFinalScore;
}
#pragma endregion

#pragma region | Setters
void ULevelManager::OnGameOver()
{
  if (CurrentGameState == EBaseGameState::GAMEOVER) // setting the game mode
  {
    return;
  }

  m_bHasTimerExceeded = false;
  if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) 
  {
    pAchSubsystem->m_iDeathCounter++;
    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Contador muertes: %d"), pAchSubsystem->m_iDeathCounter));
  }
 
  // Play Killer Sound Event.
  if (IsValid(pGameInstance)) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
    }
  }

  if (UWorld* pWorld = GetWorld()) {
    TWeakObjectPtr<ULevelManager> WeakThis(this);
    pWorld->GetTimerManager().SetTimer(
      m_oAudioHandle,
      [WeakThis]() {
        if (WeakThis.IsValid()) {
          FMOD::Studio::EventInstance* pEvent = nullptr;
          FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
          if (pStudioSystem) {
            FMOD::Studio::EventDescription* pEventDesc = nullptr;
            pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/UI/HUD/SFX_HUD_youLose", &pEventDesc);

            if (pEventDesc) pEventDesc->createInstance(&pEvent);
            if (pEvent) pEvent->start();
          }

          WeakThis->SetPauseMode(true);
        }
      },
      .1f,
      false
    );
  }

  CurrentGameState = EBaseGameState::GAMEOVER; // setting the game to gameover
  OnGameStateChanged.Broadcast(CurrentGameState); // updating the delegate

  // pausing the game and timer
  UE_LOG(LevelManager, Display, TEXT("ULevelManager::OnGameOver World '%s'"), *GetWorld()->GetMapName());
}

void ULevelManager::OnGameWin()
{
  if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>())
  {
    if (pGameInstance->m_iCurrentLevel != 0)
    {
      pAchSubsystem->SetPassLevelStinkyAchievement();
    }
    pAchSubsystem->SetWeaponCombinationAchievement();
    pAchSubsystem->m_iDeathCounter = 0;
  }

  if (CurrentGameState != EBaseGameState::WIN) // setting the game mode
  {
    CurrentGameState = EBaseGameState::WIN; // setting the game to gameover
    OnGameStateChanged.Broadcast(CurrentGameState); // updating the delegate
  }

  /// Audio
  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/UI/HUD/SFX_HUD_youWin", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }

  SetPauseMode(true); // pausing the game and timer

  UE_LOG(LevelManager, Display, TEXT("ULevelManager::OnGameWin Status is '%s'"), *UEnum::GetDisplayValueAsText(CurrentGameState).ToString());
}

void ULevelManager::OnGamePause(bool _toggle)
{
  CurrentGameState = _toggle && CurrentGameState != EBaseGameState::PAUSED ? EBaseGameState::PAUSED : EBaseGameState::PLAYING;
  OnGameStateChanged.Broadcast(CurrentGameState);

  if (pGameInstance) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      float fValue = _toggle ? 1.f : 0.f;
      pAudioManager->SetGlobalParameter({ {"isGame_paused", fValue} });
    }
  }

  SetPauseMode(_toggle);
  UE_LOG(LevelManager, Display, TEXT("ULevelManager::OnGamePause Is paused? '%s'"), _toggle ? TEXT("true") : TEXT("false"));
}

void ULevelManager::OnGameDialogue(bool _toggle, FName _sSequenceName)
{
  if (!_toggle)
  {
    SetCurrentGameState(EBaseGameState::PLAYING);
    OnGameStateChanged.Broadcast(CurrentGameState);
    UE_LOG(LogTemp, Warning, TEXT("Close game dialog"));
    return;
  }

  SetCurrentGameState(EBaseGameState::DIALOGUE);

  UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!GameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameIstance not found?"));
    return;
  }

  UDialogsManager* pDialogManager = GameInstance->GetDialoguesManager();
  pDialogManager->OnInitSequence(_sSequenceName);
}

void ULevelManager::OnGameEvent(bool _toggle)
{
  CurrentGameState = _toggle && CurrentGameState != EBaseGameState::WIDGET ? EBaseGameState::WIDGET : EBaseGameState::PLAYING;
  OnGameStateChanged.Broadcast(CurrentGameState);

  SetPauseMode(_toggle);
  UE_LOG(LevelManager, Display, TEXT("ULevelManager::OnGameWidget Is paused? '%s'"), _toggle ? TEXT("true") : TEXT("false"));
}

void ULevelManager::SetCurrentRoom(TObjectPtr<UBaseRoom> NewCurrentRoom)
{
  if (NewCurrentRoom && NewCurrentRoom != m_pCurrentRoom)
  {
    OnExitedRoom.Broadcast(m_pCurrentRoom);
    if (m_pCurrentRoom)
    {
      m_pCurrentRoom->OnEnemiesBuff.RemoveAll(this);
      m_pCurrentRoom->OnEnemiesDebuff.RemoveAll(this);
    }

    // Start Audio Ticking.
    m_bStartAudioInterp = true;
    m_fTargetAudioValue = NewCurrentRoom->GetDirtStage() == EDirtStage::CLEAN ? 0.f : 1.f;

    OnEnteredRoom.Broadcast(NewCurrentRoom);
    NewCurrentRoom->OnEnemiesBuff.AddUObject(this, &ULevelManager::HandleEnemiesBuffed);
    NewCurrentRoom->OnEnemiesDebuff.AddUObject(this, &ULevelManager::HandleEnemiesDebuffed);

    m_pCurrentRoom = NewCurrentRoom;
  }
}

void ULevelManager::AddStinkyTimes()
{
  ++m_iStinkyTimes;
}

void ULevelManager::AddConsumableUse()
{
  ++m_iConsumableUses;
}

void ULevelManager::HasDorothyBeenHit()
{
  if (!m_bHasDorothyBeenHit)
  {
    m_bHasDorothyBeenHit = true;
  }
}
#pragma endregion

// ========= THIS IS JUST FOR ALPHA DELETE THIS AFTER ============
void ULevelManager::LoadNotProceduralRooms(int32 key)
{
  switch (key)
  {
  case 1:
    AddNewRoom(1, FVector(0, 0, 0), 90, ERoomShape::R2);
    break;
  case 2:
    AddNewRoom(1, FVector(0, 0, 0), 0, ERoomShape::Q1);
    AddNewRoom(2, FVector(1600, -1600, 0), 0, ERoomShape::R2);
    break;
  case 3:
    AddNewRoom(1, FVector(0, 0, 0), 0, ERoomShape::Q1);
    AddNewRoom(2, FVector(1600, -1600, 0), 0, ERoomShape::Q4);
    break;
  case 4:
    AddNewRoom(7, FVector(1600, 0, 0), 270, ERoomShape::Q1);
    AddNewRoom(2, FVector(6400, 1600, 0), 270, ERoomShape::Q4);
    AddNewRoom(3, FVector(6400, -1600, 0), 90, ERoomShape::L3);
    AddNewRoom(4, FVector(4800, -3200, 0), 90, ERoomShape::R2);
    AddNewRoom(6, FVector(0, -1600, 0), 0, ERoomShape::Q1);
    AddNewRoom(1, FVector(6400, -3200, 0), 0, ERoomShape::Q1);
  default:
    break;
  }
  RoomsInTheList.Broadcast();
  m_bAreAllRoomsCreated = true;
}

void ULevelManager::AddNewRoom(int32 ID, FVector Position, float Rotation, ERoomShape Shape)
{
  UBaseRoom* NewRoom = NewObject<UBaseRoom>(this);
  NewRoom->NonProceduralInit(ID, Position, Rotation, Shape);
  AddRoom(NewRoom);
}

void ULevelManager::OnRoomCleaned(UBaseRoom* CleanedRoom)
{
  if (m_lRooms.Num() == 0)
  {
    return;
  }
  CleanedRooms++;

  // Start Audio Ticking.
  m_bStartAudioInterp = true;
  m_fTargetAudioValue = 0.f;

  if (m_bAreAllRoomsCreated && CleanedRooms >= GetNotQ1Rooms())
  {
    if (pGameInstance->m_iCurrentLevel == 0)
    {
      FTimerDelegate TimerDel;
      TimerDel.BindWeakLambda(this, [this]()
        {
          OnLevelFinish();
          SetPauseMode(true);
        });

      GetWorld()->GetTimerManager().SetTimer(m_oAudioHandle, TimerDel, 1.5f, false);
    }
    else if (pGameInstance->m_iCurrentLevel > 10) // @TODO: do not hardcode this!!
    {
      if (UTimeManager* MyTimerManager = GetWorld()->GetSubsystem<UTimeManager>())
      {
        pGameInstance->SecondsLastedInEndless -= MyTimerManager->GetCurrentSeconds();
      }

      if (APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
      {
        if (ABasePlayer* Player = Cast<ABasePlayer>(Character))
        {
          pGameInstance->PrimaryConsumableAmount = Player->GetPrimaryConsumableAmount();
          pGameInstance->SecondaryConsumableAmount = Player->GetSecondaryConsumableAmount();
          pGameInstance->HealthToRemove = Player->GetMaxHealth() - Player->GetCurrentHealth();
        }
      }

      TWeakObjectPtr<ULevelManager> Weakthis(this);
      GetWorld()->GetTimerManager().SetTimer(
        m_oAudioHandle,
        [Weakthis]()
        {
          if (Weakthis.IsValid() && IsValid(Weakthis->pGameInstance) && IsValid(Weakthis->GetWorld()) && Weakthis->pGameInstance->m_pKillerEvent)
          {
            if (UAudioManager* pAudioManager = Weakthis->pGameInstance->GetSubsystem<UAudioManager>())
            {
              pAudioManager->PlaySound2D(Weakthis->GetWorld(), Weakthis->pGameInstance->m_pKillerEvent);
            }

            Weakthis->pGameInstance->LoadLevelWithLoadingScreen();
          }
        },
        1.5f,
        false
      );

    }
    else
    {
      SetCurrentGameState(EBaseGameState::WIN);
    }
  }
}

int32 ULevelManager::GetNotQ1Rooms()
{
  int32 rooms = 0;
  for (UBaseRoom* Room : m_lRooms)
  {
    if (Room && Room->GetRoomShape() != ERoomShape::Q1)
    {
      rooms++;
    }
  }
  return rooms;
}
// ========= THIS IS JUST FOR ALPHA DELETE THIS AFTER ============
//EOF