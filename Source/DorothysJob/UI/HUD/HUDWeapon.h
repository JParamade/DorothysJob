/************************************************************************
 * @description: Represents the player's weapon widget.
 * @author: Jaime Paramo
 * @date: 11/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUDWeapon.generated.h"

UENUM()
enum class EBarTextureType : uint8 {
  FILL       UMETA(DisplayName = "Fill"),
  BACKGROUND UMETA(DisplayName = "Background")
};

// Component
class UImage;
class UProgressBar;
class UTextBlock;

// Widget
class UHUDHeartBar;
enum class EDirtType : uint8;
enum class EWeaponAttackType : uint8;

UCLASS()
class DOROTHYSJOB_API UHUDWeapon : public UBaseWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> @TOFILL.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UProgressBar> m_pPrimaryWeaponBar;
  /**
   * @brief -> Image widget for the currently equipped weapon icon.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryWeaponIcon;
  /**
   * @brief -> Bubble background image for the currently equipped weapon icon.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryWeaponBubble;
  /**
   * @brief -> @TOFILL.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UProgressBar> m_pSecondaryWeaponBar;
  /**
   * @brief -> Image widget for the weapon in the pocket slot.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryWeaponIcon;
  /**
   * @brief -> Bubble background image for the pocket weapon icon.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryWeaponBubble;

  /**
   * @brief -> Bubble background image for the pocket weapon icon.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UHUDHeartBar> m_pHeartBar;

  /**
   * @brief -> Icon image for the primary consumable.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryConsumableIcon;
  /**
   * @brief -> Bubble background image for the primary consumable icon.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryConsumableBubble;
  /**
   * @brief -> Text Block widget that displays the current stack count for the primary consumable.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pPrimaryConsumableStackText;
  /**
   * @brief -> Icon image for the gamepad button for the primary consumable.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryButtonIcon;
  /**
   * @brief -> Icon image for the secondary consumable.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryConsumableIcon;
  /**
   * @brief -> Bubble background image for the secondary consumable icon.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryConsumableBubble;
  /**
   * @brief -> Text block widget that displays the current stack count for the secondary consumable.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pSecondaryConsumableStackText;
  /**
   * @brief -> Icon image for the gamepad button for the secondary consumable.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryButtonIcon;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Weapon Fill Icons"))
  TArray<TObjectPtr<UTexture2D>> m_lWeaponFillIcons;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Weapon Background Icons"))
  TArray<TObjectPtr<UTexture2D>> m_lWeaponBackgroundIcons;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Weapon Bar Range"))
  TArray<FVector2D> m_lWeaponBarRange;

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
   * @brief -> @TOFILL. THIS IS TEMPORAL!
   */
  virtual void NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) override;

  /**
   * @brief -> Updates weapon icon images displayed in the HUD.
   * @param _eAttackType -> Weapon dirt clean type.
   * @param _eWeaponAttackType -> Weapon attack type (distance, melee).
   */
  void UpdateWeaponIcons(EDirtType _eAttackType, EWeaponAttackType _eWeaponAttackType);

  /**
   * @brief -> Updates the text block showing the stack count of the primary consumable.
   */
  void UpdateConsumableStackText(int32 _iNewQuantity, int32 _iConsumableID);

private:
  // @SUPER TEMPORAL
  float m_fPrimaryPercent;
  bool m_bSecondaryWeapon = false;

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
   * @brief 
   * @tparam T 
   * @param _rDataAssets 
   * @param _iIndex 
   * @param _eWidgetType 
   * @return 
   */
  template <typename T>
  TObjectPtr<UTexture2D> GetTextureFromDataAsset(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex, EBarTextureType _eWidgetType);

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
   * @brief 
   * @tparam T 
   * @param _rDataAssets 
   * @param _iIndex 
   * @param _pBar 
   * @param _pBackgroundImage 
   */
  template <typename T>
  void SetBarInfo(const TArray<TObjectPtr<const T>>& _rDataAssets, int32 _iIndex, TObjectPtr<UProgressBar> _pBar, TObjectPtr<UImage> _pBackgroundImage);

  /**
   * @brief 
   * @param _fValue 
   * @param _fMin 
   * @param _fMax 
   * @return 
   */
  float MapPercentToRange(float _fMin, float _fMax, float _fValue);

  /**
   * @brief -> Delegate handle used to manage weapon change event bindings.
   */
  FDelegateHandle m_oWeaponChangeHandle;
  /**
   * @brief -> Delegate handle used to manage primary consumable usage event bindings.
   */
  FDelegateHandle m_oConsumableUsedHandle;
};