#include "ControlsTab.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "DorothysJob/UI/Focus/GeneralFocusManager.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "DorothysJob/UI/Utils/InputIcon.h"
#include "DorothysJob/Controller/BasePlayerController.h"

void UControlsTab::NativeConstruct()
{
  Super::NativeConstruct();

  if (IsValid(m_pCloseButton))
  {
    m_pCloseButton->OnClicked.AddUniqueDynamic(this, &UControlsTab::OnCloseClicked);
  }

  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
  }

  m_bIsVisible = false;
}

void UControlsTab::NativeDestruct()
{
  if (IsValid(m_pCloseButton))
  {
    m_pCloseButton->OnClicked.RemoveAll(this);
  }

  Super::NativeDestruct();
}

void UControlsTab::Show()
{
  m_bIsVisible = true;
  SetupFocus();

  if (IsValid(m_pChangeTabIconButton))
  {
    m_pChangeTabIconButton->Show();
  }

  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Visible);
  }
}

void UControlsTab::Hide()
{
  if (IsValid(m_pChangeTabIconButton))
  {
    m_pChangeTabIconButton->Hide();
  }

  if (IsValid(m_pCanvasPanel))
  {
    m_pCanvasPanel->SetVisibility(ESlateVisibility::Collapsed);
  }

  m_bIsVisible = false;
}

void UControlsTab::SetupFocus()
{
  Super::SetupFocus();

  m_lFocusableElements.Empty();
  m_pSpecificFocusManager.Empty();

  if (IsValid(m_pCloseButton))
  {
    m_lFocusableElements.Add(m_pCloseButton);
    m_pSpecificFocusManager.Add(NewObject<USpecificFocusManager>(this));
    m_pSpecificFocusManager[0]->Initialize(m_lFocusableElements, { 1 }, EFocusNavigationType::HORIZONTAL, true, false);

    ABasePlayerController* pPlayerController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    UGeneralFocusManager* pGeneralFocusManager = IsValid(pPlayerController) ? pPlayerController->GetGeneralFocusManager() : nullptr;
    if (IsValid(pGeneralFocusManager))
    {
      pGeneralFocusManager->SetActiveMenu(m_pSpecificFocusManager);
    }
  }
}

void UControlsTab::OnCloseClicked()
{
  OnMenuClosed.Broadcast();
}