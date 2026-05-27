/************************************************************************
 * @description: Button to reload the current active world
 * @author: Josephine Esposito
 * @date: 25/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "ReloadLevelButton.generated.h"

UCLASS()
class DOROTHYSJOB_API UReloadLevelButton : public UBaseButton
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
