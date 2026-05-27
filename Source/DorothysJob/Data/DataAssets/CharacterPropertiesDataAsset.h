/************************************************************************
 * @description: It represents the Base character properties Data Asset.
 * @author: David Gonzalez
 * @date: 30/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterPropertiesDataAsset.generated.h"

class UBaseAnimInstance;
class UNiagaraSystem;
class UBaseStatus;
class UModifyStatsEffect;
class UFMODEvent;
enum class EDirtType : uint8;

UCLASS()
class DOROTHYSJOB_API UCharacterPropertiesDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

#pragma region | Stats
  /**
   * Max health for the character.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Stats", meta = (DisplayName = "Max Health", ClampMin = "0"))
  int m_iMaxHealth;

  /**
   * It is the character dirt type.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Stats", meta = (DisplayName = "Dirt Type"))
  EDirtType m_eDirtType;
#pragma endregion

#pragma region | Invulnerability
  /**
   * It represents the invulnerability time after damage received.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Stats", meta = (DisplayName = "Invulnerable Time"))
  float m_fInvulnerableTime;
#pragma endregion

#pragma region | Skin
  /**
   * Default skin for the Base Character.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Skins", meta = (DisplayName = "Base Skin"))
  TArray<TObjectPtr<UMaterialInterface>> m_lBaseMaterial;

  /**
   * It will be used when Base Character is damaged.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Skins", meta = (DisplayName = "Neutral Damaged Skin"))
  TArray<TObjectPtr<UMaterialInterface>> m_lDamagedMaterial;
  
  /**
   * It will be used when Base Character is damaged with a critical hit.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Skins", meta = (DisplayName = "Critical Damaged Skin"))
  TArray<TObjectPtr<UMaterialInterface>> m_lCriticalHitMaterial;

  /**
   * It will be used when Base Character is damaged with a reduced hit.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Skins", meta = (DisplayName = "Reduced Damaged Skin"))
  TArray<TObjectPtr<UMaterialInterface>> m_lReducedHitMaterial;

  /**
   * It will be used when Base Character is in buff stats material.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Skins", meta = (DisplayName = "Buff Stats Material Start"))
  TObjectPtr<UMaterialInterface> m_pBuffMaterialStart;

  /**
   * It will be used when Base Character is in buff stats material.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Skins", meta = (DisplayName = "Buff Stats Material Applied"))
  TObjectPtr<UMaterialInterface> m_pBuffMaterialApplied;
#pragma endregion

#pragma region | Animation
  /**
   * It's the Base Character animation blueprint.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Animations", meta = (DisplayName = "Blueprint"))
  TSubclassOf<UBaseAnimInstance> m_oAnimBlueprintClass;
#pragma endregion

#pragma region | VFX
  /**
   * It will be used when Base Character is damaged.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Impact VFX"))
  TObjectPtr<UNiagaraSystem> m_pDamagedEffect;

  /**
   * It will be used when Base Character dies.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Dead VFX"))
  TObjectPtr<UNiagaraSystem> m_pDeadEffect;
#pragma endregion

#pragma region | Status Effect
  /**
   * It will be used when Base Character has a speed up status effect.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Status Effect", meta = (DisplayName = "Speed Up"))
  TSubclassOf<UBaseStatus> m_oStatusEffectClass;

  /**
   * It is the effect that could be applied by the Grimeling.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Status Effect", meta = (DisplayName = "Polluted Effect"))
  TSubclassOf<UModifyStatsEffect> m_pPollutedStatus;

#pragma endregion

#pragma region | Audio
  /**
   * It is the min loop time to play the idle sound in the enemy.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Custom Pitch Tag"))
  FName m_sCustomPitchTag;

  /**
   * It is the max loop time to play the idle sound in the enemy.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Custom Pitch Value"))
  float m_fCustomPitchValue;

  /**
   * It will be executed when the Base Character is damaged.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Damage"))
  TObjectPtr<UFMODEvent> m_pDamageEvent;

  /**
   * It will be executed when the Base Character dies.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Death"))
  TObjectPtr<UFMODEvent> m_pDeathEvent;
#pragma endregion
};
