#include "SlowEffect.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"

USlowEffect::USlowEffect()
{
  m_statusType = EStatus::SLOW;
  m_fDuration = 2.0f;

}

void USlowEffect::StatusEffect()
{
  m_pMovementComponent = Cast<UFloatingPawnMovement>(m_pTarget->GetMovementComponent());

  if (m_pMovementComponent)
  {
    m_pMovementComponent->MaxSpeed *= m_fSlowFactor;
  }
}

void USlowEffect::RevertEffect()
{
  if (m_pMovementComponent)
  {
    m_pMovementComponent->MaxSpeed /= m_fSlowFactor;
  }
}
