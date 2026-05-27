/************************************************************************
 * @description: Represents the Consumable Info's HUD widget.
 * @author: Jaime Paramo
 * @date: 08/09/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDConsumableInfo.generated.h"

// Animation
class UWidgetAnimation;

// Component
class UImage;
class UTextBlock;

// Widget
class UInputIcon;

UCLASS()
class DOROTHYSJOB_API UHUDConsumableInfo : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryConsumableBubble;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryConsumableIcon;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pPrimaryConsumableStackText;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pPrimaryButtonIcon;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryConsumableBubble;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryConsumableIcon;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pSecondaryConsumableStackText;
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pSecondaryButtonIcon;

  /**
   * @brief -> F@TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pCantUsePrimary;
  /**
   * @brief -> F@TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pCantUseSecondary;

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
   * @param _iNewQuantity 
   * @param _iConsumableID 
   */
  void UpdateConsumableStackText(int32 _iNewQuantity, int32 _iConsumableID);

private:
  /**
   * @brief 
   * @param _iNewQuantity 
   * @param _iConsumableID 
   */
  void HandleConsumableUnavailable(int32 _iNewQuantity, int32 _iConsumableID);

  /**
   * @brief -> Retrieves the icon texture from a given DataAsset array at the specified index.
   * @tparam T -> Type of the DataAsset (must have a member "m_pIcon" of type TObjectPtr<UTexture2D>).
   * @param _rDataAssets -> Array of DataAssets containing the icons.
   * @param _iIndex -> Index of the desired element in the array.
   * @return -> Pointer to the texture icon, or nullptr if not found or invalid.
   */
  template <typename T>
  TObjectPtr<UTexture2D> GetTextureFromDataAsset(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex);

  /**
   * @brief -> Sets the brush images of two UMG Image widgets using icons from a DataAsset array.
   * @tparam T -> Type of the DataAsset (must have a member "m_pIcon" of type TObjectPtr<UTexture2D>).
   * @param _rDataAssets -> Array of DataAssets to retrieve icons from.
   * @param _iIndex -> Index of the desired element in the array.
   * @param _pImage -> Pointer to the image widget to update.
   */
  template <typename T>
  void SetImageInfo(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex, TObjectPtr<UImage> _pImage);

  /**
   * @brief -> Delegate handle used to manage primary consumable usage event bindings.
   */
  FDelegateHandle m_oConsumableUsedHandle;
  /**
   * @brief 
   */
  FDelegateHandle m_oConsumableUnavailableHandle;
};
