/************************************************************************
 * @description: Represents the UMG Selector Widget.
 * @author: Jaime Paramo
 * @date: 27/09/2025
 * @edited_by:
 ************************************************************************/

#include "Selector.h"

 // Component
#include "Components/TextBlock.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"

// FMOD
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

// Manager
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SettingsManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"

// Settings
#include "DorothysJob/Settings/VisualSettingsManager.h"
#include "DorothysJob/Settings/SettingTypes.h"

// Style
#include "Styling/SlateTypes.h"

void USelector::Show() {
  Super::Show();

  if (IsValid(m_pLeftButton)) m_pLeftButton->SetVisibility(ESlateVisibility::Visible);
  if (IsValid(m_pRightButton)) m_pRightButton->SetVisibility(ESlateVisibility::Visible);

  UpdateUI();
}

void USelector::Hide() {
  Super::Hide();

  if (IsValid(m_pLeftButton)) m_pLeftButton->SetVisibility(ESlateVisibility::Hidden);
  if (IsValid(m_pRightButton)) m_pRightButton->SetVisibility(ESlateVisibility::Hidden);
}

void USelector::NativeConstruct() {
  Super::NativeConstruct();

  // Non Focusable Elements
  if (IsValid(m_pValueText)) m_lElements.Add(m_pValueText);

  // Focusable Elements
  if (IsValid(m_pLeftButton)) {
    if (!m_pLeftButton->OnClicked.IsAlreadyBound(this, &USelector::OnLeftClicked)) m_pLeftButton->OnClicked.AddDynamic(this, &USelector::OnLeftClicked);
  }
  if (IsValid(m_pRightButton)) {
    if (!m_pLeftButton->OnClicked.IsAlreadyBound(this, &USelector::OnRightClicked)) m_pRightButton->OnClicked.AddDynamic(this, &USelector::OnRightClicked);
  }
}

void USelector::InitializeOptions(const TArray<FText>& _rInOptions, int32 _iInitialIndex) {
  m_lDisplayOptions = _rInOptions;
  iCurrentIndex = FMath::Clamp(_iInitialIndex, 0, m_lDisplayOptions.Num() - 1);
}

void USelector::OnGainFocus() {
  ApplyVariantStyle(&FButtonStyle::Hovered);

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySound2D(this, LoadObject<UFMODEvent>(nullptr, TEXT("/Game/FMOD/Events/SoundFX/Miscellaneous/UI/Generic/SFX_UI_hover.SFX_UI_hover")));
    }
  }
}

void USelector::OnLoseFocus() {
  ApplyDefaultStyle();
}

void USelector::OnAxisInput(FVector2D _vAxisValue) {
  if (_vAxisValue.X < 0) OnLeftClicked();
  else if (_vAxisValue.X > 0) OnRightClicked();
}

void USelector::ApplyDefaultStyle() {
  if (IsValid(m_pLeftButton)) m_pLeftButton->ForceLoseFocus();
  if (IsValid(m_pRightButton)) m_pRightButton->ForceLoseFocus();
}

void USelector::ApplyVariantStyle(FSlateBrush FButtonStyle::* _pBrushStyle) {
  if (IsValid(m_pLeftButton)) m_pLeftButton->ForceGainFocus();
  if (IsValid(m_pRightButton)) m_pRightButton->ForceGainFocus();
}

void USelector::OnLeftClicked() {
  if (m_lDisplayOptions.Num() == 0) return;

  if (!m_bIsCarousel)
  {
    iCurrentIndex = FMath::Clamp(iCurrentIndex - 1, 0, m_lDisplayOptions.Num() - 1);
  }
  else
  {
    iCurrentIndex = (iCurrentIndex - 1 + m_lDisplayOptions.Num()) % m_lDisplayOptions.Num();
  }
  UpdateUI();
  OnSelectionChanged.Broadcast(iCurrentIndex);
}

void USelector::OnRightClicked() {
  if (m_lDisplayOptions.Num() == 0) return;

  if (!m_bIsCarousel)
  {
    iCurrentIndex = FMath::Clamp(iCurrentIndex + 1, 0, m_lDisplayOptions.Num() - 1);
  }
  else
  {
    iCurrentIndex = (iCurrentIndex + 1) % m_lDisplayOptions.Num();
  }
  UpdateUI();
  OnSelectionChanged.Broadcast(iCurrentIndex);
}

void USelector::UpdateUI() {
  if (IsValid(m_pValueText))
  {
    if (m_bHiddenOptions)
    {
      m_pValueText->SetText(FText::FromString(TEXT("")));
    }
    else
    {
      if (m_lDisplayOptions.Num() > 0) m_pValueText->SetText(m_lDisplayOptions[iCurrentIndex]);
      else m_pValueText->SetText(FText::FromString(TEXT("-")));
    }
  }

  if (!m_bIsCarousel)
  {
    if (IsValid(m_pLeftButton)) m_pLeftButton->SetVisibility(iCurrentIndex <= 0 ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
    if (IsValid(m_pRightButton)) {
      m_pRightButton->SetVisibility(
        (m_lDisplayOptions.Num() == 0 || iCurrentIndex >= m_lDisplayOptions.Num() - 1)
        ? ESlateVisibility::Hidden
        : ESlateVisibility::Visible
      );
    }
  }
}