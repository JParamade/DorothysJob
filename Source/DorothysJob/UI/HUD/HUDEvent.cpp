/************************************************************************
 * @description: Represents the HUD's event.
 * @author: Jaime Paramo
 * @date: 27/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDEvent.h"

// Animation
#include "Animation/WidgetAnimation.h"

// Component
#include "Components/Image.h"
#include "Components/TextBlock.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Events/EventManager.h"

void UHUDEvent::Show() {
  Super::Show();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UEventManager* pEventManager = pGameInstance->GetEventManager()) {
      if (!pEventManager->OnTemporalEventShow.IsBoundToObject(this)) m_oStartEventHandle = pEventManager->OnTemporalEventShow.AddUObject(this, &UHUDEvent::StartEvent);
      if (!pEventManager->OnTemporalEventClose.IsBoundToObject(this)) m_oEndEventHandle = pEventManager->OnTemporalEventClose.AddUObject(this, &UHUDEvent::StopEvent);
    }
  }
}

void UHUDEvent::Hide() {
  Super::Hide();
}

void UHUDEvent::NativeConstruct() {
  Super::NativeConstruct();

  // Non Focusable Elements
  if (IsValid(m_pEventImage)) m_lElements.Add(m_pEventImage);
  if (IsValid(m_pEventText)) m_lElements.Add(m_pEventText);
}

void UHUDEvent::NativeDestruct() {
  Super::NativeDestruct();

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UEventManager* pEventManager = pGameInstance->GetEventManager()) {
      pEventManager->OnTemporalEventShow.Remove(m_oStartEventHandle);
      pEventManager->OnTemporalEventClose.Remove(m_oEndEventHandle);
    }
  }
}

void UHUDEvent::StartEvent(FText _sText) {
  if (!IsValid(m_pEventText) || !IsValid(m_pEventAnimation)) return;

  m_pEventText->SetText(_sText);

  PlayAnimation(m_pEventAnimation, 0.f, 1, EUMGSequencePlayMode::Forward);
}

void UHUDEvent::StopEvent() {
  if (!IsValid(m_pEventAnimation)) return;

  PlayAnimation(m_pEventAnimation, 0.f, 1, EUMGSequencePlayMode::Reverse);
}