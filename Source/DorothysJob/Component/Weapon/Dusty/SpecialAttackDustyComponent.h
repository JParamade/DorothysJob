// Fill out your copyright notice in the Description page of Project Settings.

/************************************************************************
 * @description: Represents the Dusty Special Attack Component
 *							 This class manage the behavior of the dusty's special attack
 * @author: Lucia Fernandez
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseWeaponComponent.h"
#include "SpecialAttackDustyComponent.generated.h"

/**
 * 
 */

class ADusty;
class UDustyTradicionalHandle;
class UDustyHummingbirdFeathers;
class UDustyWeaponStatsDataAsset;
class USpecialAttackDustyComponent;
class UBaseDash;
class UBoxComponent;
class UNiagaraSystem;
class UBaseStatus;
class USphereComponent;
class UFMODEvent;
class UNiagaraComponent;
class ABasePlayer;
class ABaseWeapon;

UCLASS()
class DOROTHYSJOB_API USpecialAttackDustyComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()
	
	
public:

	USpecialAttackDustyComponent();

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Use() override;


private:

	ADusty* m_pOwner;
	UDustyWeaponStatsDataAsset* m_oStatsDataAsset;

	UFUNCTION(BlueprintCallable)
	void ApplySpecialAttackDamage();

	/**
* @brief Applies the Wave visual effect
*/
	UFUNCTION(BlueprintCallable)
	void CreateWaveVFX();

	UFUNCTION()
	void SetPlayerVelocity(bool _bActivate);

  UFUNCTION()
  void ReleaseSpecialAttack();

	UPROPERTY()
	USphereComponent* m_pCollider;

	UPROPERTY()
	ABasePlayer* pPlayer;

	UPROPERTY()
	ABaseWeapon* pWeapon;

	UPROPERTY()
	FTimerHandle DamageTickHandle;
};
