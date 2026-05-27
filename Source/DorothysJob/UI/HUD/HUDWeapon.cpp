/************************************************************************
 * @description: Represents the player's weapon widget.
 * @author: Jaime Paramo
 * @date: 11/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDWeapon.h"

// Component
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

// Data Asset
#include "DorothysJob/Data/DataAssets/ConsumableDataAsset.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"

// Gameplay
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include <Kismet/GameplayStatics.h>

// Managers
#include "DorothysJob/Manager/BaseGameInstance.h"

// Style
#include "Math/Color.h"
#include "Styling/SlateBrush.h"

// Widget
#include "HUDHeartBar.h"

void UHUDWeapon::Show() {
  Super::Show();

  // Bind to the BasePlayer's combat events to react when the weapon and consumable information is updated.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    m_oWeaponChangeHandle = pPlayerPawn->OnWeaponChange.AddUObject(this, &UHUDWeapon::UpdateWeaponIcons);
    m_oConsumableUsedHandle = pPlayerPawn->OnConsumableUpdated.AddUObject(this, &UHUDWeapon::UpdateConsumableStackText);
  }

  // Initialize and update HUD visuals for weapons and consumables.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    // Update weapon info.
    SetBarInfo<UWeaponDataAsset>(pGameInstance->m_lSelectedWeapons, 0, m_pPrimaryWeaponBar, m_pPrimaryWeaponIcon);
    SetBarInfo<UWeaponDataAsset>(pGameInstance->m_lSelectedWeapons, 1, m_pSecondaryWeaponBar, m_pSecondaryWeaponIcon);
    
    // Update consumable info.
    SetImageInfo<UConsumableDataAsset>(pGameInstance->m_lSelectedConsumables, 0, m_pPrimaryConsumableIcon);
    SetImageInfo<UConsumableDataAsset>(pGameInstance->m_lSelectedConsumables, 1, m_pSecondaryConsumableIcon);
  }

  if (IsValid(m_pHeartBar)) m_pHeartBar->Show();
}

void UHUDWeapon::Hide() {
  Super::Hide();

  // Remove event bindings.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    pPlayerPawn->OnWeaponChange.Remove(m_oWeaponChangeHandle);
    pPlayerPawn->OnConsumableUpdated.Remove(m_oConsumableUsedHandle);
  }

  if (IsValid(m_pHeartBar)) m_pHeartBar->Hide();
}

void UHUDWeapon::NativeConstruct() {
  Super::NativeConstruct();

  // Primary Weapon
  if (IsValid(m_pPrimaryWeaponBar)) m_lElements.Add(m_pPrimaryWeaponBar);
  if (IsValid(m_pPrimaryWeaponIcon)) m_lElements.Add(m_pPrimaryWeaponIcon);
  if (IsValid(m_pPrimaryWeaponBubble)) m_lElements.Add(m_pPrimaryWeaponBubble);

  // Secondary Weapon
  if (IsValid(m_pSecondaryWeaponBar)) m_lElements.Add(m_pSecondaryWeaponBar);
  if (IsValid(m_pSecondaryWeaponIcon)) m_lElements.Add(m_pSecondaryWeaponIcon);
  if (IsValid(m_pSecondaryWeaponBubble)) m_lElements.Add(m_pSecondaryWeaponBubble);

  // Primary Consumable
  if (IsValid(m_pPrimaryConsumableIcon)) m_lElements.Add(m_pPrimaryConsumableIcon);
  if (IsValid(m_pPrimaryConsumableBubble)) m_lElements.Add(m_pPrimaryConsumableBubble);
  if (IsValid(m_pPrimaryConsumableStackText)) m_lElements.Add(m_pPrimaryConsumableStackText);
  if (IsValid(m_pPrimaryButtonIcon)) m_lElements.Add(m_pPrimaryButtonIcon);

  // Secondary Consuamble
  if (IsValid(m_pSecondaryConsumableIcon)) m_lElements.Add(m_pSecondaryConsumableIcon);
  if (IsValid(m_pSecondaryConsumableBubble)) m_lElements.Add(m_pSecondaryConsumableBubble);
  if (IsValid(m_pSecondaryConsumableStackText)) m_lElements.Add(m_pSecondaryConsumableStackText);
  if (IsValid(m_pSecondaryButtonIcon)) m_lElements.Add(m_pSecondaryButtonIcon);
}

void UHUDWeapon::NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) {
  Super::NativeTick(_rMyGeometry, _fInDeltaTime);

  // SHEESH MAN, YOU CRIMINAL AF
  if (ABasePlayer* pPlayer = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      m_fPrimaryPercent = MapPercentToRange(
        pGameInstance->m_lSelectedWeapons[m_bSecondaryWeapon]->m_fMinBarValue,
        pGameInstance->m_lSelectedWeapons[m_bSecondaryWeapon]->m_fMaxBarValue,
        pPlayer->GetCurrentWeaponSpecialCharge()
      );
      m_pPrimaryWeaponBar->SetPercent(m_fPrimaryPercent);
    }
  }
}

void UHUDWeapon::UpdateWeaponIcons(EDirtType _eAttackType, EWeaponAttackType _eWeaponAttackType) {
  // Temporary brushes to hold current tint information before swapping.
  FSlateBrush oPrimaryBrush = m_pPrimaryWeaponIcon->GetBrush();
  FSlateBrush oSecondaryBrush = m_pSecondaryWeaponIcon->GetBrush();

  // Apply a lighter tint to the primary weapon icon to highlight it's active.
  oSecondaryBrush.TintColor = FLinearColor::White;
  m_pPrimaryWeaponIcon->SetBrush(oSecondaryBrush);

  // Apply a darker tint to the secondary weapon icon to indicate it's not currently equipped.
  oPrimaryBrush.TintColor = FLinearColor(0.125f, 0.125f, 0.125f, 1.0f);
  m_pSecondaryWeaponIcon->SetBrush(oPrimaryBrush);

  // Update Progress Bars.
  float fSecondaryPercent = m_pSecondaryWeaponBar->GetPercent();

  // Get current styles
  FProgressBarStyle oPrimaryStyle = m_pPrimaryWeaponBar->GetWidgetStyle();
  FProgressBarStyle oSecondaryStyle = m_pSecondaryWeaponBar->GetWidgetStyle();

  // Swap only the FillImage resource objects
  UObject* pTempResource = oPrimaryStyle.FillImage.GetResourceObject();
  oPrimaryStyle.FillImage.SetResourceObject(oSecondaryStyle.FillImage.GetResourceObject());
  oSecondaryStyle.FillImage.SetResourceObject(pTempResource);

  // Apply the updated styles
  m_pPrimaryWeaponBar->SetWidgetStyle(oPrimaryStyle);
  m_pSecondaryWeaponBar->SetWidgetStyle(oSecondaryStyle);

  m_pSecondaryWeaponBar->SetPercent(m_fPrimaryPercent);

  m_bSecondaryWeapon = !m_bSecondaryWeapon;
}

void UHUDWeapon::UpdateConsumableStackText(int32 _iNewQuantity, int32 _iConsumableID) {
  // Set the primary consumable quantity as text.
  if (_iConsumableID == 0) m_pPrimaryConsumableStackText->SetText(FText::AsNumber(_iNewQuantity));
  // Set the secondary consumable quantity as text.
  else if (_iConsumableID == 1) m_pSecondaryConsumableStackText->SetText(FText::AsNumber(_iNewQuantity));
}

float UHUDWeapon::MapPercentToRange(float _fMin, float _fMax, float _fValue) {
  return FMath::Lerp(_fMin, _fMax, _fValue);
}

template <typename T>
TObjectPtr<UTexture2D> UHUDWeapon::GetTextureFromDataAsset(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex) {
  // Attempt to retrieve the DataAsset at the given index.
  if (const T* pDataAsset = Cast<T>(_rDataAssets[_iIndex])) {
    // Return its icon texture.
    return pDataAsset->m_pIcon;
  }

  // If anything failed, return null.
  return nullptr;
}

template<typename T>
TObjectPtr<UTexture2D> UHUDWeapon::GetTextureFromDataAsset(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex, EBarTextureType _eWidgetType) {
  // Attempt to retrieve the DataAsset at the given index.
  if (const T* pDataAsset = Cast<T>(_rDataAssets[_iIndex])) {
    switch (_eWidgetType) {
      break; case EBarTextureType::FILL: return pDataAsset->m_pFillImage;
      break; case EBarTextureType::BACKGROUND: return pDataAsset->m_pBackgroundImage;
    }
  }

  // If anything failed, return null.
  return nullptr;
}

template <typename T>
void UHUDWeapon::SetImageInfo(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex, TObjectPtr<UImage> _pImage) {
  // If valid, assign the texture from index _iIndex to the image.
  if (IsValid(_pImage)) _pImage->SetBrushFromTexture(GetTextureFromDataAsset<T>(_rDataAssets, _iIndex));
}

template<typename T>
void UHUDWeapon::SetBarInfo(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex, TObjectPtr<UProgressBar> _pBar, TObjectPtr<UImage> _pBackgroundImage) {
  // @TOCOMMENT
  if (IsValid(_pBar)) {
    // Get current bar style.
    FProgressBarStyle oStyle = _pBar->GetWidgetStyle();
    
    // Override texture of previous style.
    oStyle.FillImage.SetResourceObject(GetTextureFromDataAsset<T>(_rDataAssets, _iIndex, EBarTextureType::FILL));

    // Apply new style.
    _pBar->SetWidgetStyle(oStyle);
  }

  // @TOCOMMENT
  if (IsValid(_pBackgroundImage)) _pBackgroundImage->SetBrushFromTexture(GetTextureFromDataAsset<T>(_rDataAssets, _iIndex, EBarTextureType::BACKGROUND));
}