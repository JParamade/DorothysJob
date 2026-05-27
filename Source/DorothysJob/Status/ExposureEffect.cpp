#include "ExposureEffect.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"

UExposureEffect::UExposureEffect()
{
  m_statusType = EStatus::EXPOSURE;
  m_fDuration = 10.0f;
}

void UExposureEffect::StatusEffect()
{
  if (m_pTarget)
  {
    m_fOriginalBonuses = m_pTarget->GetBonusDamageMultiplier();

    m_pTarget->SetBonusDamageMultiplier(m_fExposure);
  }
}

void UExposureEffect::RevertEffect()
{
  if (m_pTarget)
  {
    m_pTarget->SetBonusDamageMultiplier(m_fOriginalBonuses);
  }
}