/************************************************************************
 * @description: Implementation of the button
 * @author: David Gonzalez
 * @date: 10/04/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/
#include "QuitGameButton.h"
#include "Kismet/KismetSystemLibrary.h"

void UQuitGameButton::SynchronizeProperties()
{
  Super::SynchronizeProperties();

  this->OnClicked.AddUniqueDynamic(this, &UQuitGameButton::OnClick);
}

void UQuitGameButton::Show()
{
  SetVisibility(ESlateVisibility::Visible);
}

void UQuitGameButton::Hide()
{
  SetVisibility(ESlateVisibility::Collapsed);
}

void UQuitGameButton::OnClick()
{
  UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
}
