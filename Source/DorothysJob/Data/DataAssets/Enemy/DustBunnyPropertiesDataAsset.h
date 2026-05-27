/************************************************************************
 * @description: It represents the Dust Bunny properties Data Asset.
 * @author: David Gonzalez
 * @date: 22/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "EnemyPropertiesDataAsset.h"
#include "DustBunnyPropertiesDataAsset.generated.h"

class ABaseProjectile;
class ADustPoop;

UCLASS()
class DOROTHYSJOB_API UDustBunnyPropertiesDataAsset : public UEnemyPropertiesDataAsset
{
	GENERATED_BODY()
	
public:

#pragma region | Attack
  /**
   * Projectile class used to attack.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Attack", meta = (DisplayName = "Projectile Class"))
  TSubclassOf<ABaseProjectile> m_oProjectileClass;

  /**
   * It represents the angle formed by the three projectiles when attack for the Ash Bunny.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Projectile Angle"))
  float m_fProjectileAngle;
#pragma endregion

#pragma region | Defense Tower Mode
  /**
   * It enables/disables the defense tower mode in dust poops.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Enable Tower Mode"))
  bool m_bEnableTowerMode;

  /**
   * It will be used when the when the dust poop where the Dust Bunny was hidden is removed.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "UnHide Effect"))
  TSubclassOf<UBaseStatus> m_oUnHideEffectClass;
#pragma endregion

#pragma region | Dirt
  /**
   * Dust poop class used.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Dirt", meta = (DisplayName = "Dust poop Class"))
  TSubclassOf<ADustPoop> m_oPoopClass;

  /**
   * It represents the cooldown to dirt.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Cooldown"))
  float m_fDirtCooldown;
#pragma endregion

#pragma region | Flee
  /**
   * Flee distance
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Flee", meta = (DisplayName = "Distance"))
  float m_fFleeDistance;

  /**
   * Flee available angle to check. This angle will create a cone from the player.
   * The initial vector is the vector from the player to the Dust Bunny.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Flee", meta = (DisplayName = "Angle"))
  float m_fFleeAngle;

  /**
   * Flee available angle to check. This angle will create a cone to the player.
   * The initial vector is the vector direct to the player.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Flee", meta = (DisplayName = "Obstacle Bounce Angle"))
  float m_fFleeBounceAngle;

  /**
   * Flee not available angle to check. This angle will create a cone to the player.
   * The initial vector is the vector direct to the player.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Flee", meta = (DisplayName = "Forbidden Bounce Angle"))
  float m_fFleeForbiddenBounceAngle;

  /**
   * The Dust Bunny can't speed up until the colldown reaches 0.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Flee", meta = (DisplayName = "Speed Up Cooldown"))
  float m_fSpeedUpCooldown;
#pragma endregion
};
