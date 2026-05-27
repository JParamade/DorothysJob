/************************************************************************
 * @description: The node class for the graph data structure
 * @author: Josephine Esposito
 * @date: 12/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"

struct FRoomData;                 // the holder of the room data
enum class ERoomShape : uint8;    // the room shape enum
enum class ERoomType : uint8;
enum class ERoomPlacement : uint8;    // the room shape enum
class Quadrant;
class Door;

class DOROTHYSJOB_API GraphNode
{
public:

  GraphNode();

  GraphNode(const GraphNode& Other);

  bool operator==(const TSharedPtr<GraphNode>& Other) const;

  bool IsOverlapping(const TSharedPtr<GraphNode>& Other) const;

#pragma region | Setters
  void SetRoomData(const FRoomData& RoomData);

  void SetPosition(const FIntPoint& NewPosition);

  void SetNodeID(int32 NewNodeID);

  void SetRotation(int32 NewRotation);

  void SetHasCleaningStation(bool _bHasCleaningStation);
#pragma endregion

#pragma region | Getters
  const FIntPoint& GetPosition() const;

  const FIntPoint& GetSize() const;

  int32 GetRoomID() const;

  int32 GetNodeID() const;

  int32 GetRotation() const;

  const TArray<TSharedPtr<Quadrant>>& GetQuadrants() const;

  const TArray<TSharedPtr<Quadrant>> GetExteriorQuadrants() const;

  const TArray<TSharedPtr<Quadrant>> GetExteriorNotConnectedQuadrants() const;

  const TArray<TSharedPtr<Quadrant>> GetNotConnectedQuadrants() const;

  ERoomType GetRoomType() const;

  ERoomPlacement GetRoomPlacement() const;

  ERoomShape GetRoomShape() const;

  TArray<TSharedPtr<Door>> GetDoors() const;

  float GetScore() const;

  const TSoftObjectPtr<UWorld>& GetLevelInstance() const;

  float GetDirtyArea() const;

  bool HasCleaningStation() const;

  TSet<FIntPoint> GetGlobalCells() const;

  static bool AreRoomsAdjacentInDirections(const TSharedPtr<GraphNode> A, const TSharedPtr<GraphNode> B, const TArray<FIntPoint>& Directions);

  TArray<TSharedPtr<GraphNode>> GetAdjacentRoomsSouthAndWest(const TArray<TSharedPtr<GraphNode>>& AllRooms) const;
#pragma endregion

private:
#pragma region | Level Variables

  FIntPoint Position;

  FIntPoint Size;

  int32 RoomID;

  int32 NodeID;

  int32 Rotation;

  TArray<TSharedPtr<Quadrant>> Quadrants;

  ERoomType RoomType;

  ERoomPlacement RoomPlacement;

  ERoomShape RoomShape;

  TSoftObjectPtr<UWorld> LevelInstance;

  float DirtyArea;

  static const TMap<ERoomShape, TArray<FIntPoint>> ShapeCellMap;

  bool bHasCleaningStation = false;
#pragma endregion

#pragma region | Private Functions
  const TArray<FIntPoint>& GetShapeCells(ERoomShape Shape) const;
  const FIntPoint  IndexToCell(int32 Index) const;
  const TArray<TSharedPtr<Quadrant>> RotateQuadrants(int32 RotationDegrees) const;
  const TArray<TSharedPtr<Door>> RotateDoors(const TArray<TSharedPtr<Door>>& Doors, int32 RotationDegrees) const;
#pragma endregion
};
//EOF