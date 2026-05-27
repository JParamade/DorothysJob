/************************************************************************
 * @description: Represents the Base Room in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Josephine Esposito - 11/04/2025
 ************************************************************************/

#include "BaseRoom.h"
#include "../Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Room/Tile/BaseTile.h"
#include "DorothysJob/Actor/Spawner/BaseSpawnPoint.h"
#include "DorothysJob/Actor/Character/Enemy/DustBunny.h"
#include "DorothysJob/Actor/Character/Enemy/AshBunny.h"
#include "DorothysJob/Actor/Character/Enemy/SlimeEnemy.h"
#include "DorothysJob/Actor/Character/Enemy/DividerSlime.h"
#include "DorothysJob/Actor/Character/Enemy/Goblin.h"
#include "DorothysJob/Actor/Dust/DustPoop.h"
#include "DorothysJob/Actor/Dissolvable/DissolvableActor.h"
#include "DorothysJob/Actor/Dissolvable/DissolvableDoor.h"
#include "DorothysJob/Utils/Random/RandomNumberGenerator.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Data/DataAssets/EnemyDataAsset.h"
#include "DorothysJob/Manager/ProceduralGeneration.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/WorldSubsystem/TimeManager.h"
#include "DorothysJob/Component/Room/EnemiesRoomManager.h"
#include "DorothysJob/Component/Room/PoopRoomManager.h"
#include "DorothysJob/Utils/DataStructure/Graph/Quadrant.h"
#include "DorothysJob/Utils/DataStructure/Graph/Door.h"
#include "DorothysJob/Data/DataAssets/Procedural/CleanRoomData.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Utils/Structs/CharactersByDistance.h"
#include "DorothysJob/Actor/Consumables/MrsCleanActor.h"
#include "Kismet/GameplayStatics.h"
#include "FMODBlueprintStatics.h"
#include "NavigationSystem.h"
#include "Algo/Count.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Actor/Tutorial/Trigger.h"
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Data/DataAssets/Enemy/EnemyPropertiesDataAsset.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

FOnFloorCleaned UBaseRoom::OnFloorCleaned;
FOnDirtStageChanged UBaseRoom::OnDirtStageChanged;

#pragma region | Initializers
UBaseRoom::UBaseRoom()
{
  m_pEnemyManagerComponent = CreateDefaultSubobject<UEnemiesRoomManager>(TEXT("Enemy Manager Component"));
  m_pPoopManagerComponent = CreateDefaultSubobject<UPoopRoomManager>(TEXT("Dust Poop Manager Component"));
}

void UBaseRoom::Init(ULevel* Level, ERoomType RoomType, FVector Location, int32 Id, float DirtyArea, float EnemiesSpawnPercentage, float Mitigation, TArray<TSharedPtr<Quadrant>> NodeQuadrants, float _fBaseDeactivateRoomByBuff, TSharedPtr<FSpawnChances> _SpawnChances, bool _bHasCleaningStation)
{
  m_pLevel = Level;
  m_eRoomType = RoomType;
  m_v3Location = Location;
  m_iId = Id;
  m_fEnemiesSpawnPercentage = EnemiesSpawnPercentage;
  m_pSpawnChances = _SpawnChances;
  m_fMitigation = Mitigation;
  m_eDirtStage = EDirtStage::DIRTY;
  m_fDirtPercentage = 0.0f;
  m_fBaseDeactivateRoomByBuff = _fBaseDeactivateRoomByBuff;
  Quadrants = NodeQuadrants;
  m_fDirtyArea = DirtyArea;
  bHasCleaningStation = _bHasCleaningStation;
  SetupEnemiesClasses();
  SetupRewards();
  CachedWorld = GetWorld();

  if (!CachedWorld && GetOuter())
  {
    CachedWorld = GetOuter()->GetWorld();
  }

  if (CachedWorld)
  {
    TimeManager = CachedWorld->GetSubsystem<UTimeManager>();
    m_pLevelManager = CachedWorld->GetSubsystem<ULevelManager>();

    if (IsValid(m_pLevelManager))
    {
      m_pLevelManager->OnEnteredRoom.AddDynamic(this, &UBaseRoom::OnRoomEnter);
      m_pLevelManager->OnExitedRoom.AddDynamic(this, &UBaseRoom::OnRoomExit);
      m_pLevelManager->OnAllRoomsCreated.AddDynamic(this, &UBaseRoom::SetupUpdatePercentageTimer);
    }

    m_pNavSys = UNavigationSystemV1::GetCurrent(CachedWorld);
  }

  UProceduralGeneration::OnSpawnersReady.AddUObject(this, &UBaseRoom::SpawnEnemies);
  if (IsValid(m_pPoopManagerComponent))
  {
    m_pPoopManagerComponent->InitPookens(CachedWorld);
    OnCanPoop.AddUObject(m_pPoopManagerComponent, &UPoopRoomManager::RoomDirtLevel);
  }

  SetupEvents();
  if (IsValid(m_pEnemyManagerComponent))
  {
    m_pEnemyManagerComponent->InitTokens(CachedWorld);
  }
}
#pragma endregion

#pragma region | Setters
void UBaseRoom::SetLocation(const FVector& _NewLocation)
{
  m_v3Location = _NewLocation;
}

void UBaseRoom::SetRotation(const float _NewRotation)
{
  m_fRotation = _NewRotation;
}

void UBaseRoom::SetRoomShape(ERoomShape _NewShape)
{
  m_eRoomShape = _NewShape;
}

void UBaseRoom::SetLevel(ULevel* _NewLevel)
{
  m_pLevel = _NewLevel;
}
void UBaseRoom::SetRoomType(ERoomType _NewRoomType)
{
  m_eRoomType = _NewRoomType;
}
void UBaseRoom::SetLevelId(int32 _NewLevelId)
{
  m_iId = _NewLevelId;
}
void UBaseRoom::SetStencilId(int32 _NewStencilId)
{
  m_iStencilID = _NewStencilId;
}
#pragma endregion

#pragma region | Getters
const FVector& UBaseRoom::GetLocation() const
{
  return m_v3Location;
}

const float UBaseRoom::GetRotation() const
{
  return m_fRotation;
}

const float UBaseRoom::GetDirtness() const
{
  return m_fDirtPercentage;
}

const ERoomShape UBaseRoom::GetRoomShape() const
{
  return m_eRoomShape;
}

const ULevel* UBaseRoom::GetLevel() const
{
  return m_pLevel;
}
const ERoomType UBaseRoom::GetRoomType() const
{
  return m_eRoomType;
}
const int32 UBaseRoom::GetLevelId() const
{
  return m_iId;
}
const int32 UBaseRoom::GetStencilId() const
{
  return m_iStencilID;
}
const bool UBaseRoom::IsCurrent() const
{
  return bIsCurrent;
}
const bool UBaseRoom::HasBeenExplored() const
{
  return bHasBeenExplored;
}
const bool UBaseRoom::CanShowVFX() const
{
  return bCanActivateCleanRoomHUD;
}
const EDirtStage& UBaseRoom::GetDirtStage() const
{
  return m_eDirtStage;
}
const EPoolType& UBaseRoom::GetSpawnMode() const
{
  return m_pSpawnChances->m_ePoolType;
}
bool UBaseRoom::HasCleaningStation() const
{
  return bHasCleaningStation;
}

ADustPoop* UBaseRoom::RequestPoop(const AActor* _Enemy) const
{
  return CheckPoopRequestConditions() ? FindClosestPoop(_Enemy) : nullptr;
}

AActor* UBaseRoom::GetTarget(const AActor* _Character) const
{
  if (m_lDecoys.Num() > 0)
  {
    return FindClosestDecoy(_Character);
  }

  // if current level + no decoys in room we must return player
  return (IsValid(m_pLevelManager) && IsValid(m_pLevelManager->GetCurrentRoom()) && m_pLevelManager->GetCurrentRoom()->GetLevelId() == m_iId) ? m_pPlayer.Get() : nullptr;
}
bool UBaseRoom::IsPoopNear(const AActor* _pPooper)
{
  ADustPoop* pNearestPoop = FindClosestPoopIgnoringHidden(_pPooper);
  float fMinDist = 500.f;
  if (IsValid(m_pPoopManagerComponent))
  {
    fMinDist = m_pPoopManagerComponent->GetMinDPDistance();
  }
  if (pNearestPoop)
  {
    return ((FVector::DistSquared(_pPooper->GetActorLocation(), pNearestPoop->GetActorLocation())) <= (fMinDist * fMinDist));
  }
  return false;
}
TArray<ABaseEnemy*> UBaseRoom::GetEnemies(ABaseCharacter* ReferenceCharacter)
{
  TArray<ABaseEnemy*> Result;

  const auto& CloseEnemies = GetCharactersByDistance(ReferenceCharacter, 50000.0f).m_lCloseEnemies;

  for (const TWeakObjectPtr<ABaseEnemy>& WeakEnemy : CloseEnemies)
  {
    if (ABaseEnemy* Enemy = WeakEnemy.Get())
    {
      Result.Add(Enemy);
    }
  }

  return Result;
}
TArray<ADustPoop*> UBaseRoom::GetDustPoops()
{
  return m_lDustPoops;
}
#pragma endregion

#pragma region | Registers
void UBaseRoom::AddNeighbor(UBaseRoom* NeighborToAdd)
{
  if (!IsValid(NeighborToAdd))
  {
    return;
  }
  m_lNeighbors.Add(NeighborToAdd);
}
void UBaseRoom::AddNonNeighbor(UBaseRoom* NonNeighborToAdd)
{
  if (!IsValid(NonNeighborToAdd))
  {
    return;
  }
  m_lOtherRooms.Add(NonNeighborToAdd);
}
void UBaseRoom::AddAdjacent(UBaseRoom* Adjacent)
{
  if (!IsValid(Adjacent))
  {
    return;
  }
  m_lAdjacentRooms.Add(Adjacent);
}
void UBaseRoom::RegisterSpawner(ABaseSpawnPoint* _Spawner)
{
  if (!IsValid(_Spawner))
  {
    return;
  }
  m_lSpawnPoints.Add(_Spawner);
}

void UBaseRoom::RegisterEnemy(ABaseEnemy* _Enemy)
{
  if (!IsValid(_Enemy))
  {
    return;
  }
  _Enemy->OnEnemyEliminate.AddDynamic(this, &UBaseRoom::OnEnemyEliminate);
  m_lEnemies.Add(_Enemy);
}

void UBaseRoom::RegisterTile(ABaseTile* _Tile)
{
  if (!IsValid(_Tile))
  {
    return;
  }
  if (m_iStencilID == -1)
  {
    m_iStencilID = _Tile->GetStaticMesh()->CustomDepthStencilValue;
  }
  m_lTiles.Add(_Tile);
}

void UBaseRoom::RegisterDissolvableActor(ADissolvableActor* _DissolvableActor)
{
  if (!IsValid(_DissolvableActor))
  {
    return;
  }

  double YawDouble = _DissolvableActor->GetActorRotation().Yaw;
  int32 Yaw = FMath::RoundToInt32(YawDouble);
  int32 Turns = (((int32)(Yaw) % 360) + 360) % 360 / 45;

  switch (Turns)
  {
  case 0:
    m_lDissolvableActors0.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT0;
    break;
  case 1:
    m_lDissolvableActors45.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT45;
    break;
  case 2:
    m_lDissolvableActors90.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT90;
    break;
  case 3:
    m_lDissolvableActors135.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT135;
    break;
  case 4:
    m_lDissolvableActors180.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT180;
    break;
  case 5:
    m_lDissolvableActors225.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT225;
    break;
  case 6:
    m_lDissolvableActors270.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT270;
    break;
  case 7:
    m_lDissolvableActors315.Add(_DissolvableActor);
    _DissolvableActor->Rotation = ERotation::ROT315;
    break;
  default:
    _DissolvableActor->Rotation = ERotation::NOE;
    break;
  }
}

void UBaseRoom::RegisterDoor(ADissolvableDoor* _DissolvableDoor)
{
  if (!IsValid(_DissolvableDoor))
  {
    return;
  }

  _DissolvableDoor->SetRoom(this);
  for (const TSharedPtr<Quadrant>& Quadrant : Quadrants)
  {
    if (HasCompatibleClosedDoor(_DissolvableDoor, Quadrant))
    {
      _DissolvableDoor->CloseDoor();
    }
    else
    {
      if (!m_lDissolvableDoors.Contains(_DissolvableDoor))
      {
        m_lDissolvableDoors.Add(_DissolvableDoor);
      }
    }
  }
}

void UBaseRoom::RegisterDustPoop(ADustPoop* _DustPoop)
{
  if (!IsValid(_DustPoop))
  {
    return;
  }
  _DustPoop->OnDustPoopEliminate.AddDynamic(this, &UBaseRoom::OnDustPoopEliminate);
  m_lDustPoops.Add(_DustPoop);
}

void UBaseRoom::RegisterDecoy(AMrsCleanActor* _Decoy)
{
  if (!IsValid(_Decoy))
  {
    return;
  }
  _Decoy->OnDecoyDestroyed.AddUObject(this, &UBaseRoom::UnRegisterDecoy);
  m_lDecoys.Add(_Decoy);
}
#pragma endregion

#pragma region | Unegisters
void UBaseRoom::UnRegisterSpawner(ABaseSpawnPoint* _Spawner)
{
  if (!IsValid(_Spawner))
  {
    return;
  }
  m_lSpawnPoints.Remove(_Spawner);
}

void UBaseRoom::UnRegisterEnemy(ABaseEnemy* _Enemy)
{
  if (!IsValid(_Enemy))
  {
    return;
  }
  m_lEnemies.Remove(_Enemy);
}

void UBaseRoom::UnRegisterTile(ABaseTile* _Tile)
{
  if (!IsValid(_Tile))
  {
    return;
  }
  m_lTiles.Remove(_Tile);
}

void UBaseRoom::UnRegisterDecoy(AMrsCleanActor* _Decoy)
{
  if (!IsValid(_Decoy))
  {
    return;
  }
  m_lDecoys.Remove(_Decoy);
}

void UBaseRoom::GetTilesAndEnemies()
{
  FName TagName = *FString::FromInt(m_iId);
  TArray<AActor*> Found;
  if (CachedWorld)
  {
    UGameplayStatics::GetAllActorsWithTag(CachedWorld, TagName, Found);
  }

  for (AActor* Actor : Found)
  {
    if (!IsValid(Actor))
    {
      continue;
    }

    if (ABaseTile* Tile = Cast<ABaseTile>(Actor))
    {
      m_lTiles.Add(Tile);
      continue;
    }

    if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(Actor))
    {
      m_lEnemies.Add(Enemy);
      Enemy->OnEnemyEliminate.AddDynamic(this, &UBaseRoom::OnEnemyEliminate);
      Enemy->SetCurrentRoom(this);
      continue;
    }
  }
}

void UBaseRoom::NonProceduralInit(int32 ID, FVector Position, float Rotation, ERoomShape Shape)
{
  m_v3Location = Position;
  m_fRotation = Rotation;
  m_eRoomShape = Shape;
  m_iId = ID;
  m_pEnemyManagerComponent->SetAllTokensToMinimum(CachedWorld);
  GetTilesAndEnemies();
  CachedWorld = GetWorld();
  if (CachedWorld)
  {
    TimeManager = CachedWorld->GetSubsystem<UTimeManager>();

    if (m_eRoomShape == ERoomShape::Q1)
    {
      m_eDirtStage = EDirtStage::CLEAN;

      for (ABaseTile* Tile : m_lTiles)
      {
        Tile->CleanFloor();
        m_eDirtStage = EDirtStage::CLEAN;
      }
    }
    else
    {
      CachedWorld->GetTimerManager().SetTimer(
        DelayUpdateDirtnessHandle,
        this,
        &UBaseRoom::UpdateDirtPercentage,
        0.5f,
        /*bLoop=*/true
      );
    }
  }
}

void UBaseRoom::OnEnemyEliminate(ABaseEnemy* EnemyEliminated)
{
  if (!IsValid(EnemyEliminated))
  {
    return;
  }

  m_lEnemies.Remove(EnemyEliminated);

  if (m_lEnemies.Num() == 0 && IsValid(pGameInstance) && pGameInstance->m_iCurrentLevel == 0 && CleanRoomEvent && !CleanRoomEvent->m_bHasAlreadyAppear)
  {
    int32 maxHealth = m_pPlayer->GetMaxHealth();
    int32 damage = maxHealth;
    m_pPlayer->Damage(damage, EDirtType::Liquid);

    TriggerEvent(TEXT("FIRST_TIME_CleanRoom"));
  }
}

void UBaseRoom::OnDustPoopEliminate(ADustPoop* DustPoopEliminated)
{
  if (!IsValid(DustPoopEliminated))
  {
    return;
  }

  m_lDustPoops.Remove(DustPoopEliminated);

  TWeakObjectPtr<UBaseRoom> WeakThis(this);
  FTimerHandle DustPoopsDirtAgainTimer;
  CachedWorld->GetTimerManager().SetTimerForNextTick(
    [WeakThis]() {
      if (WeakThis.IsValid())
      {
        for (ADustPoop* DP : WeakThis->GetDustPoops())
        {
          if (IsValid(DP))
          {
            DP->FloorDirt();
          }
        }
      }
    }
  );
}

void UBaseRoom::OnRoomEnter(UBaseRoom* EnteredRoom)
{
  if (this != EnteredRoom || !IsValid(EnteredRoom))
  {
    return;
  }

  if (!m_pPlayer.IsValid() && IsValid(CachedWorld))
  {
    m_pPlayer = Cast<ABasePlayer>(UGameplayStatics::GetPlayerPawn(CachedWorld, 0));
  }

  if (m_eDirtStage == EDirtStage::CLEAN && m_pPlayer.IsValid() && IsValid(m_pCleanRoomData))
  {
    m_pPlayer->ChangeMovementVelocity(m_pCleanRoomData->m_fSpeedMultiplierInCleanRooms);
  }

  SetRoomState(ERoomState::CURRENT);
  SetAllRoomState(m_lNeighbors, ERoomState::CLOSE);
  SetAllRoomState(m_lOtherRooms, ERoomState::FAR);

  HideSelfAndNeighbors();

  if (IsValid(m_pEnemyManagerComponent) && CachedWorld)
  {
    m_pEnemyManagerComponent->SetAllTokensToMinimum(CachedWorld);
  }

  if (DirtRoomEvent && !DirtRoomEvent->m_bHasAlreadyAppear && m_eDirtStage == EDirtStage::BUFFABLE)
  {
    TriggerEvent(TEXT("FIRST_TIME_RedRoom"));
  }

  if (EnemyEventMap.Num() == 0)
  {
    return;
  }

  for (ABaseEnemy* Enemy : m_lEnemies)
  {
    if (!IsValid(Enemy))
    {
      continue;
    }

    if (FName* FoundEvent = EnemyEventMap.Find(Enemy->GetClass()))
    {
      TriggerEvent(*FoundEvent);
      EnemyEventMap.Remove(Enemy->GetClass());

      if (EnemyEventMap.Num() == 0)
      {
        break;
      }
    }
  }
}

void UBaseRoom::OnRoomExit(UBaseRoom* EnteredRoom)
{
  if (this != EnteredRoom || !IsValid(EnteredRoom))
  {
    return;
  }

  if (m_eDirtStage == EDirtStage::CLEAN && m_pPlayer.IsValid() && IsValid(m_pCleanRoomData))
  {
    m_pPlayer->ChangeMovementVelocity(1.0f / m_pCleanRoomData->m_fSpeedMultiplierInCleanRooms);
  }

  HideSelfAndNeighbors(); // @REFACTOR Right now it is the same as room enter, this needs a refactor

  if (IsValid(TimeManager))
  {
    TimeManager->PlayTimer();
  }

  if (IsValid(m_pEnemyManagerComponent) && CachedWorld)
  {
    m_pEnemyManagerComponent->ResetAllTokens(CachedWorld);
  }
}

void UBaseRoom::SetupUpdatePercentageTimer()
{
  if (m_eRoomShape == ERoomShape::Q1)
  {
    m_eDirtStage = EDirtStage::CLEAN;

    for (ABaseTile* Tile : m_lTiles)
    {
      Tile->CleanFloor();
      m_eDirtStage = EDirtStage::CLEAN;
    }
  }
  else if (CachedWorld)
  {
    CachedWorld->GetTimerManager().SetTimer(
      DelayUpdateDirtnessHandle,
      this,
      &UBaseRoom::UpdateDirtPercentage,
      0.5f,
      /*bLoop=*/true
    );
  }

}

FCharactersByDistance UBaseRoom::GetCharactersByDistance(ABaseCharacter* _RefereceCharacter, float _AcceptableDistance, TArray<TSubclassOf<ABaseCharacter>> _ActorClasesToIgnore)
{
  FCharactersByDistance Result;

  if (!IsValid(_RefereceCharacter))
  {
    return Result;
  }

  const FVector Location = _RefereceCharacter->GetActorLocation();
  const float AcceptableDistanceSq = FMath::Square(_AcceptableDistance);
  float DistSq = -1.0f;
  bool bIgnored = false;

  // If Player is not in the same room as  the Referenced Character it must return null in player  and empty lists
  if (IsValid(m_pLevelManager) && IsValid(m_pLevelManager->GetCurrentRoom()) && IsValid(_RefereceCharacter->GetCurrentRoom())
    && m_pLevelManager->GetCurrentRoom()->GetLevelId() != _RefereceCharacter->GetCurrentRoom()->GetLevelId())
  {
    return Result;
  }

  // Player, if the Reference Character is not the Player or PlayerClass is not Ignored, we add it into the Result
  if (m_pPlayer.IsValid() && m_pPlayer != _RefereceCharacter)
  {
    for (const auto& ClassToIgnore : _ActorClasesToIgnore)
    {
      if (m_pPlayer->IsA(ClassToIgnore))
      {
        bIgnored = true;
        break;
      }
    }

    if (!bIgnored && FVector::DistSquared(Location, m_pPlayer->GetActorLocation()) < AcceptableDistanceSq)
    {
      Result.m_pPlayer = m_pPlayer.Get();
    }
  }

  // Enemies, if the Reference Character is not the Enemy or it's class is not being Ignored we must check the  distance
  for (ABaseEnemy* Enemy : m_lEnemies)
  {
    if (!IsValid(Enemy) || Enemy == _RefereceCharacter)
    {
      continue;
    }

    bIgnored = false;

    for (const auto& ClassToIgnore : _ActorClasesToIgnore)
    {
      if (!IsValid(ClassToIgnore))
      {
        continue;
      }

      if (Enemy->IsA(ClassToIgnore))
      {
        bIgnored = true;
        break;
      }
    }

    if (bIgnored)
    {
      continue;
    }

    DistSq = FVector::DistSquared(Location, Enemy->GetActorLocation());

    if (DistSq <= AcceptableDistanceSq)
    { // If distance is lower than the Acceptable, into the Close list
      Result.m_lCloseEnemies.Add(Enemy);
    }
    else
    { // If distance is higher than the Acceptable, into the Far list
      Result.m_lFarEnemies.Add(Enemy);
    }
  }

  // Sorting the List by distance to the Reference Character
  Result.m_lCloseEnemies.Sort([&Location](const TWeakObjectPtr<ABaseEnemy>& A, const TWeakObjectPtr<ABaseEnemy>& B) {
    if (!A.IsValid()) return false;
    if (!B.IsValid()) return true;
    return FVector::DistSquared(Location, A->GetActorLocation()) < FVector::DistSquared(Location, B->GetActorLocation());
    });

  Result.m_lFarEnemies.Sort([&Location](const TWeakObjectPtr<ABaseEnemy>& A, const TWeakObjectPtr<ABaseEnemy>& B) {
    if (!A.IsValid()) return false;
    if (!B.IsValid()) return true;
    return FVector::DistSquared(Location, A->GetActorLocation()) < FVector::DistSquared(Location, B->GetActorLocation());
    });

  return Result;
}

bool UBaseRoom::RequestToken(ABaseEnemy* _enemyRequester)
{
  if (IsValid(m_pEnemyManagerComponent) && IsValid(_enemyRequester))
  {
    return m_pEnemyManagerComponent->RequestOozerBunnyToken(_enemyRequester);
  }
  return false;
}

bool UBaseRoom::ReturnToken(ABaseEnemy* _enemyReturner)
{
  if (IsValid(m_pEnemyManagerComponent) && IsValid(CachedWorld) && IsValid(_enemyReturner))
  {
    return m_pEnemyManagerComponent->ReturnOozerBunnyToken(_enemyReturner, CachedWorld);
  }
  return false;
}

bool UBaseRoom::RequestGrimelingToken(ABaseEnemy* _enemyRequester)
{
  if (IsValid(m_pEnemyManagerComponent) && IsValid(_enemyRequester))
  {
    return m_pEnemyManagerComponent->RequestGrimelingToken(_enemyRequester);
  }
  return false;
}

bool UBaseRoom::ReturnGrimelingToken(ABaseEnemy* _enemyReturner)
{
  if (IsValid(m_pEnemyManagerComponent) && IsValid(CachedWorld) && IsValid(_enemyReturner))
  {
    return m_pEnemyManagerComponent->ReturnGrimelingToken(_enemyReturner, CachedWorld);
  }
  return false;
}

bool UBaseRoom::RequestDustPooken(ABaseEnemy* _enemyRequester)
{
  if (IsValid(m_pPoopManagerComponent) && IsValid(_enemyRequester))
  {
    return m_pPoopManagerComponent->RequestDustPoopToken(_enemyRequester);
  }
  return false;
}

bool UBaseRoom::ReturnDustPooken(ABaseEnemy* _enemyReturner)
{
  if (IsValid(m_pPoopManagerComponent) && IsValid(CachedWorld) && IsValid(_enemyReturner))
  {
    m_pPoopManagerComponent->ClearDustPoopReservation(_enemyReturner);
    return m_pPoopManagerComponent->ReturnDustPoopToken(_enemyReturner, CachedWorld);
  }
  return false;
}

bool UBaseRoom::AreAvailableTokens()
{
  if (IsValid(m_pEnemyManagerComponent))
  {
    return m_pEnemyManagerComponent->GetAvailableOozerBunnyTokens() > 0;
  }
  return false;
}

bool UBaseRoom::AreAvailableGrimelingTokens()
{
  if (IsValid(m_pEnemyManagerComponent))
  {
    return m_pEnemyManagerComponent->GetAvailableGrimelingTokens() > 0;
  }
  return false;
}

bool UBaseRoom::CanDustPoop(const AActor* _pDustBunny)
{
  if (!IsValid(m_pPoopManagerComponent) || !IsValid(_pDustBunny))
  {
    return false;
  }
  if (m_pPoopManagerComponent->GetAvailableDustPoopTokens() > 0 && !IsPoopNear(_pDustBunny))
  {
    return m_pPoopManagerComponent->ReservePoopPosition(_pDustBunny);
  }
  return false;
}
#pragma endregion

#pragma region | Private Functions
void UBaseRoom::UpdateDirtPercentage()
{
  // if the room is clean it means that there were no enemies or the enemies were eliminated and the dirtyness was cleanable 
  // so we don't care about the dirt percentage anymore since it is 0
  if (m_eDirtStage == EDirtStage::CLEAN)
  {
    return;
  }

  float Sum = 0.0f;
  for (ABaseTile* Tile : m_lTiles)
  {
    if (!IsValid(Tile))
    {
      continue;
    }

    Sum += Tile->GetPercetage();
  }

  m_fDirtPercentage = Sum / m_lTiles.Num();
  m_fDirtPercentage = FMath::Clamp(m_fDirtPercentage / m_fDirtyArea, 0.0f, 1.0f);
  if (IsValid(CachedWorld) && IsValid(m_pPoopManagerComponent))
  {
    if (m_fDirtPercentage >= m_pPoopManagerComponent->GetCNPPercentage() && bCanPoop)
    {
      bCanPoop = false;
      OnCanPoop.Broadcast(bCanPoop, CachedWorld);
    }
    else if (m_fDirtPercentage <= m_pPoopManagerComponent->GetCPPercentage() && !bCanPoop)
    {
      bCanPoop = true;
      OnCanPoop.Broadcast(bCanPoop, CachedWorld);
    }
  }

  if (m_fDirtPercentage >= m_fBaseDeactivateRoomByBuff && m_eRoomState == ERoomState::CLOSE)
  {
    SetUnactiveRoom();
  }
  UpdateDirtStage();
}

void UBaseRoom::SpawnEnemies()
{
  if (!m_pLevel || bSpawnedAlready)
  {
    return;
  }
  UProceduralGeneration::OnSpawnersReady.RemoveAll(this);
  bSpawnedAlready = true;

  if (!m_pSpawnChances)
  {
    return;
  }

  // 1) We get the spawn points
  int32 TotalSpawnPoitns = m_lSpawnPoints.Num();

  if (TotalSpawnPoitns == 0)
  { // In case there are no spawn points we don't have to do anything we mark the room as clean so it doesn't update it's percentage
    m_eDirtStage = EDirtStage::CLEAN;
    if (m_eRoomShape != ERoomShape::Q1)
    {
      bCanActivateCleanRoomHUD = false;
      OnFloorCleaned.Broadcast(this);
    }
    return;
  }

  int32 NumToUse = FMath::RoundToInt(TotalSpawnPoitns * m_fEnemiesSpawnPercentage);

  // 2) Calculate how many enemies of each type
  // 2.1) Adding all distributions
  float TotalDistribution =
    m_pSpawnChances->m_fOozerDistribution +
    m_pSpawnChances->m_fMiniOozerDistribution +
    m_pSpawnChances->m_fOozerDoubleDistribution +
    m_pSpawnChances->m_fDustBunnyDistribution +
    m_pSpawnChances->m_fAshBunnyDistribution +
    m_pSpawnChances->m_fGrimelingDistribution;

  // 2.1) Calculate final normaliced distribution
  int32 NumOozers = FMath::RoundToInt(NumToUse * (m_pSpawnChances->m_fOozerDistribution / TotalDistribution));
  int32 NumMiniOozers = FMath::RoundToInt(NumToUse * (m_pSpawnChances->m_fMiniOozerDistribution / TotalDistribution));
  int32 NumOozerDoubles = FMath::RoundToInt(NumToUse * (m_pSpawnChances->m_fOozerDoubleDistribution / TotalDistribution));
  int32 NumDustBunnies = FMath::RoundToInt(NumToUse * (m_pSpawnChances->m_fDustBunnyDistribution / TotalDistribution));
  int32 NumAshBunnies = FMath::RoundToInt(NumToUse * (m_pSpawnChances->m_fAshBunnyDistribution / TotalDistribution));
  int32 NumGrimelings = FMath::RoundToInt(NumToUse * (m_pSpawnChances->m_fGrimelingDistribution / TotalDistribution));

  // 3) For each type set the distribution
  TArray<TPair<EEnemyType, int32>> ToSpawn =
  {
    {EEnemyType::OOZER, NumOozers},
    {EEnemyType::MINI_OOZER, NumMiniOozers},
    {EEnemyType::OOZER_DOUBLE, NumOozerDoubles},
    {EEnemyType::DUST_BUNNY, NumDustBunnies},
    {EEnemyType::ASH_BUNNY, NumAshBunnies},
    {EEnemyType::GRIMELING, NumGrimelings},
  };

  // 4) Set random spawn points
  TArray<ABaseSpawnPoint*> ChosenPoints = PickRandom<ABaseSpawnPoint>(m_lSpawnPoints, NumToUse);

  int32 PointIdx = 0;
  for (auto& Pair : ToSpawn)
  {
    EEnemyType Type = Pair.Key;
    for (int i = 0; i < Pair.Value; ++i)
    {
      UClass* EnemyClass = GetEnemyClass(Type);
      if (PointIdx >= ChosenPoints.Num())
      {
        continue;
      }
      FVector Location = ChosenPoints[PointIdx]->GetActorLocation();
      if (ABaseEnemy* Enemy = SpawnEnemy(EnemyClass, Location))
      {
        m_lEnemies.Add(Enemy);
      }
      ++PointIdx; // Maybe i have to change this into the if, but it might cause a infinite loop. Now it can make that not all enemies spawn
    }
  }
  CleanupSpawners();
  
}

ABaseEnemy* UBaseRoom::SpawnEnemy(UClass* EnemyClass, FVector Location)
{
  if (!IsValid(EnemyClass) || !IsValid(pGameInstance))
  {
    return nullptr;
  }

  FNavLocation tNavLocation;
  FVector vInFloorLocation = Location;

  if ((m_pNavSys != nullptr) && m_pNavSys->ProjectPointToNavigation(Location, tNavLocation))
  {
    vInFloorLocation = tNavLocation.Location + FVector(0.f, 0.f, pGameInstance->m_lEnemies->m_fCapsuleHeight - pGameInstance->m_lEnemies->m_fCapsuleZOffset);
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("SpawnEnemy: Not NavMesh Point"));
    return nullptr;
  }

  if (!CachedWorld)
  {
    UE_LOG(LogTemp, Error, TEXT("SpawnEnemy: GetWorld() returned nullptr"));
    return nullptr;
  }

  FActorSpawnParameters spawnInfo;
  spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
  ABaseEnemy* EnemyToSpawn = CachedWorld->SpawnActor<ABaseEnemy>(EnemyClass, vInFloorLocation, FRotator::ZeroRotator /*@TODO: maybe make this random?*/, spawnInfo);
  if (IsValid(EnemyToSpawn))
  {
    EnemyToSpawn->SetCurrentRoom(this);
    EnemyToSpawn->OnEnemyEliminate.AddDynamic(this, &UBaseRoom::OnEnemyEliminate);
  }
  return EnemyToSpawn;
}

void UBaseRoom::CleanupSpawners()
{
  for (ABaseSpawnPoint* SpawnPoint : m_lSpawnPoints)
  {
    if (IsValid(SpawnPoint))
    {
      SpawnPoint->Destroy();
    }
  }
  m_lSpawnPoints.Empty();
}

UClass* UBaseRoom::GetEnemyClass(EEnemyType EnemyType)
{
  switch (EnemyType)
  {
  case EEnemyType::OOZER:
    return OozerClass;
  case EEnemyType::MINI_OOZER:
    return MiniOozerClass;
  case EEnemyType::OOZER_DOUBLE:
    return OozerDoubleClass;
  case EEnemyType::DUST_BUNNY:
    return DustBunnyClass;
  case EEnemyType::ASH_BUNNY:
    return AshBunnyClass;
  case EEnemyType::GRIMELING:
    return GrimelingClass;
  default:
    return OozerClass;
  }
}

void UBaseRoom::SetupEnemiesClasses()
{
  pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
  if (IsValid(pGameInstance))
  {
    OozerClass = pGameInstance->m_lEnemies->m_cSlimeClass;
    MiniOozerClass = pGameInstance->m_lEnemies->m_cMiniSlimeClass;
    OozerDoubleClass = pGameInstance->m_lEnemies->m_cSlimeDividerClass;
    AshBunnyClass = pGameInstance->m_lEnemies->m_cAshBunny;
    DustBunnyClass = pGameInstance->m_lEnemies->m_cDustBunny;
    GrimelingClass = pGameInstance->m_lEnemies->m_cGoblin;
  }
}

void UBaseRoom::SetupRewards()
{
  m_pCleanRoomData = pGameInstance->m_pCleanRoomData;
}

void UBaseRoom::SetupEvents()
{
  DirtRoomEvent = GetEventByName(TEXT("FIRST_TIME_RedRoom"));
  CleanRoomEvent = GetEventByName(TEXT("FIRST_TIME_CleanRoom"));

  OozerEvent = GetEventByName(TEXT("FIND_ENEMY_1"));
  DustBunnyEvent = GetEventByName(TEXT("FIND_ENEMY_2"));
  GrimelingEvent = GetEventByName(TEXT("FIND_ENEMY_3"));
  OozerDoubleEvent = GetEventByName(TEXT("FIND_ENEMY_4"));
  AshBunnyEvent = GetEventByName(TEXT("FIND_ENEMY_5"));

  if (OozerEvent && !OozerEvent->m_bHasAlreadyAppear)
  {
    EnemyEventMap.Add(OozerClass, TEXT("FIND_ENEMY_1"));
  }

  if (DustBunnyEvent && !DustBunnyEvent->m_bHasAlreadyAppear)
  {
    EnemyEventMap.Add(DustBunnyClass, TEXT("FIND_ENEMY_2"));
  }

  if (GrimelingEvent && !GrimelingEvent->m_bHasAlreadyAppear)
  {
    EnemyEventMap.Add(GrimelingClass, TEXT("FIND_ENEMY_3"));
  }

  if (OozerDoubleEvent && !OozerDoubleEvent->m_bHasAlreadyAppear)
  {
    EnemyEventMap.Add(OozerDoubleClass, TEXT("FIND_ENEMY_4"));
  }

  if (AshBunnyEvent && !AshBunnyEvent->m_bHasAlreadyAppear)
  {
    EnemyEventMap.Add(AshBunnyClass, TEXT("FIND_ENEMY_5"));
  }
}

FEventDataArray* UBaseRoom::GetEventByName(FName NameEvent)
{
  if (FEventDataArray* Event = pGameInstance->m_pGeneralEventsDataAsset->m_oAllEventData.Find(NameEvent))
  {
    return Event;
  }

  return nullptr;
}

void UBaseRoom::UpdateDirtStage()
{
  const bool bHasEnemies = !m_lEnemies.IsEmpty();

  EDirtStage NewStage = m_eDirtStage;
  NewStage = CalculateNewDirtPercentage();

  // In case we are between apply and withdraw buff, we must stay in the same state if we were DIRTY then DIRTY if we were BUFFABLE then BUFFABLE
  // so we don't have to do anything else in case of new dirt stage
  if (m_eDirtStage != NewStage)
  {
    ApplyDirtStage(m_eDirtStage, NewStage);
    OnDirtStageChanged.Broadcast();
  }
  else if (m_eDirtStage == EDirtStage::CLEANABLE)
  {
    if (m_lEnemies.Num() == 0 && m_lDustPoops.Num() <= (Quadrants.Num() + 1))
    {
      CleanAllFloors();
    }
  }
}

void UBaseRoom::ApplyDirtStage(EDirtStage OldStage, EDirtStage NewStage)
{
  m_eDirtStage = NewStage;

  switch (NewStage)
  {
  case EDirtStage::CLEANABLE:
    if (m_lEnemies.Num() == 0 && m_lDustPoops.Num() <= Quadrants.Num() + 1)
    {
      CleanAllFloors();
    }
    else if (OldStage == EDirtStage::BUFFABLE)
    {
      WithdrawEnemiesBuff();
      OnEnemiesDebuff.Broadcast(this);
    }
    break;
  case EDirtStage::DIRTY:
    if (OldStage == EDirtStage::BUFFABLE)
    {
      WithdrawEnemiesBuff();
      OnEnemiesDebuff.Broadcast(this);
    }
    break;
  case EDirtStage::BUFFABLE:
    if (OldStage != EDirtStage::BUFFABLE)
    {
      BuffEnemies();
      OnEnemiesBuff.Broadcast(this);
    }
    break;
  default:
    break;
  }
}
void UBaseRoom::CleanAllFloors(bool _bCanRewardPlayer)
{
  UAchievementSubsystem* AchSubsystem = nullptr;
  UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (GameInstance)
  {
    AchSubsystem = GameInstance->GetSubsystem<UAchievementSubsystem>();
  }

  if (AchSubsystem)
  {
    AchSubsystem->AddProgress("TotalSanitation", 1);
    if (GameInstance->m_iCurrentLevel != 0)
    {
      AchSubsystem->SetMoppedAndLoadedAchievement();
      AchSubsystem->SetPaidOvertimeAchievement();
    }
  }

  for (TObjectPtr<ADissolvableDoor>& MyDoorPtr : m_lDissolvableDoors)
  {
    if (!IsValid(MyDoorPtr)) continue;

    ADissolvableDoor* MyDoor = MyDoorPtr.Get();
    bool bFoundNeighbor = false;

    for (UBaseRoom* Neighbor : m_lAdjacentRooms)
    {
      if (!Neighbor) continue;

      for (TObjectPtr<ADissolvableDoor>& NeighborDoorPtr : Neighbor->m_lDissolvableDoors)
      {
        if (!IsValid(NeighborDoorPtr)) continue;

        ADissolvableDoor* NeighborDoor = NeighborDoorPtr.Get();
        float Distance = FVector::Dist(MyDoor->GetActorLocation(), NeighborDoor->GetActorLocation());

        if (Distance < 100.0f)
        {
          NeighborDoor->SetIconMaterial(GetRoomType(), false);

          bFoundNeighbor = true;
          break;
        }
      }

      if (bFoundNeighbor)
      {
        break;
      }
    }
  }

  if (!m_pPlayer.IsValid())
  {
    return;
  }

  if (IsValid(m_pCleanRoomData))
  {
    m_pPlayer->ChangeMovementVelocity(m_pCleanRoomData->m_fSpeedMultiplierInCleanRooms);
  }

  for (ADustPoop* DustPoop : m_lDustPoops)
  {
    if (!IsValid(DustPoop))
    {
      continue;
    }
    DustPoop->Destroy();
  }

  for (ABaseTile* Tile : m_lTiles)
  {
    if (!IsValid(Tile))
    {
      continue;
    }

    Tile->CleanFloor(_bCanRewardPlayer);
    m_eDirtStage = EDirtStage::CLEAN;
    OnDirtStageChanged.Broadcast();
    GetWorld()->GetTimerManager().ClearTimer(DelayUpdateDirtnessHandle);
  }

  m_lDustPoops.Empty();

  if (m_eRoomShape != ERoomShape::Q1)
  {
    m_fPercentageAmountPerCleanRoom = (m_fPercentageAmountPerCleanRoom < 0 ? CalculateAmountPerCleanRoom() : m_fPercentageAmountPerCleanRoom);
    m_fBaseDeactivateRoomByBuff += m_fPercentageAmountPerCleanRoom;

    if (_bCanRewardPlayer)
    {
      RewardPlayer();
    }

    //TODO: ADD THIS INTO A FUNCTION
    FMOD::Studio::EventInstance* pEvent = nullptr;
    FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
    if (pStudioSystem) {
      FMOD::Studio::EventDescription* pEventDesc = nullptr;
      pStudioSystem->getEvent("event:/SoundFX/Miscellaneous/Gameplay/SFX_GAM_automaticRoomCleanup", &pEventDesc);

      if (pEventDesc) pEventDesc->createInstance(&pEvent);
      if (pEvent) pEvent->start();
    }
    //////////////////////////////////////////////////
  }
  OnFloorCleaned.Broadcast(this);
}
float UBaseRoom::CalculateAmountPerCleanRoom() const
{
  // @TODO: maybe ignore q1 here?
  int32 NumberOfRooms = m_lNeighbors.Num() + m_lOtherRooms.Num() + 1;
  return (0.7f - m_fBaseDeactivateRoomByBuff) / (float)NumberOfRooms;
}
void UBaseRoom::RewardPlayer()
{
  // Time Reward
  if (IsValid(TimeManager))
  {
    int TimeToAdd = CalculateTimeToAdd();
    pGameInstance->SecondsLastedInEndless += TimeToAdd;
    TimeManager->AddTime(0, TimeToAdd);
  }

  // Extra Reward
  switch (m_eRoomType)
  {
  case ERoomType::KITCHEN:
    AddExtraTime();
    OnGiveCleanReward.Broadcast(0);
    break;
  case ERoomType::LIVINGROOM:
    ChargeSpecial();
    OnGiveCleanReward.Broadcast(1);
    break;
  case ERoomType::BATHROOM:
    HealPlayer();
    OnGiveCleanReward.Broadcast(2);
    break;
  case ERoomType::GREENHOUSE:
    AddConsumableCharge();
    OnGiveCleanReward.Broadcast(3);
    break;
  default:
    // Nothing
    OnGiveCleanReward.Broadcast(-1);
    break;
  }
}
void UBaseRoom::AddExtraTime()
{
  if (IsValid(TimeManager))
  {
    pGameInstance->SecondsLastedInEndless += m_pCleanRoomData->m_iKitchenExtraTime;
    TimeManager->AddTime(0, m_pCleanRoomData->m_iKitchenExtraTime);
  }
}
void UBaseRoom::ChargeSpecial()
{ // @TODO: maybe if it doesn't charge it full, we might need another function that charges %
  if (!m_pPlayer.IsValid())
  {
    return;
  }

  for (ABaseWeapon* CurrentWeapon : m_pPlayer->GetCurrentWeapons())
  {
    if (IsValid(CurrentWeapon))
    {
      CurrentWeapon->ChargeSpecial(m_pCleanRoomData->m_fLivingroomSpecialCharge);
    }
  }
}
void UBaseRoom::HealPlayer()
{
  if (m_pPlayer.IsValid())
  {
    m_pPlayer->Heal(100);
  }
}
void UBaseRoom::AddConsumableCharge()
{
  if (m_pPlayer.IsValid())
  {
    //TSharedPtr<RandomNumberGenerator> RNG = MakeShared<RandomNumberGenerator>();
    //int32 RandomConsumable = RNG->GetRandomInRange(0, 1);
    m_pPlayer->AddConsumableCharge(0);
    m_pPlayer->AddConsumableCharge(1);
  }
}
void UBaseRoom::WithdrawEnemiesBuff()
{
  for (ABaseEnemy* Enemy : m_lEnemies)
  {
    if (IsValid(Enemy))
    {
      Enemy->SetMitigation(0.0f);
    }
  }
}
void UBaseRoom::BuffEnemies()
{
  for (ABaseEnemy* Enemy : m_lEnemies)
  {
    if (IsValid(Enemy))
    {
      float fTotalMitigation = m_fMitigation;
      if (Enemy->GetEnemyDataAsset())
      {
        fTotalMitigation = FMath::Clamp(fTotalMitigation * Enemy->GetEnemyDataAsset()->m_fDirtMitigationMultiplier, 0.f, 1.f);
      }
      Enemy->SetMitigation(fTotalMitigation);
    }
  }

  if (DirtRoomEvent && !DirtRoomEvent->m_bHasAlreadyAppear && IsValid(m_pLevelManager->GetCurrentRoom()) && m_pLevelManager->GetCurrentRoom()->GetLevelId() == GetLevelId())
  {
    TriggerEvent(TEXT("FIRST_TIME_RedRoom"));
  }
}
int UBaseRoom::CalculateTimeToAdd() const
{
  return GetQuadrantData().TimeReward;
}
FQuadrantData UBaseRoom::GetQuadrantData() const
{
  switch (Quadrants.Num())
  {
  case 2:
    return m_pCleanRoomData->ValuesFor2Quads;
  case 3:
    return m_pCleanRoomData->ValuesFor3Quads;
  case 4:
    return m_pCleanRoomData->ValuesFor4Quads;
  default:
    return FQuadrantData();
  }
}
EDirtStage UBaseRoom::CalculateNewDirtPercentage() const
{
  FQuadrantData QuadrantData = GetQuadrantData();
  if (m_fDirtPercentage <= QuadrantData.AutoClean)
  { // CLEANABLE if the dirtnes is below autoclean threshold
    return EDirtStage::CLEANABLE;
  }
  else if (m_fDirtPercentage < QuadrantData.WithdrawBuff)
  { // DIRTY if the dirtness is higheer than autoclean but lower than withdraw buff threshold
    return EDirtStage::DIRTY;
  }
  else if (m_fDirtPercentage >= QuadrantData.Buff)
  { // BUFFABLE if the dirtness is higher than buff threshold
    return EDirtStage::BUFFABLE;
  }
  else if (m_eDirtStage != EDirtStage::BUFFABLE)
  { // DIRTY in case we are between widthdraw buff and apply buf thresholds, and we were CLEANABLE
    return EDirtStage::DIRTY;
  }
  else
  {
    return EDirtStage::BUFFABLE;
  }
}
bool UBaseRoom::HasCompatibleClosedDoor(ADissolvableDoor* _DissolvableDoor, TSharedPtr<Quadrant> Quadrant) const
{
  FVector GlobalQuadrantPosition = FVector(Quadrant->Location * 1600, 0.0f) + m_v3Location + FVector(800.0f, 800.0f, 0.0f);
  FVector DoorLocation = _DissolvableDoor->GetActorLocation();

  double YawDouble = _DissolvableDoor->GetActorRotation().Yaw;
  int32 Yaw = FMath::RoundToInt32(YawDouble);
  int32 Turns = ((((int32)(Yaw) % 360) + 360) % 360 / 90) + 1;

  for (TSharedPtr<Door> Door : Quadrant->Doors)
  {
    if (Door->IsDoorUsed)
    {
      continue;
    }

    if (Door->Direction != Turns)
    {
      continue;
    }

    float Dist = FVector::Dist(GlobalQuadrantPosition, DoorLocation);

    if (Dist <= 900.0f)
    {
      return true;
    }
  }
  return false;
}

void UBaseRoom::HideSelfAndNeighbors()
{
  for (ADissolvableActor* DA : m_lDissolvableActors270)
  {
    DA->Hide();
  }

  for (ADissolvableActor* DA : m_lDissolvableActors180)
  {
    DA->Hide();
  }

  for (ADissolvableActor* DA : m_lDissolvableActors225)
  {
    DA->Hide();
  }

  HandleConcaveCorners();

  for (UBaseRoom* Adjacent : m_lAdjacentRooms)
  {
    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors0)
    {
      DA->Hide();
    }

    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors45)
    {
      DA->Hide();
    }

    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors90)
    {
      DA->Hide();
    }

    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors135)
    {
      DA->Hide();
    }

    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors180)
    {
      DA->Hide();
    }

    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors225)
    {
      DA->Hide();
    }

    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors270)
    {
      DA->Hide();
    }

    for (ADissolvableActor* DA : Adjacent->m_lDissolvableActors315)
    {
      DA->Hide();
    }
  }
}

void UBaseRoom::HandleConcaveCorners()
{
  if (m_eRoomShape == ERoomShape::Q4 || m_eRoomShape == ERoomShape::Q1 ||
    m_eRoomShape == ERoomShape::R3 || m_eRoomShape == ERoomShape::R2)
  {
    return;
  }
  int32 Yaw = FMath::RoundToInt32(m_fRotation);
  int32 Turns = ((((int32)(Yaw) % 360) + 360) % 360 / 90) + 1;

  switch (Turns)
  {
  case 1:
    HandleConcaveCorners0();
    break;
  case 2:
    HandleConcaveCorners90();
    break;
  case 3:
    HandleConcaveCorners180();
    break;
  case 4:
    HandleConcaveCorners270();
    break;
  }
}

void UBaseRoom::HandleConcaveCorners0()
{
  for (ADissolvableActor* DA : m_lDissolvableActors135)
  {
    DA->Hide();
  }
}

void UBaseRoom::HandleConcaveCorners90()
{
  for (ADissolvableActor* DA : m_lDissolvableActors315)
  {
    DA->Hide();
  }

  for (ADissolvableActor* DA : m_lDissolvableActors135)
  {
    DA->Hide();
  }
}

void UBaseRoom::HandleConcaveCorners180()
{
  for (ADissolvableActor* DA : m_lDissolvableActors315)
  {
    DA->Hide();
  }
}

void UBaseRoom::HandleConcaveCorners270()
{
  for (ADissolvableActor* DA : m_lDissolvableActors135)
  {
    DA->Hide();
  }

  for (ADissolvableActor* DA : m_lDissolvableActors315)
  {
    DA->Hide();
  }
}

void UBaseRoom::SetRoomState(ERoomState _NewRoomState)
{
  if (_NewRoomState == m_eRoomState)
  {
    return;
  }

  m_eRoomState = _NewRoomState;

  switch (m_eRoomState)
  {
  case ERoomState::CURRENT:
    SetActiveRoom();
    break;
  case ERoomState::CLOSE:
    SetMediumActiveRoom();
    break;
  case ERoomState::FAR:
    SetUnactiveRoom();
    break;
  default:
    break;
  }
}

void UBaseRoom::SetActiveRoom()
{
  for (TObjectPtr<ABaseEnemy> Enemy : m_lEnemies)
  {
    Enemy->RestartBehaviorTree();
    Enemy->SetMitigationVFXVisibility(true);
    Enemy->GetSkeletalMesh()->SetVisibility(true, true);
  }
  for (TObjectPtr<ADustPoop> DustPoop : m_lDustPoops)
  {
    DustPoop->GetStaticMesh()->SetVisibility(true, true);
  }
  for (TObjectPtr<ABaseTile> Tile : m_lTiles)
  {
    Tile->SetNegateDirt(false);
  }
}

void UBaseRoom::SetMediumActiveRoom()
{
  for (TObjectPtr<ABaseEnemy> Enemy : m_lEnemies)
  {
    Enemy->GetSkeletalMesh()->SetVisibility(false, true);
    Enemy->SetMitigationVFXVisibility(false);

  }
  for (TObjectPtr<ADustPoop> DustPoop : m_lDustPoops)
  {
    DustPoop->GetStaticMesh()->SetVisibility(false, true);
  }
  for (TObjectPtr<ABaseTile> Tile : m_lTiles)
  {
    Tile->SetNegateDirt(true);
  }
}

void UBaseRoom::SetUnactiveRoom()
{
  for (TObjectPtr<ABaseEnemy> Enemy : m_lEnemies)
  {
    Enemy->StopBehaviorTree();
    Enemy->GetSkeletalMesh()->SetVisibility(false, true);
    Enemy->SetMitigationVFXVisibility(false);
  }
  for (TObjectPtr<ADustPoop> DustPoop : m_lDustPoops)
  {
    DustPoop->SetActorTickEnabled(false);
    DustPoop->GetStaticMesh()->SetVisibility(false, true);
  }
  for (TObjectPtr<ABaseTile> Tile : m_lTiles)
  {
    Tile->SetNegateDirt(true);
  }
}

void UBaseRoom::SetAllRoomState(TArray<TObjectPtr<UBaseRoom>> RoomsToSet, ERoomState _NewRoomState)
{
  for (TObjectPtr<UBaseRoom> RoomToSet : RoomsToSet)
  {
    if (!IsValid(RoomToSet))
    {
      continue;
    }
    RoomToSet->SetRoomState(_NewRoomState);
  }
}

bool UBaseRoom::CheckPoopRequestConditions() const
{
  // @TODO: maybe make those value customizable
  return m_lDustPoops.Num() > 4 && OccupiedPoopCount() < 2;
}

int32 UBaseRoom::OccupiedPoopCount() const
{
  return Algo::CountIf(m_lDustPoops, [](const ADustPoop* DP)
    {
      return DP && DP->IsDustBunnyHidden();
    });
}

ADustPoop* UBaseRoom::FindClosestPoop(const AActor* _Actor) const
{
  return FindClosestActor<ADustPoop>(
    _Actor,
    m_lDustPoops,
    [](const ADustPoop* DP) { return DP && !DP->IsDustBunnyHidden(); }
  );
}

ADustPoop* UBaseRoom::FindClosestPoopIgnoringHidden(const AActor* _Actor) const
{
  return FindClosestActor<ADustPoop>(
    _Actor,
    m_lDustPoops,
    [](const ADustPoop* DP) { return IsValid(DP); }
  );
}

AMrsCleanActor* UBaseRoom::FindClosestDecoy(const AActor* _Actor) const
{
  return FindClosestActor<AMrsCleanActor>(
    _Actor,
    m_lDecoys
  );
}

void UBaseRoom::TriggerEvent(FName _TriggerName) const
{
  if (!pGameInstance)
  {
    return;
  }

  TWeakObjectPtr<UEventManager> WeakEventManager = pGameInstance->GetEventManager();

  if (!WeakEventManager.IsValid() || !IsValid(CachedWorld))
  {
    return;
  }

  FTimerHandle TimerHandle;
  CachedWorld->GetTimerManager().SetTimer(
    TimerHandle,
    [WeakEventManager, _TriggerName]()
    {
      if (WeakEventManager.IsValid())
      {
        WeakEventManager->TriggerEvent(_TriggerName);
      }
    },
    0.5f,
    false
  );
}
#pragma endregion