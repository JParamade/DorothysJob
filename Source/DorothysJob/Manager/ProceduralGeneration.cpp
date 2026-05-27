#include "ProceduralGeneration.h"
#include "Engine/DataTable.h"
#include "DorothysJob/Data/DataAssets/Procedural/EndlessDataAsset.h"
#include "DorothysJob/Data/DataTables/RoomData.h"
#include "DorothysJob/Data/DataTables/ProceduralData.h"
#include "DorothysJob/Data/DataAssets/EnemyDataAsset.h"
#include "DorothysJob/Data/DataAssets/Procedural/ProceduralDataAsset.h"
#include "DorothysJob/Utils/DataStructure/Graph/GraphNode.h"
#include "DorothysJob/Utils/DataStructure/Graph/Graph.h"
#include "DorothysJob/Utils/DataStructure/Graph/GraphEdge.h"
#include "DorothysJob/Utils/DataStructure/Graph/Quadrant.h"
#include "DorothysJob/Utils/DataStructure/Graph/Door.h" 
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Utils/Random/RandomNumberGenerator.h"
#include "Engine/LevelStreamingDynamic.h" 
#include "Kismet/GameplayStatics.h" 
#include "ProceduralGameMode.h"
#include "BaseGameInstance.h"
#include "WorldSubsystem/LevelManager.h"
#include "NavigationSystem.h"
#include "DorothysJob/Actor/Room/Tile/BaseTile.h"
#include "WorldSubsystem/TimeManager.h"
#include "DorothysJob/Component/Interactive/Interactable/InteractableCleaningCupboard.h"
#include "DorothysJob/Actor/Dissolvable/DissolvableDoor.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

FOnLevelReady UProceduralGeneration::OnLevelReady;
FOnSpawnersReady UProceduralGeneration::OnSpawnersReady;

void UProceduralGeneration::GenerateLayout(int32 _LevelToLoad)
{
  CurrentLevel = _LevelToLoad;

  if (CurrentLevel == 0)
  {
    CreateTutorialLevel();
  }
  else
  {
    bEndlessMode = CurrentLevel > 10; // TODO: maybe don't hardcode this

    if (!InitializeLevel())
    {
      return;
    }

    InitializeRandomGenerator();
    InitializeRoomNodes();

    do
    {
      ResetGraph();
      SelectAndPlaceRootNode();
      BuildGraphConnections();
    } while (!FinalizeAndValidateLayout());

    ExportAndDraw();
  }
}

UWorld* UProceduralGeneration::GetWorld() const
{
  if (const UObject* Outer = GetOuter())
  {
    return Outer->GetWorld();
  }
  return nullptr;
}

#pragma region | Tutorial Methods
void UProceduralGeneration::CreateTutorialLevel()
{
  pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));

  LoadTutorialData();
  LoadLevelData();

  if (!pGameInstance)
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load Game Instance."));
    return;
  }
  m_oCleaningStationClass = pGameInstance->m_lEnemies->m_cCleaningStation;

  UWorld* World = GetWorld();
  if (!World)
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load World Instance."));
    return;
  }

  ULevelManager* MyLevelManager = World->GetSubsystem<ULevelManager>();
  if (!MyLevelManager)
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load Level Manager."));
    return;
  }

  m_pLevelManager = MyLevelManager;

  InitializeRandomGenerator();
  InitializeRoomNodesTutorial();
  ResetGraph();

  for (int32 i = 0; i < m_lAllRooms.Num(); ++i)
  {
    FIntPoint Position = pGameInstance->m_lTutorialLocations[i];
    TSharedPtr<GraphNode> Copy = MakeShared<GraphNode>(*m_lAllRooms[i]);
    Copy->SetRotation(0);
    Copy->SetPosition(Position);
    if (Copy->GetRoomShape() == ERoomShape::Q1)
    {
      Copy->SetHasCleaningStation(true);
    }
    m_pGraph->AddNode(Copy);
  }

  for (const FIntPoint& Connection : pGameInstance->m_lTutorialConnections)
  {
    if (m_lAllRooms.IsValidIndex(Connection.X) && m_lAllRooms.IsValidIndex(Connection.Y) && TryConnectStatic(m_lAllRooms[Connection.X], m_lAllRooms[Connection.Y]))
    {
      m_pGraph->AddEdge(m_lAllRooms[Connection.X], m_lAllRooms[Connection.Y]);
    }
  }
  TryConnectPlacedRooms();
  DrawLayout();
}

void UProceduralGeneration::InitializeRoomNodesTutorial()
{
  for (FRoomData* RoomData : m_pAllRoomRows)
  {
    TSharedPtr<GraphNode> Node = MakeShared<GraphNode>();
    Node->SetRoomData(*RoomData);

    if (Node->GetRoomShape() == ERoomShape::Q1)
    {
      Node->SetHasCleaningStation(true);
    }

    m_lAllRooms.Add(Node);
  }
}

bool UProceduralGeneration::LoadTutorialData()
{
  if (!IsValid(pGameInstance) || !IsValid(pGameInstance->m_pTutorialRoomData))
  {
    return false;
  }

  pGameInstance->m_pTutorialRoomData->GetAllRows(TEXT("Rooms"), m_pAllRoomRows);
  if (m_pAllRoomRows.Num() <= 0)
  {
    return false;
  }
  return true;
}

#pragma region | Private Methods
bool UProceduralGeneration::InitializeLevel()
{
  pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));

  if (bEndlessMode)
  {
    UpdateEndlessValues();
  }

  if (!LoadRoomData())
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load Level Data."));
    return false;
  }
  if (!LoadLevelData())
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load Level Rules."));
    return false;
  }
  if (!LoadProceduralData())
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load Procedural Data."));
    return false;
  }

  if (!pGameInstance)
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load Game Instance."));
    return false;
  }
  m_oCleaningStationClass = pGameInstance->m_lEnemies->m_cCleaningStation;

  UWorld* World = GetWorld();
  if (!World)
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load World Instance."));
    return false;
  }

  ULevelManager* MyLevelManager = World->GetSubsystem<ULevelManager>();
  if (!MyLevelManager)
  {
    UE_LOG(LogTemp, Display, TEXT("Could not load Level Manager."));
    return false;
  }

  m_pLevelManager = MyLevelManager;
  return true;
}

bool UProceduralGeneration::LoadRoomData()
{
  if (!IsValid(pGameInstance) || !IsValid(pGameInstance->m_pRoomData))
  {
    return false;
  }

  pGameInstance->m_pRoomData->GetAllRows(TEXT("Rooms"), m_pAllRoomRows);
  if (m_pAllRoomRows.Num() <= 0)
  {
    return false;
  }
  return true;
}

bool UProceduralGeneration::LoadLevelData()
{
  if (!IsValid(pGameInstance) || !IsValid(pGameInstance->m_pLevelData))
  {
    return false;
  }

  pGameInstance->m_pLevelData->GetAllRows(TEXT("Rooms"), m_pLevelData);
  if (m_pLevelData.Num() <= 0)
  {
    return false;
  }

  MinQuadrantNumber = bEndlessMode ? m_pLevelData[CurrentLevel]->m_iMinQuadrantNumber + CurrentEndlessValues.CurrentExtraQuads : m_pLevelData[CurrentLevel]->m_iMinQuadrantNumber;
  MaxQuadrantNumber = bEndlessMode ? m_pLevelData[CurrentLevel]->m_iMaxQuadrantNumber + CurrentEndlessValues.CurrentExtraQuads : m_pLevelData[CurrentLevel]->m_iMaxQuadrantNumber;
  CleaningStationToAdd = m_pLevelData[CurrentLevel]->m_iCleaningStationNumber;
  MaxPlacedQ1 = m_pLevelData[CurrentLevel]->m_iMaxQ1Number;
  m_lAvailableRoomTypes = m_pLevelData[CurrentLevel]->m_lAvailableRooms;
  Minutes = m_pLevelData[CurrentLevel]->Minutes;
  Seconds = m_pLevelData[CurrentLevel]->Seconds;
  m_fBaseDeactivateRoomByBuff = m_pLevelData[CurrentLevel]->BaseDeactivateRoomByBuff;
  m_fMitigation = m_pLevelData[CurrentLevel]->Mitigation;
  bLaxRules = m_pLevelData[CurrentLevel]->LaxRules;
  return true;
}

bool UProceduralGeneration::LoadProceduralData()
{
  if (!IsValid(pGameInstance) || !IsValid(pGameInstance->m_pProceduralData))
  {
    return false;
  }

  MaxTriesPerNode = pGameInstance->m_pProceduralData->MaxTriesPerNode;
  MinDistanceBetweenCleaningStationsSqr = pGameInstance->m_pProceduralData->MinDistanceBetweenCleaningStations * pGameInstance->m_pProceduralData->MinDistanceBetweenCleaningStations;
  MaxTriesAddingQ1 = pGameInstance->m_pProceduralData->MaxTriesAddingQ1;
  return true;
}

void UProceduralGeneration::InitializeRandomGenerator()
{
  m_pRandom = MakeShared<RandomNumberGenerator>();
}

void UProceduralGeneration::InitializeRoomNodes()
{
  for (FRoomData* RoomData : m_pAllRoomRows)
  {
    for (FAvailableRoom AvailableRoom : m_lAvailableRoomTypes)
    {
      if (RoomData->RoomType == AvailableRoom.m_eRoomType)
      {
        TSharedPtr<GraphNode> Node = MakeShared<GraphNode>();
        Node->SetRoomData(*RoomData);
        m_lAllRooms.Add(Node);
      }
    }
  }
  for (FAvailableRoom AvailableRoom : m_lAvailableRoomTypes)
  {
    m_fTotalProceduralWeight += AvailableRoom.m_fProceduralWeight;
    m_mTypeWeights.Add(AvailableRoom.m_eRoomType, AvailableRoom.m_fProceduralWeight);
  }
}

void UProceduralGeneration::ResetGraph()
{
  m_pGraph = MakeShared<Graph>();
  CleaningStationLocations.Empty();
  bIsLayoutValid = false;
}

TArray<ERoomType> UProceduralGeneration::GetRemainingTypes() const
{
  TArray<ERoomType> Out_;
  TArray<ERoomType> UsedRoomTypes;

  for (TSharedPtr<GraphNode> Node : m_pGraph->GetNodes())
  {
    if (Node->GetRoomShape() != ERoomShape::Q1 && !UsedRoomTypes.Contains(Node->GetRoomType()))
    {
      UsedRoomTypes.Add(Node->GetRoomType());
    }
  }

  for (FAvailableRoom Type : m_lAvailableRoomTypes)
  {
    if (!UsedRoomTypes.Contains(Type.m_eRoomType))
    {
      Out_.Add(Type.m_eRoomType);
    }
  }

  return Out_;
}

TSharedPtr<GraphNode> UProceduralGeneration::GetRandomRoomNode() const
{
  if (!m_pRandom)
  {
    return nullptr;
  }

  ERoomType ChoosenRoomType = ERoomType::KITCHEN;
  TArray<ERoomType> RemainingTypes = GetRemainingTypes();

  if (RemainingTypes.Num() > 0)
  {
    int32 Index = m_pRandom->GetRandomInRange(0, RemainingTypes.Num() - 1);
    ChoosenRoomType = RemainingTypes[Index];
  }
  else if (m_fTotalProceduralWeight > 0.0f && m_mTypeWeights.Num() > 0)
  {
    float RandomValue = m_pRandom->GetRandomFloatInRange(0, m_fTotalProceduralWeight);
    for (auto& Elem : m_mTypeWeights)
    {
      if (RandomValue < Elem.Value)
      {
        ChoosenRoomType = Elem.Key;
        break;
      }
      RandomValue -= Elem.Value;
    }
  }
  else
  {
    return nullptr;
  }

  TArray<TSharedPtr<GraphNode>> Candidates;
  for (const TSharedPtr<GraphNode>& Room : m_lAllRooms)
  {
    if (Room && Room->GetRoomType() == ChoosenRoomType)
    {
      Candidates.Add(Room);
    }
  }

  if (Candidates.Num() == 0)
  {
    return nullptr;
  }

  int32 Index = m_pRandom->GetRandomInRange(0, Candidates.Num() - 1);
  const TSharedPtr<GraphNode>& Original = Candidates[Index];
  return MakeShared<GraphNode>(*Original);
}

TSharedPtr<GraphNode> UProceduralGeneration::GetRandomQ1() const
{
  ERoomType ChoosenRoomType = ERoomType::BEDROOM;

  float RandomValue = m_pRandom->GetRandomFloatInRange(0, m_fTotalProceduralWeight);
  for (auto& Elem : m_mTypeWeights)
  {
    if (RandomValue < Elem.Value)
    {
      ChoosenRoomType = Elem.Key;
      break;
    }
    RandomValue -= Elem.Value;
  }

  TArray<TSharedPtr<GraphNode>> Candidates;
  for (TSharedPtr<GraphNode> Room : m_lAllRooms)
  {
    if (Room && Room->GetRoomType() == ChoosenRoomType && Room->GetRoomShape() == ERoomShape::Q1)
    {
      Candidates.Add(Room);
    }
  }

  int32 Index = m_pRandom->GetRandomInRange(0, Candidates.Num() - 1);
  TSharedPtr<GraphNode> Original = Candidates[Index];
  TSharedPtr<GraphNode> Copy = MakeShared<GraphNode>(*Original);
  return Copy;
}

TSharedPtr<GraphNode> UProceduralGeneration::GetRoomNode(int32 Index) const
{
  TSharedPtr<GraphNode> Original = m_lAllRooms[Index];
  TSharedPtr<GraphNode> Copy = MakeShared<GraphNode>(*Original);
  int32 Turns = Copy->GetRoomShape() == ERoomShape::Q1 ? m_pRandom->GetRandomInRange(0, 3) : 0;
  Copy->SetRotation(Turns * 90);
  return Copy;
}

void UProceduralGeneration::SelectAndPlaceRootNode()
{
  const FIntPoint StartingPosition(0, 0);
  m_pRootNode = GetRandomRoomNode();
  m_pRootNode->SetRotation(m_pRandom->GetRandomInRange(0, 3) * 90.0f);
  m_pRootNode->SetPosition(StartingPosition);
  m_pGraph->AddNode(m_pRootNode);
}

void UProceduralGeneration::BuildGraphConnections()
{
  TArray<TSharedPtr<GraphNode>> Pending;
  Pending.Add(m_pRootNode);

  while (!IsLayoutValid() && Pending.Num() > 0)
  {
    TSharedPtr<GraphNode> Current = Pending.Pop();
    int32 Tries = 0;

    while (!IsNodeDone(Current) && Tries < MaxTriesPerNode)
    {
      auto Next = GetRandomRoomNode();
      if (TryConnect(Current, Next))
      {
        m_pGraph->AddNode(Next);
        m_pGraph->AddEdge(Current, Next);
        TryConnectPlacedRooms();
        Pending.Add(Next);
        Tries = 0;
      }
      else
      {
        ++Tries;
      }
    }

    if (Tries >= MaxTriesPerNode)
    {
      bFail = true;
      UE_LOG(LogTemp, Display, TEXT("Max tries exceeded on Node ID%d - restarting"), Current->GetNodeID());
      return;
      //m_pGraph->RemoveNode(Current);
    }
  }
}

bool UProceduralGeneration::IsLayoutValid()
{
  if (m_pGraph->GetTotalQuadrants() >= MinQuadrantNumber && m_pGraph->GetTotalQuadrants() <= MaxQuadrantNumber)
  {
    return true;
  }
  return false;
}

bool UProceduralGeneration::IsNodeDone(const TSharedPtr<GraphNode>& Node) const
{
  bool bHasMinConnection = HasMinConnections(Node);
  return bHasMinConnection ? HasMinExternalConnections(Node) : false;
}

bool UProceduralGeneration::HasMinConnections(const TSharedPtr<GraphNode>& Node) const
{
  // we need at least 3 connections if we have 4 quadrants
  // if not just 2 connections except the lax rules are active
  int32 MinimumConetions = (Node->GetQuadrants().Num() == 4 && !bLaxRules) ? 3 : 2;
  int32 CurrentConnections = m_pGraph->GetConnectedNodes(Node).Num();
  return CurrentConnections >= MinimumConetions;
}

bool UProceduralGeneration::HasMinExternalConnections(const TSharedPtr<GraphNode>& Node) const
{
  const TArray<TSharedPtr<Quadrant>> ExteriorQuadrants = Node->GetExteriorQuadrants();
  int32 MinimumExteriorNodesConnected = Node->GetRoomShape() == ERoomShape::Q4 ? 3 : ExteriorQuadrants.Num(); // if room shape is q4 then we need 3 external connections
  MinimumExteriorNodesConnected = bLaxRules && MinimumExteriorNodesConnected == 3 ? 2 : MinimumExteriorNodesConnected;
  MinimumExteriorNodesConnected = bLaxRules && Node->GetRoomShape() == ERoomShape::S ? 1 : MinimumExteriorNodesConnected;
  int32 CurrentExteriorNodesConnected = ExteriorQuadrants.Num() - Node->GetExteriorNotConnectedQuadrants().Num();
  return CurrentExteriorNodesConnected >= MinimumExteriorNodesConnected;
}

bool UProceduralGeneration::TryConnect(TSharedPtr<GraphNode> NodeA, TSharedPtr<GraphNode> NodeB)
{
  if (!NodeA.IsValid() || !NodeB.IsValid())
  {
    UE_LOG(LogTemp, Display, TEXT("At least one of the Rooms in TryConnect are NULL"));
    return false;
  }

  // Variables needed in order to set NodeB when the best connection is found
  float BestScore = TNumericLimits<float>::Max();
  FIntPoint BestPosition = -1;
  int32 BestRotation = -1;
  TSharedPtr<Quadrant> BestQuadrantA = MakeShared<Quadrant>();
  TSharedPtr<Door> BestDoorA = MakeShared<Door>();
  TSharedPtr<Quadrant> BestQuadrantB = MakeShared<Quadrant>();
  TSharedPtr<Door> BestDoorB = MakeShared<Door>();

  const TArray<int32>& Rotations = { 0, 90, 180, 270 };

  // TODO: this needs a review, maybe instead of checking every quadrant, get all the doors quadrants? idk
  for (const int32 CurrentRotation : Rotations)
  {
    TSharedPtr<GraphNode> RotatedNodeB = MakeShared<GraphNode>(*NodeB);
    RotatedNodeB->SetRotation(CurrentRotation);
    for (TSharedPtr<Quadrant> CurrentQuadrantA : NodeA->GetExteriorNotConnectedQuadrants())
    {
      for (TSharedPtr<Door> CurrentDoorA : CurrentQuadrantA->Doors)
      {
        if (CurrentDoorA->IsDoorUsed)
        {
          continue;
        }
        for (TSharedPtr<Quadrant> CurrentQuadrantB : RotatedNodeB->GetExteriorNotConnectedQuadrants())
        {
          for (TSharedPtr<Door> CurrentDoorB : CurrentQuadrantB->Doors)
          {
            if (CurrentDoorB->IsDoorUsed || !CurrentDoorA->IsCompatible(CurrentDoorB))
            {
              continue;
            }

            FIntPoint DirectionOffset = { 0,0 };
            switch (CurrentDoorA->Direction)
            {
            case 1:
              DirectionOffset = { 0, -1 };
              break;
            case 2:
              DirectionOffset = { 1, 0 };
              break;
            case 3:
              DirectionOffset = { 0, 1 };
              break;
            case 4:
              DirectionOffset = { -1, 0 };
            }

            FIntPoint DesiredBPosition = (CurrentQuadrantA->Location + NodeA->GetPosition()) - CurrentQuadrantB->Location + DirectionOffset;
            RotatedNodeB->SetPosition(DesiredBPosition);
            if (!IsValidPlacement(RotatedNodeB))
            {
              continue;
            }

            float CurrentScore = RotatedNodeB->GetScore();

            if (CurrentScore < BestScore)
            {
              BestScore = CurrentScore;
              BestPosition = DesiredBPosition;
              BestRotation = CurrentRotation;
              BestQuadrantA = CurrentQuadrantA;
              BestQuadrantB = CurrentQuadrantB;
              BestDoorA = CurrentDoorA;
              BestDoorB = CurrentDoorB;
            }
          }
        }
      }
    }
  }

  if (BestScore == TNumericLimits<float>::Max())
  {
    return false;
  }

  NodeB->SetPosition(BestPosition);
  NodeB->SetRotation(BestRotation);
  UpdateQuadrantsAndDoors(NodeA, BestQuadrantA, BestDoorA);
  UpdateQuadrantsAndDoors(NodeB, BestQuadrantB, BestDoorB);
  // find all the quadrants from A and B and all the doors in order to update them (adding 1 into the quadrant count and marking the door to used)

  return true;
}

void UProceduralGeneration::TryConnectPlacedRooms()
{
  for (TSharedPtr<GraphNode> CurrentNodeA : m_pGraph->GetNodes())
  {
    for (TSharedPtr<GraphNode> CurrentNodeB : m_pGraph->GetNodes())
    {
      if (CurrentNodeA == CurrentNodeB)
      {
        continue;
      }
      if (m_pGraph->AlreadyConnectedWith(CurrentNodeA, CurrentNodeB))
      {
        continue;
      }
      if (TryConnectStatic(CurrentNodeA, CurrentNodeB))
      {
        m_pGraph->AddEdge(CurrentNodeA, CurrentNodeB);
        continue;
      }
    }
  }
}

bool UProceduralGeneration::TryConnectStatic(TSharedPtr<GraphNode> NodeA, TSharedPtr<GraphNode> NodeB)
{
  for (TSharedPtr<Quadrant> CurrentQuadrantA : NodeA->GetQuadrants())
  {
    for (TSharedPtr<Quadrant> CurrentQuadrantB : NodeB->GetQuadrants())
    {
      FIntPoint GlobalQuadrantPositionA = CurrentQuadrantA->Location + NodeA->GetPosition();
      FIntPoint GlobalQuadrantPositionB = CurrentQuadrantB->Location + NodeB->GetPosition();
      FIntPoint VectorBetweenNodes = GlobalQuadrantPositionA - GlobalQuadrantPositionB;

      FIntPoint DoorDirectionInAB = GetDoorDirection(VectorBetweenNodes);

      if (FindCompatibleDoor(CurrentQuadrantA, DoorDirectionInAB.X, CurrentQuadrantB, DoorDirectionInAB.Y))
      {
        return true;
      }
    }
  }
  return false;
}

bool UProceduralGeneration::IsValidPlacement(TSharedPtr<GraphNode> Node)
{
  for (TSharedPtr<GraphNode> PlacedNode : m_pGraph->GetNodes())
  {
    if (Node == PlacedNode)
    {
      continue;
    }
    if (Node->IsOverlapping(PlacedNode))
    {
      UE_LOG(LogTemp, Display, TEXT("Placed Node ID%d is Colliding with ID%d"), PlacedNode->GetNodeID(), Node->GetNodeID());
      return false;
    }
  }
  return true;
}

void UProceduralGeneration::UpdateQuadrantsAndDoors(TSharedPtr<GraphNode> Node, TSharedPtr<Quadrant> QuadrantToUpdate, TSharedPtr<Door> DoorToUpdate)
{
  for (TSharedPtr<Quadrant> Q : Node->GetQuadrants())
  {
    if (Q->Location == QuadrantToUpdate->Location)
    {
      // TODO: Update connections in the quadrant (maybe just count the used doors so we don't have duplicated info hehe)
      for (TSharedPtr<Door> D : Q->Doors)
      {
        if (D->Direction == DoorToUpdate->Direction)
        {
          D->IsDoorUsed = true;
          return;
        }
      }
    }
  }
}

bool UProceduralGeneration::FindCompatibleDoor(TSharedPtr<Quadrant> QuadrantA, int32 DirectionA, TSharedPtr<Quadrant> QuadrantB, int32 DirectionB)
{
  for (TSharedPtr<Door> CurrentDoorA : QuadrantA->Doors)
  {
    if (CurrentDoorA->Direction != DirectionA)
    {
      continue;
    }
    for (TSharedPtr<Door> CurrentDoorB : QuadrantB->Doors)
    {
      if (CurrentDoorB->Direction == DirectionB)
      {
        CurrentDoorA->IsDoorUsed = true;
        CurrentDoorB->IsDoorUsed = true;
        return true;
      }
    }
  }
  return false;
}

FIntPoint UProceduralGeneration::GetDoorDirection(FIntPoint VectorBetweenDoors)
{
  int32 X = VectorBetweenDoors.X;
  int32 Y = VectorBetweenDoors.Y;
  if (X == 0 && Y == 1)
  {
    return FIntPoint(1, 3);
  }
  if (X == -1 && Y == 0)
  {
    return FIntPoint(2, 4);
  }
  if (X == 0 && Y == -1)
  {
    return FIntPoint(3, 1);
  }
  if (X == 1 && Y == 0)
  {
    return FIntPoint(4, 2);
  }
  return FIntPoint(0, 0);
}

bool UProceduralGeneration::FinalizeAndValidateLayout()
{
  if (bFail)
  {
    bFail = false;
    return false;
  }

  if (!IsLayoutValid())
  {
    return false;
  }

  // Q1 extra placement
  if (!GenerateQ1Extra())
  {
    return false;
  }

  if (!AddPlayerSpawnerRoom())
  {
    return false;
  }

  // Adding cleaning stations
  if (!GenerateCleaningStations())
  {
    return false;
  }

  TryConnectPlacedRooms();

  if (m_pGraph->GetQ1Nodes().Num() > MaxPlacedQ1)
  {
    return false;
  }

  // Validate all placements
  for (auto& Node : m_pGraph->GetNodes())
  {
    if (!IsValidPlacement(Node))
    {
      UE_LOG(LogTemp, Display, TEXT("Node ID%d invalid"), Node->GetNodeID());
      return false;
    }
  }

  bIsLayoutValid = true;
  return true;
}

bool UProceduralGeneration::GenerateQ1Extra()
{
  TSharedPtr<Door> BestQ1Door = MakeShared<Door>();
  bool bAddedQ1 = false;
  for (TSharedPtr<GraphNode> Node : m_pGraph->GetNodes())
  {
    if (!HasMinExternalConnections(Node))
    {
      UE_LOG(LogTemp, Display, TEXT("Node ID%d doesn't have minimum external connections."), Node->GetNodeID());
      for (TSharedPtr<Quadrant> ExteriorQuadrant : Node->GetExteriorNotConnectedQuadrants())
      {
        for (TSharedPtr<Door> Door : ExteriorQuadrant->Doors)
        {
          if (Door->IsDoorUsed)
          {
            continue;
          }

          FIntPoint DirectionOffset = { 0,0 };
          switch (Door->Direction)
          {
          case 1:
            DirectionOffset = { 0, -1 };
            break;
          case 2:
            DirectionOffset = { 1, 0 };
            break;
          case 3:
            DirectionOffset = { 0, 1 };
            break;
          case 4:
            DirectionOffset = { -1, 0 };
          }

          FIntPoint CurrentPos = ExteriorQuadrant->Location + Node->GetPosition() + DirectionOffset;
          TSharedPtr<GraphNode> Q1ToAdd = GetRandomQ1();
          Q1ToAdd->SetPosition(CurrentPos);

          if (!IsValidPlacement(Q1ToAdd))
          {
            continue;
          }

          BestQ1Door->Direction = Door->GetOppositeDirection();
          UpdateQuadrantsAndDoors(Node, ExteriorQuadrant, Door);
          UpdateQuadrantsAndDoors(Q1ToAdd, Q1ToAdd->GetQuadrants()[0], BestQ1Door);
          m_pGraph->AddNode(Q1ToAdd);
          m_pGraph->AddEdge(Q1ToAdd, Node);
          bAddedQ1 = true;
          UE_LOG(LogTemp, Display, TEXT("Adding external connection to Node ID%d."), Node->GetNodeID());
          break;
        }
      }
      if (!bAddedQ1)
      {
        return false;
      }
      bAddedQ1 = false;
    }
  }

  for (TSharedPtr<GraphNode> Node : m_pGraph->GetNodes())
  {
    if (!HasMinConnections(Node))
    {
      UE_LOG(LogTemp, Display, TEXT("Node ID%d doesn't have minimum connections."), Node->GetNodeID());
      for (TSharedPtr<Quadrant> ExteriorQuadrant : Node->GetNotConnectedQuadrants())
      {
        for (TSharedPtr<Door> Door : ExteriorQuadrant->Doors)
        {
          if (Door->IsDoorUsed)
          {
            continue;
          }

          FIntPoint DirectionOffset = { 0,0 };
          switch (Door->Direction)
          {
          case 1:
            DirectionOffset = { 0, -1 };
            break;
          case 2:
            DirectionOffset = { 1, 0 };
            break;
          case 3:
            DirectionOffset = { 0, 1 };
            break;
          case 4:
            DirectionOffset = { -1, 0 };
          }

          FIntPoint CurrentPos = ExteriorQuadrant->Location + Node->GetPosition() + DirectionOffset;
          TSharedPtr<GraphNode> Q1ToAdd = GetRandomQ1();
          Q1ToAdd->SetPosition(CurrentPos);

          if (!IsValidPlacement(Q1ToAdd))
          {
            continue;
          }

          BestQ1Door->Direction = Door->GetOppositeDirection();
          UpdateQuadrantsAndDoors(Node, ExteriorQuadrant, Door);
          UpdateQuadrantsAndDoors(Q1ToAdd, Q1ToAdd->GetQuadrants()[0], BestQ1Door);
          m_pGraph->AddNode(Q1ToAdd);
          m_pGraph->AddEdge(Q1ToAdd, Node);
          bAddedQ1 = true;
          UE_LOG(LogTemp, Display, TEXT("Adding external connection to Node ID%d."), Node->GetNodeID());
          break;
        }
      }
      if (!bAddedQ1)
      {
        return false;
      }
      bAddedQ1 = false;
    }
  }

  return true;
}

bool UProceduralGeneration::AddPlayerSpawnerRoom()
{
  FIntPoint BestPosition = { 100000, -100000 };
  TSharedPtr<GraphNode> BestNodeA = MakeShared<GraphNode>();
  TSharedPtr<Quadrant> BestQuadrantA = MakeShared<Quadrant>();
  TSharedPtr<Door> BestDoorA = MakeShared<Door>();
  TSharedPtr<Door> BestDoorB = MakeShared<Door>();
  TSharedPtr<GraphNode> Q1ToAdd = GetRandomQ1();
  int32 BestScore = -INT_MAX;

  for (TSharedPtr<GraphNode> Node : m_pGraph->GetNodes())
  {
    for (TSharedPtr<Quadrant> Quadrant : Node->GetQuadrants())
    {
      for (TSharedPtr<Door> Door : Quadrant->Doors)
      {
        if (Door->IsDoorUsed)
        {
          continue;
        }
        FIntPoint DirectionOffset = { 0,0 };
        switch (Door->Direction)
        {
        case 1:
          DirectionOffset = { 0, -1 };
          break;
        case 2:
          DirectionOffset = { 1, 0 };
          break;
        case 3:
          DirectionOffset = { 0, 1 };
          break;
        case 4:
          DirectionOffset = { -1, 0 };
        }

        FIntPoint CurrentPos = Quadrant->Location + Node->GetPosition() + DirectionOffset;

        int32 CurrentScore = CurrentPos.Y - CurrentPos.X;

        if (CurrentScore > BestScore)
        {
          BestScore = CurrentScore;
          Q1ToAdd->SetPosition(CurrentPos);

          if (!IsValidPlacement(Q1ToAdd))
          {
            continue;
          }

          BestPosition = CurrentPos;
          BestNodeA = Node;
          BestQuadrantA = Quadrant;
          BestDoorA = Door;
          BestDoorB->Direction = BestDoorA->GetOppositeDirection();
          PlayerStartPosition = BestPosition;
        }
      }
    }
  }

  if (BestPosition == FIntPoint(100000, -100000))
  {// todo: maybe here set another spawn point?
    UE_LOG(LogTemp, Display, TEXT("FATAL ERROR: Could not Spawn a Q1 for the player"));
    return false;
  }

  if (BestNodeA->GetRoomShape() == ERoomShape::Q1)
  {
    PlayerRoomStart = BestNodeA->GetNodeID();
    PlayerStartPosition = BestNodeA->GetPosition();
    BestNodeA->SetHasCleaningStation(true);
  }
  else
  {
    Q1ToAdd->SetPosition(BestPosition);

    UpdateQuadrantsAndDoors(BestNodeA, BestQuadrantA, BestDoorA);
    UpdateQuadrantsAndDoors(Q1ToAdd, Q1ToAdd->GetQuadrants()[0], BestDoorB);
    Q1ToAdd->SetHasCleaningStation(true);
    m_pGraph->AddNode(Q1ToAdd);
    PlayerRoomStart = Q1ToAdd->GetNodeID();
    m_pGraph->AddEdge(Q1ToAdd, BestNodeA);
  }
  return true;
}

bool UProceduralGeneration::GenerateCleaningStations()
{
  TArray<TSharedPtr<GraphNode>> Q1s = m_pGraph->GetQ1Nodes();

  // 1) Adding the player spawning room as the first room that will contain a cleaning station
  CleaningStationLocations.Reserve(CleaningStationToAdd);
  CleaningStationLocations.Add(PlayerStartPosition);

  // 2) Including the Existing Positions
  for (const TSharedPtr<GraphNode>& Q1 : Q1s)
  {
    if (CleaningStationLocations.Num() >= CleaningStationToAdd)
    {
      break;
    }

    bool bTooClose = false;
    for (const FIntPoint& CleaningStationLocation : CleaningStationLocations)
    {
      if (DistanceSqr(CleaningStationLocation, Q1->GetPosition()) < MinDistanceBetweenCleaningStationsSqr)
      {
        bTooClose = true;
        break;
      }
    }
    if (!bTooClose)
    {
      Q1->SetHasCleaningStation(true);
      CleaningStationLocations.Add(Q1->GetPosition());
    }
  }

  // 3) Add more positions in case we don't have the minimum locations
  TSharedPtr<Door> BestQ1Door = MakeShared<Door>();
  int32 CurrentTries = 0;
  while (CleaningStationLocations.Num() < CleaningStationToAdd && CurrentTries < MaxTriesAddingQ1)
  {
    for (TSharedPtr<GraphNode> Node : m_pGraph->GetNodes())
    {
      for (TSharedPtr<Quadrant> ExteriorQuadrant : Node->GetQuadrants())
      {
        for (TSharedPtr<Door> Door : ExteriorQuadrant->Doors)
        {
          if (Door->IsDoorUsed)
          {
            continue;
          }

          FIntPoint DirectionOffset = { 0,0 };
          switch (Door->Direction)
          {
          case 1:
            DirectionOffset = { 0, -1 };
            break;
          case 2:
            DirectionOffset = { 1, 0 };
            break;
          case 3:
            DirectionOffset = { 0, 1 };
            break;
          case 4:
            DirectionOffset = { -1, 0 };
          }

          FIntPoint CurrentPos = ExteriorQuadrant->Location + Node->GetPosition() + DirectionOffset;
          TSharedPtr<GraphNode> Q1ToAdd = GetRandomQ1();
          Q1ToAdd->SetPosition(CurrentPos);

          if (!IsValidPlacement(Q1ToAdd) || !HasMinimumDistance(CurrentPos))
          {
            CurrentTries++;
            continue;
          }

          BestQ1Door->Direction = Door->GetOppositeDirection();
          UpdateQuadrantsAndDoors(Node, ExteriorQuadrant, Door);
          UpdateQuadrantsAndDoors(Q1ToAdd, Q1ToAdd->GetQuadrants()[0], BestQ1Door);
          m_pGraph->AddNode(Q1ToAdd);
          m_pGraph->AddEdge(Q1ToAdd, Node);
          Q1ToAdd->SetHasCleaningStation(true);
          CleaningStationLocations.Add(Q1ToAdd->GetPosition());
          UE_LOG(LogTemp, Display, TEXT("Adding external connection to Node ID%d."), Node->GetNodeID());
          break;
        }
      }
    }
  }
  return CleaningStationLocations.Num() >= CleaningStationToAdd;
}

float UProceduralGeneration::DistanceSqr(const FIntPoint& PointA, const FIntPoint& PointB) const
{
  int32 Dx = PointA.X - PointB.X;
  int32 Dy = PointA.Y - PointB.Y;

  return Dx * Dx + Dy * Dy;
}

bool UProceduralGeneration::HasMinimumDistance(const FIntPoint& NewPoint) const
{
  for (const FIntPoint& Point : CleaningStationLocations)
  {
    if (DistanceSqr(Point, NewPoint) < MinDistanceBetweenCleaningStationsSqr)
    {
      return false;
    }
  }
  return true;
}

void UProceduralGeneration::ExportAndDraw()
{
  UE_LOG(LogTemp, Display, TEXT("Cleaning Station Locaitons are:"));

  for (const FIntPoint& Point : CleaningStationLocations)
  {
    UE_LOG(LogTemp, Display, TEXT("\t (%d, %d)"), Point.X, Point.Y);
  }
  m_pGraph->ExportToFile("Graph.txt");
  DrawLayout();
  SpawnCleaningStations();
}

void UProceduralGeneration::DrawLayout()
{
  for (TSharedPtr<GraphNode> Room : m_pGraph->GetNodes())
  {
    bool bSuccess;
    FRotator FinalRotation = FRotator(0, Room->GetRotation(), 0);
    FVector Position = FVector(Room->GetPosition().X, Room->GetPosition().Y, 0);
    FVector2D Offset = FVector2D::ZeroVector;
    int32 YawRotation = Room->GetRotation();
    switch (YawRotation)
    {
    case 0:
      Offset = FVector2D(0, 0);
      break;
    case 90:
      Offset = FVector2D(Room->GetSize().Y, 0);
      break;
    case 180:
      Offset = FVector2D(Room->GetSize().X, Room->GetSize().Y);
      break;
    case 270:
      Offset = FVector2D(0, Room->GetSize().X);
      break;
    }

    FVector FinalPosition(
      (Room->GetPosition().X + Offset.X) * 1600.0f,
      (Room->GetPosition().Y + Offset.Y) * 1600.0f,
      0.0f
    );
    ULevelStreamingDynamic* StreamingLevel = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), Room->GetLevelInstance(), FinalPosition, FinalRotation, bSuccess);

    if (bSuccess && StreamingLevel)
    {
      PendingRooms.Add(StreamingLevel, Room);
      StreamingLevel->OnLevelShown.AddUniqueDynamic(this, &UProceduralGeneration::HandleLevelShown);
    }
    else
    {
      UE_LOG(LogTemp, Display, TEXT("Could not instanciate this Room."));
    }
  }
}

void UProceduralGeneration::SpawnCleaningStations()
{
  for (const FIntPoint& CleaningStationLocation : CleaningStationLocations)
  {
    const FVector& CleaningStationVectorLocation = FVector((CleaningStationLocation * 1600.0f) + FIntPoint(800, 800), 0);
    AActor* CleaningStation = GetWorld()->SpawnActor<AActor>(m_oCleaningStationClass, CleaningStationVectorLocation, FRotator::ZeroRotator);
    if (bEndlessMode && IsValid(CleaningStation))
    {
      if (UInteractableCleaningCupboard* CleaningStationComp = CleaningStation->FindComponentByClass<UInteractableCleaningCupboard>())
      {
        CleaningStationComp->AddTime(CurrentEndlessValues.CurrentExtraCleaningCupboardTime);
      }
    }
  }
}

void UProceduralGeneration::HandleLevelShown()
{
  // sice we can't know which one has finished we have to check them all, and remove the ones that are actually loaded
  for (auto It = PendingRooms.CreateIterator(); It; ++It)
  {
    TWeakObjectPtr<ULevelStreamingDynamic> Stream = It.Key();
    TSharedPtr<GraphNode> Node = It.Value();

    if (!Stream.IsValid() || !Node)
    { // we remove the level instance if it is not valid
      It.RemoveCurrent();
      continue;
    }
    if (!Stream->GetLoadedLevel() || !Stream->GetLoadedLevel()->bIsVisible)
    { // in case it is loaded but it is not visible yet we delay it until it is visible
      continue;
    }

    // Adding the new Room into the Level Manager
    UBaseRoom* NewRoom = NewObject<UBaseRoom>(this);
    float EnemyPercentage = GetEnemyPercentage(Node);
    TSharedPtr<FSpawnChances> SpawnChances = GetEnemySpawnChances(Node, EnemyPercentage);
    NewRoom->Init(Stream->GetLoadedLevel(), Node->GetRoomType(), FVector(Node->GetPosition() * 1600.0f, 0.0f), Node->GetNodeID(), Node->GetDirtyArea(), EnemyPercentage, m_fMitigation, Node->GetQuadrants(), m_fBaseDeactivateRoomByBuff, SpawnChances, Node->HasCleaningStation());
    NewRoom->SetRotation(Node->GetRotation());
    NewRoom->SetRoomShape(Node->GetRoomShape());

    m_pLevelManager->AddRoom(NewRoom);
    UProceduralGeneration::OnLevelReady.Broadcast(NewRoom);

    Stream->OnLevelShown.RemoveAll(this);
    It.RemoveCurrent();

    int32 NumberOfSameRoom = 0;

    for (UBaseRoom* CR : m_pLevelManager->GetRooms())
    {
      if (CR->GetLevelId() != NewRoom->GetLevelId())
      {
        if (CR->GetStencilId() == NewRoom->GetStencilId())
        {
          NewRoom->SetStencilId(StencilIndex);
          for (AActor* Actor : NewRoom->GetLevel()->Actors)
          {
            if (!Actor)
            {
              continue;
            }
            TArray<UStaticMeshComponent*> MeshComponents;
            Actor->GetComponents<UStaticMeshComponent>(MeshComponents);
            for (UStaticMeshComponent* SMC : MeshComponents)
            {
              if (!SMC) continue;

              SMC->SetRenderCustomDepth(true);
              SMC->SetCustomDepthStencilValue(StencilIndex);
            }
          }
          --StencilIndex;
        }
      }
    }

    if (m_pLevelManager->GetRooms().Num() == m_pGraph->GetNodes().Num())
    {
      SpawnPlayer();
      
      if (bEndlessMode && pGameInstance->m_iCurrentEndlessLevel >= 1)
      {
        if (APawn* Character = UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
        {
          if (ABasePlayer* Player = Cast<ABasePlayer>(Character))
          {
            Player->SetPrimaryConsumableAmount(pGameInstance->PrimaryConsumableAmount);
            Player->SetSecondaryConsumableAmount(pGameInstance->SecondaryConsumableAmount);
            Player->SetCurrentHealth(pGameInstance->HealthToRemove);
            if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
              pAchSubsystem->SetEndlessAchievements(pGameInstance->m_iCurrentEndlessLevel);
            }
          }
        }
      }

      if (UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
      {
        NavSys->Build();
      }
      TArray<AActor*> Tiles;
      UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseTile::StaticClass(), Tiles);
      Cast<ABaseTile>(Tiles[0])->InitTile();

      m_pLevelManager->OnAllRoomsCreated.Broadcast();

      SetupNeighbors();
      SetupAdjacents();

      UTimeManager* MyTimerManager = GetWorld()->GetSubsystem<UTimeManager>();
      if (MyTimerManager)
      {
        if (bEndlessMode && IsValid(pGameInstance) && IsValid(pGameInstance->m_pEndlessData))
        {
          SubtractSeconds(CurrentEndlessValues.CurrentLessInitialTime, pGameInstance->m_pEndlessData->MinMinutes, pGameInstance->m_pEndlessData->MinSeconds);
        }

        MyTimerManager->StartTimer(Minutes, Seconds);
        pGameInstance->SecondsLastedInEndless += Minutes * 60 + Seconds;
        MyTimerManager->PauseTimer();
      }

      for (UBaseRoom* Room : m_pLevelManager->GetRooms())
      {
        if (Room->GetLevelId() == PlayerRoomStart)
        {
          m_pLevelManager->SetCurrentRoom(Room);
        }
      }

      UProceduralGeneration::OnSpawnersReady.Broadcast();
    }
  }
}

float UProceduralGeneration::GetEnemyPercentage(const TSharedPtr<GraphNode>& Node) const
{
  for (FAvailableRoom AvailableRoom : m_lAvailableRoomTypes)
  {
    if (AvailableRoom.m_eRoomType == Node->GetRoomType())
    {
      return AvailableRoom.m_fSpawnerPercentage;
    }
  }
  return 0.0f;
}

TSharedPtr<FSpawnChances> UProceduralGeneration::GetEnemySpawnChances(const TSharedPtr<GraphNode>& Node, float& OutSpawnerPercentage) const
{
  for (FAvailableRoom AvailableRoom : m_lAvailableRoomTypes)
  { // @TODO: maybe we have to use a reference, not a copy, which means FAvailableRoom must have a Copy Constructor
    if (AvailableRoom.m_eRoomType == Node->GetRoomType())
    {
      const TArray<FSpawnChances>& SpawnArray = AvailableRoom.SpawnDistribution;

      if (SpawnArray.Num() == 0)
      {
        return nullptr;
      }

      float TotalWeight = 0.0f;
      for (const FSpawnChances& Chance : SpawnArray)
      {
        TotalWeight += Chance.m_fProbabilityWeight;
      }

      float RandomValue = m_pRandom->GetRandomFloatInRange(0.0f, TotalWeight);
      float Accumulated = 0.0f;
      for (int32 i = 0; i < SpawnArray.Num(); ++i)
      {
        Accumulated += SpawnArray[i].m_fProbabilityWeight;
        if (RandomValue <= Accumulated)
        {
          OutSpawnerPercentage = (AvailableRoom.SpawnDistribution[i].m_fSpawnerPercentageOverride > 0.0f) ? AvailableRoom.SpawnDistribution[i].m_fSpawnerPercentageOverride : OutSpawnerPercentage;
          OutSpawnerPercentage = FMath::Clamp(bEndlessMode ? OutSpawnerPercentage * CurrentEndlessValues.CurrentSpawnerMultiplier : OutSpawnerPercentage, 0.0f, 1.0f);
          return  MakeShared<FSpawnChances>(AvailableRoom.SpawnDistribution[i]);
        }
      }
      return  MakeShared<FSpawnChances>(AvailableRoom.SpawnDistribution.Last());
    }
  }
  return nullptr;
}

void UProceduralGeneration::SetupNeighbors() const
{
  for (TSharedPtr<GraphNode> Node : m_pGraph->GetNodes())
  {
    UBaseRoom* Room = GetRoomFromNode(Node);

    if (!Room)
    {
      continue;
    }

    for (TSharedPtr<GraphNode> Neighbor : m_pGraph->GetConnectedNodes(Node))
    {
      UBaseRoom* NeighborRoom = GetRoomFromNode(Neighbor);
      if (!NeighborRoom)
      {
        continue;
      }
      Room->AddNeighbor(NeighborRoom);
      
      // Set Doors Icon:
      for (TObjectPtr<ADissolvableDoor>& NeighborDoorPtr : NeighborRoom->m_lDissolvableDoors)
      {
        if (!IsValid(NeighborDoorPtr)) continue;

        ADissolvableDoor* NeighborDoor = NeighborDoorPtr.Get();
        ADissolvableDoor* ClosestDoorInRoom = nullptr;
        float MinDistance = FLT_MAX;

        for (TObjectPtr<ADissolvableDoor>& DoorPtr : Room->m_lDissolvableDoors)
        {
          if (!IsValid(DoorPtr)) continue;

          ADissolvableDoor* Door = DoorPtr.Get();
          float Distance = FVector::Dist(NeighborDoor->GetActorLocation(), Door->GetActorLocation());

          if (Distance < MinDistance)
          {
            MinDistance = Distance;
            ClosestDoorInRoom = Door;
          }
        }

        if (ClosestDoorInRoom && MinDistance < 200.0f)
        {
          bool bSetIcon = NeighborRoom->GetDirtStage() != EDirtStage::CLEAN;
          ClosestDoorInRoom->SetIconMaterial(NeighborRoom->GetRoomType(), bSetIcon);

          break;
        }
      }
    }

    for (TSharedPtr<GraphNode> Neighbor : m_pGraph->GetUnconnectedNodes(Node))
    {
      UBaseRoom* NeighborRoom = GetRoomFromNode(Neighbor);
      if (!NeighborRoom)
      {
        continue;
      }
      Room->AddNonNeighbor(NeighborRoom);
    }
  }
}

void UProceduralGeneration::SetupAdjacents() const
{
  for (TSharedPtr<GraphNode> Node : m_pGraph->GetNodes())
  {
    UBaseRoom* Room = GetRoomFromNode(Node);

    if (!Room)
    {
      continue;
    }

    for (TSharedPtr<GraphNode> Adjacent : Node->GetAdjacentRoomsSouthAndWest(m_pGraph->GetNodes()))
    {
      UBaseRoom* NeighborRoom = GetRoomFromNode(Adjacent);
      if (!NeighborRoom)
      {
        continue;
      }
      Room->AddAdjacent(NeighborRoom);
    }
  }
}

UBaseRoom* UProceduralGeneration::GetRoomFromNode(TSharedPtr<GraphNode> Node) const
{
  for (UBaseRoom* Room : m_pLevelManager->GetRooms())
  {
    if (Room->GetLevelId() == Node->GetNodeID())
    {
      return Room;
    }
  }
  return nullptr;
}

void UProceduralGeneration::SpawnPlayer() const
{
  const FVector& PlayerSpawnLocation = FVector((PlayerStartPosition * 1600.0f) + pGameInstance->OffsetPlayerStart, 110.0f);
  const FRotator& PlayerSpawnRotation = FRotator::ZeroRotator;

  if (AGameModeBase* GM = UGameplayStatics::GetGameMode(this))
  {
    if (AProceduralGameMode* PGM = Cast<AProceduralGameMode>(GM))
    {
      PGM->SpawnPlayerAt(PlayerSpawnLocation, PlayerSpawnRotation);
      OnPlayerSpawn.Broadcast();
    }
  }
}
void UProceduralGeneration::UpdateEndlessValues()
{
  if (!IsValid(pGameInstance))
  {
    return;
  }

  if (pGameInstance->m_iCurrentEndlessLevel >= 0)
  {
    int32 Step = pGameInstance->m_iCurrentEndlessLevel % 3;

    switch (Step)
    {
    case 0:
      UpdateInfiniteValues();
      break;
    case 1:
      if (!UpdateFiniteValues())
      {
        UpdateInfiniteValues();
      }
      break;
    case 2:
      UpdateFiniteValues();
      UpdateInfiniteValues();
      break;
    }
  }
  else
  {
    CurrentEndlessValues.CurrentExtraCleaningCupboardTime = 0;
    CurrentEndlessValues.CurrentExtraQuads = 0;
    CurrentEndlessValues.CurrentLessInitialTime = 0.0f;
    CurrentEndlessValues.CurrentSpawnerMultiplier = 1.0f;
    pGameInstance->SecondsLastedInEndless = 0;
    pGameInstance->EnemiesKilledInEndless = 0;
  }

  ++pGameInstance->m_iCurrentEndlessLevel;
}
void UProceduralGeneration::UpdateInfiniteValues()
{
  if (!IsValid(pGameInstance) || !IsValid(pGameInstance->m_pEndlessData))
  {
    return;
  }

  CurrentEndlessValues.CurrentExtraQuads += pGameInstance->m_pEndlessData->MaxQuadsStep;
  CurrentEndlessValues.CurrentExtraCleaningCupboardTime += pGameInstance->m_pEndlessData->CleaningStationChargeStep;
}
bool UProceduralGeneration::UpdateFiniteValues()
{
  if (!IsValid(pGameInstance) || !IsValid(pGameInstance->m_pEndlessData))
  {
    return false;
  }

  if (Minutes * 60 + Seconds > pGameInstance->m_pEndlessData->MinMinutes * 60 + pGameInstance->m_pEndlessData->MinSeconds)
  {
    CurrentEndlessValues.CurrentLessInitialTime += pGameInstance->m_pEndlessData->TimeToDeductStep;
    CurrentEndlessValues.CurrentSpawnerMultiplier += pGameInstance->m_pEndlessData->ActiveSpawnersStep;
    return true;
  }

  return false;
}
void UProceduralGeneration::SubtractSeconds(int32 DeltaSeconds, int32 MinMinutes, int32 MinSeconds)
{
  int32 CurrentTotal = Minutes * 60 + Seconds;
  int32 MinTotal = MinMinutes * 60 + MinSeconds;

  CurrentTotal -= DeltaSeconds;

  if (CurrentTotal < MinTotal)
  {
    CurrentTotal = MinTotal;
  }

  Minutes = CurrentTotal / 60;
  Seconds = CurrentTotal % 60;
}
#pragma endregion