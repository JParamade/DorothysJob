/************************************************************************
 * @description: Represents the Base Button in the User's Interfaces.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Jaime Paramo
 *             Josephine Esposito
 *             David Gonzalez
 ************************************************************************/

#include "BaseButton.h"

// Style
#include "Slate/ButtonStyleAsset.h"
#include "Styling/SlateTypes.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

// FMOD
#include "FMODEvent.h"

void UBaseButton::SetButtonSelected(bool _bSelected)
{
  m_bSelected = _bSelected;
  ApplyDefaultStyle();
}

void UBaseButton::SetButtonBlockAction(bool _bBlocked)
{
  m_bBlocked = _bBlocked;
}

void UBaseButton::ForceGainFocus() {
  OnGainFocus();
}

void UBaseButton::ForceLoseFocus() {
  OnLoseFocus();
}

void UBaseButton::SynchronizeProperties() {
  Super::SynchronizeProperties();

  ApplyDefaultStyle();

  // Bind native mouse hover events to custom focus handlers.
  // This allows the button to visually respond to mouse hover using the same logic as focus navigation.
  if (!OnHovered.IsAlreadyBound(this, &UBaseButton::OnGainFocus)) OnHovered.AddDynamic(this, &UBaseButton::OnGainFocus);
  if (!OnUnhovered.IsAlreadyBound(this, &UBaseButton::OnLoseFocus)) OnUnhovered.AddDynamic(this, &UBaseButton::OnLoseFocus);
  if (!OnClicked.IsAlreadyBound(this, &UBaseButton::PlayClickSound)) OnClicked.AddDynamic(this, &UBaseButton::PlayClickSound);

  // Disable native Unreal Engine UMG navigation by setting all directions to "Stop".
  SetNavigationRuleBase(EUINavigation::Left, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Right, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Up, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Down, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Next, EUINavigationRule::Stop);
  SetNavigationRuleBase(EUINavigation::Previous, EUINavigationRule::Stop);

  // Prevent native UMG focus behavior for this widget.
  InitIsFocusable(false);

  m_bSelected = false;
  m_bBlocked = false;
}

void UBaseButton::OnGainFocus() {
  ApplyVariantStyle(&FButtonStyle::Hovered);

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_hover.SFX_UI_hover")));
    }
  }

  OnHoveredAction.Broadcast();
}

void UBaseButton::OnLoseFocus() {
  ApplyDefaultStyle();
}

void UBaseButton::OnPressed() {
  if (!m_bBlocked)
  {
    ApplyVariantStyle(&FButtonStyle::Pressed);
  }
}

void UBaseButton::OnReleased() {
  if (!m_bBlocked)
  {
    ApplyVariantStyle(&FButtonStyle::Hovered);

    ExecuteAction();
  }
}

void UBaseButton::ExecuteAction() {
  PlayClickSound();

  OnClicked.Broadcast();
}

void UBaseButton::PlayClickSound() {
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_acceptButton.SFX_UI_acceptButton")));
    }
  }
  OnClickedAction.ExecuteIfBound();
}

const FButtonStyle* UBaseButton::GetButtonStyle() const
{
  return (m_bSelected && IsValid(m_pButtonSelectedStyleAsset)) ? m_pButtonSelectedStyleAsset->GetStyle<FButtonStyle>() :
         (IsValid(m_pButtonStyleAsset)) ? m_pButtonStyleAsset->GetStyle<FButtonStyle>() : nullptr;
}

void UBaseButton::ApplyDefaultStyle() {
  if (const FButtonStyle* oButtonStyle = GetButtonStyle()) SetStyle(*oButtonStyle);
}

void UBaseButton::ApplyVariantStyle(FSlateBrush FButtonStyle::* _pBrushStyle) {
  if (const FButtonStyle* oButtonStyle = GetButtonStyle()) {
    FButtonStyle oTemp = *oButtonStyle;
    oTemp.Normal = oButtonStyle->*_pBrushStyle;

    SetStyle(oTemp);
  }
}