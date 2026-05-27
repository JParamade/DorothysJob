/************************************************************************
 * @description: Represents the durable consumable.
 * @author: Jaime Paramo
 * @date: 11/06/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "DurableConsumable.h"

#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "TimerManager.h"

bool UDurableConsumable::Use(ABasePlayer* _pPlayer) {
  // If the consumable is already active, it cannot be used again until the effect ends.
  if (IsActive()) return false;

  // If there are no consumables left in the stack, it cannot be used.
  if (m_uStackQuantity <= 0) return false;

  // Apply the effect and start the timer for the effect duration.
  if (_pPlayer) {
    OnEffectStart(_pPlayer);

    if (UWorld* pWorld = GetWorld())
    {
      // If the consumable is already active, override duration.
      if (pWorld->GetTimerManager().IsTimerActive(m_oTimeHandler)) pWorld->GetTimerManager().ClearTimer(m_oTimeHandler);

      FTimerDelegate oTimerDelegate;
      oTimerDelegate.BindUObject(this, &UDurableConsumable::OnEffectEnd, _pPlayer);

      pWorld->GetTimerManager().SetTimer(m_oTimeHandler, oTimerDelegate, m_fEffectDuration, false);
    }
  }

  return Super::Use(_pPlayer);
}

void UDurableConsumable::OnEffectStart(ABasePlayer* _pPlayer) {}

void UDurableConsumable::OnEffectEnd(ABasePlayer* _pPlayer) {
  if (UWorld* pWorld = GetWorld()) {
    // Clear the timer to ensure the effect can be used again.
    pWorld->GetTimerManager().ClearTimer(m_oTimeHandler);
  }
}

bool UDurableConsumable::IsActive() const {
  // Return true if the timer for the effect is currently active, indicating that the consumable's effect is still in place.
  if (const UWorld* pWorld = GetWorld()) {
    return pWorld->GetTimerManager().IsTimerActive(m_oTimeHandler);
  }

  return false;
}
