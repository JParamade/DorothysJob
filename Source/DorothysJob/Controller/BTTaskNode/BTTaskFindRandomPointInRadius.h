/************************************************************************
 * @description: Task node to find a random point with a min and max radius
 * @author: David Gonzalez
 * @date: 08/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskFindRandomPointInRadius.generated.h"

class ABaseEnemy;

UCLASS()
class DOROTHYSJOB_API UBTTaskFindRandomPointInRadius : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

  UBTTaskFindRandomPointInRadius();

  /**
   * @brief It returns a short description for the task.
   */
  virtual FString GetStaticDescription() const override;

  /**
   * @brief It executes the task to find a random point between min and max radius.
   * @param OwnerComp Behavior tree component owner.
   * @param NodeMemory 
   */
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:

  /**
   * Min radius to find the target point.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Min Radius"))
  FBlackboardKeySelector m_tMinRadiusKey;

  /**
   * Max radius to find the target point.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Max Radius"))
  FBlackboardKeySelector m_tMaxRadiusKey;

  /**
   * Wander angle.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Wander Angle"))
  FBlackboardKeySelector m_tWanderAngleKey;

  /**
   * Wander bounce angle.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Wander Bounce Angle"))
  FBlackboardKeySelector m_tWanderBounceAngleKey;

  /**
   * Blackboard key where the target point will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target Location Key"))
  FBlackboardKeySelector m_tTargetLocationKey;

  /**
   * Max tries to find the target point.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Max Tries"))
  FBlackboardKeySelector m_tMaxTriesKey;

  /**
   * Bounce max tries to find the target point.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Bounce Max Tries"))
  FBlackboardKeySelector m_tBounceMaxTriesKey;

  /**
   * It allows to draw the max and min radius and the selected target.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Debug Mode"))
  FBlackboardKeySelector m_tShowDebugKey;
};