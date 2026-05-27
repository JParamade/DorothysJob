/************************************************************************
 * @description: Represents the technology logos that appear when the game is executed.
 * @author: Jaime Paramo
 * @date: 24/09/25
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "TechLogos.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UTechLogos::Show() {
  Super::Show();

  if (IsValid(m_pLogoAnimation)) {
    const float fAnimationDuration = m_pLogoAnimation->GetEndTime();
    
    PlayAnimation(m_pLogoAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
  
    FTimerHandle oHandle;
    TWeakObjectPtr<UTechLogos> WeakThis(this);
    GetWorld()->GetTimerManager().SetTimer(
      oHandle,
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

void UTechLogos::Hide() {
  Super::Hide();

  OnAnimationFinished.ExecuteIfBound();
}

void UTechLogos::NativeConstruct() {
  Super::NativeConstruct();

  // Background
  if (IsValid(m_pBackgroundImage)) m_lElements.Add(m_pBackgroundImage);
  
  // Unreal Engine
  if (IsValid(m_pUnrealEngineLogo)) m_lElements.Add(m_pUnrealEngineLogo);
  
  // FMOD
  if (IsValid(m_pPoweredByText)) m_lElements.Add(m_pPoweredByText);
  if (IsValid(m_pFmodLogo)) m_lElements.Add(m_pFmodLogo);
  
  // Utad
  if (IsValid(m_pUtadLogo)) m_lElements.Add(m_pUtadLogo);

  // Bola 13
  if (IsValid(m_pBola13Logo)) m_lElements.Add(m_pBola13Logo);
}
