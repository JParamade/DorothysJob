// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeaponComponent.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"

void UBaseWeaponComponent::BeginPlay()
{
  Super::BeginPlay();
}

ABasePlayer* UBaseWeaponComponent::GetPlayerOwner()
{
  if (!IsValid(m_pPlayer))
  {
    m_pPlayer = Cast<ABasePlayer>(GetWorld()->GetPlayerControllerIterator()->Get()->GetPawn());
  }
  return m_pPlayer;
}
