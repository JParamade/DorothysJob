/************************************************************************
 * @description: The door class for the graph data structure
 * @author: Pablo Velasco
 * @date: 01/07/2025
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"

class DOROTHYSJOB_API Door
{
public:
  bool IsDoorUsed = false;
  int32 Direction;
  bool IsCompatible(TSharedPtr<Door> DoorB);
  int32 GetOppositeDirection();
};
