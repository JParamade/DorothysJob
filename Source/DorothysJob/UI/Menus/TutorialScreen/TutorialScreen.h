/************************************************************************
 * @description: Represents the turotial menu's widget.
 * @author: David Gonzalez
 * @date: 22/04/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "TutorialScreen.generated.h"

// Component
class UBaseButtonText;
class UImage;
class ULoadLevelButton;

UCLASS()
class DOROTHYSJOB_API UTutorialScreen : public UFocusableWidget
{
  GENERATED_BODY()
public:
  /**
   * @brief -> Button used to continue to the next tutorial screen.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButtonText> m_pContinueButton;
  /**
   * @brief -> Button used to load the main game level.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<ULoadLevelButton> m_pPlayButton;

  /**
   * @brief -> Image representing the first tutorial screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pBackgroundOne;
  /**
   * @brief -> Image representing the second tutorial screen.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pBackgroundTwo;

  /**
   * @brief -> Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;
  /**
   * @brief -> Hides the UMG elements of this Widget.
   */
  virtual void Hide() override;

  /**
   * @brief -> Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief -> Configures the focus system for the current menu when it's opened.
   */
  virtual void SetupFocus() override;

private:
  /**
   * @brief -> Called when the Continue Button is clicked. This method loads the next tutorial screen.
   */
  UFUNCTION(BlueprintCallable)
  void OnClickContinue();

  /**
   * @brief -> Displays one of the two tutorial screens based on the parameter.
   * @param _bFirstScreen -> If true, shows the first screen; if false, shows the second screen.
   */
  void ShowScreen(bool _bFirstScreen);
};
