/************************************************************************
 * @description: @TOFILL.
 * @author: Jaime Paramo
 * @date: 29/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "BaseCheckBox.h"

 // Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

// FMOD
#include "FMODEvent.h"

void UBaseCheckBox::SynchronizeProperties() {
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
  //InitIsFocusable(false);
}

void UBaseCheckBox::OnGainFocus() {
  ApplyVariantStyle(IsChecked() ? &FCheckBoxStyle::CheckedHoveredImage : &FCheckBoxStyle::UncheckedImage);

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_hover.SFX_UI_hover")));
    }
  }
}

void UBaseCheckBox::OnLoseFocus() {
  ApplyDefaultStyle();
}

void UBaseCheckBox::OnPressed() {
  //ApplyVariantStyle(&FCheckBoxStyle::Pressed);
}

void UBaseCheckBox::OnReleased() {
  //ApplyVariantStyle(&FCheckBoxStyle::Hovered);

  ExecuteAction();
}

void UBaseCheckBox::ExecuteAction() {
  PlayClickSound();

  SetIsChecked(!IsChecked());
  OnCheckStateChanged.Broadcast(IsChecked());
}

void UBaseCheckBox::PlayClickSound() {
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_acceptButton.SFX_UI_acceptButton")));
    }
  }
}

void UBaseCheckBox::ApplyDefaultStyle() {
  if (IsValid(m_pCheckBoxStyleAsset)) SetWidgetStyle(*m_pCheckBoxStyleAsset->GetStyle<FCheckBoxStyle>());
}

void UBaseCheckBox::ApplyVariantStyle(FSlateBrush FCheckBoxStyle::* _pBrushStyle) {
  if (IsValid(m_pCheckBoxStyleAsset)) {
    FCheckBoxStyle oCheckBoxStyle = *m_pCheckBoxStyleAsset->GetStyle<FCheckBoxStyle>();

    if (IsChecked()) oCheckBoxStyle.CheckedImage = m_pCheckBoxStyleAsset->GetStyle<FCheckBoxStyle>()->*_pBrushStyle;
    else oCheckBoxStyle.UncheckedImage = m_pCheckBoxStyleAsset->GetStyle<FCheckBoxStyle>()->*_pBrushStyle;

    SetWidgetStyle(oCheckBoxStyle);
  }
}