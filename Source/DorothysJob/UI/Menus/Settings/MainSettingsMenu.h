/************************************************************************
 * @description: It represents the main settings menu class.
 * @author: David Gonzalez
 * @date: 13/10/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "MainSettingsMenu.generated.h"

class UCanvasPanel;
class USettingsMenu;
class UControlsTab;
class UConfirmationWindow;

DECLARE_DELEGATE(FOnMenuAction)

UCLASS()
class DOROTHYSJOB_API UMainSettingsMenu : public UBaseWidget
{
	GENERATED_BODY()
	
public:

  /**
   * @brief The default constructor of the widget.
   */
  virtual void NativeConstruct() override;

  /**
   * @brief The default destructor of the widget.
   */
  virtual void NativeDestruct() override;

  /**
   * @brief It displays the main settings screen.
   */
  virtual void Show() override;

  /**
   * @brief It hides the main settings screen.
   */
  virtual void Hide() override;

  /**
   * @brief It returns if the user can exit from the settings screen or not.
   */
  bool CanExit();

  /**
   * @brief It changes the current tab in the settings screen.
   * @param _bGoRight -> True if should go to right.
   */
  UFUNCTION(BlueprintCallable)
  void ChangeTab(bool _bGoRight);

  /**
   * @brief The signal to know when the menu is closed.
   */
  FOnMenuAction OnMenuClosed;

  /**
   * It shows reload level action is enabled or not.
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Language", meta = (DisplayName = "Reload level enabled"))
  bool m_bReloadLevelEnabled;

private:

  /**
   * @brief It complete the hide action for the menu.
   */
  void FinishToHide();

  /**
   * @brief It shows the general settings screen.
   */
  void ShowGeneralSettingsScreen();

  /**
   * @brief It hides the general settings screen.
   */
  void HideGeneralSettingsScreen();

  /**
   * @brief It shows the controls screen.
   */
  void ShowControlsScreen();

  /**
   * @brief It hides the controls screen.
   */
  void HideControlsScreen();

  /**
   * @brief It checks the confirmation window result.
   * @param _bResult -> It indicates the window result.
   */
  UFUNCTION()
  void ConfirmationResult(bool _bResult);

  /**
   * This represents the parent panel of the widget
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCanvasPanel> m_pCanvasPanel;

  /**
   * It is the general settings tab.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<USettingsMenu> m_pSettingsMenu;

  /**
   * It is the controls screen tab.
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UControlsTab> m_pControlsScreen;

  /**
   * It is the confirmation windows to confirm the selected language. 
   */
  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UConfirmationWindow> m_pConfirmationWindow;

  /**
   * It is the fade out animation when the language is changed.
   */
  UPROPERTY(meta = (BindWidgetAnim), Transient)
  TObjectPtr<UWidgetAnimation> m_pFadeOutAnimation;

  /**
   * It is the current language when the screen is shown.
   */
  FString m_sCurrentLanguage;
};