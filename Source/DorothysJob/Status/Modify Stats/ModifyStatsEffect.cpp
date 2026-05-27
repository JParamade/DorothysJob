/************************************************************************
 * @description: Represents the Modify Stats Effect.
 * @author: Jaime Paramo
 * @date: 28/06/2025
 * @edited_by: Josephine Esposito
 ************************************************************************/

#include "ModifyStatsEffect.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"

UModifyStatsEffect::UModifyStatsEffect()
{
  m_fDuration = 3.0f;
}

void UModifyStatsEffect::StatusEffect()
{
  if (!IsValid(m_pTarget))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid pointer with m_pTarget."));
    return;
  }

  m_pTarget->ModifyStats(m_mStats);
}

void UModifyStatsEffect::RevertEffect()
{
  if (!IsValid(m_pTarget))
  {
    UE_LOG(LogTemp, Error, TEXT("Invalid pointer with m_pTarget."));
    return;
  }

  m_pTarget->RevertStats(m_mStats);
}