/************************************************************************
 * @description: Represents the Base Widget in the User's Interface
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Jaime Paramo - 20/04/25
 ************************************************************************/

#include "BaseWidget.h"

void UBaseWidget::Show() {
  // Make all HUD elements visible while ignoring hit tests (SelfHitTestInvisible).
  for (UObject* pElement : m_lElements) {
    Cast<UWidget>(pElement)->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
  }

  m_bIsVisible = true;
}

void UBaseWidget::Hide() {
  // Hide all HUD elements.
  for (UObject* pElement : m_lElements) {
    Cast<UWidget>(pElement)->SetVisibility(ESlateVisibility::Hidden);
  }

  m_bIsVisible = false;
}

void UBaseWidget::NativeConstruct() {
  Super::NativeConstruct();
}

void UBaseWidget::NativeTick(const FGeometry& _rMyGeometry, float _fDeltaTime) {
  Super::NativeTick(_rMyGeometry, _fDeltaTime);
}

bool UBaseWidget::IsWidgetVisible() {
  return m_bIsVisible;
}
