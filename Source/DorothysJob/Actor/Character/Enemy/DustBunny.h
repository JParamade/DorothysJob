/************************************************************************
 * @description: Dust Bunny Class
 * @author: David Gonzalez
 * @date: 08/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "DustBunny.generated.h"

UCLASS()
class DOROTHYSJOB_API ADustBunny : public ABaseEnemy
{
	GENERATED_BODY()
	
public:

  /**
   * @brief Default Constructor of Dust Bunny
   */
  ADustBunny();

  /**
   * @brief It is called when game starts or when spawned
   */
  virtual void BeginPlay() override;

  /**
   * @brief It returns if the tower mode is available.
   */
  bool IsTowerModeAvailable();

#pragma region | Movement
  /**
   * @brief It returns if the Dust Bunny is in movement or not.
   */
  virtual bool IsInMovement() const override;
#pragma endregion

#pragma region | Hide
  /**
   * @brief Generic enemy AI to hide function.
   */
  virtual void ToHide() override;

  /**
   * @brief Generic enemy AI unhide function.
   */
  void UnHide();
#pragma endregion

  /**
    * It stunnes because of cleaning a dustpoop.
    */
  bool m_bStunnedByDustPoop = false;
protected:

#pragma region | Actions
  /**
   * @brief The enemy performs the attack when the notification arrives according to the animation.
   */
  virtual void PerformAttack(int32 _iCurrentCombo) override;

  /**
   * @brief The enemy performs to dirt action when the notification arrives according to the animation.
   */
  virtual void PerformToDirt() override;
#pragma endregion

  virtual void GameOver() override;

private:

#pragma region | Debug Mode
  /**
   * Allways attack debug mode cooldown timer.
   */
  FTimerHandle m_tTimerAttack;

  /**
   * It allows to set the dust bunny as allways attack mode.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Allways Attack Mode"))
  bool m_bAllwaysAttackDebug;
#pragma endregion
};