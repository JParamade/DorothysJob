/************************************************************************
 * @description: The node class for the graph data structure
 * @author: Josephine Esposito
 * @date: 12/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#include "GraphNode.h"

#include "DorothysJob/Data/DataTables/RoomData.h" // for the handling of the room data row structure
#include "DorothysJob/Actor/Room/BaseRoom.h"      // for the room enums
#include "DorothysJob/Utils/DataStructure/Graph/Quadrant.h"      // for the room enums
#include "DorothysJob/Utils/DataStructure/Graph/Door.h"      // for the room enums

const TMap<ERoomShape, TArray<FIntPoint>> GraphNode::ShapeCellMap =
{
    { ERoomShape::Q1, { {0,0} } },
    { ERoomShape::Q4, { {0,0}, {1,0}, {0,1}, {1,1} } },
    { ERoomShape::R2, { {0,0}, {0,1} } },
    { ERoomShape::R3, { {0,0}, {0,1}, {0,2} } },
    { ERoomShape::L3, { {0,0}, {1,0}, {0,1} } },
    { ERoomShape::L4, { {0,0}, {1,0}, {0,1}, {0,2} } },
    { ERoomShape::S,  { {0,0}, {0,1}, {1,1}, {1,2} } },
    { ERoomShape::T,  { {0,0}, {0,1}, {0,2}, {1,1} } },
};


GraphNode::GraphNode() :
  Position(0, 0),
  Size(0, 0),
  RoomID(INDEX_NONE),
  NodeID(INDEX_NONE),
  Rotation(0),
  RoomShape(ERoomShape::Q1)
{}

GraphNode::GraphNode(const GraphNode& Other) :
  Position(Other.Position),
  Size(Other.Size),
  RoomID(Other.RoomID),
  NodeID(Other.NodeID),
  Rotation(Other.Rotation),
  RoomType(Other.RoomType),
  RoomPlacement(Other.RoomPlacement),
  RoomShape(Other.RoomShape),
  LevelInstance(Other.LevelInstance),
  DirtyArea(Other.DirtyArea)
{
  Quadrants.Reserve(Other.Quadrants.Num());
  for (const TSharedPtr<Quadrant>& Q : Other.Quadrants)
  {
    // New Quadrant
    TSharedPtr<Quadrant> NewQ = MakeShared<Quadrant>();
    NewQ->Location = Q->Location;
    NewQ->bIsExterior = Q->bIsExterior;
    // Clone each door into the Quadrant
    NewQ->Doors.Reserve(Q->Doors.Num());
    for (const TSharedPtr<Door>& D : Q->Doors)
    {
      TSharedPtr<Door> NewD = MakeShared<Door>();
      NewD->Direction = D->Direction;
      NewD->IsDoorUsed = D->IsDoorUsed;
      NewQ->Doors.Add(NewD);
    }
    Quadrants.Add(NewQ);
  }
}

bool GraphNode::operator==(const TSharedPtr<GraphNode>& Other) const
{
  return Other.IsValid() && Other->NodeID == NodeID;
}

bool GraphNode::IsOverlapping(const TSharedPtr<GraphNode>& Other) const
{
  TSet<FIntPoint> Occupied;
  Occupied.Reserve(Quadrants.Num());

  for (const TSharedPtr<Quadrant>& Q : Quadrants)
  {
    Occupied.Add(Position + Q->Location);
  }

  for (const TSharedPtr<Quadrant>& Q : Other->Quadrants)
  {
    if (Occupied.Contains(Other->Position + Q->Location))
    {
      return true;
    }
  }

  return false;
}

#pragma region | Setters
void GraphNode::SetRoomData(const FRoomData& _pRoomData)
{
  Size = _pRoomData.RoomSize;
  RoomID = _pRoomData.RoomID;
  RoomType = _pRoomData.RoomType;
  RoomShape = _pRoomData.RoomShape;
  LevelInstance = _pRoomData.RoomLevelInstance;
  DirtyArea = _pRoomData.Percentage;

  TMap<FIntPoint, TSharedPtr<Quadrant>> QuadrantByCell;
  for (const FIntPoint& Cell : GraphNode::GetShapeCells(RoomShape))
  {
    TSharedPtr<Quadrant> NewQuadrant = MakeShared<Quadrant>();
    NewQuadrant->Location = Cell;

    QuadrantByCell.Add(Cell, NewQuadrant);
  }

  // Set exterior Quadrants
  for (auto& Elem : QuadrantByCell)
  {
    const FIntPoint& Location = Elem.Key;
    TSharedPtr<Quadrant>& Quad = Elem.Value;

    int32 NeighborCount = 0;
    const TArray<FIntPoint> Offsets = {
        FIntPoint(1, 0),   // right
        FIntPoint(-1, 0),  // left
        FIntPoint(0, 1),   // down
        FIntPoint(0, -1)   // up
    };

    for (const FIntPoint& Offset : Offsets)
    {
      const FIntPoint NeighborLocation = Location + Offset;
      if (QuadrantByCell.Contains(NeighborLocation))
      {
        NeighborCount++;
      }
    }

    Quad->bIsExterior = (NeighborCount <= 1);
  }

  for (const FIntPoint& CurrentDoor : _pRoomData.Doors)
  {
    const int32 QuadIndex = CurrentDoor.X;
    const int32 Dir = CurrentDoor.Y;
    const FIntPoint  Cell = IndexToCell(QuadIndex);
    if (TSharedPtr<Quadrant>* Found = QuadrantByCell.Find(Cell))
    {
      TSharedPtr<Door> NewDoor = MakeShared<Door>();
      NewDoor->Direction = Dir;
      (*Found)->Doors.Add(NewDoor);
    }
  }

  Quadrants.Empty();
  QuadrantByCell.GenerateValueArray(Quadrants);
}
void GraphNode::SetPosition(const FIntPoint& NewPosition)
{
  Position = NewPosition;
}
void GraphNode::SetNodeID(int32 NewNodeID)
{
  NodeID = NewNodeID;
}
void GraphNode::SetRotation(int32 NewRotation)
{
  Rotation = NewRotation;
  Quadrants = RotateQuadrants(NewRotation);
}
void GraphNode::SetHasCleaningStation(bool _bHasCleaningStation)
{
  bHasCleaningStation = _bHasCleaningStation;
}
#pragma endregion

#pragma region | Getters
const FIntPoint& GraphNode::GetPosition() const
{
  return Position;
}
const FIntPoint& GraphNode::GetSize() const
{
  return Size;
}
int32 GraphNode::GetRoomID() const
{
  return RoomID;
}
int32 GraphNode::GetNodeID() const
{
  return NodeID;
}
int32 GraphNode::GetRotation() const
{
  return Rotation;
}
const TArray<TSharedPtr<Quadrant>>& GraphNode::GetQuadrants() const
{
  return Quadrants;
}
const TArray<TSharedPtr<Quadrant>> GraphNode::GetExteriorQuadrants() const
{
  if (RoomShape == ERoomShape::Q4)
  {
    return Quadrants;
  }

  TArray<TSharedPtr<Quadrant>> ExteriorQuadrants;
  for (const TSharedPtr<Quadrant>& Quadrant : Quadrants)
  {
    if (Quadrant->bIsExterior)
    {
      ExteriorQuadrants.Add(Quadrant);
    }
  }
  return ExteriorQuadrants;
}
const TArray<TSharedPtr<Quadrant>> GraphNode::GetExteriorNotConnectedQuadrants() const
{
  TArray<TSharedPtr<Quadrant>> Result;

  for (const auto& Quad : GetExteriorQuadrants())
  {
    bool bHasUsedDoor = false;
    for (const auto& Door : Quad->Doors)
    {
      if (Door->IsDoorUsed)
      {
        bHasUsedDoor = true;
        break;
      }
    }
    if (!bHasUsedDoor)
    {
      Result.Add(Quad);
    }
  }

  return Result;
}
const TArray<TSharedPtr<Quadrant>> GraphNode::GetNotConnectedQuadrants() const
{
  TArray<TSharedPtr<Quadrant>> Result;

  for (const auto& Quad : Quadrants)
  {
    bool bHasUsedDoor = false;
    for (const auto& Door : Quad->Doors)
    {
      if (Door->IsDoorUsed)
      {
        bHasUsedDoor = true;
        break;
      }
    }
    if (!bHasUsedDoor)
    {
      Result.Add(Quad);
    }
  }

  return Result;
}
ERoomType GraphNode::GetRoomType() const
{
  return RoomType;
}
ERoomPlacement GraphNode::GetRoomPlacement() const
{
  return RoomPlacement;
}
ERoomShape GraphNode::GetRoomShape() const
{
  return RoomShape;
}
TArray<TSharedPtr<Door>> GraphNode::GetDoors() const
{
  TArray<TSharedPtr<Door>> Out;
  for (TSharedPtr<Quadrant> Q : Quadrants)
  {
    for (TSharedPtr<Door> D : Q->Doors)
    {
      Out.Add(D);
    }
  }
  return TArray<TSharedPtr<Door>>();
}
float GraphNode::GetScore() const
{
  double Sum = 0.0f;
  for (const TSharedPtr<Quadrant>& Q : Quadrants)
  {
    Sum += FVector2D(Q->Location + Position).Size();
  }
  return static_cast<float>(Sum / static_cast<double>(Quadrants.Num()));
}
const TSoftObjectPtr<UWorld>& GraphNode::GetLevelInstance() const
{
  return LevelInstance;
}
float GraphNode::GetDirtyArea() const
{
  return DirtyArea;
}
bool GraphNode::HasCleaningStation() const
{
    return bHasCleaningStation;
}
TSet<FIntPoint> GraphNode::GetGlobalCells() const
{
  TSet<FIntPoint> Cells;
  for (const TSharedPtr<Quadrant>& LocalQuad : Quadrants)
  {
    Cells.Add(Position + LocalQuad->Location);
  }
  return Cells;
}
bool GraphNode::AreRoomsAdjacentInDirections(const TSharedPtr<GraphNode> A, const TSharedPtr<GraphNode> B, const TArray<FIntPoint>& Directions)
{
  if (!A || !B) return false;

  TSet<FIntPoint> CellsA = A->GetGlobalCells();
  TSet<FIntPoint> CellsB = B->GetGlobalCells();

  for (const FIntPoint& CellA : CellsA)
  {
    for (const FIntPoint& Dir : Directions)
    {
      FIntPoint NeighborPos = CellA + Dir;
      if (CellsB.Contains(NeighborPos))
      {
        return true;
      }
    }
  }

  return false;
}
TArray<TSharedPtr<GraphNode>> GraphNode::GetAdjacentRoomsSouthAndWest(const TArray<TSharedPtr<GraphNode>>& AllRooms) const
{
  TArray<TSharedPtr<GraphNode>> Result;

  const TArray<FIntPoint> Directions = { FIntPoint(-1, 0), FIntPoint(0, 1), FIntPoint(-1, 1) };

  for (const TSharedPtr<GraphNode>& Other : AllRooms)
  {
    if (!Other || Other->GetNodeID() == NodeID) continue;
    TSharedPtr<GraphNode> SharedThis = MakeShared<GraphNode>(*this);
    if (AreRoomsAdjacentInDirections(SharedThis, Other, Directions))
    {
      Result.Add(Other);
    }
  }

  return Result;
}
#pragma endregion

#pragma region | Private Functions

const TArray<FIntPoint>& GraphNode::GetShapeCells(ERoomShape Shape) const
{
  static const TArray<FIntPoint> Empty;

  if (const TArray<FIntPoint>* Cells = ShapeCellMap.Find(Shape))
  {
    return *Cells;
  }
  return Empty;
}

const FIntPoint GraphNode::IndexToCell(int32 Index) const
{
  const int32 ZeroBased = Index - 1;
  const int32 Col = ZeroBased % 3;
  const int32 Row = ZeroBased / 3;
  return FIntPoint(Col, Row);
}

const TArray<TSharedPtr<Quadrant>> GraphNode::RotateQuadrants(int32 RotationDegrees) const
{
  TArray<TSharedPtr<Quadrant>> Out;
  int32 Turns = ((RotationDegrees % 360) + 360) % 360 / 90;
  if (Turns == 0)
  {
    Out = Quadrants;
    return Out;
  }

  const int32 W = Size.X;
  const int32 H = Size.Y;

  auto RotateLocal = [&](int32 x, int32 y) -> FIntPoint
    {
      switch (Turns & 3)             // 0 = 0°, 1 = 90° CCW, 2 = 180°, 3 = 270° CCW
      {
      case 1:  return { H - 1 - y,  x };
      case 2:  return { W - 1 - x,  H - 1 - y };
      case 3:  return { y,          W - 1 - x };
      default: return { x,          y };
      }
    };

  for (const TSharedPtr<Quadrant>& Q : Quadrants)
  {
    TSharedPtr<Quadrant> NewQuadrant = MakeShared<Quadrant>();

    NewQuadrant->Doors = RotateDoors(Q->Doors, Turns);
    NewQuadrant->Location = RotateLocal(Q->Location.X, Q->Location.Y);
    NewQuadrant->bIsExterior = Q->bIsExterior;
    Out.Add(NewQuadrant);
  }

  return Out;
}

const TArray<TSharedPtr<Door>> GraphNode::RotateDoors(const TArray<TSharedPtr<Door>>& Doors, int32 Turns) const
{
  TArray<TSharedPtr<Door>> Out;
  for (const TSharedPtr<Door>& D : Doors)
  {
    TSharedPtr<Door> NewDoor = MakeShared<Door>();
    int32 NewDirection = ((D->Direction - 1 + Turns) % 4 + 4) % 4 + 1;
    NewDoor->Direction = NewDirection;
    NewDoor->IsDoorUsed = D->IsDoorUsed;
    Out.Add(NewDoor);
  }
  return Out;
}
#pragma endregion
//EOF