/************************************************************************
 * @description: One Flower Slime Class
 * @author: Jorge Duart
 * @date: 03/06/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "SlimeEnemy.h"
#include "OneFlowerSlime.generated.h"

UCLASS()
class DOROTHYSJOB_API AOneFlowerSlime : public ASlimeEnemy
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Constructor of One Flower Slime
	 */
	AOneFlowerSlime();

  /**
   * @brief It is called when game starts or when spawned
   */
  virtual void BeginPlay() override;
  virtual void GameOver() override;

private:

  /**
   * It is the timer to control invulnerable time at birth.
   */
  FTimerHandle m_tInvulnerableTimerHandle;
};