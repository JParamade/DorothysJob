// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponStatsDataAsset.h"
#include "MoppresorWeaponStatsDataAsset.generated.h"

class UFMODEvent;

USTRUCT(BlueprintType)
struct FCombo
{

  GENERATED_BODY()


  UPROPERTY(EditAnywhere, meta = (DisplayName = "Start Angle"))
  FRotator m_rStartAngle = FRotator::ZeroRotator;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "End Angle"))
  FRotator m_rEndAngle = FRotator::ZeroRotator;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation Speed"))
  float m_fRotationSpeed = 0.f;

  /**
   * @brief Attack collider spawn distance
   */
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Basic Attack Spawn Distance"))
  float m_fBasicAttackSpawnDistance = 0.0f;

  /**
   * @brief Attack VFX spawn distance
   */
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Attack VFX Spawn Distance"))
  FVector m_fBasicVFXSpawnDistance = FVector::ZeroVector;

  /**
   * @brief The array of visual effects for the basic attacks
   */
  UPROPERTY(EditDefaultsOnly, meta = (DisplayName = "Combo Hit VFX"))
  TObjectPtr<UNiagaraSystem> m_lBasicVFXAttackEffect;
};

UCLASS()
class DOROTHYSJOB_API UMoppresorWeaponStatsDataAsset : public UBaseWeaponStatsDataAsset
{
  GENERATED_BODY()

public:
  
  /**
   * @brief The total charge timer for the whirl. This sets also the max timer to hold the action
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Max Whirl Charge"))
  float m_fWhirlCharge;

  /**
   * @brief Rate of increase in whirl charge
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Whirl Charge Rate"))
  float m_fWhirlChargeRate;

  /**
   * @brief The range of the wave that damages enemies on impact
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Wave Damage Range Minimum"))
  float m_fWaveDamageRangeMin;

  /**
   * @brief The range of the wave that damages enemies on impact
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Wave Damage Range Maximum"))
  float m_fWaveDamageRangeMax;

  /**
   * @brief The range of the wave that cleans the ground on impact
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Wave Clean Range Minimum"))
  float m_fWaveCleanRangeMin;

  /**
   * @brief The range of the wave that cleans the ground on impact
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Wave Clean Range Maximum"))
  float m_fWaveCleanRangeMax;

  /**
   * @brief Handles the Special Attack Damage
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Damage"))
  float m_fSpecialAttackDamage;  
  
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Growth Rate"))
  float m_fSpecialAttackGrowthRate;

  /**
   * @brief The stun effect to apply with the special attack
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Status"))
  TSubclassOf<UBaseStatus> m_pStunEffect;

  /**
   * @brief The special wave VFX Visual effects
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Wave VFX"))
  UNiagaraSystem* m_pWaveEffect;

  /**
   * @brief The special charge VFX Visual effects
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Charge VFX"))
  UNiagaraSystem* m_pSpecialChargeEffect;


  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Combo Hits Paremeters"))
  TArray<FCombo> m_lComboHitsParameters;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Movement Attack Multiplier", ClampMin = "0.1"))
  float m_fMovementAttackMultiplier;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Rotation Attack Multiplier", ClampMin = "0.01"))
  float m_fRotationAttackMultiplier;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Hit Stop Time", ClampMin = "0.01"))
  float m_fFreezeFrameTime;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Hit Stop Cooldown Time", ClampMin = "0.01"))
  float m_fCooldownFreezeFrameTime;

  /**
   *  Final attack Camera Shake Blueprint.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Final Attack", meta = (DisplayName = "Final Moppresor Camera Shake"))
  TSubclassOf<UMainLegacyCameraShake> m_oFinalAttackCameraShake;

  /**
   *  Final attack Controller Vibration.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Final Attack", meta = (DisplayName = "Final Moppresor Controller Vibration"))
  FGamePadVibrationParams m_oFinalAttackVibration;

  /**
   * @brief Texture for the liquid cleaning
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Texture"))
  TObjectPtr<UTexture> m_pCleanTexture;

  /**
   * @brief Pivot for the liquid cleaning
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Pivot"))
  FVector2D m_v2SCleaningPivot;

  /**
   * @brief Liquid cleaning size
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Size"))
  FVector2D m_v2SCleaningSize;

  /**
   * @brief Movement multiplier aplicated while cleaning.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Movement Multiplier while cleaning"))
  float m_fMovementCleaningMultiplier;

  /**
   * @brief The visual effect for the Clean action
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Visual Effect"))
  TObjectPtr<UNiagaraSystem> m_pCleanEffect;

  /**
   * @brief The attack event audio
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Audio", meta = (DisplayName = "Final Attack FMOD Event"))
  TObjectPtr<UFMODEvent> m_pFinalAttackEvent;
};
