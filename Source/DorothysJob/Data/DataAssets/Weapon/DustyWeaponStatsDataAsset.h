// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Camera/MainLegacyCameraShake.h"
#include "DustyWeaponStatsDataAsset.generated.h"


/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UDustyWeaponStatsDataAsset : public UBaseWeaponStatsDataAsset
{
	GENERATED_BODY()
	

public:

#pragma region | Clean
	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Clean", meta = (DisplayName = "Clean Damage"))
	int m_iCleanDamage;
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Clean", meta = (DisplayName = "Clean Multiplicator Velocity"))
	float  m_iVelocityClean;
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Clean", meta = (DisplayName = "Clean Multiplicator Rotation"))
	float m_iVelocityRotationClean;

#pragma endregion
#pragma region | Normal Attack
	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Mini Attacks Collider Size"))
	FVector m_vMiniAttackColliderSize;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Combo Dashes Distance"))
	float m_fDistanceComboDash;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Combo Dash Duration"))
	float m_fDurationComboDash;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Base Hit Offset"))
	FVector m_vBaseHitOffset;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Punch Hit Offset"))
	FVector m_vPunchHitOffset;

	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Mega Attack Collider Size"))
	FVector m_vMegaAttackColliderSize;

	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Mega Fist Attack Duration"))
	float m_fFistAttackDuration;

	/**
		* @brief Fist attack dash distance
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Fist Dash Distance"))
	float m_fFistDashDistance;

	/**
		* @brief Fist attack dash time
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Fist Dash Time"))
	float m_fFistDashTime;

	/**
	 *  Final attack Camera Shake Blueprint.
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Final Attack", meta = (DisplayName = "Final Fist Camera Shake"))
	TSubclassOf<UMainLegacyCameraShake> m_oFinalAttackCameraShake;

	/**
	 *  Final attack Controller Vibration.
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Final Attack", meta = (DisplayName = "Final Fist Controller Vibration"))
	FGamePadVibrationParams m_oFinalAttackVibration;
#pragma endregion

#pragma region | Special Attack
	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Cooldown"))
	float m_fCooldownSpecialAttack;

	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Duration"))
	float m_fSpecialAttackDuration;

	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Velocity Multiplier"))
	float m_fMovementMultiplier;

	/**
		* @brief Handles the Special Attack Damage
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Damage Enemy"))
	float m_fSpecialAttackEnemyDamage;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Damage Dust Poop"))
	float m_fSpecialAttackDastPoopDamage;

	/**
		* @brief The range of the wave that damages enemies on impact
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Wave Damage Range"))
	float m_fWaveDamageRange;

	/**
		* @brief The invincibility effect to apply during the special attack
		*/
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Invincibility Status"))
	TSubclassOf<UBaseStatus> m_pSpecialInvincibilityEffect;
#pragma endregion

	/**
   * @brief The attack event audio
   */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Audio", meta = (DisplayName = "Final Attack FMOD Event"))
	TObjectPtr<UFMODEvent> m_pFinalAttackEvent;

#pragma region | VFX
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|VFX", meta = (DisplayName = "Clean VFX"))
	TObjectPtr<UNiagaraSystem> m_pCleanEffect;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|VFX", meta = (DisplayName = "Clean VFX Circle"))
	TObjectPtr<UNiagaraSystem> m_pCleanEffectCircle;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|VFX", meta = (DisplayName = "Combo Initial Attack VFX"))
	TObjectPtr<UNiagaraSystem> m_pAttackComboInitialEffect;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|VFX", meta = (DisplayName = "Combo End Attack VFX"))
	TObjectPtr<UNiagaraSystem> m_pAttackComboEndEffect;

	/**
	* @brief The VFX Visual effects
	*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Bola 13|Specific Parameters|VFX", meta = (DisplayName = "Special Attack VFX"))
	UNiagaraSystem* m_pWaveEffect;

#pragma endregion

};
