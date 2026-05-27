/************************************************************************
 * @description: Represents the text displaying "Press Any Key" at the main menu.
 * @author: Jaime Paramo
 * @date: 14/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "PressAnyKeyText.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/TextBlock.h"

// Widget
#include "DorothysJob/UI/Utils/InputIcon.h"

void UPressAnyKeyText::Show() {
  Super::Show();

  if (IsValid(m_pKeyIcon)) m_pKeyIcon->Show();
}

void UPressAnyKeyText::Hide() {
  Super::Hide();

  if (IsValid(m_pKeyIcon)) m_pKeyIcon->Hide();
}

void UPressAnyKeyText::NativeConstruct() {
  Super::NativeConstruct();

  PlayAnimation(m_pBlinkAnimation, 0.f, 0.f, EUMGSequencePlayMode::Forward);

  // Text Block
  if (IsValid(m_pPressText)) m_lElements.Add(m_pPressText);
  if (IsValid(m_pToStartText)) m_lElements.Add(m_pToStartText);
}