#include "MouseIndicator.h"
#include "Components/Image.h"

void UMouseIndicator::Show()
{
  m_pIcon->SetVisibility(ESlateVisibility::Visible);
}

void UMouseIndicator::Hide()
{
  m_pIcon->SetVisibility(ESlateVisibility::Collapsed);
}
