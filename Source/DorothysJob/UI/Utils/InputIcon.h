/************************************************************************
 * @description: Represents the input icon shown depending on the last input device used.
 * @author: Jaime Paramo
 * @date: 25/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "InputIcon.generated.h"

// Component
class UImage;

// Input
enum class EHardwareDevicePrimaryType : uint8;

UCLASS()
class DOROTHYSJOB_API UInputIcon : public UBaseWidget
{
	GENERATED_BODY()
	
public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pInputIcon;

  /**
   * @brief 
   */
  UPROPERTY(EditInstanceOnly, Category = "Bola 13|UI", meta = (DisplayName = "Gamepad Icon"))
  TObjectPtr<UTexture2D> m_pGamepadIcon;
  /**
   * @brief
   */
  UPROPERTY(EditInstanceOnly, Category = "Bola 13|UI", meta = (DisplayName = "Keyboard Icon"))
  TObjectPtr<UTexture2D> m_pKeyboardIcon;

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
   * @brief 
   * @param _eDeviceType 
   */
  void UpdateInputIcon(EHardwareDevicePrimaryType _eDeviceType);

  /**
   * @brief 
   */
  FDelegateHandle m_oDelegateHandle;
};
