#include "StunEffect.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"

UStunEffect::UStunEffect()
{
  m_statusType = EStatus::STUN;
  m_fDuration = 5.0f;

}

void UStunEffect::StatusEffect()
{
   // TODO: THIS ONLY MAKES THE CHARACTERS NOT TO MOVE, WE NEED THEM NOT TO USE ACTIONS AS WELL
  //m_pMovementComponent = Cast<UFloatingPawnMovement>(m_pTarget->GetMovementComponent());

 /* if (m_pMovementComponent)
  {
    m_fOriginalSpeed = m_pMovementComponent->MaxSpeed;
    m_pMovementComponent->MaxSpeed = 0;
  }*/
  if (m_pTarget && m_pTarget->CanDoActions())
  {
    m_pTarget->SetCanDoActions(false);
  }
}

void UStunEffect::RevertEffect()
{
  // TODO: THIS ONLY MAKES THE CHARACTERS TO MOVE AGAIN, WE NEED THEM TO USE ACTIONS AS WELL

 /* if (m_pMovementComponent)
  {
    m_pMovementComponent->MaxSpeed = m_fOriginalSpeed;
  }*/
  if (m_pTarget && !m_pTarget->CanDoActions())
  {
    m_pTarget->SetCanDoActions(true);
  }
}
