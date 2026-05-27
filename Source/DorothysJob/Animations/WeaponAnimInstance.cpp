#include "WeaponAnimInstance.h"

bool UWeaponAnimInstance::PlayActionMontage(FName _sSectionName, float _fPlayRate)
{
  if ((!IsValid(m_pActionMontage)) || (!m_pActionMontage->IsValidSectionName(_sSectionName)))
  {
    return false;
  }

  if (!Montage_IsPlaying(m_pActionMontage))
  {
    Montage_Play(m_pActionMontage, _fPlayRate);
  }

  if (_sSectionName != Montage_GetCurrentSection(m_pActionMontage))
  {
    Montage_JumpToSection(_sSectionName, m_pActionMontage);
  }

  return true;
}

void UWeaponAnimInstance::StopActionMontage()
{
  if (!IsValid(m_pActionMontage))
  {
    return;
  }

  if (Montage_IsPlaying(m_pActionMontage))
  {
    Montage_Stop(0.2f, m_pActionMontage);
  }
}

void UWeaponAnimInstance::SetLeftHandSocketPosition(FVector _vHandSocketPosition)
{
  m_vLeftHandSocket = _vHandSocketPosition;
}

void UWeaponAnimInstance::SetSpeed(float _fForwardSpeed, float _fLateralSpeed)
{
  m_fForwardSpeed = _fForwardSpeed;
  m_fLateralSpeed = _fLateralSpeed;
}