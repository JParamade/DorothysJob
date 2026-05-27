#include "BaseStatus.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"


void UBaseStatus::ApplyStatus(ABaseCharacter* _pTarget)
{
  m_pTarget = _pTarget;
  if (m_pTarget)
  {

    if (m_pTarget->GetWorldTimerManager().IsTimerActive(m_oTimeHandler))
    {
      m_pTarget->GetWorldTimerManager().ClearTimer(m_oTimeHandler);
    }

    m_pTarget->GetWorldTimerManager().SetTimer(m_oTimeHandler, this, &UBaseStatus::RemoveStatus, m_fDuration, false);
    StatusEffect();
  }
}

void UBaseStatus::RemoveStatus()
{
  if (m_pTarget)
  {
    m_pTarget->RemoveStatus(GetClass());
    RevertEffect();
  }
  MarkAsGarbage();
}

float UBaseStatus::GetDuration() const
{
    return m_fDuration;
}

bool UBaseStatus::IsReactivable() const
{
    return m_bReactivable;
}

EStatus UBaseStatus::GetStatusType() const
{
  return m_statusType;
}
