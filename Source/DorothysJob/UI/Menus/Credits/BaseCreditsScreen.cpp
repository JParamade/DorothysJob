#include "BaseCreditsScreen.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/CanvasPanel.h"

// Widget
#include "DorothysJob/UI/Utils/InputIcon.h"

void UBaseCreditsScreen::Show() {
  Super::Show();

  if (IsValid(m_pSkipButtonIcon)) m_pSkipButtonIcon->Show();

  if (IsValid(m_pCreditsAnimation)) {
    const float fAnimationDuration = m_pCreditsAnimation->GetEndTime();

    PlayAnimation(m_pCreditsAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);

    TWeakObjectPtr<UBaseCreditsScreen> WeakThis(this);
    GetWorld()->GetTimerManager().SetTimer(
      m_oTimerHandle,
      [WeakThis]() {
        if (WeakThis.IsValid())
        {
          WeakThis->Hide();
        }
      },
      fAnimationDuration,
      false
    );
  }
}

void UBaseCreditsScreen::Hide() {
  Super::Hide();

  OnCreditsClosed.Broadcast();

  if (IsValid(m_pSkipButtonIcon)) m_pSkipButtonIcon->Hide();

  if (GetWorld()->GetTimerManager().IsTimerActive(m_oTimerHandle)) GetWorld()->GetTimerManager().ClearTimer(m_oTimerHandle);
}

void UBaseCreditsScreen::NativeConstruct() {
  Super::NativeConstruct();

  if (IsValid(m_pRootCanvas)) m_lElements.Add(m_pRootCanvas);
}