/************************************************************************
 * @description: Represents the base consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "BaseConsumable.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

bool UBaseConsumable::Use(ABasePlayer* _pPlayer) {
  if (!m_bAchievementManager)
  {
    //m_bHasConsumableBeenUse = true;
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        m_bAchievementManager = true;
        if (!pAchSubsystem->m_mGottaWashEmAll.FindRef(GetFName(), false))
        {
          pAchSubsystem->AddProgress("GottaWashEmAll", 1);
          pAchSubsystem->m_mGottaWashEmAll.Add(GetFName(), true);
        }
      }
    }
  }

  if (m_uStackQuantity <= 0) {
    // Add feedback.

    return false;
  }

  m_uStackQuantity--;

  return true;
}