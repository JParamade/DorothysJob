/************************************************************************
 * @description: Represents the Damageable Interface.
 *							 This class deals damage to Characters.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

enum class EDirtType : uint8;

UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

class DOROTHYSJOB_API IDamageable
{
	GENERATED_BODY()

public:

	/**
	 * @brief It returns the current health.
	 */
	virtual int32 GetCurrentHealth() const = 0;

  /**
   * @brief It returns the character current health.
   * @return Character's Current Health.
   */
  virtual void SetCurrentHealth(int32 _iDamage) = 0;

	/**
	 * @brief It returns the maximum health.
	 */
  virtual int32 GetMaxHealth() const = 0;

  /**
   * @brief It is called when the character receives a damage hit.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   * @param _eAttackType -> Type to calculate the hit effectiveness.
   * @param _vHitLocation -> Hit impact location.
   * @param _vHitRotation -> Hit impact rotation.
   * @param _bIsCritical -> Is Critical hit.
   * @return The real damage after apply bonus and mitigation effects.
   */
  virtual int32 Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation = FVector::ZeroVector, const FRotator& _vHitRotation = FRotator::ZeroRotator, bool _bIsCritical = false) = 0;

  /**
   * @brief It is called when the character is healed.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   */
  virtual void Heal(int32 _iModifyHealth) = 0;
};