// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttackComponent.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"

void UBaseAttackComponent::DeactivateAttackFeedback()
{
  ABaseWeapon* pWeapon = Cast<ABaseWeapon>(GetOwner());
  if (IsValid(pWeapon))
  {
    pWeapon->DeactivateFeedback(EMaterialFeedBack::ATTACKING);
  }
}

void UBaseAttackComponent::ActivateAttackFeedback()
{
  ABaseWeapon* pWeapon = Cast<ABaseWeapon>(GetOwner());
  if (IsValid(pWeapon))
  {
    pWeapon->ApplyFeedback(EMaterialFeedBack::ATTACKING, m_fFeedbackAttackTiming);
  }
}

void UBaseAttackComponent::ActivateKillFeedback()
{
  ABaseWeapon* pWeapon = Cast<ABaseWeapon>(GetOwner());
  if (IsValid(pWeapon))
  {
    pWeapon->ApplyFeedback(EMaterialFeedBack::ATTACKING, m_fFeedbackKillTiming);
  }
}
