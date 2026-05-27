#include "BaseAnimInstance.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
  Super::NativeInitializeAnimation();

  m_fSpeed = 0.f;
  m_pMovementBlendSpace = nullptr;
  m_pActionMontage = nullptr;
  m_bComboWindowOpen = false;
  m_bComboActive = false;
  m_sComboSectionName = "";
  m_iCurrentCombo = 0;
}

void UBaseAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
  Super::NativeUpdateAnimation(DeltaSeconds);
}

bool UBaseAnimInstance::PlayActionMontage(FName _sSectionName, float _fPlayRate)
{
  if ((!IsValid(m_pActionMontage)) || (!m_pActionMontage->IsValidSectionName(_sSectionName)))
  {
    return false;
  }

  m_sComboSectionName = _sSectionName;
  if (!Montage_IsPlaying(m_pActionMontage))
  {
    OnComboStateChanges.ExecuteIfBound(true);
    Montage_Play(m_pActionMontage, _fPlayRate);

    FOnMontageEnded MontageEndedDelegate;
    MontageEndedDelegate.BindUObject(this, &UBaseAnimInstance::OnActionMontageEnded);
    Montage_SetEndDelegate(MontageEndedDelegate, m_pActionMontage);
  }

  if (_sSectionName != Montage_GetCurrentSection(m_pActionMontage))
  {
    Montage_JumpToSection(_sSectionName, m_pActionMontage);
  }

  return true;
}

void UBaseAnimInstance::StopActionMontage()
{
  if (!IsValid(m_pActionMontage))
  {
    return;
  }

  if (IsActionMontagePlaying())
  {
    Montage_Stop(0.2f, m_pActionMontage);
  }
  else
  {
    FOnMontageEnded EmptyDelegate;
    Montage_SetEndDelegate(EmptyDelegate, m_pActionMontage);
    OnActionMontageEnded(m_pActionMontage, true);
  }
}

bool UBaseAnimInstance::IsActionMontagePlaying()
{
  return Montage_IsPlaying(m_pActionMontage);
}

void UBaseAnimInstance::OnActionMontageEnded(UAnimMontage* _pMontage, bool _bInterrupted)
{
  OnComboStateChanges.ExecuteIfBound(false);
  OnActionMontageEnds.ExecuteIfBound();
  m_bComboActive = false;
  m_bComboWindowOpen = false;
  m_sComboSectionName = "";
  m_iCurrentCombo = 0;
}

FName UBaseAnimInstance::JumptoNextSection()
{
  FName sNextSectionName;
  if (!IsValid(m_pActionMontage))
  {
    return sNextSectionName;
  }

  sNextSectionName = *(m_sComboSectionName.ToString() + FString::FromInt(m_iCurrentCombo));
  if (m_pActionMontage->IsValidSectionName(sNextSectionName))
  {
    Montage_JumpToSection(sNextSectionName, m_pActionMontage);
  }

  return sNextSectionName;
}

void UBaseAnimInstance::SetComboWindowOpen(bool _bOpen)
{
  m_bComboWindowOpen = _bOpen;

  if (m_bComboWindowOpen)
  {
    return;
  }

  if (m_bComboActive)
  {
    ++m_iCurrentCombo;
    JumptoNextSection();
  }
  m_bComboActive = false;
}

void UBaseAnimInstance::SetComboActive(bool _bActive)
{
  m_bComboActive = m_bComboWindowOpen && _bActive;
}

int32 UBaseAnimInstance::GetCurrentComboIndex()
{
  return m_iCurrentCombo;
}