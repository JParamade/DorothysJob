/************************************************************************
 * @description: Represents the Data for the Score
 * @author: Josephine Esposito
 * @date: 24/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ScoreDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FBaseScore
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Limit", ClampMin = 0))
  int32 m_iMinLimit = 0;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Limit", ClampMax = 1000))
  int32 m_iMaxLimit = 1000;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Added Points"))
  int32 m_iAddedPoints = 1;
};

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UScoreDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Score", meta = (DisplayName = "Extra Time"))
  TArray<FBaseScore> m_lExtraTime;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Score", meta = (DisplayName = "Used Consumables"))
  TArray<FBaseScore> m_lUsedConsumables;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Score", meta = (DisplayName = "Stinky Times"))
  TArray<FBaseScore> m_lCleaningStationUses;

};
