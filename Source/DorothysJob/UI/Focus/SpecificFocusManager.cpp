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
  m_lFocusableElements = _rFocusableElements;

  for (UObject* pObject : m_lFocusableElements) {
    if (UBaseButton* pButton = Cast<UBaseButton>(pObject)) {
      pButton->ForceLoseFocus();
    }
  }

  m_lRowLengths = _lRowLengths;
  m_eFocusNavigationType = _eFocusNavigationType;
  m_bWrap = _bWrap;
  m_bRestoreLastFocus = _bRestoreLastFocus;

  m_iCurrentRow = 0;
  m_iCurrentColumn = 0;
}

void USpecificFocusManager::Navigate(FVector2D _vAxisValue) {
  if (m_lFocusableElements.Num() == 0) return;

  FVector2D iDirection = _vAxisValue.GetSafeNormal();

  if (FMath::Abs(iDirection.X) > 0.5f && FMath::Abs(iDirection.X) > FMath::Abs(iDirection.Y)) {
    int32 iCurrentIndex = GetFlatIndex(m_iCurrentRow, m_iCurrentColumn);
    if (UObject* pFocusedObject = m_lFocusableElements.IsValidIndex(iCurrentIndex) ? m_lFocusableElements[iCurrentIndex] : nullptr) {
      if (IAxisNavigable* pAxisNavigable = Cast<IAxisNavigable>(pFocusedObject)) {
        pAxisNavigable->OnAxisInput(_vAxisValue);
        return; 
      }
    }

    NavigateHorizontal(FMath::Sign(iDirection.X));
    return;
  }

  if (FMath::Abs(iDirection.Y) > 0.5f) NavigateVertical(-FMath::Sign(iDirection.Y));
}

void USpecificFocusManager::ConfirmCurrentPressed() {
  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnPressed();
}

void USpecificFocusManager::ConfirmCurrentReleased() {
  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnReleased();
}

void USpecificFocusManager::ActivateFocus() {
  UpdateFocus(0, 0);
}

void USpecificFocusManager::ResetFocus() {
  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnLoseFocus();

  m_iCurrentRow = 0;
  m_iCurrentColumn = 0;
}

void USpecificFocusManager::OnMenuEnter() {
  int32 iStartingIndex = m_iLastFocusIndex != INDEX_NONE ? m_iLastFocusIndex : 0;

  iStartingIndex = FMath::Clamp(iStartingIndex, 0, m_lFocusableElements.Num() - 1);

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
  m_iLastFocusIndex = m_bRestoreLastFocus ? GetFlatIndex(m_iCurrentRow, m_iCurrentColumn) : 0;

  if (IFocusable* pFocusedElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pFocusedElement->OnLoseFocus();
}

void USpecificFocusManager::NavigateHorizontal(int32 _iDirection) {
  if (m_eFocusNavigationType == EFocusNavigationType::VERTICAL) return;

  int32 iNewColumn = m_iCurrentColumn + _iDirection;

  int32 iRowLength = m_lRowLengths[m_iCurrentRow];
  iNewColumn = m_bWrap ? (iNewColumn + iRowLength) % iRowLength : FMath::Clamp(iNewColumn, 0, iRowLength - 1);

  UpdateFocus(m_iCurrentRow, iNewColumn);
}

void USpecificFocusManager::NavigateVertical(int32 _iDirection) {
  if (m_eFocusNavigationType == EFocusNavigationType::HORIZONTAL) return;

  int32 iNewRow = m_iCurrentRow + _iDirection;

  int32 iRowNumber = m_lRowLengths.Num();
  iNewRow = m_bWrap ? (iNewRow + iRowNumber) % iRowNumber : FMath::Clamp(iNewRow, 0, iRowNumber - 1);

  int32 iMaxColumn = m_lRowLengths[iNewRow] - 1;
  int32 iNewColumn = FMath::Clamp(m_iCurrentColumn, 0, iMaxColumn);

  UpdateFocus(iNewRow, iNewColumn);
}

int32 USpecificFocusManager::GetFlatIndex(int32 _iRow, int32 _iColumn) const {
  if (!m_lRowLengths.IsValidIndex(_iRow)) return INDEX_NONE;
  if (_iColumn < 0 || _iColumn >= m_lRowLengths[_iRow]) return INDEX_NONE;

  int32 iFlatIndex = 0;
  for (int32 iIndex = 0; iIndex < _iRow; ++iIndex) {
    iFlatIndex += m_lRowLengths[iIndex];
  }
  iFlatIndex += _iColumn;

  return iFlatIndex;
}

IFocusable* USpecificFocusManager::GetFocusableAt(int32 _iIndex) const {
  if (!m_lFocusableElements.IsValidIndex(_iIndex)) return nullptr;

  return Cast<IFocusable>(m_lFocusableElements[_iIndex]);
}

void USpecificFocusManager::UpdateFocus(int32 _iRow, int32 _iColumn) {
  int32 iFlatIndex = GetFlatIndex(_iRow, _iColumn);
  if (iFlatIndex == INDEX_NONE) return;

  if (IFocusable* pPreviousElement = GetFocusableAt(GetFlatIndex(m_iCurrentRow, m_iCurrentColumn))) pPreviousElement->OnLoseFocus();

  m_iCurrentRow = _iRow;
  m_iCurrentColumn = _iColumn;

  if (IFocusable* pNewElement = GetFocusableAt(iFlatIndex)) pNewElement->OnGainFocus();
}