/************************************************************************
 * @description: Represents the turotial menu's widget.
 * @author: David Gonzalez
 * @date: 22/04/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#include "TutorialScreen.h"

// Component
#include "Components/Image.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButtonText.h"
#include "DorothysJob/UI/UMGPalette/Button/LoadLevelButton.h"

// Gameplay
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"

void UTutorialScreen::Show() {
  Super::Show();

  // Set up focus when menu is shown.
  SetupFocus();

  // Initially show the first tutorial screen.
  ShowScreen(true);
}

void UTutorialScreen::Hide() {
  Super::Hide();
}

void UTutorialScreen::NativeConstruct() {
  Super::NativeConstruct();

  // Focusable Elements
  if (IsValid(m_pContinueButton)) {
    m_pContinueButton->OnClicked.AddUniqueDynamic(this, &UTutorialScreen::OnClickContinue);
    m_lFocusableElements.Add(Cast<UObject>(m_pContinueButton));
  }
}

void UTutorialScreen::SetupFocus() {
  Super::SetupFocus();

  // Setup SpecificFocusManager
  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  m_pSpecificFocusManager[0]->Initialize(
    m_lFocusableElements, 
    { 1 }, 
    EFocusNavigationType::HORIZONTAL, 
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

void UTutorialScreen::OnClickContinue() {
  // Switch to the second screen of the tutorial.
  ShowScreen(false);

  // Setup SpecificFocusManager
  m_lFocusableElements.Add(Cast<UObject>(m_pPlayButton));
  m_lFocusableElements.Remove(Cast<UObject>(m_pContinueButton));
  m_pSpecificFocusManager[0]->Initialize(
    m_lFocusableElements, 
    { 1 }, 
    EFocusNavigationType::HORIZONTAL, 
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

void UTutorialScreen::ShowScreen(bool _bFirstScreen) {
  // Show background and button for the first screen if "_bFirstScreen" is true.
  m_pBackgroundOne->SetVisibility(_bFirstScreen ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);
  m_pContinueButton->SetVisibility(_bFirstScreen ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

  // Show background and button for the second screen if "_bFirstScreen" is false
  m_pBackgroundTwo->SetVisibility(_bFirstScreen ? ESlateVisibility::Hidden : ESlateVisibility::SelfHitTestInvisible);
  m_pPlayButton->SetVisibility(_bFirstScreen ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}