/************************************************************************
 * @description: Represents the general focus manager of UI menus.
 * @author: Jaime Paramo
 * @date: 19/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "GeneralFocusManager.h"

void UGeneralFocusManager::SetActiveMenu(TArray<TObjectPtr<USpecificFocusManager>>& _pNewActiveMenu) {
  m_pActiveMenuStack = _pNewActiveMenu;
  if (m_pActiveMenuStack.IsEmpty()) return;

  UpdateMenuStack(0);
}

void UGeneralFocusManager::Navigate(FVector2D _vAxisValue) {
  if (m_pActiveMenuStack.IsEmpty()) return;
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->Navigate(_vAxisValue);
}

void UGeneralFocusManager::ConfirmPressed() {
  if (m_pActiveMenuStack.IsEmpty()) return;
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ConfirmCurrentPressed();
}

void UGeneralFocusManager::ConfirmReleased() {
  if (m_pActiveMenuStack.IsEmpty()) return;
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ConfirmCurrentReleased();
}

void UGeneralFocusManager::Cancel() {
  int32 iNewIndex = FMath::Max(m_iCurrentMenuIndex - 1, 0);

  // UpdateMenuStack(iNewIndex);
}

void UGeneralFocusManager::ActivateFocus() {
  if (m_pActiveMenuStack.IsEmpty()) return;
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ActivateFocus();
}

void UGeneralFocusManager::ResetFocus() {
  if (m_pActiveMenuStack.IsEmpty()) return;
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ResetFocus();
}

TObjectPtr<USpecificFocusManager> UGeneralFocusManager::GetActiveMenu() const {
  return m_pActiveMenuStack[m_iCurrentMenuIndex];
}

void UGeneralFocusManager::UpdateMenuStack(int32 _iNewIndex) {
  if (m_pActiveMenuStack.IsEmpty()) return;
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->OnMenuExit();

  m_iCurrentMenuIndex = _iNewIndex;

  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->OnMenuEnter();
}