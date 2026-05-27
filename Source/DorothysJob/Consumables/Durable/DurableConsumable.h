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
class DOROTHYSJOB_API UDurableConsumable : public UBaseConsumable
{
  GENERATED_BODY()

public:
  /**
   * @brief
   * @param _pPlayer
   */
  virtual bool Use(ABasePlayer* _pPlayer) override;

  /**
   * @brief
   */
  float m_fEffectDuration;

protected:
  /**
   * @brief
   */
  virtual void OnEffectStart(ABasePlayer* _pPlayer);
  /**
   * @brief
   */
  virtual void OnEffectEnd(ABasePlayer* _pPlayer);

private:
  /**
   * @brief
   */
  UPROPERTY()
  FTimerHandle m_oTimeHandler;

  bool IsActive() const;
};
