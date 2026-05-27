/************************************************************************
 * @description: Represents the Modify Stats Effect.
 * @author: Jaime Paramo
 * @date: 28/06/2025
 * @edited_by: Josephine - 06/08/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Status/BaseStatus.h"
#include "ModifyStatsEffect.generated.h"


UENUM(BlueprintType)
enum class EModifiableStats : uint8
{
	MAXHEALTH				UMETA(DisplayName = "Max Health",				ToolTip = "This will work on all characters"),
	SPECIALCHARGE		UMETA(DisplayName = "Special Charge",		ToolTip = "This will work only for the player"),
	MAXVELOCITY			UMETA(DisplayName = "Max Velocity",			ToolTip = "This will work only on the enemies"),
	MAXDAMAGE				UMETA(DisplayName = "Max Damage",				ToolTip = "This will work only on the enemies"),
	ATTACKCOOLDOWN	UMETA(DisplayName = "Attack Cooldown",	ToolTip = "This will work only on the enemies")
};

/**
 * @brief The Status that will modify the stats of the characters
 */
UCLASS()
class DOROTHYSJOB_API UModifyStatsEffect : public UBaseStatus
{
	GENERATED_BODY()

public:
	/**
	 * @brief -> Default constructor for modify stats effect.
	 */
	UModifyStatsEffect();

protected:

	/**
	 * @brief -> Modifies the target stats.
	 */
	virtual void StatusEffect() override;

	/**
	 * @brief -> Restores the target original stats.
	 */
	virtual void RevertEffect() override;

	/**
	 * @brief The stats to apply to the characters
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Stats", meta = (DisplayName = "Modifiers", ToolTip = "Add only the needed stats"))
	TMap<EModifiableStats, float> m_mStats;
};
