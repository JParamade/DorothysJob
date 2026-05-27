/*************************************************************
 * @description: Data for Icons for Room Types
 * @author: Pablo Velasco Crespo
 * @date: 03/09/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RoomIconData.generated.h"

enum class ERoomType : uint8;
class UBaseRoom;

USTRUCT(BlueprintType)
struct FRoomTypeInfo
{
  GENERATED_BODY()

  FRoomTypeInfo();

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  ERoomType Type;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* Texture = nullptr;
};

/**
 *
 */
UCLASS()
class DOROTHYSJOB_API URoomIconData : public UDataAsset
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TArray<FRoomTypeInfo> TypesInfo;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* CleaningStationIcon;

  UFUNCTION(BlueprintCallable)
  UTexture2D* GetTextureForType(ERoomType Type) const
  {
    for (const auto& Info : TypesInfo)
    {
      if (Info.Type == Type)
        return Info.Texture;
    }
    return nullptr;
  }

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* RewardKitchen;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* RewardLivingroom;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* RewardBathroom;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* RewardGreenhouse;

  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  UTexture2D* RewardNothing;

  UFUNCTION(BlueprintCallable)
  UTexture2D* GetRewardTextureForType(UBaseRoom* Room) const;
};
