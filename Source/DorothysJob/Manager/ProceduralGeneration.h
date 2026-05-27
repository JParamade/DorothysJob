/************************************************************************
 * @description: Handles the procedural generation of the levels
 * @author: Pablo Velasco
 * @date: 01/07/2025
 * @edited_by: Josephine - 08/08/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Data/DataTables/LevelData.h" // this include is necessary because the struct that has inside of it
#include "ProceduralGeneration.generated.h"

#pragma region | Forward Declarations
struct FRoomData;
struct FProceduralData;
class GraphNode;
class UBaseGameInstance;
class RandomNumberGenerator;
class Graph;
class Quadrant;
class Door;
class UBaseRoom;
class ULevelStreamingDynamic;
class ULevelManager;
enum class ERoomType : uint8;
#pragma endregion

DECLARE_MULTICAST_DELEGATE(FOnSpawnersReady);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLevelReady, UBaseRoom*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerSpawn);

USTRUCT(BlueprintType)
struct FEndlessValues
{
  GENERATED_BODY()

  int32 CurrentLessInitialTime = 0;

  int32 CurrentExtraQuads = 0;

  float CurrentSpawnerMultiplier = 1.0f;

  float CurrentExtraCleaningCupboardTime = 0.0f;
};

static FEndlessValues CurrentEndlessValues;

UCLASS()
class DOROTHYSJOB_API UProceduralGeneration : public UObject
{
  GENERATED_BODY()
public:
  UFUNCTION(BlueprintCallable)
  void GenerateLayout(int32 _LevelToLoad);

  virtual UWorld* GetWorld() const override;

  /**
   * @brief The event delegate used to start the timer and load the GUI
   */
  UPROPERTY(BlueprintAssignable)
  FOnPlayerSpawn OnPlayerSpawn;

  static FOnLevelReady OnLevelReady;
  static FOnSpawnersReady OnSpawnersReady;

private:
#pragma region | Variables
  UBaseGameInstance* pGameInstance;

  TSubclassOf<AActor> m_oCleaningStationClass;

  TArray<FRoomData*> m_pAllRoomRows;

  TArray<FLevelData*> m_pLevelData;

  TArray<FProceduralData*> m_pProceduralData;

  TArray<TSharedPtr<GraphNode>> m_lAllRooms;

  TArray<FAvailableRoom> m_lAvailableRoomTypes;

  TSharedPtr<GraphNode> m_pRootNode;

  TSharedPtr<RandomNumberGenerator> m_pRandom;

  TSharedPtr<Graph> m_pGraph;

  TArray<FIntPoint> CleaningStationLocations;

  TMap<ERoomType, float> m_mTypeWeights;

  TMap<TWeakObjectPtr<ULevelStreamingDynamic>, TSharedPtr<GraphNode>> PendingRooms;

  TObjectPtr<ULevelManager> m_pLevelManager;

  bool bFail = false;

  bool bIsLayoutValid = false;

  bool bLaxRules = false;

  bool bEndlessMode = false;

  float MinDistanceBetweenCleaningStationsSqr = 3.0f;

  float m_fTotalProceduralWeight = 0.0f;

  float m_fBaseDeactivateRoomByBuff = 0.0f;

  float m_fMitigation = 0.0f;

  int32 CurrentLevel;

  int32 MinQuadrantNumber = 5;

  int32 MaxQuadrantNumber = 15;

  int32 Minutes = 1;

  int32 Seconds = 30;

  int32 CleaningStationToAdd = 3;

  int32 MaxPlacedQ1 = 5;

  int32 MaxTriesPerNode = 20;

  int32 MaxTriesAddingQ1 = 20;

  int32 PlayerRoomStart = 0;

  int32 StencilIndex = 99;

  FIntPoint PlayerStartPosition;
#pragma endregion

#pragma region | Tutorial Methods

  void CreateTutorialLevel();

  void InitializeRoomNodesTutorial();

  bool LoadTutorialData();
#pragma endregion

#pragma region | Private Methods
  bool InitializeLevel();

  bool LoadRoomData(); // @REFACTOR: this loads all the DT in the memory, maybe it needs an update

  bool LoadLevelData(); // @REFACTOR: this loads all the DT in the memory, maybe it needs an update

  bool LoadProceduralData(); // @REFACTOR: this loads all the DT in the memory, maybe it needs an update

  void InitializeRandomGenerator();

  void InitializeRoomNodes();

  void ResetGraph();

  TArray<ERoomType> GetRemainingTypes() const;

  TSharedPtr<GraphNode> GetRandomRoomNode() const;

  TSharedPtr<GraphNode> GetRandomQ1() const;

  TSharedPtr<GraphNode> GetRoomNode(int32 Index) const;

  void SelectAndPlaceRootNode();

  void BuildGraphConnections();

  bool IsLayoutValid();

  bool IsNodeDone(const TSharedPtr<GraphNode>& Node) const;

  bool HasMinConnections(const TSharedPtr<GraphNode>& Node) const;

  bool HasMinExternalConnections(const TSharedPtr<GraphNode>& Node)const;

  bool TryConnect(TSharedPtr<GraphNode> NodeA, TSharedPtr<GraphNode> NodeB);

  void TryConnectPlacedRooms();

  bool TryConnectStatic(TSharedPtr<GraphNode> NodeA, TSharedPtr<GraphNode> NodeB);

  bool IsValidPlacement(TSharedPtr<GraphNode> Node);

  void UpdateQuadrantsAndDoors(TSharedPtr<GraphNode> Node, TSharedPtr<Quadrant> QuadrantToUpdate, TSharedPtr<Door> DoorToUpdate);

  bool FindCompatibleDoor(TSharedPtr<Quadrant> QuadrantA, int32 DirectionA, TSharedPtr<Quadrant> QuadrantB, int32 DirectionB);

  FIntPoint GetDoorDirection(FIntPoint VectorBetweenDoors);

  bool FinalizeAndValidateLayout();

  bool GenerateQ1Extra();

  bool AddPlayerSpawnerRoom();

  bool GenerateCleaningStations();

  float DistanceSqr(const FIntPoint& PointA, const FIntPoint& PointB) const;

  bool HasMinimumDistance(const FIntPoint& NewPoint) const;

  void ExportAndDraw();

  void DrawLayout();

  void SpawnCleaningStations();

  UFUNCTION()
  void HandleLevelShown();

  float GetEnemyPercentage(const TSharedPtr<GraphNode>& Node) const;

  TSharedPtr<FSpawnChances> GetEnemySpawnChances(const TSharedPtr<GraphNode>& Node, float& OutSpawnerPercentage) const;

  void SetupNeighbors() const;

  void SetupAdjacents() const;

  UBaseRoom* GetRoomFromNode(TSharedPtr<GraphNode> Node) const;

  void SpawnPlayer() const;

  void UpdateEndlessValues();

  void UpdateInfiniteValues();

  bool UpdateFiniteValues();

  void SubtractSeconds(int32 _DeltaSeconds, int32 _MinMinutes, int32 _MinSeconds);
#pragma endregion
};
