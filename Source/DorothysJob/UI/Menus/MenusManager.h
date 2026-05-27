/************************************************************************
 * @description: The GUI main handler's widget implementaion.
 * @author: Josephine Esposito
 * @date: 27/03/2025
 * @edited_by: David Gonzalez
 *			       Jaime Páramo
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "MenusManager.generated.h"

class UHUDPlayer;
class UPauseMenu;
class UDialogueScreen;
class UResultsScreen;
class UTutorialMenu;
enum class EBaseGameState : uint8;

/**
 * This represents the gui manager
 */
UCLASS()
class DOROTHYSJOB_API UMenusManager : public UBaseWidget
{
	GENERATED_BODY()
public:

#pragma region | Virtual Methods

	/**
	 * @brief This will activate the widget
	 */
	virtual void Show() override;

	/**
	 * @brief This will deactivate the widget
	 */
	virtual void Hide() override;

	//@review ERASE WHEN MADRID OTAKU ENDS!!!
	UFUNCTION(BlueprintCallable)
	void ShowTutorial();

#pragma endregion

	/**
	 * @brief This represents the player's HUD
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHUDPlayer* m_pHUDPlayer;

	/**
	 * @brief This represents the Pause menu
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPauseMenu* m_pPauseMenu;

  /**
   * @brief This represents the results screen.
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  TObjectPtr<UResultsScreen> m_pResultsScreen;

	/**
   * @brief This represents the Dialog Screen
   */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDialogueScreen* m_pDialogueScreen;

	/**
	 * @brief This represents the Dialog Screen
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTutorialMenu* m_pPermanentScreen;

private:

	EBaseGameState m_oStatus;

  /**
   * @brief This function handles the cancel action for the menus.
   */
  UFUNCTION()
  void HandleCancelAction(bool _bPressed);

  /**
   * @brief This function handles the change tab action for the menus.
   */
  UFUNCTION()
  void HandleChangeTabAction(bool _bGoRight);

	/**
	 * @brief This handles the game level status's changes
	 * @param _status, The level status (delegate)
	 */
	void LevelStateHandler(EBaseGameState _status);

	void SafeShow(UBaseWidget* Screen);

	void SafeHide(UBaseWidget* Screen);

	void SetFocusAndController();
};
//EOF