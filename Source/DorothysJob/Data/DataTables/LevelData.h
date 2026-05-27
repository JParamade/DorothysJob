/************************************************************************
 * @description: Represents the data table for the rooms
 * @author: Josephine Esposito
 * @date: 30/04/2025
 * @edited_by: Pablo
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "LevelData.generated.h"

 /**
  * @brief Represents the Types of Rooms
  */
UENUM(BlueprintType)
enum class ERoomType : uint8
{
  LIVINGROOM  UMETA(DisplayName = "Living Room"),

  KITCHEN     UMETA(DisplayName = "Kitchen"),

  BATHROOM    UMETA(DisplayName = "Bathroom"),

  GREENHOUSE   UMETA(DisplayName = "Green House"),

  BEDROOM     UMETA(DisplayName = "Bedroom")
};

UENUM(BlueprintType)
enum class EPoolType : uint8
{
  LOCAL_1  UMETA(DisplayName = "LOCAL_1"),

  LOCAL_2  UMETA(DisplayName = "LOCAL_2"),

  LOCAL_1_GRIMELING     UMETA(DisplayName = "LOCAL_1 + GRIMELING"),

  LOCAL_1_FOREIGN_1    UMETA(DisplayName = "LOCAL_1 + FOREIGN_1"),

  LOCAL_2_GRIMELING   UMETA(DisplayName = "LOCAL_2 + GRIMELING"),

  LOCAL_2_FOREIGN_1     UMETA(DisplayName = "LOCAL_2 + FOREIGN_1"),

  LOCAL_2_FOREIGN_2     UMETA(DisplayName = "LOCAL_2 + FOREIGN_2"),

  LOCAL_1_FOREIGN_1_GRIMELING    UMETA(DisplayName = "LOCAL_1 + FOREIGN_1 + GRIMELING"),

  LOCAL_2_FOREIGN_1_GRIMELING    UMETA(DisplayName = "LOCAL_2 + FOREIGN_1 + GRIMELING"),

  LOCAL_2_FOREIGN_2_GRIMELING    UMETA(DisplayName = "LOCAL_2 + FOREIGN_2 + GRIMELING"),

  CUSTOM    UMETA(DisplayName = "CUSTOM")
};

USTRUCT(BlueprintType)
struct FSpawnChances
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution", meta = (DisplayName = "Pool Type"))
  EPoolType m_ePoolType = EPoolType::CUSTOM;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution", meta = (DisplayName = "Probability Weight", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fProbabilityWeight = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution", meta = (DisplayName = "Active Spawner Distribution Override", ClampMin = "-1.0", ClampMax = "1.0", ToolTip = "This value overrides the original one ONLY when it is GREATER than 0"))
  float m_fSpawnerPercentageOverride = -1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution|Distributions", meta = (DisplayName = "Oozer Distribution", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fOozerDistribution = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution|Distributions", meta = (DisplayName = "Mini Oozer Distribution", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fMiniOozerDistribution = 0.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution|Distributions", meta = (DisplayName = "Oozer Double Distribution", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fOozerDoubleDistribution = 0.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution|Distributions", meta = (DisplayName = "Dust Bunny Distribution", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fDustBunnyDistribution = 0.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution|Distributions", meta = (DisplayName = "Ash BunnyDistribution", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fAshBunnyDistribution = 0.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnDistribution|Distributions", meta = (DisplayName = "Grimeling Distribution", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fGrimelingDistribution = 0.0f;
};

USTRUCT(BlueprintType)
struct FAvailableRoom
{
  GENERATED_BODY()

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Room Type"))
  ERoomType m_eRoomType = ERoomType::LIVINGROOM;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Procedural Weight", ClampMin = "0.0"))
  float m_fProceduralWeight = 1.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Active Spawner Distribution", ClampMin = "0.0", ClampMax = "1.0"))
  float m_fSpawnerPercentage = 0.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Spawner Pools"))
  TArray<FSpawnChances> SpawnDistribution;
};

/**
  * The struct of the row of the data table
  */
USTRUCT(BlueprintType)
struct FLevelData : public FTableRowBase
{
  GENERATED_BODY()

public:
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Level ID"))
  int32 m_iLevelID = 0;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Min Quad Count"))
  int32 m_iMinQuadrantNumber = 10;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Max Quad Count"))
  int32 m_iMaxQuadrantNumber = 15;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Cleaning Station Count", ToolTip = "This has to be Lower or Equal than Max Q1 Count.", ClampMin = 1))
  int32 m_iCleaningStationNumber = 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Max Q1 Count", ToolTip = "This has to be Equal or Higher than Cleaning Station Count.", ClampMin = 1))
  int32 m_iMaxQ1Number = 3;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Room Type Misc Data", ToolTip = "Add max one per Room Type. If you don't add a Room Type that type of room won't be used."))
  TArray<FAvailableRoom> m_lAvailableRooms;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Duration|Minutes"))
  int32 Minutes = 1;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Duration|Seconds"))
  int32 Seconds = 30;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Lax Rules"))
  bool LaxRules = false;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Base % Deactivate Enemies in Close Rooms", ClampMin = "0.0", ClampMax = "1.0"))
  float BaseDeactivateRoomByBuff = 0.3f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Mitigation Applied when Buffing Enemies", ClampMin = "0.0", ClampMax = "1.0"))
  float Mitigation = 0.5f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Available Weapons"))
  TArray<TObjectPtr<const class UWeaponDataAsset>> m_lUnlockedWeapons;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Available Consumables"))
  TArray<TObjectPtr<const class UConsumableDataAsset>> m_lUnlockedConsumables;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = LevelData, meta = (DisplayName = "Level Description"))
  FText m_sDescription;
};