/************************************************************************
 * @description: Represents the Dogrothy consumable.
 * @author: Jaime Paramo
 * @date: 29/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "Dogrothy.h"

#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

bool UDogrothy::Use(ABasePlayer* _pPlayer) {
  // Achievement Logic.
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->SetDogsDeployAchievement();;
    }
  }

  return Super::Use(_pPlayer);
}