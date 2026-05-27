/************************************************************************
 * @description: Represents the HUD widget that displays FPS.
 * @author: Jaime Paramo
 * @date: 21/08/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDFPS.generated.h"

class UTextBlock;

UCLASS()
class DOROTHYSJOB_API UHUDFPS : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> TextBlock reference used to display the FPS value.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pFPSText;

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
   * @brief -> Updates the FPS text displayed on the HUD.
   */
  void UpdateFPSText();

  /**
   * @brief -> Delegate handle used to manage the moment the text updates.
   */
  UPROPERTY()
  FTimerHandle m_oFPSHandle;
};
