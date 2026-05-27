/************************************************************************
 * @description: Represents the Mrs Clean consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Consumables/Spawnable/SpawnableConsumable.h"
#include "MrsClean.generated.h"

class ABasePlayer;

UCLASS()
class DOROTHYSJOB_API UMrsClean : public USpawnableConsumable {
  GENERATED_BODY()

public:
  /**
   * @brief Uses the Mrs Clean consumable.
   * @param _pPlayer The player using the consumable.
   */
  virtual bool Use(ABasePlayer* _pPlayer) override;
};
