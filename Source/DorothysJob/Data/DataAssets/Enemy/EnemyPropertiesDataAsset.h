/************************************************************************
 * @description: It represents the Enemy properties Data Asset.
 * @author: David Gonzalez
 * @date: 21/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Data/DataAssets/CharacterPropertiesDataAsset.h"
#include "EnemyPropertiesDataAsset.generated.h"

class UBehaviorTree;
class UWidgetComponent;
class UFMODEvent;

UCLASS()
class DOROTHYSJOB_API UEnemyPropertiesDataAsset : public UCharacterPropertiesDataAsset
{
  GENERATED_BODY()

public:

#pragma region | AI
  /**
   * Behavior tree for the enemy.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|AI", meta = (DisplayName = "Behavior Tree"))
  TObjectPtr<UBehaviorTree> m_pBehaviorTree;
#pragma endregion

#pragma region | Movement
  /**
   * Arrival radius.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Arrival Radius"))
  float m_fArrivalRadius;

  /**
   * Arrival rotation radius.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Arrival rotation Radius"))
  float m_fAcceptableRadius;

  /**
   * Interpolation movement speed.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Inter. Mov. Speed"))
  float m_fMoveInterpSpeed;

  /**
   * Interpolation rotation speed.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Inter. Rot. Speed"))
  float m_fRotationInterpSpeed;

  /**
   * Wander available angle to check. This angle will create a cone around the enemy forward vector.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Wander", meta = (DisplayName = "Angle"))
  float m_fWanderAngle;

  /**
   * Wander bounce available angle to check. This angle will create a cone around the enemy backward vector.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Wander", meta = (DisplayName = "Obstacle Bounce Angle"))
  float m_fWanderBounceAngle;

  /**
   * Max speed in wander state.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Wander", meta = (DisplayName = "Max Speed"))
  float m_fMaxWanderSpeed;

  /**
   * Min radius to find the target point.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Wander", meta = (DisplayName = "Min Radius"))
  float m_fWanderMinRadius;

  /**
   * Max radius to find the target point.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Wander", meta = (DisplayName = "Max Radius"))
  float m_fWanderMaxRadius;
#pragma endregion

#pragma region | Brave
  /**
   * When the enemy performs brave actions.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Brave", meta = (DisplayName = "Duration"))
  float m_fBraveDuration;
  /**
   * Interpolation rotation speed when the enemy is in brave mode.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Brave", meta = (DisplayName = "Inter. Rot. Speed"))
  float m_fBraveRotInterpSpeed;
#pragma endregion

#pragma region | Attack
  /**
   * Attack Damage to the Player.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Damage"))
  int32 m_iDamage;

  /**
   * Radius to detect to the player.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Detect Radius"))
  float m_fDetectRadius;

  /**
   * Radius to danger from the player.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Danger Radius"))
  float m_fDangerRadius;

  /**
   * It represents the cooldown to attack.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Cooldown"))
  float m_fAttackCooldown;
#pragma endregion

#pragma region | Animations
  /**
   * Montage to manage attack and dirt actions.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Animations|Action", meta = (DisplayName = "Action Montage"))
  TObjectPtr<UAnimMontage> m_pActionMontage;
#pragma endregion

#pragma region VFX
  /**
   * It will be used when enemy attacks.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Attack VFX"))
  TObjectPtr<UNiagaraSystem> m_pAttackEffect;

  /**
   * It will be used when enemy is stunned.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Stun VFX"))
  TObjectPtr<UNiagaraSystem> m_pStunEffect;
#pragma endregion

#pragma region | Skin
  /**
   * It will be used when enemy is polluted.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Skins", meta = (DisplayName = "Polluted Overlay"))
  TObjectPtr<UMaterialInterface> m_pPollutedEffect;

  /**
   * It will be used when enemy is polluted.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Skins", meta = (DisplayName = "Polluted Effect"))
  TObjectPtr<UNiagaraSystem> m_pPollutedNiagara;

  /**
   * It will be used when Base Character is in dirt buff state.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Skins", meta = (DisplayName = "Dirt Buff Effect"))
  TObjectPtr<UNiagaraSystem> m_pBuffNiagara;
#pragma endregion

#pragma region | Audio
  /**
   * It is the min loop time to play the idle sound in the enemy.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Min idle loop time"))
  float m_fMinIdleLoopTime;

  /**
   * It is the max loop time to play the idle sound in the enemy.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Max idle loop time"))
  float m_fMaxIdleLoopTime;

  /**
   * It will be executed when the enemy is in idle state.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Idle"))
  TObjectPtr<UFMODEvent> m_pIdleEvent;

  /**
   * It will be executed when the enemy attacks.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Attack"))
  TObjectPtr<UFMODEvent> m_pAttackEvent;

  /**
   * It will be executed when the enemy dirts.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "To Dirt"))
  TObjectPtr<UFMODEvent> m_pToDirtEvent;
#pragma endregion

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Stats", meta = (DisplayName = "Dirt Mitigation Multiplier", ClampMin = 0.f, ClampMax = 2.f))
  float m_fDirtMitigationMultiplier = 1.f;
};
