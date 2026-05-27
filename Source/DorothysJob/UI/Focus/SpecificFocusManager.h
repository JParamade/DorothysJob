/************************************************************************
 * @description: Represents the specific focus manager of UI menus.
 * @author: Jaime Paramo
 * @date: 19/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SpecificFocusManager.generated.h"

UENUM(BlueprintType)
enum class EFocusNavigationType : uint8 {
	VERTICAL   UMETA(DisplayName = "Vertical"),
	HORIZONTAL UMETA(DisplayName = "Horizontal"),
	GRID       UMETA(DisplayName = "Grid"),
	CUSTOM     UMETA(DisplayName = "Custom"),
};

class IFocusable;

UCLASS()
class DOROTHYSJOB_API USpecificFocusManager : public UObject
{
	GENERATED_BODY()
public:
	/**
	 * @brief Initializes the focus manager with a list of focusable elements and a navigation type.
	 *
	 * @param _rFocusableElements Array of focusable UI elements this manager will control.
	 * @param _eFocusNavigationType Determines how navigation behaves (e.g. Vertical, Horizontal, Grid).
	 */
	void Initialize(const TArray<TObjectPtr<UObject>>& _rFocusableElements, const TArray<int32> _lRowLengths, EFocusNavigationType _eFocusNavigationType, bool _bWrap, bool _bRestoreLastFocus);

	/**
   * @brief Handles navigation input to move focus between elements based on the provided axis values.
   * @param _vAxisValue A 2D vector representing the navigation input, where X is horizontal input and Y is vertical input. The method will determine the direction of navigation and move focus accordingly.
	 */
	void Navigate(FVector2D _vAxisValue);
	/**
   * @brief Handles the press event for the confirm action, typically used to activate or select the currently focused element.
	 */
	void ConfirmCurrentPressed();
	/**
   * @brief Handles the release event for the confirm action, typically used to deactivate or deselect the currently focused element.
	 */
	void ConfirmCurrentReleased();

	/**
	 * @brief Activates focus on the current element.
	 */
	void ActivateFocus();
  /**
   * @brief Resets the focus to the initial state, typically called when the menu is re-entered or needs to reset its state.
   */
	void ResetFocus();

	/**
	 * @brief Called when the menu is entered or becomes active. Restores focus to the last focused element if available otherwise, defaults to the first valid focusable element.
	 */
	void OnMenuEnter();
	/**
	 * @brief Called when the menu is exited or loses focus. Saves the current focus index and calls OnLoseFocus on the focused element, then resets the focus index.
	 */
	void OnMenuExit();

private:
	/**
	 * @brief Navigates horizontally based on the provided direction.
	 * @param _iDirection The direction of navigation (1 for right, -1 for left).
	 */
	void NavigateHorizontal(int32 _iDirection);
	/**
	 * @brief Navigates vertically based on the provided direction.
	 * @param _iDirection The direction of navigation (1 for down, -1 for up).
	 */
	void NavigateVertical(int32 _iDirection);

	/**
	 * @brief Converts 2D row and column indices to a flat index.
	 * @param _iRow The row index.
	 * @param _iColumn The column index.
	 * @return The flat index corresponding to the 2D coordinates.
	 */
	int32 GetFlatIndex(int32 _iRow, int32 _iColumn) const;

	/**
	 * @brief Array of focusable UI elements this manager will control.
	 */
	UPROPERTY()
	TArray<TObjectPtr<UObject>> m_lFocusableElements;
	/**
   * @brief Array representing the number of focusable elements in each row, used for grid navigation.
	 */
	TArray<int32> m_lRowLengths;
	/**
	 * @brief Retrieves the focusable element at the specified index.
	 * @param _iIndex The index of the focusable element to retrieve.
	 * @return A pointer to the focusable element at the specified index, or nullptr if not found.
	 */
	IFocusable* GetFocusableAt(int32 _iIndex) const;

	/**
	 * @brief The current row index of the focused element.
	 */
	int32 m_iCurrentRow = INDEX_NONE;
	/**
	 * @brief The current column index of the focused element.
	 */
	int32 m_iCurrentColumn = INDEX_NONE;

	/**
	 * @brief The type of focus navigation to use.
	 */
	EFocusNavigationType m_eFocusNavigationType;

	/**
	 * @brief Whether to wrap navigation when reaching the end of a row or column.
	 */
	bool m_bWrap = false;
	/**
	 * @brief Whether to restore the last focused element when the menu is re-entered.
	 */
	bool m_bRestoreLastFocus = false;
	/**
	 * @brief The index of the last focused element.
	 */
	int32 m_iLastFocusIndex = INDEX_NONE;

	/**
	 * @brief Updates the focus to the element at the specified row and column.
	 * @param _iRow The row index of the element to focus.
	 * @param _iColumn The column index of the element to focus.
	 */
	void UpdateFocus(int32 _iRow, int32 _iColumn);
};
