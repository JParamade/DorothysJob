/************************************************************************
 * @description: Task node for enemy action
 * @author: David Gonzalez
 * @date: 25/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "BTTaskAction.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTTaskAction : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

  UBTTaskAction();

  /**
   * @brief It returns a short description for the task.
   */
  virtual FString GetStaticDescription() const override;

  /**
   * @brief It executes the task  for enemy.
   * @param OwnerComp Behavior tree component owner.
   * @param NodeMemory
   */
  virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

  /**
   * @brief When the task is aborted in the bahvior tree.
   * @param OwnerComp Behavior tree component owner.
   * @param NodeMemory
   */
  virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

  /**
   * @brief It starts the enemy action.
   * @param _pEnemy AI enemy 
   */
  virtual void StartEnemyAction(ABaseEnemy* _pEnemy) PURE_VIRTUAL(UBTTaskAction::StartEnemyAction, );

  /**
   * @brief It cancels the enemy action.
   * @param _pEnemy AI enemy
   */
  virtual void CancelEnemyAction(ABaseEnemy* _pEnemy) PURE_VIRTUAL(UBTTaskAction::CancelEnemyAction, );

  /**
   * @brief It returns the delegate to connect the enemy end action.
   * @param _pEnemy AI enemy
   */
  virtual FOnEnemyActionComplete& GetCompletionDelegate(ABaseEnemy* _pEnemy) PURE_VIRTUAL(UBTTaskAction::GetCompletionDelegate, return *(FOnEnemyActionComplete*)nullptr;);

private:

  /**
   * @brief It should be called when the action is finished.
   */
  UFUNCTION()
  void OnActionCompleted(ABaseEnemy* _pEnemy);
};
