/************************************************************************
 * @description: It represents Grimeling enemy in the game.
 * @author: Raul Fernandez
 * @date: 19/05/2025
 * @edited_by: Jaime Paramo
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "Goblin.generated.h"

UCLASS()
class DOROTHYSJOB_API AGoblin : public ABaseEnemy
{
  GENERATED_BODY()

public:

  /**
   * @brief Default constructor of Grimeling
   */
  AGoblin();

  /**
   * @brief It is called when game starts or when spawned.
   */
  virtual void BeginPlay() override;

#pragma region | Enemy Manager
  /**
   * @brief It returns if there are available tokens to attack in the current room.
   */
  virtual bool AreAvailableTokens() override;

  /**
   * @brief It request a token to attack in the current room.
   */
  virtual bool RequestToken() override;

  /**
   * @brief It returns the token to the pool.
   */
  virtual bool ReturnToken() override;
#pragma endregion

protected:

#pragma region | Actions
  /**
   * @brief The enemy performs the attack when the notification arrives according to the animation.
   */
  virtual void PerformAttack(int32 _iCurrentCombo) override;
#pragma endregion

  virtual void GameOver() override;
private:

  /**
   * It resets the brave mode.
   */
  FTimerHandle m_tBraveTimerHandle;

  /**
   * For the achivement Faster than you Stink.
   */
  bool m_bHasBufEnemies = false;
};