// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "DaysScreen.generated.h"

class UImage;
class UTextBlock;
class UDaysDataAsset;
class UCanvasPanel;

// Animation
class UWidgetAnimation;

UCLASS()
class DOROTHYSJOB_API UDaysScreen : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> F@TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeInAnimation;
  /**
   * @brief -> F@TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeOutAnimation;

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
   * @brief -> Backgrounrd Image reference. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  /**
   * @brief -> Backgrounrd Image reference. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pDayBackground;

  /**
  * @brief -> Backgrounrd Image data assets.
  */
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
  TObjectPtr<UDaysDataAsset> m_pDaysDataAsset;
  
private:
  /**
   * @brief -> @TOFILL.
   */
  FTimerHandle oHideDayHandler;

  /**
   * @brief -> @TOFILL.
   */
  void PlayFadeOutAnimation();
};
