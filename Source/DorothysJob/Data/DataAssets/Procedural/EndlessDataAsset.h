/************************************************************************
 * @description: Represents the Endless Data.
 * @author: Pablo Velasco
 * @date: 05/10/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EndlessDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UEndlessDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	int32 MinMinutes = 0;

	UPROPERTY(EditAnywhere)
	int32 MinSeconds = 30;

	UPROPERTY(EditAnywhere)
	int32 TimeToDeductStep = 5;

	UPROPERTY(EditAnywhere)
	int32 MaxQuadsStep = 3;

	UPROPERTY(EditAnywhere)
	float ActiveSpawnersStep = 0.05f;

	UPROPERTY(EditAnywhere)
	float CleaningStationChargeStep = 5.0f;
};
