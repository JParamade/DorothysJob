/************************************************************************
 * @description: Represents the HUD widget that displays FPS.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#include "HUDFPS.h"

// Component
#include "Components/TextBlock.h"

void UHUDFPS::Show() {
  Super::Show();

  // Start a repeating timer to update the FPS text every 0.2 seconds.
  GetWorld()->GetTimerManager().SetTimer(
    m_oFPSHandle,
    this,
    &UHUDFPS::UpdateFPSText,
    .2f,
    true  
  );
}

void UHUDFPS::Hide() {
  Super::Hide();

  // Stop the FPS update timer when hiding the widget.
  GetWorld()->GetTimerManager().ClearTimer(m_oFPSHandle);
}

void UHUDFPS::NativeConstruct() {
  Super::NativeConstruct();

  // Text Block
  if (IsValid(m_pFPSText)) m_lElements.Add(m_pFPSText);
}

void UHUDFPS::UpdateFPSText() {
  if (IsValid(m_pFPSText)) {
    // Get DeltaTime.
    float fDeltaTime = FApp::GetDeltaTime();

    // Update text (function converts DeltaTime into FPS).
    if (fDeltaTime > .0f) {
      // Get rid of decimal part.
      int32 iFPS = FMath::RoundToInt(1.f / fDeltaTime);
      
      // Format text into the Frames Per Second value with FPS text.
      FString sFPS = FString::Printf(TEXT("%i FPS"), iFPS);

      // Set text in the UMG TextBlock.
      m_pFPSText->SetText(FText::FromString(sFPS));
    }
  }
}