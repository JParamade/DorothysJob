/************************************************************************
 * @description: @TOFILL.
 * @author: Jaime Paramo
 * @date: 29/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "DorothysJob/Interface/Focus/Focusable.h"

#include "CoreMinimal.h"
#include "Components/CheckBox.h"
#include "BaseCheckBox.generated.h"

UCLASS()
class DOROTHYSJOB_API UBaseCheckBox : public UCheckBox, public IFocusable
{
	GENERATED_BODY()
	
protected:
  /**
  * @brief -> Virtual method called when the button is constructed.
  */
  virtual void SynchronizeProperties() override;

  /**
   * @brief -> @TOFILL.
   */
  UFUNCTION()
  virtual void OnGainFocus() override;

  /**
   * @brief -> @TOFILL.
   */
  UFUNCTION()
  virtual void OnLoseFocus() override;

  /**
   * @brief -> @TOFILL.
   */
  virtual void OnPressed() override;
  /**
   * @brief -> @TOFILL.
   */
  virtual void OnReleased() override;
  /**
   * @brief -> @TOFILL.
   */
  virtual void ExecuteAction() override;

  /**
   * @brief -> @TOFILL.
   */
  UFUNCTION()
  virtual void PlayClickSound();

  /**
   * @brief -> Reference to the custom Slate style asset for the button.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | UI", meta = (DisplayName = "Custom Button Style"))
  TObjectPtr<const USlateWidgetStyleAsset> m_pCheckBoxStyleAsset;

  /**
   * @brief -> Applies the default visual style to the button.
   */
  virtual void ApplyDefaultStyle();
  /**
   * @brief -> Applies the visual style for when the button is focused.
   */
  virtual void ApplyVariantStyle(FSlateBrush FCheckBoxStyle::* _pBrushStyle);
};
