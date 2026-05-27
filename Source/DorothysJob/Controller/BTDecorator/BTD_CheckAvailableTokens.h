/************************************************************************
 * @description: Decorator to check if there are tokens available when the enemy can attack.
 * @author: Jorge Duart
 * @date: 24/07/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckAvailableTokens.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTD_CheckAvailableTokens : public UBTDecorator
{
	GENERATED_BODY()

public:

  UBTD_CheckAvailableTokens();

  /**
   * @brief It returns a short description for the decorator.
   */
  virtual FString GetStaticDescription() const override;

protected:

  /**
   * @brief It checks if it pass
   * @param OwnerComp Reference to the Behaviour Tree
   * @param NodeMemory Memory
   * @return If it pass the node or it is beeing abort
   */
  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

  /**
   * @brief It is called when underlying node is activated.
   * @param SearchData
   */
  virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;

  /**
   * @brief It is called when underlying node is deactivated.
   * @param SearchData
   * @param NodeResult
   */
  virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;

private:

  /**
   * Blackboard key if the AI can attack or not.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Can Attack"))
  FBlackboardKeySelector m_tCanAttackKey;

  /**
   * Blackboard key if the AI is polluted or not.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Is Polluted"))
  FBlackboardKeySelector m_tIsPollutedKey;
};
