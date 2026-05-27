/************************************************************************
 * @description: Decorator to check the condition of both parameters.
 * @author: David Gonzalez
 * @date: 01/09/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_OrCondition.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTD_OrCondition : public UBTDecorator
{
	GENERATED_BODY()

public:

  UBTD_OrCondition();

  /**
   * @brief It returns a short description for the decorator.
   */
  virtual FString GetStaticDescription() const override;

protected:

  /**
   * @brief It calculates raw, core value of decorator's condition. Should not include calling IsInversed.
   * @param OwnerComp Behavior tree component owner.
   * @param NodeMemory 
   */
  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const;

private:

  /**
   * First condition.
   */
  UPROPERTY(EditAnywhere, Category = "Decorator", meta = (DisplayName = "First Condition"))
  FBlackboardKeySelector m_tFirstConditionKey;

  /**
   * Second condition.
   */
  UPROPERTY(EditAnywhere, Category = "Decorator", meta = (DisplayName = "Second Condition"))
  FBlackboardKeySelector m_tSecondConditionKey;
};