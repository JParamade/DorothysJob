/************************************************************************
 * @description: Represents the main menu's widget.
 * @author: Jaime Paramo
 * @date: 19/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#include "MainMenu.h"

// Audio
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"

// Component
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButtonText.h"
#include "DorothysJob/UI/UMGPalette/Button/LoadLevelButton.h"
#include "DorothysJob/UI/UMGPalette/Button/QuitGameButton.h"

// Gameplay
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SaveGameManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

// Widget
#include "DorothysJob/UI/Menus/Main/PressAnyKeyText.h"
#include "DorothysJob/UI/Menus/Settings/MainSettingsMenu.h"
#include "DorothysJob/UI/Menus/Credits/BaseCreditsScreen.h"
#include "DorothysJob/UI/Menus/ConfirmationWindow.h"
// #include "DorothysJob/UI/Menus/Main/VersionText.h"
#include "DorothysJob/UI/Menus/Cinematics/CinematicScreen.h"
#include "DorothysJob/UI/Menus/Main/TechLogos.h"

void UMainMenu::InitMenu() {
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (!pGameInstance->m_bMenuLaunched) {
      if (IsValid(m_pTechLogos)) {
        m_pTechLogos->OnAnimationFinished.BindUObject(this, &UMainMenu::CustomShow);
        m_pTechLogos->Show();
      }
    }
    else CustomShow();
  }
}

void UMainMenu::Show() {
  m_pCinematicScreen->Hide();

  Super::Show();
  // Hide blinking Text Block displaying the "Press Any Key" prompt.
  if (IsValid(m_pPressAnyKeyText)) m_pPressAnyKeyText->Hide();

  // Set up focus when menu is shown.
  SetupFocus();

  // Show VersionText if it's valid.
  // if (IsValid(m_pVersionText)) m_pVersionText->Show();
}

void UMainMenu::CustomShow() {
  // Show blinking Text Block displaying the "Press Any Key" prompt.
  if (IsValid(m_pPressAnyKeyText)) m_pPressAnyKeyText->Show();

  // Show VersionText if it's valid.
  // if (IsValid(m_pVersionText)) m_pVersionText->Show();

  // Show background and title Images.
  if (IsValid(m_pBackgroundImage)) m_pBackgroundImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
  if (IsValid(m_pTitleImage)) m_pTitleImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

  m_pCinematicScreen->Hide();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      if (IsValid(m_pMusicEvent)) m_pEventInstance = pAudioManager->PlayEventInstance(m_pMusicEvent);
    }
  }

  if (IsValid(m_pSettingsMenu)) m_pSettingsMenu->OnMenuClosed.BindLambda([this]() { SetupFocus(); });
  if (IsValid(m_pCreditsScreen)) m_pCreditsScreen->OnCreditsClosed.AddLambda([this]() { SetupFocus(); });
}

void UMainMenu::Hide() {
  Super::Hide();

  // Hide VersionText if it's valid.
  // if (IsValid(m_pVersionText)) m_pVersionText->Hide();
}

void UMainMenu::NativeConstruct() {
  Super::NativeConstruct();

  // Get Save Game Manager.
  if (UBaseGameInstance* pBaseGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) m_pSaveGameManager = pBaseGameInstance->GetSubsystem<USaveGameManager>();

  // Non Focusable Elements
  if (IsValid(m_pBackgroundImage)) m_lElements.Add(m_pBackgroundImage);
  if (IsValid(m_pTitleImage)) m_lElements.Add(m_pTitleImage);
  if (IsValid(m_pNewGameText)) m_lElements.Add(m_pNewGameText);
  if (IsValid(m_pContinueText)) m_lElements.Add(m_pContinueText);
  if (IsValid(m_pSettingsText)) m_lElements.Add(m_pSettingsText);
  if (IsValid(m_pCreditsText)) m_lElements.Add(m_pCreditsText);
  if (IsValid(m_pQuitText)) m_lElements.Add(m_pQuitText);

  // Focusable Elements
  if (IsValid(m_pContinueButton)) {
    if (m_pSaveGameManager->CheckSaveGameExists()) {
      m_lFocusableElements.Add(Cast<UObject>(m_pContinueButton));
      m_pContinueButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnContinue);
    }
    else {
      m_lElements.Add(Cast<UObject>(m_pContinueButton));
      m_pContinueButton->SetIsEnabled(false);
    }
  }
  if (IsValid(m_pNewGameButton)) {
    m_lFocusableElements.Add(Cast<UObject>(m_pNewGameButton));
    m_pNewGameButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnNewGame);
  }
  if (IsValid(m_pSettingsButton)) {
    m_lFocusableElements.Add(Cast<UObject>(m_pSettingsButton));
    m_pSettingsButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnSettings);
  }
  if (IsValid(m_pCreditsButton)) {
    m_lFocusableElements.Add(Cast<UObject>(m_pCreditsButton));
    m_pCreditsButton->OnClicked.AddUniqueDynamic(this, &UMainMenu::OnCredits);
  }
  if (IsValid(m_pQuitButton)) m_lFocusableElements.Add(Cast<UObject>(m_pQuitButton));

  m_pCinematicScreen->Hide();
}

void UMainMenu::SetupFocus() {
  Super::SetupFocus();

  // Focus navigation flow for when the continue button is enabled.
  TArray<int32> lContinueFocus = { 1, 1, 1, 1, 1 };
  // Focus navigation flow for when the continue button is disabled.
  TArray<int32> lNewGameFocus = { 1, 1, 1, 1 };

  // Setup SpecificFocusManager
  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  m_pSpecificFocusManager[0]->Initialize(
    m_lFocusableElements,
    m_pSaveGameManager->CheckSaveGameExists() ? lContinueFocus : lNewGameFocus,
    EFocusNavigationType::VERTICAL,
    true,
    false
  );

  // Set this menu as the currently active focus manager.
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    if (UGeneralFocusManager* pGeneralFocusManager = pPlayerController->GetGeneralFocusManager()) {
      pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
    }
  }
}

void UMainMenu::OnNewGame() {
  if (m_pSaveGameManager->CheckSaveGameExists())
  {
    if (m_pConfirmationWindow)
    {
      m_pConfirmationWindow->Show();

      m_pConfirmationWindow->OnConfirmResult.AddLambda([this](bool bAccepted)
        {
          if (bAccepted)
          {
            if (m_pEventInstance) {
              m_pEventInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
              m_pEventInstance->release();
              m_pEventInstance = nullptr;
            }

            for (UObject* Element : m_lFocusableElements) {
              if (UButton* Button = Cast<UButton>(Element)) {
                Button->SetIsEnabled(false);
              }
            }

            UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Cinematic"));
          }
          else Show();

        });
    }
  }
  else {
    // Play Killer Sound Event.
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
      if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
        pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
      }
    }

    for (UObject* Element : m_lFocusableElements) {
      if (UButton* Button = Cast<UButton>(Element)) {
        Button->SetIsEnabled(false);
      }
    }
    
    UGameplayStatics::OpenLevel(GetWorld(), TEXT("L_Cinematic"));
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_fStartGameTime = UGameplayStatics::GetTimeSeconds(GetWorld());
    }
  }
}

void UMainMenu::OnContinue() {
  if (IsValid(m_pSaveGameManager)) {
    if (!m_pSaveGameManager->LoadAllGame()) {
      OnNewGame();
      return;
    }
  }

  // Play Killer Sound Event.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(GetWorld(), pGameInstance->m_pKillerEvent);
    }
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_fStartGameTime = UGameplayStatics::GetTimeSeconds(GetWorld());
    }
  }

  UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), m_pHubMap);
}

void UMainMenu::OnSettings() {
  // Show the settings screen.
  m_pSettingsMenu->Show();
}

void UMainMenu::OnCredits() {
  if (ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0))) {
    if (UGeneralFocusManager* pGeneralFocusManager = pPlayerController->GetGeneralFocusManager()) {
      TArray<TObjectPtr<USpecificFocusManager>> lEmptyArray;
      pGeneralFocusManager->SetActiveMenu(lEmptyArray);
    }
  }

  if (IsValid(m_pCreditsScreen)) m_pCreditsScreen->Show();
}