/************************************************************************
 * @description: Represents the Pristine Time consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Consumables/BaseConsumable.h"
#include "PristineTime.generated.h"

class ABasePlayer;

UCLASS()
class DOROTHYSJOB_API UPristineTime : public UBaseConsumable {
	GENERATED_BODY()
	
public:
	/**
   * @brief Uses the Pristine Time consumable, healing the player to full health and triggering related effects.
   * @param _pPlayer The player using the consumable.
	 */
	virtual bool Use(ABasePlayer* _pPlayer) override;
};