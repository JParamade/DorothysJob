#include "BTD_CheckAvailablePoop.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Actor/Dust/DustPoop.h"
#include "DorothysJob/Actor/Character/Enemy/DustBunny.h"

UBTD_CheckAvailablePoop::UBTD_CheckAvailablePoop()
{
  NodeName = "Check Available Poop";
}

FString UBTD_CheckAvailablePoop::GetStaticDescription() const
{
  Super::GetStaticDescription();

  return TEXT("It checks if there is any available dust poop to hide");
}

bool UBTD_CheckAvailablePoop::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
  bool bReturn = false;

  AAIController* pAIController = OwnerComp.GetAIOwner();
  ABaseEnemy* pEnemy = IsValid(pAIController) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();

  if ((!IsValid(pAIController)) || (!IsValid(pEnemy)) || (!IsValid(pBlackboardComp)))
  {
    return bReturn;
  }

  ADustBunny* pDustBunny = Cast<ADustBunny>(pEnemy);
  if (pDustBunny && pDustBunny->IsTowerModeAvailable())
  {
    ADustPoop* pDustPoop = Cast<ADustPoop>(pBlackboardComp->GetValueAsObject(m_tDustPoopActorKey.SelectedKeyName));
    if ((!pDustBunny->HasEnemyBeenHiddenAnyTime()) && (!IsValid(pDustPoop)) && pDustBunny->GetCurrentRoom())
    {
      pDustPoop = pDustBunny->GetCurrentRoom()->RequestPoop(pDustBunny);
    }

    if (IsValid(pDustPoop) && IsValid(pDustBunny))
    {
      pDustPoop->SetDustBunnyHidden(pDustBunny);
      pBlackboardComp->SetValueAsVector(m_tTargetLocationKey.SelectedKeyName, pDustPoop->GetActorLocation());
      bReturn = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
    }
  }

  return bReturn;
}