/************************************************************************
 * @description: Represents the Clean 4 consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Consumables/Spawnable/SpawnableConsumable.h"
#include "Clean4.generated.h"

class ABasePlayer;

UCLASS()
class DOROTHYSJOB_API UClean4 : public USpawnableConsumable
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 
	 * @param _pPlayer 
	 */
	virtual bool Use(ABasePlayer* _pPlayer) override;
};
