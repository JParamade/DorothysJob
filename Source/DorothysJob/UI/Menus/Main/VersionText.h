/************************************************************************
 * @description: Represents the project's version widget.
 * @author: Jaime Paramo
 * @date: 19/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "VersionText.generated.h"

// Component
class UTextBlock;

UCLASS()
class DOROTHYSJOB_API UVersionText : public UBaseWidget
{
	GENERATED_BODY()

public:
  /**
   * @brief -> Reference to the Text Block that displays the project's version. Must match this name in UMG to compile.
   */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> m_pVersionText;

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
   * @brief -> Updates the version text displayed in the UI. Typically called during HUD initialization or when the version changes.
   */
	void UpdateVersionText();
};
