/************************************************************************
 * @description: Task node to move the controlled AI Pawn
 * @author: David Gonzalez
 * @date: 08/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskMoveToTarget.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTTaskMoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

  UBTTaskMoveToTarget();

  /**
   * @brief It returns a short description for the task.
   */
  virtual FString GetStaticDescription() const override;

  /**
   * @brief It executes the task to move to the target location.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   */
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

  /**
   * @brief It moves the AI Pawn to the target location with a smooth movement.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   * @param DeltaSeconds -> Seconds since the last execution.
   */
  virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

  /**
   * @brief It is called when the task is finished.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   * @param TaskResult -> Task result.
   */
  virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

private:

  /**
   * Arrival radius.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Arrival Radius"))
  FBlackboardKeySelector m_tAcceptableRadiusKey;

  /**
   * Interpolation movement speed.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Inter. Mov. Speed"))
  FBlackboardKeySelector m_tMoveInterpSpeedKey;

  /**
   * Interpolation rotation speed.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Inter. Rot. Speed"))
  FBlackboardKeySelector m_tRotationInterpSpeedKey;

  /**
   * Blackboard key where the target point will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target Location Key"))
  FBlackboardKeySelector m_tTargetLocationKey;
};