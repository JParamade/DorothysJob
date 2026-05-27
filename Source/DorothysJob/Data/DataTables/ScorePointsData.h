/************************************************************************
 * @description: Represents the structure for the info used to calculate the score
 * @author: Josephine Esposito
 * @date: 24/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "ScorePointsData.generated.h"

enum class EScore : uint8;

USTRUCT(BlueprintType)
struct FScorePointsData : public FTableRowBase
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Min Limit", ClampMin = 0))
  int32 m_iMinLimit = 0;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Max Limit", ClampMax = 100))
  int32 m_iMaxLimit = 100;

  UPROPERTY(EditAnywhere, meta = (DisplayName = "Score Letter"))
  EScore m_sScoreLetter;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Score", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pScoreImage;
};
