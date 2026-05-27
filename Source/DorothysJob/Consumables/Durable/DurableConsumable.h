/************************************************************************
 * @description: Represents the durable consumable.
 * @author: Jaime Paramo
 * @date: 11/06/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Consumables/BaseConsumable.h"
#include "DurableConsumable.generated.h"

class ABasePlayer;

DECLARE_DELEGATE(FOnEffectBegin)
DECLARE_DELEGATE(FOnEffectFinished)

UCLASS()
class DOROTHYSJOB_API UDurableConsumable : public UBaseConsumable {
  GENERATED_BODY()

public:
  /**
   * @brief Uses the durable consumable, applying its effect to the player and starting a timer for the effect duration.
   * @param _pPlayer The player using the consumable.
   */
  virtual bool Use(ABasePlayer* _pPlayer) override;

  /**
   * @brief Effect duration in seconds.
   */
  float m_fEffectDuration;

protected:
  /**
   * @brief Called when the consumable's effect starts, applying the specific effect to the player.
   */
  virtual void OnEffectStart(ABasePlayer* _pPlayer);
  /**
   * @brief Called when the consumable's effect ends, removing the specific effect from the player and allowing the consumable to be used again.
   */
  virtual void OnEffectEnd(ABasePlayer* _pPlayer);

private:
  /**
   * @brief Timer handle for managing the duration of the consumable's effect.
   */
  UPROPERTY()
  FTimerHandle m_oTimeHandler;

  /**
   * @brief Checks if the consumable's effect is currently active.
   * @return True if the effect is active, false otherwise.
   */
  bool IsActive() const;
};
