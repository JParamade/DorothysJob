/************************************************************************
 * @description: Represents the UMG Selector Widget.
 * @author: Jaime Paramo
 * @date: 27/09/2025
 * @edited_by:
 ************************************************************************/

#pragma once

// Interface
#include "DorothysJob/Interface/Focus/Focusable.h"
#include "DorothysJob/Interface/Focus/AxisNavigable.h"

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "Selector.generated.h"

// Component
class UBaseButton;
class UTextBlock;

// Enum
enum class EGraphicsQuality : uint8;

// Manager
class USettingsManager;

// Style
struct FButtonStyle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnumSelectionChanged, int32, NewIndex);

UCLASS()
class DOROTHYSJOB_API USelector : public UBaseWidget, public IFocusable, public IAxisNavigable
{
	GENERATED_BODY()
	
public:
  /**
   * It shows the current selected option name or not.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Options", meta = (DisplayName = "Hidden Text"))
  bool m_bHiddenOptions;

  /**
   * It shows if the selector is circular or not.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Options", meta = (DisplayName = "Circular"))
  bool m_bIsCarousel;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pLeftButton;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pRightButton;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pValueText;

  UPROPERTY(BlueprintReadWrite)
  int32 iCurrentIndex = 0;

  UPROPERTY(BlueprintAssignable)
  FOnEnumSelectionChanged OnSelectionChanged;

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
   * @brief 
   * @param InOptions 
   * @param InitialIndex 
   */
  void InitializeOptions(const TArray<FText>& _rInOptions, int32 _iInitialIndex = 0);

  /**
   * @brief 
   * @tparam T 
   * @param _rValues 
   * @param _fToTextFunc 
   * @param _iInitialIndex 
   */
  template<typename T>
  void InitializeDynamicOptions(const TArray<T>& _rValues, TFunction<FText(const T&)> _fToTextFunc, int32 _iInitialIndex = 0) {
    m_lDisplayOptions.Empty();

    for (const T& rValue : _rValues) m_lDisplayOptions.Add(_fToTextFunc(rValue));

    iCurrentIndex = FMath::Clamp(_iInitialIndex, 0, m_lDisplayOptions.Num() - 1);
  }

protected:
  /**
   * @brief 
   */
  UFUNCTION()
  virtual void OnGainFocus() override;

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void OnLoseFocus() override;

  /**
   * @brief 
   */
  virtual void OnPressed() {};
  /**
   * @brief 
   */
  virtual void OnReleased() {};
  /**
   * @brief 
   */
  virtual void ExecuteAction() {};

  /**
   * @brief 
   * @param _vAxisValue 
   */
  virtual void OnAxisInput(FVector2D _vAxisValue) override;

  /**
   * @brief 
   */
  virtual void OnGainInteraction() {};
  /**
   * @brief 
   */
  virtual void OnLoseInteraction() {};

  /**
   * @brief -> Applies the default visual style to the button.
   */
  virtual void ApplyDefaultStyle();
  /**
   * @brief -> Applies the visual style for when the button is focused.
   */
  virtual void ApplyVariantStyle(FSlateBrush FButtonStyle::* _pBrushStyle);

private:
  /**
   * @brief 
   */
  TArray<FText> m_lDisplayOptions;

  /**
   * @brief 
   */
  UFUNCTION()
  void OnLeftClicked();
  /**
   * @brief 
   */
  UFUNCTION()
  void OnRightClicked();

  /**
   * @brief 
   */
  void UpdateUI();
};