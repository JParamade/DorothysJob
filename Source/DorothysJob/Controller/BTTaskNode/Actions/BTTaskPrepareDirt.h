/************************************************************************
 * @description: Task node for enemy prepare dirt action
 * @author: David Gonzalez
 * @date: 25/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BTTaskAction.h"
#include "BTTaskPrepareDirt.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UBTTaskPrepareDirt : public UBTTaskAction
{
	GENERATED_BODY()
	
public:

  UBTTaskPrepareDirt();

  /**
   * @brief It returns a short description for the task
   */
  virtual FString GetStaticDescription() const override;

protected:

  /**
   * @brief It starts the enemy action.
   * @param _pEnemy AI enemy
   */
  virtual void StartEnemyAction(ABaseEnemy* _pEnemy) override;

  /**
   * @brief It cancels the enemy action.
   * @param _pEnemy AI enemy
   */
  virtual void CancelEnemyAction(ABaseEnemy* _pEnemy) override;

  /**
   * @brief It returns the delegate to connect the enemy end action.
   * @param _pEnemy AI enemy
   */
  virtual FOnEnemyActionComplete& GetCompletionDelegate(ABaseEnemy* _pEnemy) override;
};