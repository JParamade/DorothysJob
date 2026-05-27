/************************************************************************
 * @description: Represents internal navigation for focusable UI elements interface.
 * @author: Jaime Paramo
 * @date: 12/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AxisNavigable.generated.h"

 // This class does not need to be modified !
UINTERFACE(MinimalAPI, Blueprintable)
class UAxisNavigable : public UInterface
{
  GENERATED_BODY()
};

class DOROTHYSJOB_API IAxisNavigable
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Handles axis-based input events, such as movement or navigation.
   * @param _vAxisValue -> 2D vector representing the axis input values.
   */
  virtual void OnAxisInput(FVector2D _vAxisValue) = 0;

  /**
   * @brief -> Called when this object gains interaction focus or becomes active.
   */
  virtual void OnGainInteraction() = 0;
  /**
   * @brief -> Called when this object loses interaction focus or becomes inactive.
   */
  virtual void OnLoseInteraction() = 0;
};