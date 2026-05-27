#include "MenusManager.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/UI/Menus/SelectWeaponsMenu/SelectWeaponsMenu.h"
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/Controller/BasePlayerController.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/UI/Menus/Dialogue/DialogueScreen.h"
#include "DorothysJob/UI/HUD/HUDPlayer.h"
#include "DorothysJob/UI/Menus/Pause/PauseMenu.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/UI/Menus/ResultsScreen.h"
#include "DorothysJob/UI/Menus/TutorialScreen/TutorialMenu.h"

#pragma region | Virtual Methods

void UMenusManager::Show()
{
  m_pHUDPlayer->Show(); // only hud
  m_pPauseMenu->Hide();
  m_pResultsScreen->Hide();
  m_pDialogueScreen->Hide();
  m_pPermanentScreen->Hide();

  UWorld* pCachedWorld = GetWorld();
  if (!IsValid(pCachedWorld))
  {
    return;
  }

  ULevelManager* pLevelManager = pCachedWorld->GetSubsystem<ULevelManager>();
  if (IsValid(pLevelManager))
  {
    pLevelManager->OnGameStateChanged.AddUObject(this, &UMenusManager::LevelStateHandler);
    pLevelManager->OnCancelButtonPressed.BindUObject(this, &UMenusManager::HandleCancelAction);
    pLevelManager->OnChangeTabButtonPressed.BindUObject(this, &UMenusManager::HandleChangeTabAction);
  }
  
  APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(pCachedWorld, 0);

  if (!IsValid(PlayerPawn))
  {
    return;
  }

  if (ABasePlayer* pPlayer = Cast<ABasePlayer>(PlayerPawn))
  {
    pPlayer->UpdateConsumableInfo();
  }
}

void UMenusManager::Hide()
{
  m_pHUDPlayer->Hide();
  m_pPauseMenu->Hide();
  m_pResultsScreen->Hide();
  m_pDialogueScreen->Hide();
  m_pPermanentScreen->Hide();

  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (pLevelManager)
  {
    pLevelManager->OnGameStateChanged.RemoveAll(this);
    pLevelManager->OnCancelButtonPressed.Unbind();
    pLevelManager->OnChangeTabButtonPressed.Unbind();
  }
}

void UMenusManager::ShowTutorial()
{
  m_pHUDPlayer->ShowTutorial(); // only hud
  m_pPauseMenu->Hide();
  m_pResultsScreen->Hide();
  m_pDialogueScreen->Hide();
  m_pPermanentScreen->Hide();

  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (pLevelManager)
  {
      pLevelManager->OnGameStateChanged.AddUObject(this, &UMenusManager::LevelStateHandler);
  }
}

#pragma endregion

void UMenusManager::HandleCancelAction(bool _bPressed)
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (m_pPauseMenu->IsWidgetVisible() && m_pPauseMenu->CanExit() && IsValid(pLevelManager))
  {
    pLevelManager->OnGamePause(false);
  }
}

void UMenusManager::HandleChangeTabAction(bool _bGoRight)
{
  if (m_pPauseMenu->IsWidgetVisible())
  {
    m_pPauseMenu->ChangeSettingsTab(_bGoRight);
  }
}

void UMenusManager::LevelStateHandler(EBaseGameState _status)
{
  // If the status is diferent than before
  if (m_oStatus != _status)
  {
      // Determine which widget corresponds to the previous status
      switch (m_oStatus)
      {
      case EBaseGameState::PLAYING:          SafeHide(m_pHUDPlayer); break;
      case EBaseGameState::GAMEOVER:         
      case EBaseGameState::WIN:              SafeHide(m_pResultsScreen); break;
      case EBaseGameState::PAUSED:           SafeHide(m_pPauseMenu); break;
      case EBaseGameState::DIALOGUE:         SafeHide(m_pDialogueScreen); break;
      case EBaseGameState::WIDGET:           SafeHide(m_pPermanentScreen); break;
      default: break;
      }

      // Update to the new status
      m_oStatus = _status;
  }

  switch (_status)
  {
  case EBaseGameState::PLAYING:
    SafeShow(m_pHUDPlayer);
    break;
  case EBaseGameState::GAMEOVER:
    m_pResultsScreen->SetWidgetState(EBaseGameState::GAMEOVER); // GAME OVER
    SafeShow(m_pResultsScreen);
    break;
  case EBaseGameState::WIN:
    m_pResultsScreen->SetWidgetState(EBaseGameState::WIN); // WIN
    SafeShow(m_pResultsScreen);
    break;
  case EBaseGameState::PAUSED:
    SafeShow(m_pPauseMenu);
    break;
  case EBaseGameState::DIALOGUE:
    SafeShow(m_pDialogueScreen);
    break;
  case EBaseGameState::WIDGET:
    SafeShow(m_pPermanentScreen);
    break;
  default:
    SafeHide(m_pHUDPlayer);
    SafeHide(m_pResultsScreen);
    SafeHide(m_pPauseMenu); 
    SafeHide(m_pDialogueScreen); 
    SafeHide(m_pPermanentScreen);
    break;
  }

  SetFocusAndController();
}

void UMenusManager::SafeShow(UBaseWidget* Screen)
{
  if (Screen && !Screen->IsWidgetVisible())
  {
    Screen->Show();
  }
}

void UMenusManager::SafeHide(UBaseWidget* Screen)
{
  if (Screen && Screen->IsWidgetVisible())
  {
    Screen->Hide();
  }
}

void UMenusManager::SetFocusAndController()
{
  ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  if (!IsValid(pPlayerController))
  {
    return;
  }

  if (UGeneralFocusManager* pGeneralFocusManager = pPlayerController->GetGeneralFocusManager())
  {
    if (m_oStatus == EBaseGameState::PAUSED && m_pPauseMenu)
    {
      pGeneralFocusManager->SetActiveMenu(m_pPauseMenu->GetSpecificFocusManager());
    }
    else if ((m_oStatus == EBaseGameState::WIN || m_oStatus == EBaseGameState::GAMEOVER) && m_pResultsScreen)
    {
      pGeneralFocusManager->SetActiveMenu(m_pResultsScreen->GetSpecificFocusManager());
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