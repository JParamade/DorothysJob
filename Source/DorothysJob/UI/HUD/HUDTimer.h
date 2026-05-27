/************************************************************************
 * @description: Represents the in-game level timer's HUD widget.
 * @author: Jaime Paramo
 * @date: 12/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDTimer.generated.h"

// Component
class UTextBlock;
class UImage;

UCLASS()
class DOROTHYSJOB_API UHUDTimer : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Reference to the Text Block displaying the minutes of the game timer. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pMinutesText;
  /**
   * @brief -> Reference to the Text Block displaying the seconds of the game timer. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pSecondsText;
  /**
   * @brief -> Reference to the Text Block displaying the separator ':' between minutes and seconds. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pDotsSeparator;
  /**
   * @brief -> Reference to the image used as the timer's background in the HUD. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pTimerImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pTimerGlow;

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
   * @brief -> Time representation in minutes and seconds. Used to convert raw seconds from the Time Manager into a format suitable for UI display (minutes and seconds).
   */
  int32 m_iMinutes, m_iSeconds;

  /**
   * @brief -> Updates the minutes and seconds Text Blocks with new time values. Adds a leading zero if the values are single digits.
   * @param _iNewMinutes -> New value for timer minutes.
   * @param _iNewSeconds -> New value for timer seconds.
   */
  void UpdateTimerText(int32 _iNewSeconds);

  /**
   * @brief -> Delegate handle used to manage the binding for the timer change event.
   */
  FDelegateHandle m_oTimerChangeHandle;

  /**
   * @brief -> @TOFILL
   */
  void PlayGlowAnimation();

  /**
   * @brief -> @TOFILL
   */
  void StopGlowAnimation();

  /**
   * @brief -> @TOFILL
   */
  bool m_bGlowing = false;
};