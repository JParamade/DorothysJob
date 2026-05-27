/************************************************************************
 * @description: Represents the main menu's widget.
 * @author: Jaime Paramo
 * @date: 19/03/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/FocusableWidget.h"
#include "MainMenu.generated.h"

// Audio
class UFMODEvent;
namespace FMOD {
  namespace Studio {
    class EventInstance;
    class System;
  }
}

// Component
class UBaseButtonText;
class UImage;
class ULoadLevelButton;
class UQuitGameButton;
class UTextBlock;

// Manager
class USaveGameManager;

// Widget
class UBaseCreditsScreen;
class UPressAnyKeyText;
class UMainSettingsMenu;
//class UVersionText;
class UCinematicScreen;
class UTechLogos;
class UConfirmationWindow;

UCLASS()
class DOROTHYSJOB_API UMainMenu : public UFocusableWidget
{
  GENERATED_BODY()

public:
  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTechLogos> m_pTechLogos;

  /**
   * @brief -> Reference to the background Image of the main menu. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pBackgroundImage;

  /**
   * @brief -> Reference to the title Image of the main menu. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UImage> m_pTitleImage;

  /**
   * @brief -> Reference to the "Press Any Key" widget. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UPressAnyKeyText> m_pPressAnyKeyText;

  /**
   * @brief -> Reference to the SettingsMenu widget. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget, DisplayName = "Settings Menu"))
  TObjectPtr<UMainSettingsMenu> m_pSettingsMenu;

  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UConfirmationWindow> m_pConfirmationWindow;

  /**
   * @brief -> Reference to the VersionText widget. Must match this name in UMG to compile.
   */
  // UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  // TObjectPtr<UVersionText> m_pVersionText;

  /**
   * @brief -> Reference to the Button that opens the tutorial screen. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButtonText> m_pNewGameButton;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pNewGameText;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButtonText> m_pContinueButton;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pContinueText;

  /**
   * @brief -> Reference to the Button that opens the settings menu. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButtonText> m_pSettingsButton;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pSettingsText;

  /**
   * @brief -> Reference to the Button that quits the game. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseButtonText> m_pCreditsButton;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pCreditsText;

  /**
   * @brief -> Reference to the Button that quits the game. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UQuitGameButton> m_pQuitButton;

  /**
 * @brief -> @TOFILL. Must match this name in UMG to compile.
 */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UTextBlock> m_pQuitText;

  /**
   * @brief -> @TOFILL. Must match this name in UMG to compile.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UBaseCreditsScreen> m_pCreditsScreen;

  /**
   * @brief @TOFILL.
   */
  UFUNCTION(BlueprintCallable)
  virtual void InitMenu();

  /**
   * @brief -> Displays the UMG elements of this Widget on screen.
   */
  virtual void Show() override;

  /**
   * @brief -> Display the custom UMG elements of this Widget on screen.
   */
  virtual void CustomShow();

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

private:
  /**
   * @brief -> Called when clickin on new game
   */
  UFUNCTION(BlueprintCallable)
  void OnNewGame();

  /**
   * @brief -> Called when clicking on continue
   */
  UFUNCTION(BlueprintCallable)
  void OnContinue();

  /**
   * @brief -> Called when the player clicks on the Settings Button in the UI.
   */
  UFUNCTION(BlueprintCallable)
  void OnSettings();

  /**
   * @brief -> Called when the player clicks on the Settings Button in the UI.
   */
  UFUNCTION(BlueprintCallable)
  void OnCredits();

  /**
   * @brief Reference to the SaveGame manager
   */
  TObjectPtr<USaveGameManager> m_pSaveGameManager;

  /**
   * @brief Reference to the HUB map to load
   */
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Maps", meta = (DisplayName = "HUB", AllowPrivateAccess = "true"))
  TSoftObjectPtr<UWorld> m_pHubMap;

  UPROPERTY(meta = (BindWidget))
  TObjectPtr<UCinematicScreen> m_pCinematicScreen;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Music Event", AllowPrivateAccess = true))
  TObjectPtr<UFMODEvent> m_pMusicEvent;

  FMOD::Studio::EventInstance* m_pEventInstance = nullptr;
};