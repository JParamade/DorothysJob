/************************************************************************
 * @description: Represents the in-game level timer's HUD widget.
 * @author: Jaime Paramo
 * @date: 12/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDTimer.h"

// Component
#include "Components/TextBlock.h"
#include "Components/Image.h"

// Manager
#include "DorothysJob/Manager/WorldSubsystem/TimeManager.h"

void UHUDTimer::Show() {
  Super::Show();

  // Bind to the TimeManager's OnTimeChanged event to react when the timer is updated.
  if (UTimeManager* pTimeManager = GetWorld()->GetSubsystem<UTimeManager>()) {
    m_oTimerChangeHandle = pTimeManager->OnTimeChanged.AddUObject(this, &UHUDTimer::UpdateTimerText);
  }
}

void UHUDTimer::Hide() {
  Super::Hide();

  // Hide Glow Animation.
  if (m_bGlowing) StopGlowAnimation();

  // Remove event bindings.
  if (UTimeManager* pTimeManager = GetWorld()->GetSubsystem<UTimeManager>()) {
    pTimeManager->OnTimeChanged.Remove(m_oTimerChangeHandle);
  }
}

void UHUDTimer::NativeConstruct() {
  Super::NativeConstruct();

  // Timer
  if (IsValid(m_pMinutesText)) m_lElements.Add(m_pMinutesText);
  if (IsValid(m_pSecondsText)) m_lElements.Add(m_pSecondsText);
  if (IsValid(m_pDotsSeparator)) m_lElements.Add(m_pDotsSeparator);
  if (IsValid(m_pTimerImage)) m_lElements.Add(m_pTimerImage);
}

void UHUDTimer::UpdateTimerText(int32 _iNewSeconds) {
  // Calculate minutes and seconds from total seconds.
  m_iMinutes = _iNewSeconds / 60;
  m_iSeconds = _iNewSeconds % 60;

  // Format minutes with leading zero if needed.
  const FText sMinutesText = (m_iMinutes < 10)
    ? FText::FromString(FString::Printf(TEXT("0%d"), m_iMinutes))
    : FText::AsNumber(m_iMinutes);

  // Format seconds with leading zero if needed.
  const FText sSecondsText = (m_iSeconds < 10)
    ? FText::FromString(FString::Printf(TEXT("0%d"), m_iSeconds))
    : FText::AsNumber(m_iSeconds);

  // Set the formatted text on the HUD.
  m_pMinutesText->SetText(sMinutesText);
  m_pSecondsText->SetText(sSecondsText);

  if (UTimeManager* pTimeManager = GetWorld()->GetSubsystem<UTimeManager>()) {
    // Handle glow animation. IMPORTANT: Sustitue magic number with data driven value.
    if (_iNewSeconds <= pTimeManager->GetRunningOutTime() && !m_bGlowing) PlayGlowAnimation();
    else if (_iNewSeconds > pTimeManager->GetRunningOutTime()) StopGlowAnimation();
  }
}

void UHUDTimer::PlayGlowAnimation() {
  // Show the timer glow image, if valid.
  if (IsValid(m_pTimerGlow)) m_pTimerGlow->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

  m_bGlowing = true;
}

void UHUDTimer::StopGlowAnimation() {
  // Hide the timer glow image, if valid.
  if (IsValid(m_pTimerGlow)) m_pTimerGlow->SetVisibility(ESlateVisibility::Hidden);

  m_bGlowing = false;
}