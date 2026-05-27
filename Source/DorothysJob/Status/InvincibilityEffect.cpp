// Fill out your copyright notice in the Description page of Project Settings.


#include "InvincibilityEffect.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"

UInvincibilityEffect::UInvincibilityEffect()
{
  m_fDuration = 10.0f;
}

void UInvincibilityEffect::StatusEffect()
{
  if (IsValid(m_pTarget) && (!m_pTarget->IsInvulnerable()))
  {
    m_pTarget->SetIsInvulnerable(true);
  }
}

void UInvincibilityEffect::RevertEffect()
{
  if (IsValid(m_pTarget))
  {
    m_pTarget->SetIsInvulnerable(false);
  }
}
