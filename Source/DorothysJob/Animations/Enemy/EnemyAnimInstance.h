/************************************************************************
 * @description: Enemy animation instance class
 * @author: David Gonzalez
 * @date: 21/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Animations/BaseAnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class ABaseEnemy;

UCLASS()
class DOROTHYSJOB_API UEnemyAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:

  /**
   * Delegate to notify when to dirt action should be done.
   */
  FOnActionChanges OnToDirtAction;
	
protected:

  /**
   * @brief It initializes the animation.
   */
  virtual void NativeInitializeAnimation() override;

  /**
   * @brief It updates the current animation parameters.
   * @param _fDeltaSeconds -> Senconds since last time executed. 
   */
  virtual void NativeUpdateAnimation(float _fDeltaSeconds) override;

private:

  /**
   * Animation owner enemy reference.
   */
  TObjectPtr<ABaseEnemy> m_pEnemy;
};