/************************************************************************
 * @description: Represents the manager class for tip handling in the loading screen.
 * @author: Jaime Paramo
 * @date: 13/08/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "TipManager.h"

// Data Table
#include "DorothysJob/Data/DataTables/TipData.h"

void UTipManager::Initialize(UDataTable* _pTipDT) {
  // Store reference to the DataTable containing the tips.
  m_pTipTable = _pTipDT;

  // If DataTable is valid, get all row names.
  if (m_pTipTable) m_pAvailableTips = m_pTipTable->GetRowNames();
}

FText UTipManager::GetNextTip() {
  if (m_pAvailableTips.Num() == 0) {
    // If there are no more tips available, the array is refilled.
    if (m_pTipTable) m_pAvailableTips = m_pTipTable->GetRowNames();
    else return FText::FromString(TEXT("DT_ERROR: DataTable is not valid."));
  }

  // Pick a random tip index from the remaining ones.
  int32 iIndex = FMath::RandRange(0, m_pAvailableTips.Num() - 1);
  FName sRowName = m_pAvailableTips[iIndex];

  // Try to find the tip struct in the DataTable.
  if (FTipData* pTip = m_pTipTable->FindRow<FTipData>(sRowName, TEXT("GetNextTip"))) {
    // Remove this tip from the available list so it won't repeat immediately.
    m_pAvailableTips.RemoveAt(iIndex);
    return pTip->m_sTipText;
  }

  // Fallback if no tip could be found for the given row.
  return FText::FromString(TEXT("TIP_ERROR: No tips available in DataTable."));
}
