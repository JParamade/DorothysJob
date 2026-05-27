/************************************************************************
 * @description: Represents the HUD's Room Cleaned Widget.
 * @author: Jaime Paramo
 * @date: 10/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDRoomCleaned.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/Image.h"

// Data
#include "DorothysJob/Data/DataTables/LevelData.h"

// Gameplay
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"

void UHUDRoomCleaned::Show() {
  Super::Show();

  // Bind to the static floor cleaned event to react when a floor is cleaned.
  m_oRoomCleanedHandle = UBaseRoom::OnFloorCleaned.AddUObject(this, &UHUDRoomCleaned::UpdateFloorCleanedImage);
}

void UHUDRoomCleaned::Hide() {
  Super::Hide();

  // Non Focusable Elements
  if (IsValid(m_pBannerImage)) m_pBannerImage->SetVisibility(ESlateVisibility::Hidden);
  if (IsValid(m_pRewardImage)) m_pRewardImage->SetVisibility(ESlateVisibility::Hidden);

  // Remove event bindings.
  UBaseRoom::OnFloorCleaned.Remove(m_oRoomCleanedHandle);
}

void UHUDRoomCleaned::NativeConstruct() {
  Super::NativeConstruct();
}

void UHUDRoomCleaned::UpdateFloorCleanedImage(UBaseRoom* Room) {
  if (!IsValid(m_pBannerImage) || !IsValid(m_pRewardImage)) return;
  
  // @REVISIT: Comments are not updated to the new version.

  if (Room) {
    if (!Room->CanShowVFX()) return;
    switch (Room->GetRoomType()) {
      break; case ERoomType::BATHROOM: m_pRewardImage->SetBrushFromTexture(m_pCleaningRewardTexture);
      break; case ERoomType::BEDROOM: m_pRewardImage->SetBrushFromTexture(nullptr);
      break; case ERoomType::KITCHEN: m_pRewardImage->SetBrushFromTexture(m_pTimeRewardTexture);
      break; case ERoomType::LIVINGROOM: m_pRewardImage->SetBrushFromTexture(m_pSpecialRewardTexture);
      break; case ERoomType::GREENHOUSE: m_pRewardImage->SetBrushFromTexture(m_pConsumableRewardTexture);
    }
  }

  // Show the floor cleaned text.
  m_pBannerImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
  m_pRewardImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

  if (IsValid(m_pFadeInAnimation)) PlayAnimation(m_pFadeInAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);

  TWeakObjectPtr<UHUDRoomCleaned> WeakThis(this);
  // Set a timer to hide the message after 'm_fRoomCleanedTime' seconds.
  GetWorld()->GetTimerManager().SetTimer(
    oHideTextHandler,
    [WeakThis]() {
      if (WeakThis.IsValid() && IsValid(WeakThis->m_pBannerImage) && IsValid(WeakThis->m_pRewardImage)) {
        WeakThis->PlayFadeOutAnimation();
      }
    },
    m_fRoomCleanedTextTime,
    false
  );
}

void UHUDRoomCleaned::PlayFadeOutAnimation() {
  // If the fade out animation isn't valid, there’s nothing to play.
  if (!IsValid(m_pFadeOutAnimation)) return;

  // Get the length of the fade out animation (used for timing the next step).
  const float fFadeDuration = m_pFadeOutAnimation->GetEndTime();

  PlayAnimation(m_pFadeOutAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);

  TWeakObjectPtr<UHUDRoomCleaned> WeakThis(this);
  GetWorld()->GetTimerManager().SetTimer(
    oHideTextHandler,
    [WeakThis]() {
      if (WeakThis.IsValid() && IsValid(WeakThis->m_pBannerImage) && IsValid(WeakThis->m_pRewardImage)) {
        WeakThis->m_pBannerImage->SetVisibility(ESlateVisibility::Hidden);
        WeakThis->m_pRewardImage->SetVisibility(ESlateVisibility::Hidden);
      }
    },
    fFadeDuration,
    false
  );
}