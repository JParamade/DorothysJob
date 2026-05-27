/************************************************************************
 * @description: Represents the SaveGame Object in our game.
 * @author: Jorge Duart
 * @date: 05/03/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJobSaveGame.generated.h"

class UWeaponDataAsset;
class UConsumableDataAsset;
struct FFinalSavedScore;


UCLASS()
class DOROTHYSJOB_API UDorothysJobSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UDorothysJobSaveGame();

	UPROPERTY()
	int32 m_iSavedCurrentLevel;

	UPROPERTY() 
	int32 m_iSavedHighestLevelAchieved;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Saved Selected Weapons"))
	TArray<TObjectPtr<const UWeaponDataAsset>> m_lSavedSelectedWeapons;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Saved Selected Consumables"))
	TArray<TObjectPtr<const UConsumableDataAsset>> m_lSavedSelectedConsumables;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Saved Event Use Data"))
	TMap<FName, bool> m_oSavedEventUseData;

	UPROPERTY(BlueprintReadWrite, meta = (DisplayName = "Saved HUD Event Use Data"))
	TMap<EHUDSection, bool> m_oSavedHUDData;

	UPROPERTY()
	TMap<int32, int32> m_mSavedHubEventTriggered;

	UPROPERTY()
	TArray<FFinalSavedScore> m_lFinalScores;

};
