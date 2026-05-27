/************************************************************************
 * @description: Decorator to check if there is any available dust poop to hide.
 * @author: David Gonzalez
 * @date: 02/09/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckAvailablePoop.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTD_CheckAvailablePoop : public UBTDecorator
{
	GENERATED_BODY()
	
public:

  UBTD_CheckAvailablePoop();

  /**
   * @brief It returns a short description for the decorator.
   */
  virtual FString GetStaticDescription() const override;

protected:

  /**
   * @brief It calculates raw, core value of decorator's condition. Should not include calling IsInversed.
   * @param OwnerComp -> Behavior tree component owner.
   * @param NodeMemory -> Node memory.
   */
  virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:

  /**
   * Blackboard key where the dust poop target will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Dust Poop Actor"))
  FBlackboardKeySelector m_tDustPoopActorKey;

  /**
   * Blackboard key where the target point will be stored.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Target Location Key"))
  FBlackboardKeySelector m_tTargetLocationKey;
};
