/************************************************************************
 * @description: Represents the unitary Heart Widget from the HUD's Health Bar.
 * @author: Jaime Paramo
 * @date: 24/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDHeart.generated.h"

UENUM(BlueprintType)
enum class EHeartState : uint8 {
  EMPTY UMETA(DisplayName = "Empty"),
  HALF  UMETA(DisplayName = "Half"),
  FULL  UMETA(DisplayName = "Full")
};

// Component
class UImage;

UCLASS()
class DOROTHYSJOB_API UHUDHeart : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> Reference to the unitary heart Image. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pHeartImage;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Empty Heart Texture"))
  TObjectPtr<UTexture2D> m_pEmptyHeartTexture;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = " Half Heart Texture"))
  TObjectPtr<UTexture2D> m_pHalfHeartTexture;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Full Heart Texture"))
  TObjectPtr<UTexture2D> m_pFullHeartTexture;

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
   * @brief -> @TOFILL
   */
  void SetHeartState(EHeartState _eNewState);
};