/************************************************************************
 * @description: Represents the UI widget that displays the button icon of the interactable.
 * @author: Jaime Paramo
 * @date: 15/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "InteractPromt.h"

// Widget
#include "DorothysJob/UI/Utils/InputIcon.h"

void UInteractPromt::Show() {
  Super::Show();

  if (IsValid(m_pButtonPrompt)) m_pButtonPrompt->Show();
}

void UInteractPromt::Hide() {
  Super::Hide();

  if (IsValid(m_pButtonPrompt)) m_pButtonPrompt->Hide();
}

void UInteractPromt::NativeConstruct() {
  Super::NativeConstruct();
}
