/************************************************************************
 * @description: Represents the specific focus manager of UI menus.
 * @author: Jaime Paramo
 * @date: 19/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "SpecificFocusManager.h"

#include "DorothysJob/Interface/Focus/Focusable.h"

// UI
#include "DorothysJob/UI/UMGPalette/Button/BaseButton.h"
#include "DorothysJob/UI/UMGPalette/Slider/BaseSlider.h"

void USpecificFocusManager::Initialize(const TArray<TObjectPtr<UObject>>& _rFocusableElements, const TArray<int32> _lRowLengths, EFocusNavigationType _eFocusNavigationType, bool _bWrap, bool _bRestoreLastFocus) {
  // Store the provided focusable elements and configuration settings.
  m_lFocusableElements = _rFocusableElements;

  // Ensure any currently focused buttons lose focus before reinitializing.
  for (UObject* pObject : m_lFocusableElements) {
    if (UBaseButton* pButton = Cast<UBaseButton>(pObject)) {
      pButton->ForceLoseFocus();
    }
  }

  // Store row lengths and navigation settings.
  m_lRowLengths = _lRowLengths;
  m_eFocusNavigationType = _eFocusNavigationType;
  m_bWrap = _bWrap;
  m_bRestoreLastFocus = _bRestoreLastFocus;

  // Reset current focus to the initial position.
  m_iCurrentRow = 0;
  m_iCurrentColumn = 0;
}

void USpecificFocusManager::Navigate(FVector2D _vAxisValue) {
  // If there are no focusable elements, exit early.
  if (m_lFocusableElements.Num() == 0) return;

  // Normalize the input axis value to determine the direction of navigation.
  FVector2D iDirection = _vAxisValue.GetSafeNormal();

  // Prioritize horizontal navigation if the horizontal input is stronger than the vertical input.
  if (FMath::Abs(iDirection.X) > 0.5f && FMath::Abs(iDirection.X) > FMath::Abs(iDirection.Y)) {
    int32 iCurrentIndex = GetFlatIndex(m_iCurrentRow, m_iCurrentColumn);
    if (UObject* pFocusedObject = m_lFocusableElements.IsValidIndex(iCurrentIndex) ? m_lFocusableElements[iCurrentIndex] : nullptr) {
      // If the currently focused object implements IAxisNavigable, call its OnAxisInput method and return early to prevent default navigation.
      if (IAxisNavigable* pAxisNavigable = Cast<IAxisNavigable>(pFocusedObject)) {
        pAxisNavigable->OnAxisInput(_vAxisValue);
        return;
      }
    }

    // If the currently focused object does not handle axis input, navigate horizontally based on the input direction.
    NavigateHorizontal(FMath::Sign(iDirection.X));
    return;
  }

  // If vertical input is stronger than horizontal input, navigate vertically.
  if (FMath::Abs(iDirection.Y) > 0.5f) NavigateVertical(-FMath::Sign(iDirection.Y));
}

void USpecificFocusManager::ConfirmCurrentPressed() {
  // If there is a currently focused element, call its OnPressed method to trigger the press action.
  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnPressed();
}

void USpecificFocusManager::ConfirmCurrentReleased() {
  // If there is a currently focused element, call its OnReleased method to trigger the release action.
  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnReleased();
}

void USpecificFocusManager::ActivateFocus() {
  // Activate focus on the current element, which will trigger its OnGainFocus method and visually indicate that it is focused.
  UpdateFocus(0, 0);
}

void USpecificFocusManager::ResetFocus() {
  // If there is a currently focused element, call its OnLoseFocus method to clear the focus state before resetting.
  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnLoseFocus();

  // Reset the current row and column to the initial position (0, 0).
  m_iCurrentRow = 0;
  m_iCurrentColumn = 0;
}

void USpecificFocusManager::OnMenuEnter() {
  // Determine the starting index for focus based on whether we should restore the last focus or start from the beginning.
  int32 iStartingIndex = m_iLastFocusIndex != INDEX_NONE ? m_iLastFocusIndex : 0;

  // Clamp the starting index to ensure it is within the valid range of focusable elements.
  iStartingIndex = FMath::Clamp(iStartingIndex, 0, m_lFocusableElements.Num() - 1);

  // Iterate through the row lengths to find the corresponding row and column for the starting index, then update the focus accordingly.
  int32 iAccumulator = 0;
  for (int32 iRow = 0; iRow < m_lRowLengths.Num(); ++iRow) {
    int32 iCount = m_lRowLengths[iRow];
    if (iStartingIndex < iAccumulator + iCount) {
      int32 iColumn = iStartingIndex - iAccumulator;
      UpdateFocus(iRow, iColumn);
      return;
    }
    iAccumulator += iCount;
  }
}

void USpecificFocusManager::OnMenuExit() {
  // Save the current focus index to restore it later if needed.
  m_iLastFocusIndex = m_bRestoreLastFocus ? GetFlatIndex(m_iCurrentRow, m_iCurrentColumn) : 0;

  // If there is a currently focused element, call its OnLoseFocus method to clear the focus state before exiting the menu.
  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnLoseFocus();
}

void USpecificFocusManager::NavigateHorizontal(int32 _iDirection) {
  // If the navigation type is set to vertical, do not perform horizontal navigation.
  if (m_eFocusNavigationType == EFocusNavigationType::VERTICAL) return;

  // Calculate the new column index by adding the direction to the current column index.
  int32 iNewColumn = m_iCurrentColumn + _iDirection;

  // Clamp or wrap the new column index based on the current row length and wrapping settings.
  int32 iRowLength = m_lRowLengths[m_iCurrentRow];
  iNewColumn = m_bWrap ? (iNewColumn + iRowLength) % iRowLength : FMath::Clamp(iNewColumn, 0, iRowLength - 1);

  // Update the focus to the new column while keeping the current row unchanged.
  UpdateFocus(m_iCurrentRow, iNewColumn);
}

void USpecificFocusManager::NavigateVertical(int32 _iDirection) {
  // If the navigation type is set to horizontal, do not perform vertical navigation.
  if (m_eFocusNavigationType == EFocusNavigationType::HORIZONTAL) return;

  // Calculate the new row index by adding the direction to the current row index.
  int32 iNewRow = m_iCurrentRow + _iDirection;

  // Clamp or wrap the new row index based on the total number of rows and wrapping settings.
  int32 iRowNumber = m_lRowLengths.Num();
  iNewRow = m_bWrap ? (iNewRow + iRowNumber) % iRowNumber : FMath::Clamp(iNewRow, 0, iRowNumber - 1);

  // When navigating vertically, we need to ensure that the new column index is valid for the new row, so we clamp it based on the length of the new row.
  int32 iMaxColumn = m_lRowLengths[iNewRow] - 1;
  int32 iNewColumn = FMath::Clamp(m_iCurrentColumn, 0, iMaxColumn);

  // Update the focus to the new row and column.
  UpdateFocus(iNewRow, iNewColumn);
}

int32 USpecificFocusManager::GetFlatIndex(int32 _iRow, int32 _iColumn) const {
  // Calculate the flat index in the focusable elements array based on the provided row and column indices.
  if (!m_lRowLengths.IsValidIndex(_iRow)) return INDEX_NONE;
  if (_iColumn < 0 || _iColumn >= m_lRowLengths[_iRow]) return INDEX_NONE;

  // Sum the lengths of all previous rows to get the starting index for the current row, then add the column index to get the final flat index.
  int32 iFlatIndex = 0;
  for (int32 iIndex = 0; iIndex < _iRow; ++iIndex) {
    iFlatIndex += m_lRowLengths[iIndex];
  }
  iFlatIndex += _iColumn;

  // Return the calculated flat index.
  return iFlatIndex;
}

IFocusable* USpecificFocusManager::GetFocusableAt(int32 _iIndex) const {
  // Check if the provided index is valid within the focusable elements array.
  if (!m_lFocusableElements.IsValidIndex(_iIndex)) return nullptr;

  // Return the focusable element at the specified index, cast to the IFocusable interface.
  return Cast<IFocusable>(m_lFocusableElements[_iIndex]);
}

void USpecificFocusManager::UpdateFocus(int32 _iRow, int32 _iColumn) {
  // Calculate the flat index for the new row and column. If the index is invalid, exit early.
  int32 iFlatIndex = GetFlatIndex(_iRow, _iColumn);
  if (iFlatIndex == INDEX_NONE) return;

  // If there is a currently focused element, call its OnLoseFocus method to clear the focus state before updating to the new element.
  if (IFocusable* pPreviousElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pPreviousElement->OnLoseFocus();

  // Update the current row and column to the new values.
  m_iCurrentRow = _iRow;
  m_iCurrentColumn = _iColumn;

  // If there is a new element at the calculated flat index, call its OnGainFocus method to set the focus state on the new element.
  if (IFocusable* pNewElement = GetFocusableAt(iFlatIndex)) pNewElement->OnGainFocus();
}