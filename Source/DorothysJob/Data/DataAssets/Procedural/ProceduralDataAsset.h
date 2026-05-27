// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProceduralDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UProceduralDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProceduralData, meta = (DisplayName = "Max Tries Per Node", ClampMin = "10"))
	int32 MaxTriesPerNode = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProceduralData, meta = (DisplayName = "Max Tries Adding Q1", ClampMin = "10"))
	int32 MaxTriesAddingQ1 = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProceduralData, meta = (DisplayName = "Max Tries Per Node", ClampMin = "1.0"))
	float MinDistanceBetweenCleaningStations = 3.0f;
};
