/*************************************************************
 * @description: Data for Icons for Room Types
 * @author: Pablo Velasco Crespo
 * @date: 03/09/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomShapeImageData.generated.h"

enum class ERoomShape : uint8;

USTRUCT(BlueprintType)
struct FRoomShapeInfo
{
  GENERATED_BODY()

  FRoomShapeInfo();

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  ERoomShape Shape;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* TextureClean = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* TextureMid = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* TextureDirty = nullptr;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* Frame = nullptr;
};

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API URoomShapeImageData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TArray<FRoomShapeInfo> ShapesInfo;

  const FRoomShapeInfo* GetInfoForShape(ERoomShape Shape) const;
};
