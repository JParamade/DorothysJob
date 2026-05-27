/************************************************************************
 * @description: Represents the Room Info's HUD widget.
 * @author: Jaime Paramo
 * @date: 06/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDRoomInfo.generated.h"

// Component
class UImage;

// Widget
class UHUDMinimap;
class UHUDTimer;

UCLASS()
class DOROTHYSJOB_API UHUDRoomInfo : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> Reference to the minimap widget displayed in the HUD. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDMinimap> m_pHUDMinimapWidget;
  /**
   * @brief -> Reference to the minimap widget displayed in the HUD. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDTimer> m_pHUDTimerWidget;

  /**
   * @brief -> Reference to the minimap widget displayed in the HUD. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pCalendarImage;

  /**
   * @brief -> Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;

  /**
   * @brief 
   */
  void ShowMinimap();
  /**
   * @brief 
   */
  void ShowTimer();

  /**
   * @brief
   */
  void ShowCalendar();

  /**
   * @brief -> Hides the UMG elements of this Widget.
   */
  virtual void Hide() override;

  /**
   * @brief -> Called when the widget is constructed.
   */
  virtual void NativeConstruct() override;

  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Calendar Textures"))
  TArray<TObjectPtr<UTexture2D>> m_lCalendarImages;
};