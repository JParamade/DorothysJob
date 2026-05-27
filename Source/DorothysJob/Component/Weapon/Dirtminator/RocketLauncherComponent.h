/************************************************************************
 * @description: Represents the Rocket Launcher Component of the Dirtminator
 * @author: Pablo Velasco
 * @date: 10/05/2025
 * @edited_by: Aurora Maria Fernandez - 21/08/25
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "../BaseAttackComponent.h"
#include "RocketLauncherComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChargeChangeSignature, int, _charge);

class ADirtminator;
class UDirtminatorWeaponStatsDataAsset;
class UNiagaraSystem;
class ABaseProjectile;

/**
 * @brief Component of the dirminator in charge of the attack.
 */
UCLASS()
class DOROTHYSJOB_API URocketLauncherComponent : public UBaseAttackComponent
{
  GENERATED_BODY()

public:

#pragma region Events
  FOnChargeChangeSignature OnChargeChange;
#pragma endregion

protected:

  /**
   * @brief Function that happends when the game starts.
   */
  virtual void BeginPlay() override;

public:

  /**
   * @brief Use the dirtminator attack
   */
  virtual void Use() override;

  /**
   * @brief What happends when the attack is release
   */
  virtual void Release() override;

  void SetCooldownMultiplier(float _fCooldownMultiplier);

private:

#pragma region General Variables  
  /** The owner of this component. */
  UPROPERTY()
  TWeakObjectPtr<ADirtminator> m_pOwnerWeapon;

  UPROPERTY()
  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

  /** The data asset of the dirtminator. */
  UPROPERTY()
  TObjectPtr<UDirtminatorWeaponStatsDataAsset> m_pDataAsset;

  /** The skeletal mesh of the Dirtminator. */
  UPROPERTY()
  TObjectPtr<USkeletalMeshComponent> m_pOwnerSkeletalMesh;

  /** The default material of the dirtminator. */
  UPROPERTY()
  TObjectPtr<UMaterialInterface> m_pDefaultOwnerMaterial;

  /** Params for the projectile. */
  FActorSpawnParameters m_oSpawnParams;

  /** The time the charge has been on. */
  float m_fChargeTime = 0.0f;

  float m_fCooldownMultiplier = 1.0f;

  /** The cuurent tier the charge is in. */
  int m_iCurrentTier = -1;

  /** the last time that passm this is for the current time calculation. */
  double m_dLastTime = -1.0f;

  /** If the Dirtminatos has shot a bullet. */
  bool m_bHasShot = false;

  /** If the dirtminator is going slow. */
  bool m_bIsSlow = false;
#pragma endregion

#pragma region Functions
  /**
   * @brief Check if it's the first time the dirtminator attack button is press.
   */
  bool IsFirstUse();

  /**
   * @brief Update the charge time base on the delta time of the game.
   */
  void UpdateChargeTime();

  /**
   * @brief Check if it's a different tier.
   */
  void CheckTierChange();

  /**
   * @brief Manage the diferents tiers only if the tier has change.
   */
  void ManageChargeTiers();

  /**
   * @brief Shoot and spawn the projectile.
   * @param _ProjectileClass The projectile that is going to be shoot.
   */
  void Shoot(TSubclassOf<ABaseProjectile> _ProjectileClass);

  /**
 * @brief Check if it has reach the charging maxtime permited.
 */
  void CheckMaxChargeAndShoot();

  /**
   * @brief Set the player velocity.
   */
  void SetPlayerVelocity(bool _bActivate);
#pragma endregion

#pragma region VFX & Material Function
  /**
 * @brief Set the dirtminator vfx and material needed.
 */
  void SetDirtminatorTierVisualEffect(int32 _tier);

  /**
   * @brief Set the dirminator charge vfx.
   * @param _pVFX The new vfx to use.
   */
  void SetOwnerVFX(UNiagaraSystem* _pVFX);

  /**
   * @brief Set the dirminator charge material.
   * @param NewMaterial The new material to use.
   */
  void SetOwnerMaterial(UMaterialInterface* _pNewMaterial);
#pragma endregion

#pragma region Sound Functions
  /**
   * @brief Play the charging sound.
   */
  void PlayChargeSound();
#pragma endregion

  
};
