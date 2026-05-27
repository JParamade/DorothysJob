/************************************************************************
 * @description: Represents the UMG Loading Screen class.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "LoadingScreen.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Widget
#include "DorothysJob/UI/Menus/LoadingScreen/LoadingThrobber.h"
#include "DorothysJob/UI/Menus/LoadingScreen/TipBox.h"

void ULoadingScreen::Show() {
  Super::Show();

  if (IsValid(m_pLoadingThrobberWidget)) m_pLoadingThrobberWidget->Show();
  if (IsValid(m_pTipBoxWidget)) m_pTipBoxWidget->Show();
}

void ULoadingScreen::Hide() {
  Super::Hide();

  if (IsValid(m_pLoadingThrobberWidget)) m_pLoadingThrobberWidget->Hide();
  if (IsValid(m_pTipBoxWidget)) m_pTipBoxWidget->Hide();

  FadeBackground(EUMGSequencePlayMode::Forward);
}

void ULoadingScreen::NativeConstruct() {
  Super::NativeConstruct();
}

void ULoadingScreen::FadeBackground(EUMGSequencePlayMode::Type _eMode) {
   if (IsValid(m_pFadeAnimation)) PlayAnimation(m_pFadeAnimation, 0.f, 1.f, _eMode);
}