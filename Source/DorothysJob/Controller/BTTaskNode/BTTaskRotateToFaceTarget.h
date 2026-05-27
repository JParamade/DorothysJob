/************************************************************************
 * @description: Task node to rotate to face the player
 * @author: David Gonzalez
 * @date: 13/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskRotateToFaceTarget.generated.h"

class ABaseEnemy;

UCLASS()
class DOROTHYSJOB_API UBTTaskRotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

  UBTTaskRotateToFaceTarget();

  /**
   * @brief It returns a short description for the task.
   */
  virtual FString GetStaticDescription() const override;

  /**
   * @brief It executes the task to rotate to face the target.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   */
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

  /**
   * @brief It rotates the AI Pawn to the face the target with a smooth rotation.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   * @param DeltaSeconds -> Seconds since the last execution.
   */
  virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:

  /**
   * Interpolation rotation speed.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Inter. Rot. Speed"))
  FBlackboardKeySelector m_tRotationInterpSpeedKey;

  /**
   * Arrival radius.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Arrival Radius"))
  FBlackboardKeySelector m_tAcceptableRadiusKey;

  /**
   * Target to face.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target To Face"))
  FBlackboardKeySelector m_tTargetToFaceKey;
};