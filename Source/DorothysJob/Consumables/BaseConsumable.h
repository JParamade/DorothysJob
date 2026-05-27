/************************************************************************
 * @description: Represents the base consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BaseConsumable.generated.h"

class ABasePlayer;
class UAchievementSubsystem;

UCLASS()
class DOROTHYSJOB_API UBaseConsumable : public UObject {
  GENERATED_BODY()

public:
  /**
   * @brief Uses the consumable item.
   * @param _pPlayer The player using the consumable.
   * @return True if the consumable was used successfully, false otherwise.
   */
  virtual bool Use(ABasePlayer* _pPlayer);

  /**
   * @brief The quantity of the consumable item available.
   */
  uint32 m_uStackQuantity;

  /**
   * @brief The amount of reward added when the consumable is used.
   */
  int32 m_iRewardAddedAmount;

  /**
   * @brief Indicates whether the achievement manager is enabled for this consumable.
   */
  UPROPERTY()
  bool m_bAchievementManager = false;
};