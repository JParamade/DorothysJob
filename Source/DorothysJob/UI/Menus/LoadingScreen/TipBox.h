/************************************************************************
 * @description: Represents the UMG TipBox class used in the loading screen.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "TipBox.generated.h"

// Animation
class UWidgetAnimation;

// Component
class UTextBlock;

UCLASS()
class DOROTHYSJOB_API UTipBox : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Text Block representing the predefined tip text on the loading screen. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pPredefinedText;
  /**
   * @brief -> Text Block representing the current tip shown on the loading screen. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pTipText;

  /**
   * @brief -> Fade animation reference. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeAnimation;

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

private:
  /**
   * @brief -> Plays the fade-out animation for the current tip, then schedules the next tip to appear.
   */
  void FadeCurrentText();
  /**
   * @brief -> Updates the tip text and plays the fade-in animation.
   */
  void FadeNextText();

  /**
   * @brief -> @TOFILL.
   */
  void WaitForNextText();
  /**
   * @brief -> Duration in seconds each tip remains visible before changing.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Tips", meta = (DisplayName = "Tip Duration"))
  float m_fTipDuration = 3.f;
  /**
   * @brief -> Timer handle for scheduling periodic tip changes.
   */
  UPROPERTY()
  FTimerHandle m_oTipTimeHandle;

  /**
   * @brief -> Retrieves and sets the next tip text from the TipManager.
   */
  void UpdateTipText();
};
