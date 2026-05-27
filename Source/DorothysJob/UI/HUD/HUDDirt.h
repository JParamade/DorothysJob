/************************************************************************
 * @description: Represents the dirt meter widget.
 * @author: Jaime Paramo
 * @date: 13/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDDirt.generated.h"

// Component
class UImage;
class UProgressBar;

UCLASS()
class DOROTHYSJOB_API UHUDDirt : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Progress bar representing the current level of dirtiness in the HUD. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UProgressBar> m_pDirtBar;
  /**
   * @brief -> Background image for the level of dirtiness's Progress Bar in the HUD. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pDirtBarImage;

  /**
   * @brief -> Decorative separator bar element 1 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar1;
  /**
   * @brief -> Decorative separator bar element 2 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar2;
  /**
   * @brief -> Decorative separator bar element 3 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar3;
  /**
   * @brief -> Decorative separator bar element 4 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar4;
  /**
   * @brief -> Decorative separator bar element 5 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar5;
  /**
   * @brief -> Decorative separator bar element 6 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar6;
  /**
   * @brief -> Decorative separator bar element 7 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar7;
  /**
   * @brief -> Decorative separator bar element 8 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar8;
  /**
   * @brief -> Decorative separator bar element 9 in the HUD layout. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSeparatorBar9;

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
   * @brief -> Updates the dirt Progress Bar with new dirt percentage.
   * @param _iNewDirtValue -> New level's dirt percentage.
   */
  void UpdateDirtBar(float _iNewDirtValue);

  /**
   * @brief -> Delegate handle used to manage the binding for the dirt percentage change event.
   */
  FDelegateHandle m_oPercentageChangeHandle;
};