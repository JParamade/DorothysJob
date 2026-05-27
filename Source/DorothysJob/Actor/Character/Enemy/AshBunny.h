/************************************************************************
 * @description: Ash Bunny Class
 * @author: David Gonzalez
 * @date: 03/06/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DustBunny.h"
#include "AshBunny.generated.h"

UCLASS()
class DOROTHYSJOB_API AAshBunny : public ADustBunny
{
	GENERATED_BODY()
	
public:

  /**
   * @brief Default Constructor of Ash Bunny
   */
  AAshBunny();

protected:

#pragma region | Actions
  /**
   * @brief The enemy performs the attack when the notification arrives according to the animation.
   */
  virtual void PerformAttack(int32 _iCurrentCombo) override;
#pragma endregion
};