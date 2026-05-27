/************************************************************************
 * @description: Represents the Base Room in the Game.
 * @author: Pablo Velasco
 * @date: 23/03/2025
 * @edited_by: Josephine Esposito - 11/04/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "ProceduralData.generated.h"

/**
 * The struct of the row of the data table
 */
USTRUCT(BlueprintType)
struct FProceduralData : public FTableRowBase
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProceduralData, meta = (DisplayName = "Max Number of Trying to connect with another Room", ClampMin = 1))
  int32 MaxTriesPerNode = 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProceduralData, meta = (DisplayName = "Max Number of Trying to add a Q1 to a Room", ClampMin = 1))
  int32 MaxTriesAddingQ1 = 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ProceduralData, meta = (DisplayName = "Min Distance between Cleaning stations", ToolTip = "One unit is the width of a Quad = 16 meters.", ClampMin = 1))
  float MinDistanceBetweenCleaningStations = 1;
};