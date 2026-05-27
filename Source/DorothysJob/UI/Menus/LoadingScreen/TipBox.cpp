/************************************************************************
 * @description: Represents the UMG TipBox class used in the loading screen.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "TipBox.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/TextBlock.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/NonPersistent/TipManager.h"

void UTipBox::Show() {
  Super::Show();

  // Update the text to the next random tip.
  UpdateTipText();
  
  // Starts the periodic fade/update cycle. 
  WaitForNextText();
}

void UTipBox::Hide() {
  Super::Hide();
}

void UTipBox::NativeConstruct() {
  Super::NativeConstruct();

  // Text Block
  if (IsValid(m_pPredefinedText)) m_lElements.Add(m_pPredefinedText);
  if (IsValid(m_pTipText)) m_lElements.Add(m_pTipText);
}

void UTipBox::FadeCurrentText() {
  // If the fade animation isn't valid, there’s nothing to play.
  if (!IsValid(m_pFadeAnimation)) return;

  // Get the length of the fade animation (used for timing the next step).
  const float fFadeDuration = m_pFadeAnimation->GetEndTime();

  // Play the fade animation forward (current text fades out).
  PlayAnimation(m_pFadeAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);

  // Set a timer to trigger FadeNextText after the fade-out finishes.
  FTimerHandle oHandle;
  GetWorld()->GetTimerManager().SetTimer(
    oHandle,
    this,
    &UTipBox::FadeNextText,
    fFadeDuration,
    false
  );
}

void UTipBox::FadeNextText() {
  // Update the text to the next random tip.
  UpdateTipText();

  // If the fade animation isn't valid, there’s nothing to play.
  if (!IsValid(m_pFadeAnimation)) return;

  // Get the length of the fade animation (used for timing the next step).
  const float fFadeDuration = m_pFadeAnimation->GetEndTime();

  // Play the fade animation forward (current text fades out).
  PlayAnimation(m_pFadeAnimation, 0.f, 1, EUMGSequencePlayMode::Reverse);

  // Set a timer to trigger FadeNextText after the fade-out finishes.
  FTimerHandle oHandle;
  GetWorld()->GetTimerManager().SetTimer(
    oHandle,
    this,
    &UTipBox::WaitForNextText,
    fFadeDuration,
    false
  );
}

void UTipBox::WaitForNextText() {
  // Set a timer to trigger FadeCurrentText after the wait time finishes.
  GetWorld()->GetTimerManager().SetTimer(
    m_oTipTimeHandle,
    this,
    &UTipBox::FadeCurrentText,
    m_fTipDuration,
    false
  );
}

void UTipBox::UpdateTipText() {
  // Get the game instance.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    // Retrieve TipManager and update displayed text with the next tip.
    if (UTipManager* pTipManager = pGameInstance->GetTipManager()) {
      m_pTipText->SetText(pTipManager->GetNextTip());
    }
  }
}