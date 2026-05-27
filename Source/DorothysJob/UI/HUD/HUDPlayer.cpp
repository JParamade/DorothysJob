
/************************************************************************
 * @description: Represents the player's HUD widget.
 * @author: Jaime Paramo
 * @date: 10/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDPlayer.h"

 // Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/SettingsManager.h"
#include "DorothysJob/Manager/Events/EventManager.h"

// Settings
#include "DorothysJob/Settings/GeneralSettingsManager.h"

// Widget
#include "HUDFPS.h"
#include "HUDConsumableInfo.h"
#include "HUDDorothy.h"
#include "HUDRoomCleaned.h"
#include "HUDRoomInfo.h"
#include "HUDEvent.h"

void UHUDPlayer::Show() {
  Super::Show();

  if (IsValid(m_pGameInstance)) {
    if (UEventManager* pEventManager = m_pGameInstance->GetEventManager()) {
      oDelegateHandle = pEventManager->OnRevealHUD.AddUObject(this, &UHUDPlayer::HandleRevealElement);
    }

    if (IsValid(m_pHUDConsumableInfoWidget) && m_pGameInstance->m_oHUDData[EHUDSection::CONSUMABLES]) m_pHUDConsumableInfoWidget->Show();
  }

  if (IsValid(m_pHUDFPSWidget) && IsValid(m_pSettingsManager)) {
    if (m_pSettingsManager->GetGeneralSettingsManager()->GetSettings().m_bShowFPS) m_pHUDFPSWidget->Show();
  }
  if (IsValid(m_pHUDEvent)) m_pHUDEvent->Show();
  if (IsValid(m_pHUDRoomInfoWidget)) m_pHUDRoomInfoWidget->Show();
  if (IsValid(m_pHUDRoomCleanedWidget)) m_pHUDRoomCleanedWidget->Show();
  if (IsValid(m_pHUDDorothyWidget)) m_pHUDDorothyWidget->Show();
}

void UHUDPlayer::Hide() {
  Super::Hide();

  if (IsValid(m_pGameInstance)) {
    if (UEventManager* pEventManager = m_pGameInstance->GetEventManager()) {
      pEventManager->OnRevealHUD.Remove(oDelegateHandle);
    }
  }

  if (IsValid(m_pHUDFPSWidget) && IsValid(m_pSettingsManager)) {
    if (m_pSettingsManager->GetGeneralSettingsManager()->GetSettings().m_bShowFPS) m_pHUDFPSWidget->Hide();
  }
  if (IsValid(m_pHUDEvent)) m_pHUDEvent->Hide();
  if (IsValid(m_pHUDRoomInfoWidget)) m_pHUDRoomInfoWidget->Hide();
  if (IsValid(m_pHUDRoomCleanedWidget)) m_pHUDRoomCleanedWidget->Hide();
  if (IsValid(m_pHUDConsumableInfoWidget)) m_pHUDConsumableInfoWidget->Hide();
  if (IsValid(m_pHUDDorothyWidget)) m_pHUDDorothyWidget->Hide();
}

void UHUDPlayer::ShowTutorial() {
  if (IsValid(m_pHUDFPSWidget) && IsValid(m_pSettingsManager)) {
    if (m_pSettingsManager->GetGeneralSettingsManager()->GetSettings().m_bShowFPS) m_pHUDFPSWidget->Show();
  }
  if (IsValid(m_pHUDEvent)) m_pHUDEvent->Show();
  if (IsValid(m_pHUDRoomInfoWidget)) m_pHUDRoomInfoWidget->Show();
  if (IsValid(m_pHUDRoomCleanedWidget)) m_pHUDRoomCleanedWidget->Show();
  if (IsValid(m_pHUDDorothyWidget)) m_pHUDDorothyWidget->Show();
}

void UHUDPlayer::ShowHUB() {
  if (IsValid(m_pGameInstance)) {
    if (UEventManager* pEventManager = m_pGameInstance->GetEventManager()) {
      oDelegateHandle = pEventManager->OnRevealHUD.AddUObject(this, &UHUDPlayer::HandleRevealElement);
    }

    if (IsValid(m_pHUDConsumableInfoWidget) && m_pGameInstance->m_oHUDData[EHUDSection::CONSUMABLES]) m_pHUDConsumableInfoWidget->Show();
  }

  if (IsValid(m_pHUDFPSWidget) && IsValid(m_pSettingsManager)) {
    if (m_pSettingsManager->GetGeneralSettingsManager()->GetSettings().m_bShowFPS) m_pHUDFPSWidget->Show();
  }
  if (IsValid(m_pHUDEvent)) m_pHUDEvent->Show();
  if (IsValid(m_pHUDRoomInfoWidget)) m_pHUDRoomInfoWidget->ShowCalendar();
  if (IsValid(m_pHUDDorothyWidget)) m_pHUDDorothyWidget->Show();
}

void UHUDPlayer::HandleRevealElement(EHUDSection _eHUDSection) {
  switch (_eHUDSection) {
    break; case EHUDSection::CONSUMABLES: RevealConsumables();
    break; case EHUDSection::MINIMAP: RevealMinimap();
    break; case EHUDSection::CLOCK: RevealClock();
    break; case EHUDSection::LIFE: RevealHealth();
    break; case EHUDSection::SPECIAL: RevealWeaponSpecial();
  }

  if (IsValid(m_pGameInstance)) m_pGameInstance->m_oHUDData[_eHUDSection] = true;
}

void UHUDPlayer::RevealConsumables() {
  if (IsValid(m_pHUDConsumableInfoWidget)) m_pHUDConsumableInfoWidget->Show();
}

void UHUDPlayer::RevealMinimap() {
  if (IsValid(m_pHUDRoomInfoWidget)) m_pHUDRoomInfoWidget->ShowMinimap();
}

void UHUDPlayer::RevealClock() {
  if (IsValid(m_pHUDRoomInfoWidget)) m_pHUDRoomInfoWidget->ShowTimer();
}

void UHUDPlayer::RevealHealth() {
  if (IsValid(m_pHUDDorothyWidget)) m_pHUDDorothyWidget->ShowHealth();
}

void UHUDPlayer::RevealWeaponSpecial() {
  if (IsValid(m_pHUDDorothyWidget)) m_pHUDDorothyWidget->ShowSpecial();
}

void UHUDPlayer::RevealEverything() {
  RevealConsumables();
  RevealMinimap();
  RevealClock();
  RevealHealth();
  RevealWeaponSpecial();

  if (IsValid(m_pGameInstance)) {
    for (TPair<EHUDSection, bool>& rSection : m_pGameInstance->m_oHUDData) {
      rSection.Value = true;
    }
  }
}

void UHUDPlayer::NativeConstruct() {
  Super::NativeConstruct();

  m_pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
    // Access the SettingsManager subsystem from the GameInstance.
  if (IsValid(m_pGameInstance)) m_pSettingsManager = m_pGameInstance->GetSubsystem<USettingsManager>();
}