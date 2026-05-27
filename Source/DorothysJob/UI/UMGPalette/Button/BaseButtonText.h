/************************************************************************
 * @description: Represents a base button with text
 * @author: Josephine Esposito
 * @date: 30/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "BaseButton.h"
#include "BaseButtonText.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UBaseButtonText : public UBaseButton
{
	GENERATED_BODY()

protected:

  /**
   * @brief method called when the button is constructed.
   */
  virtual void SynchronizeProperties() override;

	/**
	 * @brief This represents the text of the button
	 */
	UPROPERTY(VisibleInstanceOnly)
	UTextBlock* m_pText;
};
