// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PostProcessRooms.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UPostProcessRooms : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere, Category = "PostProcess", meta = (DisplayName = "Post Process Clean Room Settings"))
  FPostProcessSettings m_fSettings;

  UPROPERTY(EditAnywhere, Category = "PostProcess", meta = (DisplayName = "Post Process Dirt Room Settings"))
  FPostProcessSettings m_fDirtSettings;

  UPROPERTY(EditAnywhere, Category = "PostProcess", meta = (DisplayName = "Level"))
  TSoftObjectPtr<UWorld> Level;
};
