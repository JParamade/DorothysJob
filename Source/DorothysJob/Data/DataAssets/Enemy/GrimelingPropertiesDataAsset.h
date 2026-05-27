/************************************************************************
 * @description: It represents the Grimeling properties Data Asset.
 * @author: Jorge Duart Marzo
 * @date: 04/08/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "EnemyPropertiesDataAsset.h"
#include "GrimelingPropertiesDataAsset.generated.h"

UCLASS()
class DOROTHYSJOB_API UGrimelingPropertiesDataAsset : public UEnemyPropertiesDataAsset
{
	GENERATED_BODY()

public:

#pragma region | Flee
  /**
   * Flee distance
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Flee", meta = (DisplayName = "Distance"))
  float m_fFleeDistance;

  /**
   * Flee available angle to check. This angle will create a cone from the player.
   * The initial vector is the vector from the player to the Grimeling.
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
