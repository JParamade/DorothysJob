/************************************************************************
 * @description: Represents the technology logos that appear when the game is executed.
 * @author: Jaime Paramo
 * @date: 24/09/25
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "TechLogos.generated.h"

// Animation
class UWidgetAnimation;

// Component
class UImage;
class UTextBlock;

DECLARE_DELEGATE(FOnAnimationFinished)

UCLASS()
class DOROTHYSJOB_API UTechLogos : public UBaseWidget
{
	GENERATED_BODY()

public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pLogoAnimation;
  FOnAnimationFinished OnAnimationFinished;

  /**
   * @brief -> @TOFILL.  Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pBackgroundImage;

  /**
   * @brief -> @TOFILL.  Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pUnrealEngineLogo;

  /**
   * @brief -> @TOFILL.  Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pPoweredByText;
  /**
   * @brief -> @TOFILL.  Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pFmodLogo;

  /**
   * @brief -> @TOFILL.  Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pUtadLogo;

  /**
   * @brief -> @TOFILL.  Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pBola13Logo;

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
