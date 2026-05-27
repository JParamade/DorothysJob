#include "PauseMenu.h"
#include "Components/CanvasPanel.h"
#include "Components/Spacer.h"
#include "Components/TextBlock.h"
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/UI/UMGPalette/Button/ReloadLevelButton.h"
#include "DorothysJob/UI/UMGPalette/Button/LoadLevelButton.h"
#include "DorothysJob/UI/Menus/Settings/MainSettingsMenu.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Events/EventManager.h"

// Controller
#include "DorothysJob/Controller/BasePlayerController.h"

// Manager
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"

// Achievements
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

void UPauseMenu::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pContinueButton))
  {
    m_pContinueButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::PauseGame);
  }

  if (IsValid(m_pSettingsButton)) {
    m_pSettingsButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OpenSettingsScreen);
  }

  if (IsValid(m_pReturnToHubButton))
  {
    m_pReturnToHubButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::OnReturnToHubClicked);
  }

  if (IsValid(m_pExitButton))
  {
    m_pExitButton->OnClicked.AddUniqueDynamic(this, &UPauseMenu::ExitClicked);
  }
}

void UPauseMenu::Show()
{
  m_bIsMenuActive = true;
  UWorld* pWorld = GetWorld();
  m_bIsInHub = IsValid(pWorld) && pWorld->GetMapName().Contains("L_Hub");

  SetupFocus();

  if (IsValid(m_pReturnToHubText))
  {
    UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
    m_pReturnToHubText->SetText(IsValid(pGameInstance) && (pGameInstance->m_iCurrentLevel == 0) ? m_sSkipText : m_sReturnToHubText);
  }

  if (m_pPParent) m_pPParent->SetVisibility(ESlateVisibility::Visible);

  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/UI/Generic/SFX_UI_openPauseMenu", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }

  if (IsValid(m_pSettingsMenu)) m_pSettingsMenu->OnMenuClosed.BindLambda([this]() { SetupFocus(); });

  m_bIsVisible = true;
}

void UPauseMenu::Hide()
{
  if (m_pPParent) m_pPParent->SetVisibility(ESlateVisibility::Collapsed);

  if (m_bIsMenuActive) {
    FMOD::Studio::EventInstance* pEvent = nullptr;
    FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
    if (pStudioSystem) {
      FMOD::Studio::EventDescription* pEventDesc = nullptr;
      pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/UI/Generic/SFX_UI_closePauseMenu", &pEventDesc);

      if (pEventDesc) pEventDesc->createInstance(&pEvent);
      if (pEvent) pEvent->start();
    }
  }

  m_pSettingsMenu->Hide();

  if (IsValid(m_pSettingsMenu)) m_pSettingsMenu->OnMenuClosed.Unbind();

  m_bIsMenuActive = false;
  m_bIsVisible = false;
}

void UPauseMenu::SetupFocus()
{
  Super::SetupFocus();

  m_pRestartSpacer->SetSize(FVector2D::ZeroVector);
  m_pReturnToHubSpacer->SetSize(FVector2D::ZeroVector);
  m_pRestartButton->SetVisibility(ESlateVisibility::Collapsed);
  m_pReturnToHubButton->SetVisibility(ESlateVisibility::Collapsed);
  TArray<int32> lFocus = { 1, 1, 1 };

  m_lFocusableElements.Add(Cast<UObject>(m_pContinueButton));
  if (!m_bIsInHub)
  {
    UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
    if (IsValid(pGameInstance) && (pGameInstance->m_iCurrentLevel != 0))
    {
      m_pRestartSpacer->SetSize(FVector2D(1., 25.));
      m_pRestartButton->SetVisibility(ESlateVisibility::Visible);
      m_lFocusableElements.Add(Cast<UObject>(m_pRestartButton));
      lFocus.Append({ 1 });
    }
    m_pReturnToHubSpacer->SetSize(FVector2D(1., 25.));
    m_pReturnToHubButton->SetVisibility(ESlateVisibility::Visible);
    m_lFocusableElements.Add(Cast<UObject>(m_pReturnToHubButton));
    lFocus.Append({ 1 });
  }
  m_lFocusableElements.Add(Cast<UObject>(m_pSettingsButton));
  m_lFocusableElements.Add(Cast<UObject>(m_pExitButton));

  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, lFocus, EFocusNavigationType::VERTICAL, true, false);

  // Set this menu as the currently active focus manager.
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    if (UGeneralFocusManager* pGeneralFocusManager = pPlayerController->GetGeneralFocusManager()) {
      pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
    }
  }
}

void UPauseMenu::PauseGame()
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (pLevelManager)
  {
    pLevelManager->OnGamePause(false);
  }
}

void UPauseMenu::OnReturnToHubClicked()
{
  UWorld* pWorld = GetWorld();
  if (IsValid(pWorld))
  {
    UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
    if (IsValid(pGameInstance))
    {
      pGameInstance->m_iCurrentEndlessLevel = -1;

      if (pGameInstance->m_iCurrentLevel == 0)
      {
        pGameInstance->m_iCurrentLevel++;
        pGameInstance->m_iHighestLevelAchieved++;

        if (USaveGameManager* pSaveGameManager = pGameInstance->GetSubsystem<USaveGameManager>())
        {
          pSaveGameManager->SaveCurrentLevel(pGameInstance->m_iCurrentLevel, pGameInstance->m_iHighestLevelAchieved);
        }

        UEventManager* pEventManager = pGameInstance->GetEventManager();
        if (pEventManager)
        {
          pEventManager->ActiveTriggersFromLevel(0);
        }
      }
    }

    UGameplayStatics::OpenLevelBySoftObjectPtr(pWorld, m_pHubMap);

    if (IsValid(pGameInstance))
    {
      if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>())
      {
        pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/Hub/SFX_HUB_exitDoor_startGame.SFX_HUB_exitDoor_startGame")));
      }
    }

    // Play Killer Sound Event.
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
    }
  }
}

void UPauseMenu::OpenSettingsScreen()
{
  m_pSettingsMenu->Show();
}

bool UPauseMenu::CanExit()
{
  return (IsValid(m_pSettingsMenu) && m_pSettingsMenu->IsWidgetVisible()) ? m_pSettingsMenu->CanExit() : true;
}

void UPauseMenu::ChangeSettingsTab(bool _bGoRight)
{
  if (IsValid(m_pSettingsMenu) && m_pSettingsMenu->IsWidgetVisible())
  {
    m_pSettingsMenu->ChangeTab(_bGoRight);
  }
}

void UPauseMenu::ExitClicked()
{
  if (GetWorld())
  {
    UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
    if (pBaseGameInstance)
    {
      pBaseGameInstance->m_iCurrentEndlessLevel = -1;

      USaveGameManager* pSaveGameManager = pBaseGameInstance->GetSubsystem<USaveGameManager>();
      if (pSaveGameManager && pBaseGameInstance->m_iCurrentLevel == 0)
      {
        pSaveGameManager->DeleteSave();
      }
      else if (pSaveGameManager && pBaseGameInstance->m_iCurrentLevel >= pBaseGameInstance->m_iHighestLevelAchieved)
      {
        pSaveGameManager->SaveWeaponsBuild();
        pSaveGameManager->SaveConsumablesBuild();
      }

      // Play Killer Sound Event.
      if (UAudioManager* pAudioManager = pBaseGameInstance->GetSubsystem<UAudioManager>()) {
        pAudioManager->PlaySound2D(GetWorld(), pBaseGameInstance->m_pKillerEvent);
      }

      if (UAchievementSubsystem* pAchSubsystem = pBaseGameInstance->GetSubsystem<UAchievementSubsystem>())
      {
        pAchSubsystem->SetDeathCounterAchievement();
      }

    }

  }
}
