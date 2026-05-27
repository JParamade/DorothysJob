/************************************************************************
 * @description: Decorator to check the line of sight between enemy and player.
 * @author: Jorge Duart
 * @date: 02/08/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckLineOfSight.generated.h"

UCLASS()
class DOROTHYSJOB_API UBTD_CheckLineOfSight : public UBTDecorator
{
  GENERATED_BODY()

public:

  UBTD_CheckLineOfSight();

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

private:

  /**
   * Enemy to see.
   */
  UPROPERTY(EditAnywhere, Category = "Blackboard", meta = (DisplayName = "Enemy To See"))
  FBlackboardKeySelector m_tEnemyToSee;
};
