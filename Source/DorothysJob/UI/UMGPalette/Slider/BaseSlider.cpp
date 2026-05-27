/************************************************************************
 * @description: Represents our custom Slider UMG element class.
 * @author: Jaime Paramo
 * @date: 12/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "BaseSlider.h"

// Style
#include "Styling/SlateTypes.h"

void UBaseSlider::SynchronizeProperties() {
  Super::SynchronizeProperties();

  ApplyDefaultStyle();

  // Disable native Unreal Engine UMG navigation by setting all directions to "Stop".
  SetNavigationRuleBase(EUINavigation::Left, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Right, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Next, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Previous, EUINavigationRule::Stop);

  // Prevent native UMG focus behavior for this widget.
  IsFocusable = false;
}

void UBaseSlider::OnGainFocus() {
  ApplyVariantStyle(&FSliderStyle::HoveredThumbImage);
}

void UBaseSlider::OnLoseFocus() {
  ApplyDefaultStyle();
}

void UBaseSlider::OnAxisInput(FVector2D _vAxisValue) {
  float fCurrentValue = GetValue();
  float fStep = 0.05f;

  if (FMath::Abs(_vAxisValue.X) > KINDA_SMALL_NUMBER) {
    float fNewValue = FMath::Clamp(fCurrentValue + (_vAxisValue.X * fStep), 0.0f, 1.0f);
    if (!FMath::IsNearlyEqual(fNewValue, fCurrentValue)) {
      SetValue(fNewValue);
      OnValueChanged.Broadcast(fNewValue);
    }
  }
}

void UBaseSlider::ApplyDefaultStyle() {
  SetWidgetStyle(m_oSliderStyle);
}

void UBaseSlider::ApplyVariantStyle(FSlateBrush FSliderStyle::* _pBrushStyle) {
  FSliderStyle oTemp = m_oSliderStyle;
  oTemp.NormalThumbImage = m_oSliderStyle.*_pBrushStyle;

  SetWidgetStyle(oTemp);
}