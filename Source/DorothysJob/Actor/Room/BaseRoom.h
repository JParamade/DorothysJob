/************************************************************************
 * @description: Represents the Base Room in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Josephine Esposito - 11/04/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "../BaseActor.h"
#include "BaseRoom.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFloorCleaned, UBaseRoom*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGiveCleanReward, int32, m_iRewardPoints);
DECLARE_MULTICAST_DELEGATE(FOnDirtStageChanged);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBuffEnemies, UBaseRoom*);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnCanPoop, bool, UWorld*);

#pragma region | Forward Declarations
enum class EPoolType : uint8;
enum class ERoomType : uint8;

class UStaticMeshComponent;
class UConnectionPoint;
class ABaseTile;
class ABaseSpawnPoint;
class ABaseEnemy;
class ABaseCharacter;
class UTimeManager;
class ADissolvableActor;
class ADissolvableDoor;
class Quadrant;
class UEnemiesRoomManager;
class UPoopRoomManager;
class ADustPoop;
class UNavigationSystemV1;
class UCleanRoomData;
class ULevelManager;
class UBaseGameInstance;
class AMrsCleanActor;
class ABasePlayer;
struct FQuadrantData;
struct FCharactersByDistance;
struct FSpawnChances;
struct FEventDataArray;
#pragma endregion

#pragma region | Enumerators
/**
 * @brief Represents the Types of Rooms
 */
UENUM(BlueprintType)
enum class ERoomShape : uint8
{

  Q1      UMETA(DisplayName = "Q1"),

  R2      UMETA(DisplayName = "R2"),

  R3      UMETA(DisplayName = "R3"),

  L3      UMETA(DisplayName = "L3"),

  L4      UMETA(DisplayName = "L4"),

  S       UMETA(DisplayName = "S"),

  T       UMETA(DisplayName = "T"),

  Q4      UMETA(DisplayName = "Q4"),

};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
  OOZER  UMETA(DisplayName = "OOZER"),

  MINI_OOZER  UMETA(DisplayName = "Mini Oozer"),

  OOZER_DOUBLE  UMETA(DisplayName = "Oozer Double"),

  DUST_BUNNY  UMETA(DisplayName = "Dust Bunny"),

  ASH_BUNNY  UMETA(DisplayName = "Ash Bunny"),

  GRIMELING  UMETA(DisplayName = "Grimeling")
};

UENUM(BlueprintType)
enum class EDirtStage : uint8
{
  /**
   * @brief The room's dirtyness is 0 and/or there are no enemies.
   */
  CLEAN  UMETA(DisplayName = "Clean"),

  /**
   * @brief The room is dirty but under the cleanable threshold, in case there are no enemies as well the room will be cleaned automatically.
   */
  CLEANABLE  UMETA(DisplayName = "Cleanable"),

  /**
   * @brief The room's dirtyness is between both thresholds, so it doesn't buff neither clean automatically.
   */
  DIRTY  UMETA(DisplayName = "Dirty"),

  /**
   * @brief The room's dirtyness is higher than the Buff threshold so the enemies get a buff.
   */
  BUFFABLE  UMETA(DisplayName = "Buffable")
};

UENUM(BlueprintType)
enum class ERoomState : uint8
{
  /**
   * @brief The room that the player is in right now
   */
  CURRENT  UMETA(DisplayName = "Clean"),

  /**
   * @brief Neightbor of the  current  room
   */
  CLOSE  UMETA(DisplayName = "Cleanable"),

  /**
   * @brief The room is not neightbor of the current room
   */
  FAR  UMETA(DisplayName = "Dirty")
};
#pragma endregion

UCLASS(BlueprintType, Blueprintable)
class DOROTHYSJOB_API UBaseRoom : public UObject
{
  GENERATED_BODY()
public:
#pragma region | Initializers
  UBaseRoom();

  void Init(ULevel* Level, ERoomType RoomType, FVector Location, int32 Id, float DirtyArea, float EnemiesSpawnPercentage, float Mitigation, TArray<TSharedPtr<Quadrant>> NodeQuadrants, float _fBaseDeactivateRoomByBuff, TSharedPtr<FSpawnChances> _SpawnChances, bool _bHasCleaningStation);
#pragma endregion

#pragma region | Setters
  UFUNCTION(BlueprintCallable)
  void SetLocation(const FVector& _NewLocation);

  void SetRotation(const float _NewRotation);

  void SetRoomShape(ERoomShape _NewShape);

  void SetLevel(ULevel* _NewLevel);

  void SetRoomType(ERoomType _NewLevelType);

  void SetLevelId(int32 _NewLevelId);

  void SetStencilId(int32 _NewStencilId);
#pragma endregion

#pragma region | Getters
  UFUNCTION(BlueprintCallable)
  const FVector& GetLocation() const;

  const float GetRotation() const;

  const float GetDirtness() const;

  const ERoomShape GetRoomShape() const;

  const ULevel* GetLevel() const;

  const ERoomType GetRoomType() const;

  const int32 GetLevelId() const;

  const int32 GetStencilId() const;

  const bool IsCurrent() const;

  const bool HasBeenExplored() const;

  const bool CanShowVFX() const;

  const EDirtStage& GetDirtStage() const;

  UFUNCTION(BlueprintCallable)
  const EPoolType& GetSpawnMode() const;

  bool HasCleaningStation() const;

  ADustPoop* RequestPoop(const AActor* _Enemy) const;

  AActor* GetTarget(const AActor* _Character) const;

  bool IsPoopNear(const AActor* _Pooper);

  UFUNCTION(BlueprintCallable)
  TArray<ABaseEnemy*> GetEnemies(ABaseCharacter* ReferenceCharacter);

  TArray<ADustPoop*> GetDustPoops();
#pragma endregion

#pragma region | Registers
  void AddNeighbor(UBaseRoom* NeighborToAdd);
  void AddNonNeighbor(UBaseRoom* NonNeighborToAdd);
  void AddAdjacent(UBaseRoom* Adjacent);
  void RegisterSpawner(ABaseSpawnPoint* _Spawner);
  void RegisterEnemy(ABaseEnemy* _Enemy);
  void RegisterTile(ABaseTile* _Tile);
  void RegisterDissolvableActor(ADissolvableActor* _DissolvableActor);
  void RegisterDoor(ADissolvableDoor* _DissolvableDoor);
  void RegisterDustPoop(ADustPoop* _DustPoop);
  void RegisterDecoy(AMrsCleanActor* _Decoy);
#pragma endregion

#pragma region | UnRegisters
  void UnRegisterSpawner(ABaseSpawnPoint* _Spawner);
  void UnRegisterEnemy(ABaseEnemy* _Enemy);
  void UnRegisterTile(ABaseTile* Tile);

  UFUNCTION()
  void UnRegisterDecoy(AMrsCleanActor* _Decoy);
#pragma endregion
  // ============== DELETE THIS AFTER ALPHA ======================
  void GetTilesAndEnemies();

  void NonProceduralInit(int32 ID, FVector Position, float Rotation, ERoomShape Shape);
  // =============================================================

  //delegate subscription
  UFUNCTION()
  void OnEnemyEliminate(ABaseEnemy* EnemyEliminated);

  UFUNCTION()
  void OnDustPoopEliminate(ADustPoop* DustPoopEliminated);

  static FOnFloorCleaned OnFloorCleaned;

  static FOnDirtStageChanged OnDirtStageChanged;

  FOnBuffEnemies OnEnemiesBuff;
  FOnBuffEnemies OnEnemiesDebuff;

  FOnCanPoop OnCanPoop;

  UPROPERTY(BlueprintAssignable, Category = "Bola 13|Room")
  FOnGiveCleanReward OnGiveCleanReward;
  // =============================================================

  bool RequestToken(ABaseEnemy* _enemyRequester);
  bool ReturnToken(ABaseEnemy* _enemyReturner);

  bool RequestGrimelingToken(ABaseEnemy* _enemyRequester);
  bool ReturnGrimelingToken(ABaseEnemy* _enemyReturner);

  bool RequestDustPooken(ABaseEnemy* _enemyRequester);
  bool ReturnDustPooken(ABaseEnemy* _enemyReturner);

  bool AreAvailableTokens();
  bool AreAvailableGrimelingTokens();
  bool CanDustPoop(const AActor* _pDustBunny);

  UFUNCTION()
  void OnRoomEnter(UBaseRoom* EnteredRoom);

  UFUNCTION()
  void OnRoomExit(UBaseRoom* EnteredRoom);

  UFUNCTION()
  void SetupUpdatePercentageTimer();

  FCharactersByDistance GetCharactersByDistance(ABaseCharacter* _RefereceCharacter, float _AcceptableDistance, TArray<TSubclassOf<ABaseCharacter>> _ActorClasesToIgnore = {});

  TArray<TObjectPtr<ADissolvableDoor>> m_lDissolvableDoors;
protected:
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Room", meta = (DisplayName = "Dirt Percentage"))

  float m_fDirtPercentage;
private:
#pragma region | Variables
  TObjectPtr<UWorld> CachedWorld;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Level"))
  TObjectPtr<ULevel> m_pLevel;

  /**
  * @brief This represents the type of the room
  */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Room Type"))
  ERoomType m_eRoomType;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Room Shape"))
  ERoomShape m_eRoomShape;

  UPROPERTY(VisibleAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Room Global Position"))
  FVector m_v3Location;

  UPROPERTY(VisibleAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Room Global Rotation"))
  float m_fRotation;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "ID"))
  int32 m_iId;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "StencilID"))
  int32 m_iStencilID = -1;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Tiles"))
  TArray<TObjectPtr<ABaseTile>> m_lTiles;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Spawn Points"))
  TArray<TObjectPtr<ABaseSpawnPoint>> m_lSpawnPoints;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Enemies"))
  TArray<TObjectPtr<ABaseEnemy>> m_lEnemies;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "DustPoop"))
  TArray<TObjectPtr<ADustPoop>> m_lDustPoops;

  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors0;
  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors45;

  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors90;
  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors135;

  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors180;
  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors225;

  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors270;
  TArray<TObjectPtr<ADissolvableActor>> m_lDissolvableActors315;



  TArray<TObjectPtr<AMrsCleanActor>> m_lDecoys;

  TArray<TSharedPtr<Quadrant>> Quadrants;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Room", meta = (DisplayName = "Enemies Spawn Percentage"))

  float m_fEnemiesSpawnPercentage;

  float m_fDirtyArea = 1.0f;

  float m_fBaseDeactivateRoomByBuff = 0.3f;

  float m_fPercentageAmountPerCleanRoom = -1.0f;

  float m_fMitigation = 0.5f;

  UPROPERTY()
  FTimerHandle DelayUpdateDirtnessHandle;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Enemies")
  TObjectPtr<UEnemiesRoomManager> m_pEnemyManagerComponent;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Enemies")
  TObjectPtr<UPoopRoomManager> m_pPoopManagerComponent;

  TObjectPtr<UBaseGameInstance> pGameInstance;

  TSubclassOf<ABaseEnemy> OozerClass;

  TSubclassOf<ABaseEnemy> OozerDoubleClass;

  TSubclassOf<ABaseEnemy> MiniOozerClass;

  TSubclassOf<ABaseEnemy> DustBunnyClass;

  TSubclassOf<ABaseEnemy> AshBunnyClass;

  TSubclassOf<ABaseEnemy> GrimelingClass;

  EDirtStage m_eDirtStage = EDirtStage::DIRTY;

  ERoomState m_eRoomState = ERoomState::FAR;

  bool bSpawnedAlready = false;

  TArray<TObjectPtr<UBaseRoom>> m_lNeighbors;

  TArray<TObjectPtr<UBaseRoom>> m_lOtherRooms;

  TArray<TObjectPtr<UBaseRoom>> m_lAdjacentRooms;

  bool bIsCurrent = false;

  bool bHasBeenExplored = false;

  bool bHasCleaningStation = false;

  bool bCanPoop = true;

  TObjectPtr<UTimeManager> TimeManager;

  TObjectPtr<UNavigationSystemV1> m_pNavSys;

  TObjectPtr<UCleanRoomData> m_pCleanRoomData;

  TObjectPtr<ULevelManager> m_pLevelManager;

  TSharedPtr<FSpawnChances> m_pSpawnChances = nullptr;

  bool bCanActivateCleanRoomHUD = true;

  FEventDataArray* OozerEvent;

  FEventDataArray* DustBunnyEvent;

  FEventDataArray* GrimelingEvent;

  FEventDataArray* OozerDoubleEvent;

  FEventDataArray* AshBunnyEvent;

  FEventDataArray* DirtRoomEvent;

  FEventDataArray* CleanRoomEvent;

  TMap<UClass*, FName> EnemyEventMap;

  TWeakObjectPtr<ABasePlayer> m_pPlayer;
#pragma endregion

#pragma region | Private Functions
  void UpdateDirtPercentage();

  void SpawnEnemies();

  ABaseEnemy* SpawnEnemy(UClass* EnemyClass, FVector Location);

  template<typename T>
  TArray<T*> PickRandom(const TArray<T*>& Source, int32 Count);

  template <typename T>
  T* FindClosestActor(
    const AActor* ReferenceActor,
    const TArray<T*>& Actors,
    TFunctionRef<bool(const T*)> IsValid = [](const T* Actor) { return Actor != nullptr; }
  ) const;

  void CleanupSpawners();

  UClass* GetEnemyClass(EEnemyType EnemyType);

  void SetupEnemiesClasses();

  void SetupRewards();

  void SetupEvents();

  FEventDataArray* GetEventByName(FName NameEvent);

  void UpdateDirtStage();

  void ApplyDirtStage(EDirtStage OldStage, EDirtStage NewStage);

  void CleanAllFloors(bool bCanRewardPlayer = true);

  float CalculateAmountPerCleanRoom() const;

  void RewardPlayer();

  void AddExtraTime();

  void ChargeSpecial();

  void HealPlayer();

  void AddConsumableCharge();

  void WithdrawEnemiesBuff();

  void BuffEnemies();

  int CalculateTimeToAdd() const;

  FQuadrantData GetQuadrantData() const;

  EDirtStage CalculateNewDirtPercentage() const;

  bool HasCompatibleClosedDoor(ADissolvableDoor* _DissolvableDoor, TSharedPtr<Quadrant> Quadrant) const;

  void HideSelfAndNeighbors();

  void HandleConcaveCorners();

  void HandleConcaveCorners0();

  void HandleConcaveCorners90();

  void HandleConcaveCorners180();

  void HandleConcaveCorners270();

  void SetRoomState(ERoomState _NewRoomState);

  void SetActiveRoom();

  void SetMediumActiveRoom();

  void SetUnactiveRoom();

  void SetAllRoomState(TArray<TObjectPtr<UBaseRoom>> RoomsToSet, ERoomState _NewRoomState);

  bool CheckPoopRequestConditions() const;

  int32 OccupiedPoopCount() const;

  ADustPoop* FindClosestPoop(const AActor* _Actor) const;

  ADustPoop* FindClosestPoopIgnoringHidden(const AActor* _Actor) const;

  AMrsCleanActor* FindClosestDecoy(const AActor* _Actor) const;

  void TriggerEvent(FName _TriggerName) const;
#pragma endregion
};

template<typename T>
inline TArray<T*> UBaseRoom::PickRandom(const TArray<T*>& Source, int32 Count)
{
  TArray<T*> Copy = Source;
  TArray<T*> Out;
  for (int i = 0; i < Count && Copy.Num() > 0; ++i)
  {
    int32 Idx = FMath::RandRange(0, Copy.Num() - 1);
    Out.Add(Copy[Idx]);
    Copy.RemoveAt(Idx);
  }
  return Out;
}

template<typename T>
inline T* UBaseRoom::FindClosestActor(const AActor* ReferenceActor, const TArray<T*>& Actors, TFunctionRef<bool(const T*)> IsValid) const
{
  T* ClosestActor = nullptr;
  float MinDistanceSqr = FLT_MAX;

  if (!ReferenceActor)
  {
    return nullptr;
  }

  const FVector RefLocation = ReferenceActor->GetActorLocation();

  for (T* Actor : Actors)
  {
    if (!IsValid(Actor))
    {
      continue;
    }

    const float DistSqr = FVector::DistSquared(RefLocation, Actor->GetActorLocation());

    if (DistSqr < MinDistanceSqr)
    {
      MinDistanceSqr = DistSqr;
      ClosestActor = Actor;
    }
  }

  return ClosestActor;
}