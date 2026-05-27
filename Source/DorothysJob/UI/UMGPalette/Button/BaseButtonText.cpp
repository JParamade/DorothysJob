/************************************************************************
 * @description: Represents a the base button with text implementation
 * @author: Josephine Esposito
 * @date: 30/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/
#include "BaseButtonText.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"

void UBaseButtonText::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  if (!m_pText)
  { 
    m_pText = NewObject<UTextBlock>(this, UTextBlock::StaticClass(), FName(TEXT("Text") + this->GetFName().ToString()));
    m_pText->SetText(FText::FromString("Button"));
    m_pText->SetJustification(ETextJustify::Center);
    AddChild(m_pText); // adding the text as a child to the button component here
  }
}
