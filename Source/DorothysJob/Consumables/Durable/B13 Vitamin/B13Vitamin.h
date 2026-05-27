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
class DOROTHYSJOB_API UB13Vitamin : public UDurableConsumable
{
  GENERATED_BODY()

public:
  /**
   * @brief
   * @param _pPlayer
   */
  virtual bool Use(ABasePlayer* _pPlayer) override;

protected:
  /**
   * @brief
   */
  virtual void OnEffectStart(ABasePlayer* _pPlayer) override;

  /**
   * @brief
   */
  virtual void OnEffectEnd(ABasePlayer* _pPlayer) override;

private:
  FMOD::Studio::EventInstance* pBuffEvent = nullptr;
  FMOD::Studio::EventInstance* pBreatheEvent = nullptr;

  /**
   * @brief
   */
  UFUNCTION()
  virtual void PauseFMODEvents();
  /**
   * @brief
   */
  UFUNCTION()
  virtual void UnpauseFMODEvents();

  /**
   * @brief
   */
  bool m_bBuffPaused = false;
  /**
   * @brief 
   */
  bool m_bBreathePaused = false;

  /**
   * @brief
   */
  FDelegateHandle m_oPauseHandle;
  /**
   * @brief
   */
  FDelegateHandle m_oUnpauseHandle;

  /**
   * @brief
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | Consumables", meta = (DisplayName = "Movement Speed Increment"))
  float m_fMovementSpeedIncrement = 1.3f;

  /**
   * @brief
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | Consumables", meta = (DisplayName = "Attack Speed Increment"))
  float m_fAttackSpeedMultiplier = 2.0f;
};