#include "Goblin.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DorothysJob/Data/DataAssets/Enemy/GrimelingPropertiesDataAsset.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Utils/Structs/CharactersByDistance.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"
#include "DorothysJob/Manager/BaseGameInstance.h"

AGoblin::AGoblin()
  : ABaseEnemy()
{
  // Capsule
  UpdateCapsule(50.f);
}

void AGoblin::BeginPlay()
{
  Super::BeginPlay();

  AAIController* pAIController = Cast<AAIController>(GetController());
  UGrimelingPropertiesDataAsset* pGrimelingPropertiesDA = Cast<UGrimelingPropertiesDataAsset>(m_pCharacterDataAsset);
  if (IsValid(pAIController) && IsValid(pGrimelingPropertiesDA))
  {
    if (UBlackboardComponent* pBlackboard = pAIController->GetBlackboardComponent())
    {
      // Flee properties
      pBlackboard->SetValueAsFloat("FleeDistance", pGrimelingPropertiesDA->m_fFleeDistance);
      pBlackboard->SetValueAsFloat("FleeAngle", pGrimelingPropertiesDA->m_fFleeAngle);
      pBlackboard->SetValueAsFloat("FleeBounceAngle", pGrimelingPropertiesDA->m_fFleeBounceAngle);
      pBlackboard->SetValueAsFloat("FleeForbiddenBounceAngle", pGrimelingPropertiesDA->m_fFleeForbiddenBounceAngle);
      pBlackboard->SetValueAsFloat("SpeedUpCooldown", pGrimelingPropertiesDA->m_fSpeedUpCooldown);
      // Brave properties
      pBlackboard->SetValueAsBool("BraveMode", true);
    }
  }

  m_bHasBufEnemies = false;
}

#pragma region | Enemy Manager
bool AGoblin::AreAvailableTokens()
{
  if (UBaseRoom* pMyRoom = GetCurrentRoom())
  {
    return pMyRoom->AreAvailableGrimelingTokens();
  }
  return false;
}

bool AGoblin::RequestToken()
{
  if (UBaseRoom* pMyRoom = GetCurrentRoom())
  {
    return pMyRoom->RequestGrimelingToken(this);
  }
  return false;
}

bool AGoblin::ReturnToken()
{
  if (UBaseRoom* pMyRoom = GetCurrentRoom())
  {
    return pMyRoom->ReturnGrimelingToken(this);
  }
  return false;
}
#pragma endregion

#pragma region | Actions
void AGoblin::PerformAttack(int32 _iCurrentCombo)
{
  Super::PerformAttack(_iCurrentCombo);

  FCharactersByDistance tCharacters = m_pCurrentRoom->GetCharactersByDistance(this, m_fDetectRadius, { AGoblin::StaticClass() });
  for (TWeakObjectPtr<ABaseEnemy> pEnemy : tCharacters.m_lCloseEnemies)
  {
    pEnemy->BePolluted();
    m_bHasBufEnemies = true;
  }

  ABasePlayer* pPlayer = tCharacters.m_pPlayer;
  if (IsValid(pPlayer))
  {
    pPlayer->BePolluted();
  }

  if (GetWorldTimerManager().IsTimerActive(m_tBraveTimerHandle))
  {
    GetWorldTimerManager().ClearTimer(m_tBraveTimerHandle);
  }
  AAIController* pAIController = Cast<AAIController>(GetController());
  UGrimelingPropertiesDataAsset* pGrimelingPropertiesDA = Cast<UGrimelingPropertiesDataAsset>(m_pCharacterDataAsset);
  if (IsValid(pAIController) && IsValid(pGrimelingPropertiesDA))
  {
    TWeakObjectPtr<UBlackboardComponent> pBlackboardComp = pAIController->GetBlackboardComponent();
    if (pBlackboardComp.IsValid())
    {
      pBlackboardComp->SetValueAsBool("BraveMode", false);
      GetWorldTimerManager().SetTimer(m_tBraveTimerHandle,
        [pBlackboardComp]() {
          if (pBlackboardComp.IsValid())
          {
            pBlackboardComp->SetValueAsBool("BraveMode", true);
          }
        },
        pGrimelingPropertiesDA->m_fAttackCooldown, false);
    }
  }
}
void AGoblin::GameOver()
{
  // Obtener subsistema de logros una sola vez
  UAchievementSubsystem* AchSubsystem = nullptr;
  if (UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    AchSubsystem = GameInstance->GetSubsystem<UAchievementSubsystem>();
    if (!AchSubsystem)
    {
      return;
    }
  }

  AchSubsystem->AddProgress("ImpeccablePerformance", 1);

  if (!m_bHasBufEnemies)
  {
    AchSubsystem->UnlockAchievement("FasterThanYouStink");
  }

  
  Super::GameOver();
}
#pragma endregion