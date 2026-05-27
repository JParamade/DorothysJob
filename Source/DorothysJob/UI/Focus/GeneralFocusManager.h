/************************************************************************
 * @description: Represents the general focus manager of UI menus.
 * @author: Jaime Paramo
 * @date: 19/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "DorothysJob/UI/Focus/SpecificFocusManager.h"
#include "GeneralFocusManager.generated.h"

UCLASS()
class DOROTHYSJOB_API UGeneralFocusManager : public UObject
{
	GENERATED_BODY()

public:
	/**
	 * @brief Sets the currently active menu or focusable widget group.
	 * @param pNewFocusedWidget Pointer to the new specific manager to activate.
	 */
	void SetActiveMenu(TArray<TObjectPtr<USpecificFocusManager>>& _pNewActiveMenu);

	/**
	 * @brief Handles navigation within the currently active menu.
	 */
	void Navigate(FVector2D _vAxisValue);
	/**
	 * @brief Handles the press event for the confirm button.
	 */
	void ConfirmPressed();
	/**
	 * @brief Handles the release event for the confirm button.
	 */
	void ConfirmReleased();
	/**
	 * @brief Handles the cancel event.
	 */
	void Cancel();

	/**
   * @brief Activates the focus on the current active menu, allowing it to respond to navigation and confirm inputs.
	 */
	void ActivateFocus();
  /**
   * @brief Resets the focus of the current active menu, typically called when the menu is re-entered or needs to reset its state.
   */
	void ResetFocus();

	/**
	 * @brief Gets the currently active menu.
	 * @return Pointer to the active USpecificFocusManager.
	 */
	TObjectPtr<USpecificFocusManager> GetActiveMenu() const;

protected:
	/**
	 * @brief Pointer to the currently active menu being navigated.
	 */
	UPROPERTY()
	TArray<TObjectPtr<USpecificFocusManager>> m_pActiveMenuStack;

private:
	/**
	 * @brief Updates the menu stack to the specified index.
	 * @param iNewIndex The index of the new active menu.
	 */
	void UpdateMenuStack(int32 _iNewIndex);
	/**
   * @brief Index of the currently active menu in the stack. Used to track which menu is currently being navigated.
	 */
	int32 m_iCurrentMenuIndex;
};
