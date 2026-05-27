/************************************************************************
 * @description: Represents the unitary Heart Widget from the HUD's Health Bar.
 * @author: Jaime Paramo
 * @date: 24/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDHeart.h"

// Component
#include "Components/Image.h"

void UHUDHeart::Show() {
  Super::Show();
}

void UHUDHeart::Hide() {
  Super::Hide();
}

void UHUDHeart::NativeConstruct() {
  Super::NativeConstruct();

  // Image
  if (IsValid(m_pHeartImage)) m_lElements.Add(m_pHeartImage);
}

void UHUDHeart::SetHeartState(EHeartState _eNewState) {
  if (IsValid(m_pHeartImage)) {
    switch (_eNewState) {
      break; case EHeartState::EMPTY: m_pHeartImage->SetBrushFromTexture(m_pEmptyHeartTexture);
      break; case EHeartState::HALF: m_pHeartImage->SetBrushFromTexture(m_pHalfHeartTexture);
      break; case EHeartState::FULL: m_pHeartImage->SetBrushFromTexture(m_pFullHeartTexture);
    }
  }
}