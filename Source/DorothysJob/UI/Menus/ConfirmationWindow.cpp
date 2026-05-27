// Fill out your copyright notice in the Description page of Project Settings.



#include "ConfirmationWindow.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Gameplay
#include "DorothysJob/Controller/BasePlayerController.h"
#include "Kismet/GameplayStatics.h"

// Manager
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"

void UConfirmationWindow::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pCanvasPanel)) m_lElements.Add(m_pCanvasPanel);
  if (IsValid(m_pBackgroundImage)) m_lElements.Add(m_pBackgroundImage);
  if (IsValid(m_pConfirmationText)) m_lElements.Add(m_pConfirmationText);

  if (m_bCancelButton)
  {
    m_lFocusableElements.Add(m_bCancelButton);
    m_bCancelButton->OnClicked.AddDynamic(this, &UConfirmationWindow::Cancel);
  }
  if (m_bAcceptButton)
  {
    m_lFocusableElements.Add(m_bAcceptButton);
    m_bAcceptButton->OnClicked.AddDynamic(this, &UConfirmationWindow::Accept);

  }
}

void UConfirmationWindow::Show()
{
  Super::Show();
 
  if (IsValid(m_pConfirmationText))
  {
    m_pConfirmationText->SetText(m_sConfirmationTitleText);
  }

  SetupFocus();
}

void UConfirmationWindow::Hide()
{
  Super::Hide();
  
}

void UConfirmationWindow::SetupFocus() {
  Super::SetupFocus();

  // Setup SpecificFocusManager
  m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
  m_pSpecificFocusManager[0]->Initialize(
    m_lFocusableElements,
    { 2 },
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

void UConfirmationWindow::Accept()
{
  OnConfirmResult.Broadcast(true);
  Hide();
}

void UConfirmationWindow::Cancel()
{
  OnConfirmResult.Broadcast(false);
  Hide();
}