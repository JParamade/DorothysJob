/************************************************************************
 * @description: Represents the durable consumable.
 * @author: Jaime Paramo
 * @date: 11/06/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "DurableConsumable.h"

#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "TimerManager.h"

bool UDurableConsumable::Use(ABasePlayer* _pPlayer)
{
  if (IsActive())
  {
    return false;
  }

  if (m_uStackQuantity <= 0) return false;

  if (_pPlayer)
  {
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

void UDurableConsumable::OnEffectStart(ABasePlayer* _pPlayer)
{
}

void UDurableConsumable::OnEffectEnd(ABasePlayer* _pPlayer)
{
  if (UWorld* pWorld = GetWorld())
  {
    pWorld->GetTimerManager().ClearTimer(m_oTimeHandler);
  }
}

bool UDurableConsumable::IsActive() const
{
  if (const UWorld* pWorld = GetWorld())
  {
    return pWorld->GetTimerManager().IsTimerActive(m_oTimeHandler);
  }
  return false;
}
