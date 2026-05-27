/************************************************************************
 * @description: Represents the UMG loading Throbber class used in the loading screen.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "LoadingThrobber.h"

// Component
#include "Components/Image.h"

void ULoadingThrobber::Show() {
  Super::Show();

  // Starts a repeating timer that changes the throbber's texture every m_fTimeBetweenFrames seconds.
  GetWorld()->GetTimerManager().SetTimer(
    m_oTimeBetweenFramesHandle,
    this,
    &ULoadingThrobber::NextFrame,
    m_fTimeBetweenFrames,
    true
  );
}

void ULoadingThrobber::Hide() {
  Super::Hide();
}

void ULoadingThrobber::NativeConstruct() {
  Super::NativeConstruct();

  // Loading Throbber
  if (IsValid(m_pLoadingThrobber)) m_lElements.Add(m_pLoadingThrobber);
}

void ULoadingThrobber::NextFrame() {
  // Move to the next texture index, wrapping around to the start when the end is reached.
  m_iCurrentFrame = (m_iCurrentFrame + 1) % m_lLoadingTextures.Num();
  // Update the throbber widget to display the current texture frame.
  m_pLoadingThrobber->SetBrushFromTexture(m_lLoadingTextures[m_iCurrentFrame]);
}