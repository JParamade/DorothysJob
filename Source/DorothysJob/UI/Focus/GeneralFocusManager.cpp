/************************************************************************
 * @description: Represents the general focus manager of UI menus.
 * @author: Jaime Paramo
 * @date: 19/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "GeneralFocusManager.h"

void UGeneralFocusManager::SetActiveMenu(TArray<TObjectPtr<USpecificFocusManager>>& _pNewActiveMenu) {
  // If the new menu stack is the same as the current one, do nothing.
  m_pActiveMenuStack = _pNewActiveMenu;
  if (m_pActiveMenuStack.IsEmpty()) return;

  // Update the menu stack to the first menu in the new stack.
  UpdateMenuStack(0);
}

void UGeneralFocusManager::Navigate(FVector2D _vAxisValue) {
  // If there are no active menus, do nothing.
  if (m_pActiveMenuStack.IsEmpty()) return;
  // Navigate the current active menu with the given axis value.
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->Navigate(_vAxisValue);
}

void UGeneralFocusManager::ConfirmPressed() {
  // If there are no active menus, do nothing.
  if (m_pActiveMenuStack.IsEmpty()) return;
  // Confirm the current pressed element in the current active menu.
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ConfirmCurrentPressed();
}

void UGeneralFocusManager::ConfirmReleased() {
  // If there are no active menus, do nothing.
  if (m_pActiveMenuStack.IsEmpty()) return;
  // Confirm the current released element in the current active menu.
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ConfirmCurrentReleased();
}

void UGeneralFocusManager::Cancel() {
  // Calculate the new menu index by moving up the stack, ensuring it doesn't go below 0.
  int32 iNewIndex = FMath::Max(m_iCurrentMenuIndex - 1, 0);

  // UpdateMenuStack(iNewIndex);
}

void UGeneralFocusManager::ActivateFocus() {
  // If there are no active menus, do nothing.
  if (m_pActiveMenuStack.IsEmpty()) return;
  // Activate focus on the current active menu.
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ActivateFocus();
}

void UGeneralFocusManager::ResetFocus() {
  // If there are no active menus, do nothing.
  if (m_pActiveMenuStack.IsEmpty()) return;
  // Reset focus on the current active menu.
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->ResetFocus();
}

TObjectPtr<USpecificFocusManager> UGeneralFocusManager::GetActiveMenu() const {
  // Return the current active menu if the stack is not empty; otherwise, return nullptr.
  return m_pActiveMenuStack[m_iCurrentMenuIndex];
}

void UGeneralFocusManager::UpdateMenuStack(int32 _iNewIndex) {
  // If the new index is out of bounds, do nothing.
  if (m_pActiveMenuStack.IsEmpty()) return;
  // Call OnMenuExit on the current active menu if it's valid before changing the index.
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->OnMenuExit();

  // Update the current menu index to the new index.
  m_iCurrentMenuIndex = _iNewIndex;

  // Call OnMenuEnter on the new current active menu if it's valid after changing the index.
  if (IsValid(m_pActiveMenuStack[m_iCurrentMenuIndex])) m_pActiveMenuStack[m_iCurrentMenuIndex]->OnMenuEnter();
}