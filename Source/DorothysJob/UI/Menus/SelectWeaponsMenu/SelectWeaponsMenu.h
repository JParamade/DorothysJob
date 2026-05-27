/************************************************************************
 * @description: Represents the select weapons menu's widget.
 * @author: David Gonzalez
 * @date: 16/06/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "SelectWeaponsMenu.generated.h"

class UImage;
class UCanvasPanel;
class UBaseButton;
class UBaseButtonText;
class UWeaponDataAsset;
class UTextBlock;
class UInputIcon;

UCLASS()
class DOROTHYSJOB_API USelectWeaponsMenu : public UFocusableWidget
{
	GENERATED_BODY()
	
public:

#pragma region | Virtual methods
  /**
   * @brief The default Constructor of the widget
   */
  virtual void NativeConstruct() override;

  /**
   * @brief This will activate the widget
   */
  virtual void Show() override;

  /**
   * @brief This will deactivate the widget
   */
  virtual void Hide() override;

  /**
   * @brief This will setup the focus of the widget
   */
  virtual void SetupFocus() override;
#pragma endregion

  /**
   * @brief this function init the weapon menu.
   */
  void InitMenu();

  /**
   * @brief It stores the selected weapons and hides the weapons menu.
   */
  void CancelAction();

private:

#pragma region | Private Functions
  /**
   * @brief It updates the selected weapon when the button is clicked.
   * @param _pWeapon -> Selected weapon.
   */
  void SelectWeapon(const UWeaponDataAsset* _pWeapon);

  /**
   * @brief It updates the hovered weapon.
   * @param _pWeapon -> Hovered weapon details text list.
   */
  void HoverWeapon(const TArray<FText>& _pWeapon);

  /**
   * @brief It restores the selected weapons to the current selected weapons stored in teh game instance.
   */
  void RestoreSelected();

  /**
   * @brief It updates the selected weapons images.
   */
  void UpdateSelectedImages();

  /**
   * @brief It updates the can exit action state.
   */
  void UpdateCanExitState();

  /**
   * @brief It initialises the focus manager.
   */
  void InitFocusManager();

  /**
   * @brief It will be called when the menushould be shown.
   */
  UFUNCTION()
  void OnShowMenu();

  /**
   * @brief It will be called when the mopressor button is clicked.
   */
  UFUNCTION()
  void OnMopressorClicked();

  /**
   * @brief It will be called when the mopressor button is hovered.
   */
  UFUNCTION()
  void OnMopressorHovered();

  /**
   * @brief It will be called when the washee & flashee button is clicked.
   */
  UFUNCTION()
  void OnWasheeFlasheeClicked();

  /**
   * @brief It will be called when the washee & flashee button is hovered.
   */
  UFUNCTION()
  void OnWasheeFlasheeHovered();

  /**
   * @brief It will be called when the dusty button is clicked.
   */
  UFUNCTION()
  void OnDustyClicked();

  /**
   * @brief It will be called when the dusty button is hovered.
   */
  UFUNCTION()
  void OnDustyHovered();

  /**
   * @brief It will be called when the dirtminator button is clicked.
   */
  UFUNCTION()
  void OnDirtminatorClicked();

  /**
   * @brief It will be called when the dirtminator button is hovered.
   */
  UFUNCTION()
  void OnDirtminatorHovered();
#pragma endregion

#pragma region | Properties
  /**
   * Washee&Flashee title.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Title Images", meta = (DisplayName = "Washee&Flashee", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pWasheeFlasheeName;

  /**
   * Mopressor title.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Title Images", meta = (DisplayName = "Mopressor", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pMopressorName;

  /**
   * Dirtminator title.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Title Images", meta = (DisplayName = "Dirtminator", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pDirtminatorName;

  /**
   * Dusty title.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Title Images", meta = (DisplayName = "Dusty", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pDustyName;

  /**
   * Washee&Flashee details text list.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Washee&Flashee Details", AllowPrivateAccess = "true"))
  TArray<FText> m_lWasheeFlasheeDetailsText;

  /**
   * Mopressor details text list.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Mopressor Details", AllowPrivateAccess = "true"))
  TArray<FText> m_lMopressorDetailsText;

  /**
   * Dirtminator details text list.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Dirtminator Details", AllowPrivateAccess = "true"))
  TArray<FText> m_lDirtminatorDetailsText;

  /**
   * Dusty details text list.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Dusty Details", AllowPrivateAccess = "true"))
  TArray<FText> m_lDustyDetailsText;

  /**
   * @brief It is the fade in animation.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeInAnimation;
#pragma endregion

#pragma region | Panel - Images
  /**
   * This represents the parent panel of the widget
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  /**
   * It is the details background image.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pDetailsBackgroundImage;

  /**
   * It is the focused weapon name for the details background.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UImage> m_pWeaponNameImage;
#pragma endregion

#pragma region | Texts
  /**
   * This represents the parent panel of the widget
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pWeaponDetailsAttack;

  /**
   * It is the details background image.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pWeaponDetailsClean;

  /**
   * It is the focused weapon name for the details background.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pWeaponDetailsSpecial;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pExitText;

#pragma endregion

#pragma region | Buttons
  /**
   * This button will select the Whashee&Flashee weapon.
   */
  UPROPERTY(meta = (BindWidget, DisplayName = "Washee&Flashee"))
  TObjectPtr<UBaseButton> m_pWasheeFlasheeButton;
  /**
   * This button will select the Mopressor weapon.
   */
  UPROPERTY(meta = (BindWidget, DisplayName = "Mopressor"))
  TObjectPtr<UBaseButton> m_pMopressorButton;
  /**
   * This button will select the Dirtminator weapon.
   */
  UPROPERTY(meta = (BindWidget, DisplayName = "Dirtminator"))
  TObjectPtr<UBaseButton> m_pDirtminatorButton;
  /**
   * This button will select the Dusty weapon.
   */
  UPROPERTY(meta = (BindWidget, DisplayName = "Dusty"))
  TObjectPtr<UBaseButton> m_pDustyButton;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UInputIcon> m_pExitIconButton;

#pragma endregion

#pragma region | Selected weapons
  /**
   * This represents the selected liquid weapon.
   */
  TObjectPtr<const UWeaponDataAsset> m_pLiquidWeapon;

  /**
   * This represents the selected dust weapon.
   */
  TObjectPtr<const UWeaponDataAsset> m_pDustWeapon;
#pragma endregion

  /**
   * This represents the mopressor weapon.
   */
  TObjectPtr<const UWeaponDataAsset> m_pMopressorWeapon;

  /**
   * This represents the washee&flashee weapon.
   */
  TObjectPtr<const UWeaponDataAsset> m_pWasheeFlasheeWeapon;

  /**
   * This represents the dusty weapon.
   */
  TObjectPtr<const UWeaponDataAsset> m_pDustyWeapon;

  /**
   * This represents the dirminator weapon.
   */
  TObjectPtr<const UWeaponDataAsset> m_pDirtminatorWeapon;

  /**
   * It delays the show menu action.
   */
  FTimerHandle m_tShowMenuTimerHandle;

  int32 m_iFirtsFocusLineSize;
  bool m_bCanSaveExit;
};