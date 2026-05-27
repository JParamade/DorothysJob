/************************************************************************
 * @description: Represents the Consumable Info's HUD widget.
 * @author: Jaime Paramo
 * @date: 08/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDConsumableInfo.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Data Asset
#include "DorothysJob/Data/DataAssets/ConsumableDataAsset.h"

// Gameplay
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include <Kismet/GameplayStatics.h>

// Managers
#include "DorothysJob/Manager/BaseGameInstance.h"

// Widget
#include "DorothysJob/UI/Utils/InputIcon.h"

void UHUDConsumableInfo::Show() {
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  if (pGameInstance) if (pGameInstance->m_lSelectedConsumables.IsEmpty()) return;
  
  Super::Show();

  // Bind to the BasePlayer's combat events to react when consumable information is updated.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    m_oConsumableUsedHandle = pPlayerPawn->OnConsumableUpdated.AddUObject(this, &UHUDConsumableInfo::UpdateConsumableStackText);
    m_oConsumableUnavailableHandle = pPlayerPawn->OnConsumableUnavailable.AddUObject(this, &UHUDConsumableInfo::HandleConsumableUnavailable);
  }

  if (IsValid(m_pPrimaryButtonIcon)) m_pPrimaryButtonIcon->Show();
  if (IsValid(m_pSecondaryButtonIcon)) m_pSecondaryButtonIcon->Show();

  if (pGameInstance) {
    // Update consumable info.
    SetImageInfo<UConsumableDataAsset>(pGameInstance->m_lSelectedConsumables, 0, m_pPrimaryConsumableIcon);
    SetImageInfo<UConsumableDataAsset>(pGameInstance->m_lSelectedConsumables, 1, m_pSecondaryConsumableIcon);
  }
}

void UHUDConsumableInfo::Hide() {
  Super::Hide();

  // Remove event bindings.
  if (ABasePlayer* pPlayerPawn = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0))) {
    pPlayerPawn->OnConsumableUpdated.Remove(m_oConsumableUsedHandle);
    pPlayerPawn->OnConsumableUnavailable.Remove(m_oConsumableUnavailableHandle);
  }

  if (IsValid(m_pPrimaryButtonIcon)) m_pPrimaryButtonIcon->Hide();
  if (IsValid(m_pSecondaryButtonIcon)) m_pSecondaryButtonIcon->Hide();
}

void UHUDConsumableInfo::NativeConstruct() {
  Super::NativeConstruct();

  // Primary Consumable
  if (IsValid(m_pPrimaryConsumableIcon)) m_lElements.Add(m_pPrimaryConsumableIcon);
  if (IsValid(m_pPrimaryConsumableBubble)) m_lElements.Add(m_pPrimaryConsumableBubble);
  if (IsValid(m_pPrimaryConsumableStackText)) m_lElements.Add(m_pPrimaryConsumableStackText);

  // Secondary Consuamble
  if (IsValid(m_pSecondaryConsumableIcon)) m_lElements.Add(m_pSecondaryConsumableIcon);
  if (IsValid(m_pSecondaryConsumableBubble)) m_lElements.Add(m_pSecondaryConsumableBubble);
  if (IsValid(m_pSecondaryConsumableStackText)) m_lElements.Add(m_pSecondaryConsumableStackText);
}

void UHUDConsumableInfo::UpdateConsumableStackText(int32 _iNewQuantity, int32 _iConsumableID) {
  // Set the primary consumable quantity as text.
  if (_iConsumableID == 0) m_pPrimaryConsumableStackText->SetText(FText::AsNumber(_iNewQuantity));
  // Set the secondary consumable quantity as text.
  else if (_iConsumableID == 1) m_pSecondaryConsumableStackText->SetText(FText::AsNumber(_iNewQuantity));
}

void UHUDConsumableInfo::HandleConsumableUnavailable(int32 _iNewQuantity, int32 _iConsumableID) {
  if (_iConsumableID == 0) PlayAnimation(m_pCantUsePrimary, 0.f, 1, EUMGSequencePlayMode::Forward);
  else if (_iConsumableID == 1) PlayAnimation(m_pCantUseSecondary, 0.f, 1, EUMGSequencePlayMode::Forward);
}

template <typename T>
TObjectPtr<UTexture2D> UHUDConsumableInfo::GetTextureFromDataAsset(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex) {
  // Attempt to retrieve the DataAsset at the given index.
  if (_rDataAssets.IsValidIndex(_iIndex)) {
    if (const T* pDataAsset = Cast<T>(_rDataAssets[_iIndex])) {
      // Return its icon texture.
      return pDataAsset->m_pIcon;
    }
  }

  // If anything failed, return null.
  return nullptr;
}

template <typename T>
void UHUDConsumableInfo::SetImageInfo(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex, TObjectPtr<UImage> _pImage) {
  // If valid, assign the texture from index _iIndex to the image.
  if (IsValid(_pImage)) _pImage->SetBrushFromTexture(GetTextureFromDataAsset<T>(_rDataAssets, _iIndex));
}