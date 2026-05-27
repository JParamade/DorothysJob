/************************************************************************
 * @description: The quadrant class for the graph data structure
 * @author: Pablo Velasco
 * @date: 01/07/2025
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"

class Door;

class DOROTHYSJOB_API Quadrant
{
public:
  FIntPoint Location;
  TArray<TSharedPtr<Door>> Doors;
  bool bIsExterior = false;
};
