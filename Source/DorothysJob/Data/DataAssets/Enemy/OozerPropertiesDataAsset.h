/************************************************************************
 * @description: It represents the Oozer properties Data Asset.
 * @author: David Gonzalez
 * @date: 22/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "EnemyPropertiesDataAsset.h"
#include "OozerPropertiesDataAsset.generated.h"

class ASlimeEnemy;
enum class EDrawColor : uint8;

UCLASS()
class DOROTHYSJOB_API UOozerPropertiesDataAsset : public UEnemyPropertiesDataAsset
{
	GENERATED_BODY()
	
public:
#pragma region | Attack
  /**
   * It is the movement speed when the Oozer attacks.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Dash Speed"))
  float m_fDashSpeed;
#pragma endregion

#pragma region | Dirt
  /**
   * It represents the cooldown to dirt.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Cooldown"))
  float m_fDirtCooldown;

  /**
   * It represents the cooldown to dirt when the Oozer is Dashing.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dashing Cooldown"))
  float m_fDashingDirtCooldown;

  /**
   * It represents the step rotation angle for the dirt texture.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Step rotation angle"))
  float m_fStepRotationAngle;

  /**
   * Dirt Size.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Size"))
  FVector2D m_vDirtSize;

  /**
   * Dirt texture.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Texture"))
  TObjectPtr<UTexture2D> m_pDirtTexture;

  /**
   * Clean texture.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Clean Texture"))
  TObjectPtr<UTexture2D> m_pCleanTexture;

  /**
   * Draw color.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Draw Color"));
  EDrawColor m_eDrawColor;
#pragma endregion

#pragma region | Spawn
  /**
   * Child slime class.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Spawn", meta = (DisplayName = "Child Slime Class"))
  TSubclassOf<ASlimeEnemy> m_oChildSlimeClass;

  /**
   * Children number.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Spawn", meta = (DisplayName = "Children Number", ClampMin = "0"))
  int m_iChildrenNumber;
#pragma endregion

#pragma region VFX
  /**
   * It will be used when Oozer is walking.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Walking VFX"))
  TObjectPtr<UNiagaraSystem> m_pWanderEffect;
#pragma endregion

#pragma region | Audio
  /**
   * It will be executed when the Base Character walks.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Walk"))
  TObjectPtr<UFMODEvent> m_pWalkEvent;
#pragma endregion
};
