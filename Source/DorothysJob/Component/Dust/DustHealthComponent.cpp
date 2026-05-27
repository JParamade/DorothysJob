// Fill out your copyright notice in the Description page of Project Settings.


#include "DustHealthComponent.h"
#include "DorothysJob/Interface/Damageable.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"

// FMOD
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"

// Sets default values for this component's properties
UDustHealthComponent::UDustHealthComponent()
{
  m_fHealingDelay = 3.f;
  m_fHealingRate = 0.5f;
  m_iRegenerateAmount = 1;
}


// Called when the game starts
void UDustHealthComponent::BeginPlay()
{
	Super::BeginPlay();
  m_pOwner = GetOwner();
}

void UDustHealthComponent::ModifyHealth(int _modifyHealth)
{
  if (_modifyHealth == 0)
  {
    UE_LOG(LogTemp, Error, TEXT("Tried to modify health with value 0"));
    return;
  }

  m_iCurrentHealth = FMath::Clamp(m_iCurrentHealth + _modifyHealth, 0, m_iMaxHealth);

  if (_modifyHealth < 0)
  {
    GetWorld()->GetTimerManager().ClearTimer(m_thRegenTimer);
    GetWorld()->GetTimerManager().SetTimer(m_thRegenTimer, this, &UDustHealthComponent::RegenerateHealth,
      m_fHealingRate, true, m_fHealingDelay);
  }
 
  UE_LOG(LogTemp, Warning, TEXT("Current Health: %d"), m_iCurrentHealth);


  if (m_iCurrentHealth == 0)
  {
    FMOD::Studio::EventInstance* pEvent = nullptr;
    FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
    if (pStudioSystem) {
      FMOD::Studio::EventDescription* pEventDesc = nullptr;
      pStudioSystem->getEvent("event:/SoundFX/Characters/Enemies/DustBunnies/SFX_ENE_dustBunny_dustSpeckDeath", &pEventDesc);

      if (pEventDesc) pEventDesc->createInstance(&pEvent);
      if (pEvent) pEvent->start();
    }

    UE_LOG(LogTemp, Warning, TEXT("DEAD"));
    GetWorld()->GetTimerManager().ClearTimer(m_thRegenTimer);
    OnDeath.Broadcast();
  }
}

void UDustHealthComponent::SetCurrentHealth(int _iCurrentHealth)
{
  m_iCurrentHealth = _iCurrentHealth;
}

void UDustHealthComponent::SetMaxHealth(int _iMaxHealth)
{
  m_iMaxHealth = _iMaxHealth;
}

int UDustHealthComponent::GetCurrentHealth()
{
  return m_iCurrentHealth;
}

void UDustHealthComponent::RegenerateHealth()
{
  if (m_pOwner && m_pOwner->Implements<UDamageable>())
  {
      IDamageable* damageableActor = Cast<IDamageable>(m_pOwner);
      if (damageableActor)
      {
        UE_LOG(LogTemp, Warning, TEXT("regenerate"));
        damageableActor->Heal(m_iRegenerateAmount);
        m_iCurrentHealth = damageableActor->GetCurrentHealth();
      }
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("Owner is null or does not implement IDamageable"));
  }

  if (m_iCurrentHealth == m_iMaxHealth)
  {
    GetWorld()->GetTimerManager().ClearTimer(m_thRegenTimer);
  }
}