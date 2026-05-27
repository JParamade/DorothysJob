/************************************************************************
 * @description: Represents the UI widget that displays the button icon of the interactable.
 * @author: Jaime Paramo
 * @date: 15/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "InteractPromt.generated.h"

// Widget
class UInputIcon;

UCLASS()
class DOROTHYSJOB_API UInteractPromt : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pButtonPrompt;
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
