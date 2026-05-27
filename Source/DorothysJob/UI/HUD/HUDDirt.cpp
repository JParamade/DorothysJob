/************************************************************************
 * @description: Represents the dirt meter widget.
 * @author: Jaime Paramo
 * @date: 13/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "HUDDirt.h"

// Component
#include "Components/Image.h"
#include "Components/ProgressBar.h"

// Gameplay
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"

void UHUDDirt::Show() {
  Super::Show();

  // Bind to the Level Manager's OnPercentageChange event to react when the dirt percentage is updated.
  if (ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>()) {
    //m_oPercentageChangeHandle = pLevelManager->OnPercentageChange.AddUObject(this, &UHUDDirt::UpdateDirtBar);
  }
}

void UHUDDirt::Hide() {
  Super::Hide();

  // Remove event bindings.
  if (ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>()) {
    //pLevelManager->OnPercentageChange.Remove(m_oPercentageChangeHandle);
  }
}

void UHUDDirt::NativeConstruct() {
  Super::NativeConstruct();

  // Dirt Progress Bar
  if (IsValid(m_pDirtBar)) m_lElements.Add(m_pDirtBar);
  if (IsValid(m_pDirtBarImage)) m_lElements.Add(m_pDirtBarImage);
  
  // Separator Bars
  if (IsValid(m_pSeparatorBar1)) m_lElements.Add(m_pSeparatorBar1);
  if (IsValid(m_pSeparatorBar2)) m_lElements.Add(m_pSeparatorBar2);
  if (IsValid(m_pSeparatorBar3)) m_lElements.Add(m_pSeparatorBar3);
  if (IsValid(m_pSeparatorBar4)) m_lElements.Add(m_pSeparatorBar4);
  if (IsValid(m_pSeparatorBar5)) m_lElements.Add(m_pSeparatorBar5);
  if (IsValid(m_pSeparatorBar6)) m_lElements.Add(m_pSeparatorBar6);
  if (IsValid(m_pSeparatorBar7)) m_lElements.Add(m_pSeparatorBar7);
  if (IsValid(m_pSeparatorBar8)) m_lElements.Add(m_pSeparatorBar8);
  if (IsValid(m_pSeparatorBar9)) m_lElements.Add(m_pSeparatorBar9);
}

void UHUDDirt::UpdateDirtBar(float _iNewDirtValue) {
  // The value is normalized to a percentage (0.0 to 1.0).
  // It is also multiplied by 3.33, which represents the estimated maximum playable floor area (excluding space covered by furniture).
  m_pDirtBar->SetPercent(((_iNewDirtValue / 100) * 3.33f));
}