/************************************************************************
 * @description: Represents the B13 Vitamin consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Consumables/Durable/DurableConsumable.h"
#include "B13Vitamin.generated.h"

class ABasePlayer;
namespace FMOD {
  namespace Studio {
    class EventInstance;
  }
}

UCLASS()
class DOROTHYSJOB_API UB13Vitamin : public UDurableConsumable {
  GENERATED_BODY()

public:
  /**
   * @brief Uses the B13 Vitamin consumable, applying its effect to the player and starting a timer for the effect duration.
   * @param _pPlayer The player using the consumable.
   */
  virtual bool Use(ABasePlayer* _pPlayer) override;

protected:
  /**
   * @brief Called when the B13 Vitamin's effect starts, applying the specific effect to the player.
   * @param _pPlayer The player using the consumable.
   */
  virtual void OnEffectStart(ABasePlayer* _pPlayer) override;
  /**
   * @brief Called when the B13 Vitamin's effect ends, removing the specific effect from the player and allowing the consumable to be used again.
   * @param _pPlayer The player using the consumable.
   */
  virtual void OnEffectEnd(ABasePlayer* _pPlayer) override;

private:
  /**
   * @brief FMOD Event Instance for the buff sound effect.
   */
  FMOD::Studio::EventInstance* pBuffEvent = nullptr;
  /**
   * @brief FMOD Event Instance for the breathing sound effect.
   */
  FMOD::Studio::EventInstance* pBreatheEvent = nullptr;

  /**
   * @brief Handles pausing FMOD events when the game is paused to ensure they are paused correctly.
   */
  UFUNCTION()
  virtual void PauseFMODEvents();
  /**
   * @brief Handles unpausing FMOD events when the game is unpaused to ensure they resume correctly.
   */
  UFUNCTION()
  virtual void UnpauseFMODEvents();

  /**
   * @brief Indicates whether the buff FMOD event is currently paused.
   */
  bool m_bBuffPaused = false;
  /**
   * @brief Indicates whether the breathing FMOD event is currently paused.
   */
  bool m_bBreathePaused = false;

  /**
   * @brief Delegate handle for managing the pause event binding to ensure proper unbinding when the effect ends.
   */
  FDelegateHandle m_oPauseHandle;
  /**
   * @brief Delegate handle for managing the unpause event binding to ensure proper unbinding when the effect ends.
   */
  FDelegateHandle m_oUnpauseHandle;

  /**
   * @brief Movement speed increment multiplier applied to the player when the B13 Vitamin effect is active.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | Consumables", meta = (DisplayName = "Movement Speed Increment"))
  float m_fMovementSpeedIncrement = 1.3f;

  /**
   * @brief Attack speed multiplier applied to the player when the B13 Vitamin effect is active.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | Consumables", meta = (DisplayName = "Attack Speed Increment"))
  float m_fAttackSpeedMultiplier = 2.0f;
};