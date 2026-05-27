/************************************************************************
 * @description: Represents the player's health widget.
 * @author: Jaime Paramo
 * @date: 10/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDHealth.generated.h"

// Component
class UProgressBar;
class URichTextBlock;
class UTextBlock;

UCLASS()
class DOROTHYSJOB_API UHUDHealth : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Reference to the health Progress Bar. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UProgressBar> m_pHealthBar;
  /**
   * @brief -> Reference to the Text Block displaying the text "Dorothy" inside the health Progress Bar. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pDorothyText;

  /**
   * @brief -> Reference to the Rich Text Block that displays the "Dorothy is dirty!" message. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<URichTextBlock> m_pDirtinessText;

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
   * @brief -> Updates the player's health Progress Bar with new health values.
   * @param _iNewCurrentHealth -> New value for player's current health.
   * @param _iMaxHealth -> New value for player's max health.
   */
  void UpdateHealthBar(int32 _iNewCurrentHealth, int32 _iMaxHealth);
};