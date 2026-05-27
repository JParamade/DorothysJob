// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DirtTypesDataAsset.generated.h"


UENUM()
enum class EDirtType : uint8
{
  Liquid UMETA(DisplayName = "Liquid"),
  Dust UMETA(DisplayName = "Dust"),
  Neutral UMETA(DisplayName = "Neutral")
};

USTRUCT(BlueprintType)
struct FTypeEffectiveness
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TMap<EDirtType, float> EffectivenessMap;
};

UCLASS()
class DOROTHYSJOB_API UDirtTypesDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Type", meta = (DisplayName = "Dirt Type"))
  TMap<EDirtType, FTypeEffectiveness> m_mTypeTable;
	
};
