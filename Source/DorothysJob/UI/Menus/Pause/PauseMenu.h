/************************************************************************
 * @description: Represents the pause menu's widget.
 * @author: Jaime Paramo
 * @date: 24/03/2025
 * @edited_by: Josephine
 * @edited_by: David Gonzalez
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "PauseMenu.generated.h"

class UTextBlock;
class UCanvasPanel;
class UBaseButton;
class UReloadLevelButton;
class ULoadLevelButton;
class USpacer;
class UMainSettingsMenu;

UCLASS()
class DOROTHYSJOB_API UPauseMenu : public UFocusableWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief To initialize the panel and set the button text
   */
  virtual void NativeConstruct() override;

  virtual void Show() override;
  virtual void Hide() override;

  virtual void SetupFocus() override;

  /**
   * @brief It returns if the user can exit from the pause menu or not.
   */
  bool CanExit();

  /**
   * @brief It will change the tab in settings screen.
   */
  UFUNCTION(BlueprintCallable)
  void ChangeSettingsTab(bool _bGoRight);

protected:

#pragma region | OnClickMethods
  /**
   * @brief This method will show the load screen
   */
  UFUNCTION(BlueprintCallable)
  void PauseGame();

  /**
   * @brief It will return to the HUB.
   */
  UFUNCTION(BlueprintCallable)
  void OnReturnToHubClicked();

  /**
   * @brief It will show the settings screen.
   */
  UFUNCTION(BlueprintCallable)
  void OpenSettingsScreen();

  /**
  * @brief It will save the ewapons and consumables build
  */
  UFUNCTION(BlueprintCallable)
  void ExitClicked();
#pragma endregion

private:

#pragma region | Buttons
  /**
   * @brief This button will continue in the game.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pContinueButton;

  /**
   * @brief This button will restart the level
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UReloadLevelButton> m_pRestartButton;

  /**
   * @brief The level results.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USpacer> m_pRestartSpacer;

  /**
   * @brief This button will return to the hub level.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pReturnToHubButton;

  /**
   * @brief Text for the return to HUB button.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pReturnToHubText;

  /**
   * @brief The level results.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USpacer> m_pReturnToHubSpacer;

  /**
   * @brief Thid Button will open the settings menu.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UBaseButton> m_pSettingsButton;

  /**
   * @brief This button will show the main menu panel
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<ULoadLevelButton> m_pExitButton;
#pragma endregion

  /**
   * Hub map used to load the level after the win screen.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Maps", meta = (DisplayName = "HUB", AllowPrivateAccess = "true"))
  TSoftObjectPtr<UWorld> m_pHubMap;

  /**
   * Text for the return to HUB button if the current level is the tutorial.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Skip", AllowPrivateAccess = "true"))
  FText m_sSkipText;

  /**
   * Text for the return to HUB button.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Texts", meta = (DisplayName = "Return to HUB", AllowPrivateAccess = "true"))
  FText m_sReturnToHubText;

  /**
   * @brief The parent panel of the widget
   */
  UPROPERTY(EditInstanceOnly, meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pPParent;

  /**
   * @brief The Settings Menu widget.
   */
  UPROPERTY(EditInstanceOnly, meta = (BindWidget))
  TObjectPtr<UMainSettingsMenu> m_pSettingsMenu;

  bool m_bIsMenuActive = false;

  bool m_bIsInHub;
};