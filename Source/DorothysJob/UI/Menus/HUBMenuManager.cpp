#include "HUBMenuManager.h"
// WIDGETS
#include "DorothysJob/UI/HUD/HUDPlayer.h"
#include "DorothysJob/UI/Menus/Pause/PauseMenu.h"
#include "DorothysJob/UI/Menus/SelectWeaponsMenu/SelectWeaponsMenu.h"
#include "DorothysJob/UI/Menus/SelectConsumablesMenu/SelectConsumablesMenu.h"
#include "DorothysJob/UI/Menus/PreLevelScreen/PreLevelScreen.h"
#include "DorothysJob/UI/Menus/Dialogue/DialogueScreen.h"
#include "DorothysJob/UI/Menus/TutorialScreen/TutorialMenu.h"
#include "DorothysJob/UI/Menus/ReplayMenu.h"
// WORLD SUBSYSTEMS
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Dialogs/DialogsManager.h"
#include "DorothysJob/Manager/Events/EventManager.h"
// FOCUS
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
// PLAYER
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"


// UTILS
#include "Logging/StructuredLog.h"    // for logging

DEFINE_LOG_CATEGORY(HUBMenuManager);

void UHUBMenuManager::Show()
{
  UE_LOG(LogTemp, Warning, TEXT("UHUBMenuManager::Show() called"));

  m_pHUDPlayer->ShowHUB();
  m_pPauseMenu->Hide();
  m_pWeaponSelectMenu->Hide();
  m_pConsumableSelectMenu->Hide();
  m_pDialogueScreen->Hide();
  m_pPermanentScreen->Hide();
  SafeHide(m_pReplayMenu);

  if (IsValid(m_pPreLevelScreen))
  {
    m_pPreLevelScreen->Hide();
    m_pPreLevelScreen->OnPreLevelFinish.AddUObject(this, &UHUBMenuManager::LoadDialog);
  }

  // Binding to the level manager
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (IsValid(pLevelManager))
  {
    pLevelManager->OnGameStateChanged.AddUObject(this, &UHUBMenuManager::LevelStateHandler);
    pLevelManager->OnReceivedConsumableInput.AddUObject(this, &UHUBMenuManager::HandleSelectedConsumable);
    pLevelManager->OnReceivedConsumableNavigation.BindUObject(this, &UHUBMenuManager::HandleConsumableNavigation);
    pLevelManager->OnCancelButtonPressed.BindUObject(this, &UHUBMenuManager::HandleCancelAction);
    pLevelManager->OnChangeTabButtonPressed.BindUObject(this, &UHUBMenuManager::HandleChangeTabAction);
  }
}

void UHUBMenuManager::Hide()
{
  UE_LOG(LogTemp, Warning, TEXT("UHUBMenuManager::Hide() called"));

  m_pHUDPlayer->Hide();
  m_pPauseMenu->Hide();
  m_pWeaponSelectMenu->Hide();
  m_pConsumableSelectMenu->Hide();
  m_pDialogueScreen->Hide();
  m_pPermanentScreen->Hide();
  SafeHide(m_pReplayMenu);

  if (IsValid(m_pPreLevelScreen))
  {
    m_pPreLevelScreen->Hide();
    m_pPreLevelScreen->OnPreLevelFinish.RemoveAll(this);
  }

  // Unbinding to the level manager
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (IsValid(pLevelManager))
  {
    pLevelManager->OnGameStateChanged.RemoveAll(this);
    pLevelManager->OnReceivedConsumableInput.RemoveAll(this);
    pLevelManager->OnReceivedConsumableNavigation.Unbind();
    pLevelManager->OnCancelButtonPressed.Unbind();
    pLevelManager->OnChangeTabButtonPressed.Unbind();
  }
}

void UHUBMenuManager::RevealAllElements() {
  if (IsValid(m_pHUDPlayer)) m_pHUDPlayer->RevealEverything();
}

void UHUBMenuManager::LevelStateHandler(EBaseGameState _status)
{
  UE_LOG(HUBMenuManager, Log, TEXT("LevelStateHandler triggered: %d"), static_cast<uint8>(_status));

  // If the status is diferent than before
  if (m_oStatus != _status)
  {
    UE_LOG(HUBMenuManager, Log, TEXT("Level changed: previous = %d, new = %d"),
      static_cast<uint8>(m_oStatus), static_cast<uint8>(_status));
    // Determine which widget corresponds to the previous status
    switch (m_oStatus)
    {
    case EBaseGameState::PLAYING:          SafeHide(m_pHUDPlayer); break;
    case EBaseGameState::PAUSED:           SafeHide(m_pPauseMenu); break;
    case EBaseGameState::SELECTWEAPON:
      if (_status != EBaseGameState::DIALOGUE && _status != EBaseGameState::WIDGET)
      {
        SafeHide(m_pWeaponSelectMenu);
      }
      break;
    case EBaseGameState::SELECTCONSUMABLE:
      if (_status != EBaseGameState::DIALOGUE && _status != EBaseGameState::WIDGET)
      {
        SafeHide(m_pConsumableSelectMenu);
      }
      break;
    case EBaseGameState::PRELEVEL:
      if (_status != EBaseGameState::DIALOGUE && _status != EBaseGameState::WIDGET)
      {
        SafeHide(m_pPreLevelScreen);
      }
      break;
    case EBaseGameState::DIALOGUE:         SafeHide(m_pDialogueScreen); break;
    case EBaseGameState::WIDGET:           SafeHide(m_pPermanentScreen); break;
    case EBaseGameState::REPLAY:
      if (_status != EBaseGameState::DIALOGUE && _status != EBaseGameState::WIDGET)
      {
        SafeHide(m_pReplayMenu);
      }
      break;
    default: break;
    }
    // Update to the new status
    m_oStatus = _status;
  }
  

  // Set the event to play (This is only for the weapon and the chest screens)
  SetEvent(_status);

  // Depending on the status show the correct screen
  switch (_status)
  {
  case EBaseGameState::PLAYING:
    if (IsValid(m_pHUDPlayer) && !m_pHUDPlayer->IsWidgetVisible()) m_pHUDPlayer->ShowHUB(); // Need to call specific ShowHUB function.
    break;

  case EBaseGameState::PAUSED:
    SafeShow(m_pPauseMenu);
    break;

  case EBaseGameState::SELECTWEAPON:
    m_oStatusEvent = _status;
    if (m_pWeaponSelectMenu)
    {
      m_pWeaponSelectMenu->InitMenu();
      SafeShow(m_pWeaponSelectMenu);
    }
    break;

  case EBaseGameState::SELECTCONSUMABLE:
    m_oStatusEvent = _status;
    if (m_pConsumableSelectMenu)
    {
      m_pConsumableSelectMenu->InitMenu();
      SafeShow(m_pConsumableSelectMenu);
    }
    break;

  case EBaseGameState::PRELEVEL:
    m_oStatusEvent = _status;
    SafeShow(m_pPreLevelScreen);
    break;

  case EBaseGameState::DIALOGUE:
    SafeShow(m_pDialogueScreen);
    break;

  case EBaseGameState::WIDGET:
    SafeShow(m_pPermanentScreen);
    break;

  case EBaseGameState::REPLAY:
    m_oStatusEvent = _status;
    if (m_pReplayMenu)
    {
      SafeShow(m_pReplayMenu);
    }
    break;

  default:
    UE_LOG(HUBMenuManager, Error, TEXT("Unidentified state. Did you add a new state to the LevelManager and forgot to handle it?"));
    break;
  }
  UE_LOG(LogTemp, Warning, TEXT("Status actual: %d"), (int32)m_oStatus);
  // Set the focus and the correct inputs
  SetFocusAndController();
}

void UHUBMenuManager::SafeShow(UBaseWidget* Screen)
{
  if (Screen && !Screen->IsWidgetVisible())
  {
    Screen->Show();
  }
}

void UHUBMenuManager::SafeHide(UBaseWidget* Screen)
{
  if (Screen && Screen->IsWidgetVisible())
  {
    Screen->Hide();
  }
}

void UHUBMenuManager::SetFocusAndController()
{
   ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  if (!pPlayerController)
  {
    return;
  }

  if (UGeneralFocusManager* pGeneralFocusManager = pPlayerController->GetGeneralFocusManager())
  {
    if (m_oStatus == EBaseGameState::PAUSED && m_pPauseMenu)
    {
      pGeneralFocusManager->SetActiveMenu(m_pPauseMenu->GetSpecificFocusManager());
    }
  }

  EPlayerInputMode InputMode = EPlayerInputMode::UI;

  switch (m_oStatus)
  {
  case EBaseGameState::PLAYING:
    InputMode = EPlayerInputMode::GAMEPLAY;
    break;
  case EBaseGameState::DIALOGUE:
  case EBaseGameState::WIDGET:
    InputMode = EPlayerInputMode::EVENT;
    break;

  default:
    InputMode = EPlayerInputMode::UI;
    break;
  }

  pPlayerController->SwitchInputMode(InputMode);
}

void UHUBMenuManager::SetEvent(EBaseGameState _status)
{
  if (m_oStatus == EBaseGameState::SELECTWEAPON)
  {
    m_pWeaponSelectMenu->OnMenuShow.AddUObject(this, &UHUBMenuManager::StartEvent);
  }
  else if (m_oStatus == EBaseGameState::SELECTCONSUMABLE)
  {
    m_pConsumableSelectMenu->OnMenuShow.AddUObject(this, &UHUBMenuManager::StartEvent);
  }
  else if (m_oStatus == EBaseGameState::PRELEVEL)
  {
    m_pPreLevelScreen->OnMenuShow.AddUObject(this, &UHUBMenuManager::StartEvent);
  }
  else if (m_oStatus == EBaseGameState::REPLAY)
  {
    m_pReplayMenu->OnMenuShow.AddUObject(this, &UHUBMenuManager::StartEvent);
  }
}

void UHUBMenuManager::HandleSelectedConsumable()
{
  if (IsValid(m_pConsumableSelectMenu) && m_pConsumableSelectMenu->IsWidgetVisible())
  { // Only pass it if it is active
    m_pConsumableSelectMenu->HandleReceivedInput(); // Pass the input
  }
}

void UHUBMenuManager::HandleConsumableNavigation(int32 _iDirection)
{
  if (IsValid(m_pConsumableSelectMenu) && m_pConsumableSelectMenu->IsWidgetVisible())
  { // Only pass it if it is active
    m_pConsumableSelectMenu->OnScroll(_iDirection);
  }
}

void UHUBMenuManager::LoadDialog(bool _toggle)
{
  // Early exit if the toggle is off or required managers are invalid
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
  if (!_toggle || !IsValid(pLevelManager) || !IsValid(pGameInstance))
  {
    return;
  }

  // Get the dialog manager
  UDialogsManager* pDialogManager = pGameInstance->GetDialoguesManager();
  if (!IsValid(pDialogManager))
  {
    return;
  }

  // Set the current game state to DIALOGUE
  pLevelManager->SetCurrentGameState(EBaseGameState::DIALOGUE);

  // Build the sequence name based on current level
  FString SequenceNameText = FString::Printf(TEXT("Level_%d_Start"), pGameInstance->m_iCurrentLevel);
  FName SequenceName(*SequenceNameText);

  // Bind a callback for when the dialog finishes
  pDialogManager->OnDialogFinish.AddUObject(this, &UHUBMenuManager::LoadNextLevel);

  // Initialize the dialog sequence
  pDialogManager->OnInitSequence(SequenceName);
}

void UHUBMenuManager::LoadNextLevel()
{
  // Get the level manager subsystem
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return;
  }

  // Set the game state back to PLAYING
  pLevelManager->SetCurrentGameState(EBaseGameState::PLAYING);

  // Open the procedural level
  UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Procedural"));
}

void UHUBMenuManager::HandleCancelAction(bool _bPressed)
{
  if (m_pWeaponSelectMenu->IsWidgetVisible())
  {
    m_pWeaponSelectMenu->CancelAction();
  }
  
  if (m_pConsumableSelectMenu->IsWidgetVisible())
  {
    m_pConsumableSelectMenu->CancelAction();
  }

  if (m_pPreLevelScreen->IsWidgetVisible())
  {
    m_pPreLevelScreen->CancelAction();
  }

  if (m_pReplayMenu->IsWidgetVisible())
  {
    m_pReplayMenu->CancelAction();
  }

  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (m_pPauseMenu->IsWidgetVisible() && m_pPauseMenu->CanExit() && IsValid(pLevelManager))
  {
    pLevelManager->OnGamePause(false);
  }
}

void UHUBMenuManager::HandleChangeTabAction(bool _bGoRight)
{
  if (m_pPauseMenu->IsWidgetVisible())
  {
    m_pPauseMenu->ChangeSettingsTab(_bGoRight);
  }
}

void UHUBMenuManager::StartEvent()
{
  if (!GetWorld()) return;

  // Get the game instance
  UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!GameInstance) return;

  // Get the event manager
  UEventManager* EventManager = GameInstance->GetEventManager();
  if (!EventManager) return;

  // Get the dialog manager
  UDialogsManager* DialogManager = GameInstance->GetDialoguesManager();
  if (!DialogManager) return;

  // Set the current status in both managers
  EventManager->SetCurrentStatus(m_oStatus);
  DialogManager->SetCurrentStatus(m_oStatus);

  // Bind callbacks for when the event or dialog finishes
  EventManager->OnAllEventFinish.AddUObject(this, &UHUBMenuManager::ContinueAfterEvent);
  DialogManager->OnDialogFinish.AddUObject(this, &UHUBMenuManager::ContinueAfterDialogue);

  // Determine the sequence name based on current menu type
  FName SequenceName = "";
  if (m_oStatus == EBaseGameState::SELECTWEAPON)
  {
    SequenceName = FName(*FString::Printf(TEXT("LV_%d_WEAPON_CHEST"), GameInstance->m_iCurrentLevel));
  }
  else if (m_oStatus == EBaseGameState::SELECTCONSUMABLE)
  {
    SequenceName = FName(*FString::Printf(TEXT("LV_%d_CONSUMABLES_CHEST"), GameInstance->m_iCurrentLevel));
  }
  else if (m_oStatus == EBaseGameState::PRELEVEL)
  {
    SequenceName = FName(*FString::Printf(TEXT("INIT_LEVEL_%d"), GameInstance->m_iCurrentLevel));
  }
  else if (m_oStatus == EBaseGameState::REPLAY)
  {
    SequenceName = FName(*FString::Printf(TEXT("REPLAY_EVENT")));
  }

  // Trigger the event sequence
  EventManager->TriggerEvent(SequenceName);
}

void UHUBMenuManager::ContinueAfterEvent()
{
  if (!GetWorld()) return;

  // Get the game instance
  UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!GameInstance) return;

  // Get the event manager
  UEventManager* EventManager = GameInstance->GetEventManager();
  if (!EventManager) return;

  // Get the dialog manager
  UDialogsManager* DialogManager = GameInstance->GetDialoguesManager();
  if (!DialogManager) return;

  // Reset the status to PLAYING in both managers for when the event comes from the game.
  EventManager->SetCurrentStatus(EBaseGameState::PLAYING);
  DialogManager->SetCurrentStatus(EBaseGameState::PLAYING);

  // // Unbind all previously added callbacks to prevent multiple triggers
  EventManager->OnAllEventFinish.RemoveAll(this);
  DialogManager->OnDialogFinish.RemoveAll(this);
  if (m_pWeaponSelectMenu) m_pWeaponSelectMenu->OnMenuShow.RemoveAll(this);
  if (m_pConsumableSelectMenu) m_pConsumableSelectMenu->OnMenuShow.RemoveAll(this);
  if (m_pPreLevelScreen) m_pPreLevelScreen->OnMenuShow.RemoveAll(this);
  if (m_pReplayMenu) m_pReplayMenu->OnMenuShow.RemoveAll(this);

  // Hide the dialogue and permanent screens JUST IN CASE
  if (m_pDialogueScreen) SafeHide(m_pDialogueScreen);
  if (m_pPermanentScreen) SafeHide(m_pPermanentScreen);

  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return;
  }

  pLevelManager->CurrentGameState = m_oStatusEvent;
  m_oStatus = m_oStatusEvent;
  // Reset the player input mode back to UI
  if (ABasePlayerController* PlayerController = Cast<ABasePlayerController>(
    UGameplayStatics::GetPlayerController(GetWorld(), 0)))
  {
    PlayerController->SwitchInputMode(EPlayerInputMode::UI);
  }
}

void UHUBMenuManager::ContinueAfterDialogue()
{
  // Hide the dialogue and permanent screens JUST IN CASE
  if (m_pDialogueScreen) SafeHide(m_pDialogueScreen);
}