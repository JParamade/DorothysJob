/************************************************************************
 * @description: Task node to flee from the player
 * @author: David Gonzalez
 * @date: 08/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskFlee.generated.h"

class ABaseEnemy;

UCLASS()
class DOROTHYSJOB_API UBTTaskFlee : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

  UBTTaskFlee();

  /**
   * @brief It returns a short description for the task.
   */
  virtual FString GetStaticDescription() const override;

  /**
   * @brief It executes the task to flee from the player.
   * @param OwnerComp Behavior tree component owner.
   * @param NodeMemory
   */
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

  /**
   * Flee distance.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Flee Distance"))
  FBlackboardKeySelector m_tFleeDistanceKey;

  /**
   * Flee angle.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Flee Angle"))
  FBlackboardKeySelector m_tFleeAngleKey;

  /**
   * Flee bounce angle.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Flee Bounce Angle"))
  FBlackboardKeySelector m_tFleeBounceAngleKey;

  /**
   * Flee forbidden bounce angle.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Flee Forbidden Bounce Angle"))
  FBlackboardKeySelector m_tFleeForbiddenBounceAngleKey;

  /**
   * Max tries to find the flee point.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Max Tries"))
  FBlackboardKeySelector m_tMaxTriesKey;

  /**
   * Bounce max tries to find the flee point.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Bounce Max Tries"))
  FBlackboardKeySelector m_tBounceMaxTriesKey;

  /**
   * Target from flee.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target From Flee"))
  FBlackboardKeySelector m_tTargetFromFleeKey;

  /**
   * Blackboard key where the target point will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target Location Key"))
  FBlackboardKeySelector m_tTargetLocationKey;

  /**
   * Blackboard key where the last flee time will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Last Flee Date"))
  FBlackboardKeySelector m_tLastFleeDateKey;

  /**
   * It allows to draw the max and min radius and the selected target.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Debug Mode"))
  FBlackboardKeySelector m_tShowDebugKey;
};