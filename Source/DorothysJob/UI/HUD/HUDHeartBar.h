/************************************************************************
 * @description: Represents the HUD's Heart Bar Widget containing unitary hearts.
 * @author: Jaime Paramo
 * @date: 24/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDHeartBar.generated.h"

// Component
class UHorizontalBox;

// Widget
class UHUDHeart;

UCLASS()
class DOROTHYSJOB_API UHUDHeartBar : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHorizontalBox> m_pHeartContainer;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Heart Widget Class"))
  TSubclassOf<UHUDHeart> m_oHeartWidgetClass;

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
   * @param MaxHearts
   */
  UFUNCTION(BlueprintCallable)
  void InitializeHearts(int32 _iMaxHearts);

  /**
   * @brief
   * @param CurrentHealth
   */
  UFUNCTION(BlueprintCallable)
  void UpdateHearts(int32 _iCurrentHealth, int32 _iMaxHealth);

private:
  /**
   * @brief -> @TOFILL
   */
  bool m_bIsInitialized = false;

  /**
   * @brief -> @TOFILL
   */
  TArray<TObjectPtr<UHUDHeart>> m_lHearts;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Heart Padding", AllowPrivateAccess = "true"))
  FMargin m_fHeartPadding;
};
