/************************************************************************
 * @description: Represents the Focusable Widgets in the User's Interface
 * @author: Jaime Paramo
 * @date: 20/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "FocusableWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Controller/BasePlayerController.h"

void UFocusableWidget::Show() {
  Super::Show();

  // Make all HUD elements visible while ignoring hit tests (SelfHitTestInvisible).
  for (UObject* pElement : m_lFocusableElements) {
    Cast<UWidget>(pElement)->SetVisibility(ESlateVisibility::Visible);
  }
}

void UFocusableWidget::Hide() {
  Super::Hide();
  
  // Make all HUD elements visible while ignoring hit tests (SelfHitTestInvisible).
  for (UObject* pElement : m_lFocusableElements) {
    Cast<UWidget>(pElement)->SetVisibility(ESlateVisibility::Hidden);
  }
}

void UFocusableWidget::NativeConstruct() {
    Super::NativeConstruct();
}

void UFocusableWidget::NativeTick(const FGeometry& _rMyGeometry, float _fDeltaTime) {
    Super::NativeTick(_rMyGeometry, _fDeltaTime);
}

void UFocusableWidget::SetupFocus() {

}

TArray<TObjectPtr<USpecificFocusManager>>& UFocusableWidget::GetSpecificFocusManager() {
    return m_pSpecificFocusManager;
}

TArray<TObjectPtr<UObject>>& UFocusableWidget::GetFocusableElements() {
    return m_lFocusableElements;
}

void UFocusableWidget::SetPauseMode(bool _bStatus)
{
  ABasePlayerController* pController = Cast<ABasePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
  if (IsValid(pController))
  {
    UGameplayStatics::SetGamePaused(GetWorld(), _bStatus);
    if (_bStatus)
    {
      pController->SwitchInputMode(EPlayerInputMode::UI);
    }
    else
    {
      pController->SwitchInputMode(EPlayerInputMode::GAMEPLAY);
    }
  }
}