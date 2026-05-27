/************************************************************************
 * @description: Button to load a level.
 * @author: Josephine Esposito
 * @date: 25/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "LoadLevelButton.generated.h"

UCLASS()
class DOROTHYSJOB_API ULoadLevelButton : public UBaseButton
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

private:
	/**
	 * @brief This represents the level to load on click
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13 | Load", meta = (DisplayName = "Level", AllowPrivateAccess = "true"))
  TSoftObjectPtr<UWorld>  m_pLevel;
};
