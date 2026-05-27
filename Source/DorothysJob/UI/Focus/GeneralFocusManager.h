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
	 * @brief -> Sets the currently active menu or focusable widget group.
	 * @param pNewFocusedWidget -> Pointer to the new specific manager to activate.
	 */
	void SetActiveMenu(TArray<TObjectPtr<USpecificFocusManager>>& _pNewActiveMenu);

	/**
	 * @brief -> Handles navigation within the currently active menu.
	 */
	void Navigate(FVector2D _vAxisValue);
	void ConfirmPressed();
	void ConfirmReleased();
	void Cancel();

	void ActivateFocus();
	void ResetFocus();

	/**
	 * @brief -> Gets the currently active menu.
	 * @return -> Pointer to the active USpecificFocusManager.
	 */
	TObjectPtr<USpecificFocusManager> GetActiveMenu() const;

protected:
	/**
	 * @brief -> Pointer to the currently active menu being navigated.
	 */
	UPROPERTY()
	TArray<TObjectPtr<USpecificFocusManager>> m_pActiveMenuStack;

private:
	void UpdateMenuStack(int32 _iNewIndex);

	int32 m_iCurrentMenuIndex;
};
