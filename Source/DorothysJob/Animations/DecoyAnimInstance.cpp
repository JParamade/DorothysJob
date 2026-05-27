#include "DecoyAnimInstance.h"

void UDecoyAnimInstance::PlayInflateAnimation()
{
  if ((!IsValid(m_pActionMontage)) || (!m_pActionMontage->IsValidSectionName("Inflate")))
  {
    return;
  }

  Montage_Play(m_pActionMontage);
  Montage_JumpToSection("Inflate", m_pActionMontage);
}

void UDecoyAnimInstance::PlayHitAnimation()
{
  if ((!IsValid(m_pActionMontage)) || (!m_pActionMontage->IsValidSectionName("Hit")))
  {
    return;
  }

  if (!Montage_IsPlaying(m_pActionMontage))
  {
    Montage_Play(m_pActionMontage);
  }
  Montage_JumpToSection("Hit", m_pActionMontage);
}

void UDecoyAnimInstance::PlayDeathAnimation()
{
  if ((!IsValid(m_pActionMontage)) || (!m_pActionMontage->IsValidSectionName("Death")))
  {
    return;
  }

  Montage_Play(m_pActionMontage);
  Montage_JumpToSection("Death", m_pActionMontage);
}

void UDecoyAnimInstance::SetActionMontage(TObjectPtr<UAnimMontage> _pActionMontage)
{
  m_pActionMontage = _pActionMontage;
}