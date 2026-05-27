/************************************************************************
 * @description: Represents the data table for the rooms
 * @author: Josephine Esposito
 * @date: 30/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "RoomData.generated.h"

enum class ERoomType : uint8;
enum class ERoomShape : uint8;

/**
 * The struct of the row of the data table
 */
USTRUCT(BlueprintType)
struct FRoomData : public FTableRowBase
{
  GENERATED_BODY()

  FRoomData();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomData)
  int32 RoomID = 0;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomData)
  FIntPoint RoomSize = 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomData)
  TArray<FIntPoint> Doors;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomData)
  ERoomType RoomType;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomData)
  ERoomShape RoomShape;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomData)
  TSoftObjectPtr<UWorld> RoomLevelInstance;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RoomData, meta = (ClampMin = "0.0", ClampMax = "1.0"))
  float Percentage = 1.0f;
};
