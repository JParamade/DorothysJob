/************************************************************************
 * @description: Represents the UMG Loading Screen class.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "LoadingScreen.generated.h"

// Animation
class UWidgetAnimation;
namespace EUMGSequencePlayMode {
  enum Type : int;
}

// Widget
class UImage;
class ULoadingThrobber;
class UTipBox;

UCLASS()
class DOROTHYSJOB_API ULoadingScreen : public UBaseWidget
{
	GENERATED_BODY()

public:
  /**
   * @brief -> Loading throbber widget. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<ULoadingThrobber> m_pLoadingThrobberWidget;
  /**
   * @brief -> TipBox widget reference. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTipBox> m_pTipBoxWidget;

  /**
   * @brief -> Backgrounrd Image reference. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pBackground;

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
   * @brief 
   * @param _eMode 
   */
  UFUNCTION(BlueprintCallable)
  void FadeBackground(EUMGSequencePlayMode::Type _eMode);
};
