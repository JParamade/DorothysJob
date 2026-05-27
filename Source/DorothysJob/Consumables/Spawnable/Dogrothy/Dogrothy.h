/************************************************************************
 * @description: Represents the Dogrothy consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Consumables/Spawnable/SpawnableConsumable.h"
#include "Dogrothy.generated.h"

class ABasePlayer;

UCLASS()
class DOROTHYSJOB_API UDogrothy : public USpawnableConsumable {
	GENERATED_BODY()
	
public:
	/**
   * @brief Uses the Dogrothy consumable.
   * @param _pPlayer The player using the consumable.
	 */
	virtual bool Use(ABasePlayer* _pPlayer) override;
};
