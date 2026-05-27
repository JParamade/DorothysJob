// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTD_CheckAvailablePookens.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UBTD_CheckAvailablePookens : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTD_CheckAvailablePookens();

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

};
