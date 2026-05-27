// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EventListDataAsset.generated.h"

UENUM()
enum class ETutorialType
{
	DIALOGUE,
	TEMPORAL_WIDGET,
	PERMANENT_WIDGET,
	ARROW,
	HIDEARROW,
	REVEAL
};

USTRUCT(BlueprintType)
struct FTriggerData
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	ETutorialType m_eTutorialType = ETutorialType::DIALOGUE;

	UPROPERTY(EditAnywhere)
	FName m_iId;
};

USTRUCT(BlueprintType)
struct FEventDataArray
{
	GENERATED_BODY();

	UPROPERTY(EditAnywhere)
	TArray<FTriggerData> m_aTutorialData;

	bool m_bHasAlreadyAppear = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tutorial")
	int m_iEventLevel = -1;
};

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UEventListDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola13|Data", meta = (DisplayName = "All Events"));
	TMap<FName, FEventDataArray> m_oAllEventData;
};
