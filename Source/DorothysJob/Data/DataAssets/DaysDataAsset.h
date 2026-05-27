// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DaysDataAsset.generated.h"

class UTexture2D;

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UDaysDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief The list of registered events
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Data", meta = (DisplayName = "Days List"))
	TMap<int, UTexture2D*> m_oDaysList;
};
