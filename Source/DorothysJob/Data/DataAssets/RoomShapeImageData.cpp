#include "RoomShapeImageData.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"

FRoomShapeInfo::FRoomShapeInfo() :
  Shape(ERoomShape::Q1)
{
}

const FRoomShapeInfo* URoomShapeImageData::GetInfoForShape(ERoomShape Shape) const
{
  for (const FRoomShapeInfo& Info : ShapesInfo)
  {
    if (Info.Shape == Shape)
      return &Info;
  }
  return nullptr;
}