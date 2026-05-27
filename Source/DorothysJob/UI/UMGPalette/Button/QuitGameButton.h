/************************************************************************
 * @description: Button to quit from the game
 * @author: David Gonzalez
 * @date: 10/04/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButtonText.h"
#include "QuitGameButton.generated.h"

UCLASS()
class DOROTHYSJOB_API UQuitGameButton : public UBaseButtonText
{
	GENERATED_BODY()
	
public:

  /**
   * @brief Method used to show self, with Visible
   */
  UFUNCTION(BlueprintCallable)
  virtual void Show() override;

  /**
   * @brief Method used to hide self, with Collapse
   */
  UFUNCTION(BlueprintCallable)
  virtual void Hide() override;

  /**
   * @brief Method used to handle onclick events
   */
  UFUNCTION(BlueprintCallable)
  void OnClick();

protected:
  /**
   * @brief method called when the button is constructed.
   */
  virtual void SynchronizeProperties() override;
};
