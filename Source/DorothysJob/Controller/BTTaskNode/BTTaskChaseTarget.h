/************************************************************************
 * @description: Task to chase the target.
 * @author: David Gonzalez
 * @date: 20/08/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskChaseTarget.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTTaskChaseTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:

  UBTTaskChaseTarget();

  /**
   * @brief It returns a short description for the task.
   */
  virtual FString GetStaticDescription() const override;

  /**
   * @brief It executes the task to rotate to face the target.
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
   * Blackboard key where the target point will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target Location Key"))
  FBlackboardKeySelector m_tTargetLocationKey;

  /**
   * Enemy to chase.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Enemy To Chase"))
  FBlackboardKeySelector m_tEnemyToChase;

  /**
   * It allows to draw the max and min radius and the selected target.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Debug Mode"))
  FBlackboardKeySelector m_tShowDebugKey;
};