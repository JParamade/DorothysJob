// Fill out your copyright notice in the Description page of Project Settings.
#include "Trigger.h"
#include "DorothysJob/Manager/BaseGameInstance.h"

FOnTriggerEnterSignature ATrigger::OnTriggerEnter;

void ATrigger::ActivateTrigger()
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  int32 iCurrentLevel = pGameInstance->m_iCurrentLevel;

  if (m_iCurrentLevel != iCurrentLevel/* && m_iCurrentLevel != 100*/)
  {
    return;
  }

  if (!m_bIsActive)
  {
    return;
  }

  m_bIsActive = false;
  OnTriggerEnter.Broadcast(m_sTriggerId);
}

void ATrigger::ResetTrigger()
{
  m_bIsActive = true;
}
