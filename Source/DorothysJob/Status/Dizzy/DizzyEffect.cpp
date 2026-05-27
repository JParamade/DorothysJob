#include "DizzyEffect.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DorothysJob/Status/Stun/StunEffect.h"
#include "DorothysJob/Status/Slow/SlowEffect.h"

UDizzyEffect::UDizzyEffect()
{
  m_statusType = EStatus::DIZZY;
  m_fDuration = 5.0f;
  m_bReactivable = true;
}

int UDizzyEffect::GetStacks() const
{
  return m_iStacks;
}

void UDizzyEffect::StatusEffect()
{
  m_iStacks++;

  switch (m_iStacks)
  {
  case 1:
    m_pTarget->ApplyStatus(_SlowEffectE1);
    break;
  case 2:
    m_pTarget->ApplyStatus(_SlowEffectE2);
    break;
  default:
    m_pTarget->ApplyStatus(_StunEffect);
    RevertEffect();
    break;
  }
}

void UDizzyEffect::RevertEffect()
{
  m_iStacks = 0;
  m_pTarget->RemoveStatus(this->StaticClass());
}
