// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Data/DataAssets/Enemy/TokenManagerDataAsset.h"
#include "PookenManagerDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UPookenManagerDataAsset : public UTokenManagerDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific|Pookens", meta = (DisplayName = "Minimum Dust Poops distance"))
	float m_fDustPoopDistance;

	/**
	 * @brief Lower than this percentage, Dust Bunny will poop
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific|Pookens", meta = (DisplayName = "Dirt Percentage Can Poop", ClampMin = 0.f, ClampMax = 1.f))
	float m_fDirtPercentageCanPoop;

	/**
	 * @brief Higher than this percentage, Dust Bunny won't poop
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Specific|Pookens", meta = (DisplayName = "Dirt Percentage Can not Poop", ClampMin = 0.f, ClampMax = 1.f))
	float m_fDirtPercentageCanNotPoop;
};
