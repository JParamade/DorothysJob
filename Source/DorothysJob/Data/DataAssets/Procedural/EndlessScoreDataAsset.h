/************************************************************************
 * @description: Represents the Endless Score Data.
 * @author: Pablo Velasco
 * @date: 05/10/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EndlessScoreDataAsset.generated.h"

enum class EScore : uint8;

USTRUCT(BlueprintType)
struct FDaysScore
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 Days = 0;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UTexture2D> Score = nullptr;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Score Letter"))
	EScore m_sScoreLetter;
};

UCLASS()
class DOROTHYSJOB_API UEndlessScoreDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TArray<FDaysScore> Scores;
};
