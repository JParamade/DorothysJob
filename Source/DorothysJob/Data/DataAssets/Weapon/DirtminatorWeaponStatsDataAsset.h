// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeaponStatsDataAsset.h"
#include "DirtminatorWeaponStatsDataAsset.generated.h"

class UNiagaraSystem;
class ABaseProjectile;
class UNiagaraComponent;
class UMainLegacyCameraShake;

USTRUCT(BlueprintType)
struct FDirtminatorTier
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Thresholds"))
  float m_fThresholdTime = 0.01f;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Movement Multiplier"))
  float m_fMovementMultiplier = 0.01f;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation Multiplier"))
  float m_fRotationMultiplier = 1.0f;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Projectile"))
  TSubclassOf<ABaseProjectile> m_pProjectilClass;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Material"))
  TObjectPtr<UMaterialInterface> m_pMaterial;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Charge Camera Shake BP"))
  TSubclassOf<UMainLegacyCameraShake> m_oChargeShakeBP;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Release Camera Shake BP"))
  TSubclassOf<UMainLegacyCameraShake> m_oReleaseShakeBP;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Tier 1 VFX"))
  TObjectPtr<UNiagaraSystem> m_pVFX;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Gamepad Charge Vibration Params"))
  FGamePadVibrationParams m_oChargeVibrationParams;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Gamepad Release Vibration Params"))
  FGamePadVibrationParams m_oReleaseVibrationParams;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|VFX", meta = (DisplayName = "Shot VFX"))
  TObjectPtr<UNiagaraSystem> m_pShotVFX;

};

UCLASS()
class DOROTHYSJOB_API UDirtminatorWeaponStatsDataAsset : public UBaseWeaponStatsDataAsset
{
  GENERATED_BODY()

public:
#pragma region Explosion
  /**
   *  Dirtminator Explosion damage.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Explosion", meta = (DisplayName = "Explosion Damage", ClampMin = "0.3", ToolTip = "This value has to be higher than 0"))
  TArray<int> m_oExplosionDamage;

  /**
   *  Dirtminator explosion Tier 2 camera shake.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Explosion", meta = (DisplayName = "Tier 2 Explosion Camera Shake BP"))
  TSubclassOf<UMainLegacyCameraShake> m_oExplosionT2ShakeBP;

  /**
   *  Dirtminator explosion tier 2 controller vibration.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Explosion", meta = (DisplayName = "Tier 2 Explosion Controller Vibration"))
  FGamePadVibrationParams m_oExplosionT2VibrationParams;

  /**
   *  Dirtminator explosion tier 3 camera shake.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Explosion", meta = (DisplayName = "Tier 3 Explosion Camera Shake BP"))
  TSubclassOf<UMainLegacyCameraShake> m_oExplosionT3ShakeBP;

  /**
   *  Dirtminator explosion tier 3 controller vibration.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Explosion", meta = (DisplayName = "Tier 3 Explosion Controller Vibration"))
  FGamePadVibrationParams m_oExplosionT3VibrationParams;
#pragma endregion

#pragma region Attack
  /**
   *  Dirtminator Aim VFX.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Aim VFX"))
  TObjectPtr<UNiagaraSystem> m_pAimVFX;

  /**
   *  The tier array.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Tier Thresholds", ClampMin = "0.1", ToolTip = "This values need to be higher than the previus one"))
  TArray<FDirtminatorTier> m_oTierThresholds;

  /**
   *  Dirtminator max charge time.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Limit Charge Time", ClampMin = "0.3", ToolTip = "This value has to be higher than the Thresholds"))
  float m_fMaxChargeTime = 3.0f;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Attack", meta = (DisplayName = "Atack Offset"))
  FVector m_vAttackOffset;

#pragma endregion

#pragma region Cleaning
  /**
   *  Clean movement multiplicator to make the player go slow.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Movement Multiplier"))
  float m_fCleanMovementMultiplicator;

  /**
   *  Clean movement multiplicator to make the player rotation go slow.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Rotation Multiplier"))
  float m_fCleanRotatorMultiplicator;

  /**
   *  Width clean range in cm.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Width"))
  float m_fCleanAttackWidth;

  /**
   *  Initial clean length in cm.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Initial Length"))
  float m_fMinCleanLength;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Cleaning Offset"))
  FVector m_vCleaningOffset;

  /**
   *  Maximum clean length in cm.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Max Clean Length"))
  float m_fMaxCleanLength;

  /**
   *  Clean damage to Dust Poops each time the clean attack is called.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Damage"))
  int m_iCleanDamage;

  /**
   *  Clean Dirtiminator collision offset for the damage.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Collision Offset"))
  float m_fCleanCollisionOffset;

  /**
   *  Clean Dirtiminator growth length every step.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Step Length"))
  float m_fStepLength = 0.1;

  /**
   *  Clean camera shake Blueprint.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Camera Shake"))
  TSubclassOf<UMainLegacyCameraShake> m_oCleanShakeBP;

  /**
   *  Clean controller vibration params.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Cleaning", meta = (DisplayName = "Clean Controller Vibration"))
  FGamePadVibrationParams m_oCleanVibrationParams;
#pragma endregion

#pragma region SpecialAttack
  /**
   *  Special attack time duration.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Duration"))
  float m_fSpecialAttackTime;

  /**
   *  Special attack distance range.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Distance"))
  float m_fSpecialAttackDistance;

  /**
   *  Special attack cone angle amplitud.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Cone Angle"))
  float m_fSpecialAttackAngle;

  /**
   *  Special attack cone start amplitud.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Cone Start Amplitude"))
  float m_fSpecialAttackStartWidth;

  /**
   *  Clean movement multiplicator to make the player go slow.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Movement Multiplier"))
  float m_fSpecialMovementMultiplicator;

  /**
   *  Clean movement multiplicator to make the player rotation go slow.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Rotation Multiplier"))
  float m_fSpecialRotatorMultiplicator;

  /**
   *  Special attack enemy damage.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Enemy Damage"))
  float m_fSpecialAttackEnemyDamage;

  /**
   *  Special attack Dust damage.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack Dust Damage"))
  float m_fSpecialAttackDustDamage;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Specific Parameters|Special Attack", meta = (DisplayName = "Special Attack VFX"))
  TObjectPtr<UNiagaraSystem> m_pSpecialAttackVFX;
#pragma endregion

#pragma region Audio

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Attack New Charge Audio"))
  TObjectPtr<UFMODEvent> m_pAttackNewChargeSound;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Attack Charging Audio"))
  TObjectPtr<UFMODEvent> m_pAttackChargingSound;

#pragma endregion



};

