#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "BaseCreditsScreen.generated.h"

// Animation
class UWidgetAnimation;

// Component
class UCanvasPanel;

// Widget
class UInputIcon;

DECLARE_MULTICAST_DELEGATE(FOnCreditsClosed)

UCLASS()
class DOROTHYSJOB_API UBaseCreditsScreen : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pRootCanvas;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pCreditsAnimation;
  FTimerHandle m_oTimerHandle;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pSkipButtonIcon;

  FOnCreditsClosed OnCreditsClosed;

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
};