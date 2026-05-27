/************************************************************************
 * @description: The GUI main handler's widget implementaion for the HUB
 * @author: Josephine Esposito
 * @date: 04/07/2025
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/UI/BaseWidget.h"
#include "HUBMenuManager.generated.h"

#pragma region | Forward declaration
enum class EBaseGameState : uint8;
class UPauseMenu;
class USelectWeaponsMenu;
class UHUDPlayer;
class USelectConsumablesMenu;
class UPreLevelScreen;
class UDialogueScreen;
class UTutorialMenu;
class UReplayMenu;
#pragma endregion

/**
* @brief Declaration of the Category for Logs
*/
DECLARE_LOG_CATEGORY_EXTERN(HUBMenuManager, Log, All);

/**
 * This represents the GUI manager for the HUB
 */
UCLASS()
class DOROTHYSJOB_API UHUBMenuManager : public UBaseWidget
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

#pragma endregion

#pragma region | GUI Menus
	/**
	 * @brief This represents the Pause Menu
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPauseMenu* m_pPauseMenu;

	/**
	 * @brief This represents the Select Weapon Menu
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USelectWeaponsMenu* m_pWeaponSelectMenu;

	/**
	 * @brief This represents the player's HUD
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UHUDPlayer* m_pHUDPlayer;

	/**
	 * @brief This represents the Select Consumable Menu
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	USelectConsumablesMenu* m_pConsumableSelectMenu;

	/**
	 * @brief This represents the Pre-Level Screen.
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UPreLevelScreen* m_pPreLevelScreen;

	/**
	 * @brief This represents the Dialogue Screen.
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UDialogueScreen* m_pDialogueScreen;

	/**
		* @brief This represents the Dialog Screen
		*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTutorialMenu* m_pPermanentScreen;

  /**
   * @brief This represents the Replay Menu
   */
  UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
  UReplayMenu* m_pReplayMenu;
#pragma endregion

	UFUNCTION(BlueprintCallable, Category = "Bola 13|UI")
	void RevealAllElements();

private:
	/**
	 * @brief This handles the game level status's changes
	 * @param _status The level status (delegate)
	 */
	UFUNCTION()
	void LevelStateHandler(EBaseGameState _status);

	void SafeShow(UBaseWidget* Screen);

	void SafeHide(UBaseWidget* Screen);

	void SetFocusAndController();

	void SetEvent(EBaseGameState _status);

	EBaseGameState m_oStatus;

	EBaseGameState m_oStatusEvent;
	/**
	 * @brief This will send to the consumable selector the received input
	 * @param _bIsFirst a flag stating if it was the first input or the second
	 */
	UFUNCTION()
	void HandleSelectedConsumable();

	/**
	 * @brief This will send to the consumable selector the received direction for the carousel
	 * @param _iDirection The direction of the input horizontally
	 */
	UFUNCTION()
	void HandleConsumableNavigation(int32 _iDirection);

  UFUNCTION()
  void LoadDialog(bool _toggle);

  /**
   * @brief This will load the next level
   */
  UFUNCTION()
  void LoadNextLevel();

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

	void StartEvent();

	/**
	 * @brief This will load the next level
	 */
	UFUNCTION()
	void ContinueAfterEvent();

	/**
   * @brief This will load the next level
   */
	UFUNCTION()
	void ContinueAfterDialogue();
};
//EOF