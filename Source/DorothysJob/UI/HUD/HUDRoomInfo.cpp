/************************************************************************
 * @description: Represents the Room Info's HUD widget.
 * @author: Jaime Paramo
 * @date: 06/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDRoomInfo.h"

// Component
#include "Components/Image.h"

 // Widget
#include "HUDMinimap.h"
#include "HUDTimer.h"

#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Data/DataAssets/Tutorial/EventListDataAsset.h"


void UHUDRoomInfo::Show() {
  Super::Show();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (pGameInstance->m_oHUDData[EHUDSection::MINIMAP]) ShowMinimap();
    if (pGameInstance->m_oHUDData[EHUDSection::CLOCK]) ShowTimer();
  }
}

void UHUDRoomInfo::ShowMinimap() {
  if (IsValid(m_pHUDMinimapWidget)) m_pHUDMinimapWidget->Show();
}

void UHUDRoomInfo::ShowTimer() {
   if (IsValid(m_pHUDTimerWidget)) m_pHUDTimerWidget->Show();
}

void UHUDRoomInfo::ShowCalendar() {
  if (IsValid(m_pCalendarImage)) {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (m_lCalendarImages.IsValidIndex(pGameInstance->m_iCurrentLevel - 1)) {
        m_pCalendarImage->SetBrushFromTexture(m_lCalendarImages[pGameInstance->m_iCurrentLevel - 1]);
        m_pCalendarImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
      }
    }
  }
}

void UHUDRoomInfo::Hide() {
  Super::Hide();

  if (IsValid(m_pHUDMinimapWidget)) m_pHUDMinimapWidget->Hide();
  if (IsValid(m_pHUDTimerWidget)) m_pHUDTimerWidget->Hide();
  if (IsValid(m_pCalendarImage)) m_pCalendarImage->SetVisibility(ESlateVisibility::Hidden);
}

void UHUDRoomInfo::NativeConstruct() {
  Super::NativeConstruct();
}