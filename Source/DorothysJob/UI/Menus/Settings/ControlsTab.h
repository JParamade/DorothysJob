/************************************************************************
 * @description: It represents the controls tab in the main settings menu.
 * @author: David Gonzalez
 * @date: 13/10/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "ControlsTab.generated.h"

class UCanvasPanel;
class UBaseButton;
class UInputIcon;

UCLASS()
class DOROTHYSJOB_API UControlsTab : public UFocusableWidget
{
	GENERATED_BODY()
	
public:

  /**
   * @brief The default constructor of the widget.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief The default destructor of the widget.
   */
  virtual void NativeDestruct() override;

  /**
   * @brief It displays the main settings screen.
   */
  virtual void Show() override;

  /**
   * @brief It hides the main settings screen.
   */
  virtual void Hide() override;

  /**
   * @brief It will setup the focus of the widget
   */
  virtual void SetupFocus() override;

private:

  /**
   * @brief It is called when the player clicks the close button.
   */
  UFUNCTION()
  void OnCloseClicked();

  /**
   * This represents the parent panel of the widget
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  /**
   * The button icon to change the tab.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pChangeTabIconButton;

  /**
   * The button that closes the controls tab and returns to the previous screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pCloseButton;
};