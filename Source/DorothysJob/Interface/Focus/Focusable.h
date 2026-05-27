/************************************************************************
 * @description: Represents focusable UI elements interface.
 * @author: Jaime Paramo
 * @date: 19/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Focusable.generated.h"

// This class does not need to be modified !
UINTERFACE(MinimalAPI, Blueprintable)
class UFocusable : public UInterface
{
  GENERATED_BODY()
};

class DOROTHYSJOB_API IFocusable
{
  GENERATED_BODY()
public:
  /**
  * @brief -> Called when the element gains focus. Use this to trigger visual or logical changes when the UI element becomes the active or highlighted element.
  */
  virtual void OnGainFocus() = 0;

  /**
   * @brief -> Called when the element loses focus. Use this to revert changes or handle logic when the UI element is no longer the active one.
   */
  virtual void OnLoseFocus() = 0;

  /**
   * @brief -> Called when the element is being pressed.
   */
  virtual void OnPressed() = 0;
  /**
   * @brief -> Called when the element has being released.
   */
  virtual void OnReleased() = 0;

  /**
   * @brief -> Executes the action associated with the currently focused UI element.
   */
  virtual void ExecuteAction() = 0;
};
