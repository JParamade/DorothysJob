#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BaseWeaponStatsDataAsset.generated.h"

enum class EDirtType : uint8;
enum class EWeaponHandSocket : uint8;
enum class EWeaponAttackType : uint8;
class UBaseStatus;
class UNiagaraSystem;
class UFMODEvent;
class UMainLegacyCameraShake;

USTRUCT(BlueprintType)
struct FGamePadVibrationParams
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Intensity"))
  float m_fIntensity = 0.01f;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Durantion"))
  float m_fDuration = 0.01f;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Left Large"))
  bool m_bAffectsLeftLarge = true;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Left Samll"))
  bool m_bAffectsLeftSmall = true;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Right Large"))
  bool m_bAffectsRightLarge = true;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Vibration Affects Right Small"))
  bool m_bAffectsRightSmall = true;
};

UCLASS()
class DOROTHYSJOB_API UBaseWeaponStatsDataAsset : public UPrimaryDataAsset
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Cooldowns", meta = (DisplayName = "Attack Cooldown"))
  float m_fAttackCooldown;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Cooldowns", meta = (DisplayName = "Clean Cooldown"))
  float m_fCleanCooldown;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Types", meta = (DisplayName = "Dirt Type"))
  EDirtType m_eWeaponDirtType;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Weapon Attack Type"))
  EWeaponAttackType m_eWeaponAttackType;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Damage", meta = (DisplayName = "Damage"))
  TArray<int> m_oDamage;

  /**
   * @brief Weapon damage camera shake. For the hit of the bullets, the first tier of the dirtminator, de normal hits of the mopressor and the dusty.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|General Camera Shake & Controller Vibration", meta = (DisplayName = "Shoot or Base Damage Camera Shake"))
  TSubclassOf<UMainLegacyCameraShake> m_oDamageCameraShake;

  /**
   * @brief Weapon damage vibration. For the hit of the bullets, the first tier of the dirtminator, de normal hits of the mopressor and the dusty.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|General Camera Shake & Controller Vibration", meta = (DisplayName = "Shoot or Base Damage Controller Vibration"))
  FGamePadVibrationParams m_oDamageVibrationParams;

  /**
   * @brief Weapon special attack camera shake.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|General Camera Shake & Controller Vibration", meta = (DisplayName = "Special Attack Camera Shake"))
  TSubclassOf<UMainLegacyCameraShake> m_oSpecialAttackCameraShake;

  /**
   * @brief Weapon special attack vibration.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|General Camera Shake & Controller Vibration", meta = (DisplayName = "Special Attack Controller Vibration"))
  FGamePadVibrationParams m_oSpecialAttackVibrationParams;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Weapon Special Cost"))
  float m_fMaxUltimateStamina;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Damage Special Charge"))
  float m_fDamageUltimateCharge;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Cleaning Liquid Special Charge"))
  float m_fCleaningLiquidUltimateCharge;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters", meta = (DisplayName = "Cleaning Dust Special Charge"))
  float m_fCleaningDustUltimateCharge;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Animations|Movement", meta = (DisplayName = "Blend Space"))
  TObjectPtr<UBlendSpace> m_pMovementBlendSpace;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Animations|Attack", meta = (DisplayName = "Attack Montage"))
  TObjectPtr<UAnimMontage> m_pAttackMontage;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Status", meta = (DisplayName = "Attack Status"))
  TArray<TSubclassOf<UBaseStatus>> m_oAttackStatus;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Status", meta = (DisplayName = "Clean Status"))
  TArray<TSubclassOf<UBaseStatus>> m_oCleanStatus;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Status", meta = (DisplayName = "Special Status"))
  TArray<TSubclassOf<UBaseStatus>> m_oSpecialStatus;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Attack Audio"))
  TObjectPtr<UFMODEvent> m_pAttackSound;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Clean Audio"))
  TObjectPtr<UFMODEvent> m_pCleanSound;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Special Audio"))
  TObjectPtr<UFMODEvent> m_pSpecialAttackSound;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Switch Audio"))
  TObjectPtr<UFMODEvent> m_pSwitchSound;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Special charge Audio"))
  TObjectPtr<UFMODEvent> m_pSpecialChargeSound;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Base Parameters|Audio", meta = (DisplayName = "Impact Audio"))
  TObjectPtr<UFMODEvent> m_pImpactSound;
};