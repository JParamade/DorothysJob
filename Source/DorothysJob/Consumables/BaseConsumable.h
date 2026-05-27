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
class DOROTHYSJOB_API UBaseConsumable : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 
	 */
	virtual bool Use(ABasePlayer* _pPlayer);

	/**
	 * @brief 
	 */
	uint32 m_uStackQuantity;

	int32 m_iRewardAddedAmount;

	UPROPERTY()
	bool m_bAchievementManager = false;
};