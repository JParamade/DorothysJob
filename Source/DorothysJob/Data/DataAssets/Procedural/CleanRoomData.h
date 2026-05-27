// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CleanRoomData.generated.h"

USTRUCT(BlueprintType)
struct DOROTHYSJOB_API FQuadrantData
{
  GENERATED_BODY()

  FQuadrantData() : 
    TimeReward(0),
    Buff(0.3),
    WithdrawBuff(0.15),
    AutoClean(0.06)
  {}

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ExtraTimeAfterCleaning, meta = (DisplayName = "Time Reward", ClampMin = "0"))
  int32 TimeReward = 30;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DirtnessThresholds, meta = (DisplayName = "Buff Threshold", ClampMin = "0.0", ClampMax = "1.0"))
  float Buff = 0.4f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DirtnessThresholds, meta = (DisplayName = "Withdraw Buff Threshold", ClampMin = "0.0", ClampMax = "1.0"))
  float WithdrawBuff = 0.25f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DirtnessThresholds, meta = (DisplayName = "AutoClean Thresholds", ClampMin = "0.0", ClampMax = "1.0"))
  float AutoClean = 0.12f;
};

UCLASS()
class DOROTHYSJOB_API UCleanRoomData : public UDataAsset
{
	GENERATED_BODY()
	
public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Values for Rooms With 2 Quads"))
  FQuadrantData ValuesFor2Quads;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Values for Rooms With 3 Quads"))
  FQuadrantData ValuesFor3Quads;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Values for Rooms With 4 Quads"))
  FQuadrantData ValuesFor4Quads;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Movement Speed Multiplier in clean rooms", ClampMin = "1.0"))
  float m_fSpeedMultiplierInCleanRooms = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Extra time added when cleaning a Kitchen", ClampMin = "1.0"))
  int32 m_iKitchenExtraTime = 7;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Special charge added when cleaning a Livingroom", ClampMin = "0.0"))
  float m_fLivingroomSpecialCharge = 40.0f;
};
