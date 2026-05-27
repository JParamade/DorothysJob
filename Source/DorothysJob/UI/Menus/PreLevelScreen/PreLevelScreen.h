/************************************************************************
 * @description: Represents the UMG Pre-Level Screen class.
 * @author: Jaime Paramo
 * @date: 23/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "PreLevelScreen.generated.h"

// Audio
class UFMODEvent;

// Component
class UBaseButton;
class UImage;
class UTextBlock;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPreLevelFinish, bool /* flag */);

UCLASS()
class DOROTHYSJOB_API UPreLevelScreen : public UFocusableWidget
{
	GENERATED_BODY()

public:
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pBackgroundImage;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pBackgroundBarsImage;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pDorothyImage;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pWeaponsBanner;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pWeaponsText;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryWeaponImage;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryWeaponBG;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryWeaponImage;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryWeaponBG;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pConsumablesBanner;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pConsumablesText;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryConsumableImage;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pPrimaryConsumableBG;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryConsumableImage;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pSecondaryConsumableBG;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pTimerText;
  
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pLevelDescriptionImage;
  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pLevelDescriptionText;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pCancelButton;

  /**
   * @brief -> @TOFILL
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pConfirmButton;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pCurrentDayText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Day Text"))
  FText m_sDayText;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Images", meta = (DisplayName = "Day Image"))
  TObjectPtr<UImage> m_pCurrentDayImage;

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
   * @brief -> Configures the focus system for the current menu when it's opened.
   */
  virtual void SetupFocus() override;

  /**
   * @brief The cancel action
   */
  void CancelAction();

  /**
   * @brief The delegate that will signal when the panel has been closed
   */
  FOnPreLevelFinish OnPreLevelFinish;

private:

  /**
   * @brief
   */
  void UpdateArsenalInfo();
  /**
   * @brief
   */
  void UpdateLevelInfo();

  /**
   * @brief 
   */
  template<typename T>
  void SetBrushFromDataAsset(const TObjectPtr<T> _pDataAsset, TObjectPtr<UImage> _pImageWidget);

  /**
   * @brief
   */
  UFUNCTION()
  void OnCancel();
  /**
   * @brief 
   */
  UFUNCTION()
  void OnConfirm();

  /**/
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Steps Event", AllowPrivateAccess = true))
  TObjectPtr<UFMODEvent> m_pStepsEvent;

  void UpdateCurrentDay();

  /**
   * @brief It will be called when the menushould be shown.
   */
  UFUNCTION()
  void OnShowMenu();

  /**
   * It delays the show menu action.
   */
  FTimerHandle m_tShowMenuTimerHandle;
};