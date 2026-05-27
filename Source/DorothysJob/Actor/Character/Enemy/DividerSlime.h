/************************************************************************
 * @description: Slime Divider Enemy Class
 * @author: Aurora
 * @edited_by: Jorge Duart
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "SlimeEnemy.h"
#include "DividerSlime.generated.h"

class AOneFlowerSlime;

UCLASS()
class DOROTHYSJOB_API ADividerSlime : public ASlimeEnemy
{
	GENERATED_BODY()
	
public:

  /**
   * @brief Default constructor of Divider Slime
   */
	ADividerSlime();

protected:

#pragma region | Game Over
	/**
	 * @brief It is callen when the current health is less than 0.
	 */
	virtual void GameOver() override;
#pragma endregion

private:

  /**
   * @brief It ends the division.
   */
  void DivisionEnd();

  /**
   * It is the timer to control death.
   */
  FTimerHandle m_tDeathTimerHandle;
};