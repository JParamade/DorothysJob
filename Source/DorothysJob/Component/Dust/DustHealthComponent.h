// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DustHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathActor);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API UDustHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDustHealthComponent();

  void ModifyHealth(int _modifyHealth);

  UPROPERTY(BlueprintAssignable)
  FOnDeathActor OnDeath;

  void SetCurrentHealth(int _iCurrentHealth);
  void SetMaxHealth(int _iMaxHealth);

  int GetCurrentHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

  UPROPERTY()
  FTimerHandle m_thRegenTimer;

  //Seconds interval in which it is regenerating
  UPROPERTY(EditAnywhere, Category = "Bola13|Health", meta = (DisplayName = "Healing Rate"))
  float m_fHealingRate;

  //Minimum time without damage needed to start to heal itself
  UPROPERTY(EditAnywhere, Category = "Bola13|Health", meta = (DisplayName = "Healing Delay"))
  float m_fHealingDelay;

  UPROPERTY(EditAnywhere, Category = "Bola13|Health", meta = (DisplayName = "Healing Rate"))
  int m_iRegenerateAmount;

private:

  TObjectPtr<AActor> m_pOwner;
  int m_iCurrentHealth;
  int m_iMaxHealth;

  void RegenerateHealth();
};
