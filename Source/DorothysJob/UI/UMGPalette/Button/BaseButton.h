/************************************************************************
 * @description: Represents the Base Button in the User's Interfaces.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Jaime Paramo
 * @edited_by: Josephine Esposito
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "DorothysJob/Interface/Focus/Focusable.h"

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "BaseButton.generated.h"

class USlateWidgetStyleAsset;
struct FButtonStyle;

DECLARE_DELEGATE(FOnClickedAction)

UCLASS()
class DOROTHYSJOB_API UBaseButton : public UButton, public IFocusable
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Displays the UMG element on screen.
   */
  virtual void Show() {};
  /**
   * @brief -> Hides the UMG element.
   */
  virtual void Hide() {};

  /**
   * @brief It modify the button to selected state or not.
   * @param _bSelected -> Selected or not.
   */
  virtual void SetButtonSelected(bool _bSelected);

  /**
   * @brief It blocks the button action and sound.
   * @param _bBlocked -> Blocked or not.
   */
  virtual void SetButtonBlockAction(bool _bBlocked);

  /**
   * @brief -> @TOFILL
   */
  virtual void ForceGainFocus();
  /**
   * @brief -> @TOFILL
   */
  virtual void ForceLoseFocus();

  /**
   * @brief Event to execute when the button is hovered.
   */
  FOnButtonHoverEvent OnHoveredAction;

  /**
   * @brief Event to execute when the button is clicked.
   */
  FOnClickedAction OnClickedAction;

  /**
   * @brief -> Reference to the custom Slate style asset for the button.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | UI", meta = (DisplayName = "Custom Button Style"))
  TObjectPtr<const USlateWidgetStyleAsset> m_pButtonStyleAsset;

  /**
   * @brief It references to the custom Slate style asset for the button when it is selected.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13 | UI", meta = (DisplayName = "Custom Button Selected Style"))
  TObjectPtr<const USlateWidgetStyleAsset> m_pButtonSelectedStyleAsset;

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
   * @brief -> Retrieves the current button style.
   * @return -> Pointer to the associated FButtonStyle.
   */
  virtual const FButtonStyle* GetButtonStyle() const;

  /**
   * @brief -> Applies the default visual style to the button.
   */
  virtual void ApplyDefaultStyle();
  /**
   * @brief -> Applies the visual style for when the button is focused.
   */
  virtual void ApplyVariantStyle(FSlateBrush FButtonStyle::* _pBrushStyle);

private:
  /**
   * @brief 
   */
  bool m_bSelected;

  bool m_bBlocked;
};