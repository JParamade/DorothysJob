/************************************************************************
 * @description: Represents the select consumables menu's widget.
 * @author: Josephine Esposito
 * @date: 18/08/2025
 * @edited_by: David Gonzalez
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "SelectConsumablesMenu.generated.h"

#pragma region | Forward Declaration
class UImage;
class UInputIcon;
class UTextBlock;
class UCanvasPanel;
class URichTextBlock;
class UBaseGameInstance;
class UConsumableDataAsset;
class USelector;
#pragma endregion

/**
 * Represents the select consumables menu's widget
 */
UCLASS()
class DOROTHYSJOB_API USelectConsumablesMenu : public UFocusableWidget
{
	GENERATED_BODY()

public:
	/**
	 * @brief 
	 * @param MyGeometry 
	 * @param InDeltaTime 
	 */
	virtual void NativeTick(const FGeometry& _rMyGeometry, float _fInDeltaTime) override;

	/**
	 * @brief The default constructor of the widget
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

	/**
	 * @brief this function init the consumables menu.
	 */
	void InitMenu();

	/**
	 * @brief This will handle the received input and will select the corresponding consumable
	 */
	void HandleReceivedInput();

	/**
	 * @brief Method called when receiving input for direction
	 * @param _iDirection The direction
	 */
	UFUNCTION()
	void OnScroll(int32 _iDirection);

	/**
	 * @brief The cancel action
	 */
	void CancelAction();

private:

	/**
	 * @brief This will select the chosen consumable
	 * @param _pConsumable
	 */
	void SelectConsumable(const UConsumableDataAsset* _pConsumable);

	/**
	 * @brief This will restore the seleted consumables
	 */
	void RestoreSelected();

	/**
	 * @brief This will update the selected consumable images below
	 */
	void UpdateSelectedImages();

	/**
	 * @brief This will update the images in the carousel
	 */
	void UpdateCarouselImages();

  /**
   * @brief Delegate to change the current consumable.
   * @param _iIndex -> The selected consumable index.
   */
  UFUNCTION()
  void OnConsumableChanged(int32 _iIndex);

  /**
   * @brief It will be called when the menushould be shown.
   */
  UFUNCTION()
  void OnShowMenu();

#pragma region | Background and mother panel

	/**
	 * @brief This represents the background image of the menu
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pSCBackground;

	/**
	 * @brief This represents the main Canvas panel of the menu
	 */
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* m_pSCPanel;

#pragma endregion

#pragma region | Images

	/**
	 * @brief The Central image, represents the selected one, always on focus
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pCentralImage;
	/**
	 * @brief The Central image, represents the selected one, always on focus
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pCentralBanner;

	/**
	 * @brief The Left image, representes the second option, never on focus
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pLeftImage;
	/**
	 * @brief The Left image, representes the second option, never on focus
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pLeftBanner;

	/**
	 * @brief The Right image, represents the second option, never on focus
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pRightImage;
	/**
	 * @brief The Right image, represents the second option, never on focus
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pRightBanner;

	/**
	 * @brief The first selected consumable
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pFirstConsumableImage;

	/**
	 * @brief The first selected consumable selected image
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pFirstConsumableSelectedImage;

	/**
	 * @brief The second selected consumable
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pSecondConsumableImage;

	/**
	 * @brief The first selected consumable selected image
	 */
	UPROPERTY(meta = (BindWidget))
	UImage* m_pSecondConsumableSelectedImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UInputIcon> m_pExitIconButton;

#pragma endregion

  /**
   * Selector to select the consumable in the carousel.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USelector> m_pConsumablesCarousel;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<URichTextBlock> m_pInfoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> m_pShortInfoText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> m_pExitText;

	/**
	 * @brief This will hold the current index of the carousel
	 */
	int32 m_iCurrentIndex;

	/**
	 * @brief This will hold the total number of elements added
	 */
	int32 m_iTotalElements;

	/**
	 * @brief This represents the first selected consumables
	 */
	TObjectPtr<const UConsumableDataAsset> m_pFirstConsumable;

	/**
	 * @brief This represents the first selected consumables
	 */
	TObjectPtr<const UConsumableDataAsset> m_pSecondConsumable;

	/**
	 * @brief Reference to the game instance
	 */
	UBaseGameInstance* m_pGameInstance;

  /**
   * It delays the show menu action.
   */
  FTimerHandle m_tShowMenuTimerHandle;

	/**
	 * @brief Current slot
	 */
	int32 m_iSelectedIndex;

	/**
	 * @brief 
	 */
	bool m_bCanGorchSpeak = true;
	/**
	 * @brief 
	 */
	float m_fElapsedTime = 0.f;
	/**
	 * @brief 
	 */
	float m_fTimeBetweenYaps = 2.5f;
};