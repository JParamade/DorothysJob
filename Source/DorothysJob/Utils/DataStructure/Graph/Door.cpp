#include "Door.h"

bool Door::IsCompatible(TSharedPtr<Door> DoorB)
{
  return ((Direction - DoorB->Direction + 4) % 4) == 2;
}

int32 Door::GetOppositeDirection()
{
  switch (Direction)
  {
  case 1:
    return 3;
  case 2:
    return 4;
  case 3:
    return 1;
  case 4:
    return 2;
  default:
    return 0;
  }
}
