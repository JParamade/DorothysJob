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
class DOROTHYSJOB_API UDogrothy : public USpawnableConsumable
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 
	 * @param _pPlayer 
	 */
	virtual bool Use(ABasePlayer* _pPlayer) override;
};
