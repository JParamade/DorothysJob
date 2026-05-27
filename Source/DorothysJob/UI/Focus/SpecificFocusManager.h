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
	 * @brief -> Initializes the focus manager with a list of focusable elements and a navigation type.
	 *
	 * @param _rFocusableElements -> Array of focusable UI elements this manager will control.
	 * @param _eFocusNavigationType -> Determines how navigation behaves (e.g. Vertical, Horizontal, Grid).
	 */
	void Initialize(const TArray<TObjectPtr<UObject>>& _rFocusableElements, const TArray<int32> _lRowLengths, EFocusNavigationType _eFocusNavigationType, bool _bWrap, bool _bRestoreLastFocus);

	/**
	 * @brief -> Calculates navigation .
	 */
	void Navigate(FVector2D _vAxisValue);
	void ConfirmCurrentPressed();
	void ConfirmCurrentReleased();

	void ActivateFocus();
	void ResetFocus();

	/**
	 * @brief -> Called when the menu is entered or becomes active. Restores focus to the last focused element if available; 
	 *           otherwise, defaults to the first valid focusable element.
	 */
	void OnMenuEnter();
	/**
	 * @brief -> Called when the menu is exited or loses focus. Saves the current focus index and calls OnLoseFocus on the 
	 *           focused element, then resets the focus index.
	 */
	void OnMenuExit();

private:
	void NavigateHorizontal(int32 _iDirection);
	void NavigateVertical(int32 _iDirection);

	int32 GetFlatIndex(int32 _iRow, int32 _iColumn) const;

	UPROPERTY()
	TArray<TObjectPtr<UObject>> m_lFocusableElements;
	TArray<int32> m_lRowLengths;
	IFocusable* GetFocusableAt(int32 _iIndex) const;

	int32 m_iCurrentRow = INDEX_NONE;
	int32 m_iCurrentColumn = INDEX_NONE;

	EFocusNavigationType m_eFocusNavigationType;

	bool m_bWrap = false;
	bool m_bRestoreLastFocus = false;
	int32 m_iLastFocusIndex = INDEX_NONE;

	void UpdateFocus(int32 _iRow, int32 _iColumn);
};
