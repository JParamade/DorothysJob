/************************************************************************
 * @description: Represents the HUD's event.
 * @author: Jaime Paramo
 * @date: 27/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDEvent.generated.h"

// Animation
class UWidgetAnimation;

// Component
class UImage;
class UTextBlock;

UCLASS()
class DOROTHYSJOB_API UHUDEvent : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pEventAnimation;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pEventImage;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pEventText;

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
   * @brief -> Called when the widget is constructed.
   */
  virtual void NativeDestruct() override;

private:
  /**
   * @brief 
   */
  UFUNCTION()
  void StartEvent(FText _sText);
  /**
   * @brief 
   */
  UFUNCTION()
  void StopEvent();

  /**
   * @brief 
   */
  FDelegateHandle m_oStartEventHandle;
  /**
   * @brief 
   */
  FDelegateHandle m_oEndEventHandle;
};