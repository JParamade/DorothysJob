/************************************************************************
 * @description: Represents our custom Slider UMG element class.
 * @author: Jaime Paramo
 * @date: 12/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

// Interface
#include "DorothysJob/Interface/Focus/Focusable.h"
#include "DorothysJob/Interface/Focus/AxisNavigable.h"

#include "CoreMinimal.h"
#include "Components/Slider.h"
#include "BaseSlider.generated.h"

struct FSliderStyle;

UCLASS()
class DOROTHYSJOB_API UBaseSlider : public USlider, public IFocusable, public IAxisNavigable
{
	GENERATED_BODY()

public:
  /**
   * @brief -> Reference to the custom Slate style asset for the button.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Custom Slider Style"))
  FSliderStyle m_oSliderStyle;

protected:
  /**
   * @brief -> Virtual method called when the button is constructed.
   */
  virtual void SynchronizeProperties() override;

  UFUNCTION()
  virtual void OnGainFocus() override;

  UFUNCTION()
  virtual void OnLoseFocus() override;

  virtual void OnPressed() {};
  virtual void OnReleased() {};
  virtual void ExecuteAction() {};

  virtual void OnAxisInput(FVector2D _vAxisValue) override;

  virtual void OnGainInteraction() {};
  virtual void OnLoseInteraction() {};

  /**
   * @brief -> Applies the default visual style to the button.
   */
  virtual void ApplyDefaultStyle();
  /**
   * @brief -> Applies the visual style for when the button is focused.
   */
  virtual void ApplyVariantStyle(FSlateBrush FSliderStyle::* _pBrushStyle);
};
