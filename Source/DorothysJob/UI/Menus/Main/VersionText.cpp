/************************************************************************
 * @description: Represents the project's version widget.
 * @author: Jaime Paramo
 * @date: 19/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "VersionText.h"

// Component
#include "Components/TextBlock.h"

// Project
#include "DorothysJob/Utils/ProjectSettings.h"

void UVersionText::Show() {
  Super::Show();
}

void UVersionText::Hide() {
  Super::Hide();
}

void UVersionText::NativeConstruct() {
  Super::NativeConstruct();

  UpdateVersionText();

  // Version Text
  if (IsValid(m_pVersionText)) m_lElements.Add(m_pVersionText);
}

void UVersionText::UpdateVersionText() {
  // Retrieves the project version string from the ProjectSettings and sets it as the text of the version Text Block.
  if (IsValid(m_pVersionText)) m_pVersionText->SetText(FText::FromString(ProjectSettings::GetProjectVersion()));
}
