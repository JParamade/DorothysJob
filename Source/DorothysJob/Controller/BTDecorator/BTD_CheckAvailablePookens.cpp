#include "BTD_CheckAvailablePookens.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "DorothysJob/Actor/Character/Enemy/DustBunny.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheckAvailablePookens::UBTD_CheckAvailablePookens()
{
  NodeName = "Check Available Dust Pookens";
  INIT_DECORATOR_NODE_NOTIFY_FLAGS();
  bNotifyDeactivation = true;
}

FString UBTD_CheckAvailablePookens::GetStaticDescription() const
{
  Super::GetStaticDescription();
  return TEXT("Checks if there is any available pooken and the actor isn't near another Dust Poop");
}

bool UBTD_CheckAvailablePookens::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
  Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

  AAIController* pAIController = OwnerComp.GetAIOwner();
  ABaseEnemy* pAIPawn = (pAIController != nullptr) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;

  return (pAIPawn && pAIPawn->GetCurrentRoom() && pAIPawn->GetCurrentRoom()->CanDustPoop(pAIPawn));
}

void UBTD_CheckAvailablePookens::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
  Super::OnNodeActivation(SearchData);

  AAIController* pAIController = SearchData.OwnerComp.GetAIOwner();
  ABaseEnemy* pAIPawn = (pAIController != nullptr) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;

  if (pAIPawn && pAIPawn->GetCurrentRoom())
  {
    pAIPawn->GetCurrentRoom()->RequestDustPooken(pAIPawn);
  }
}

void UBTD_CheckAvailablePookens::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
  Super::OnNodeDeactivation(SearchData, NodeResult);

  AAIController* pAIController = SearchData.OwnerComp.GetAIOwner();
  ABaseEnemy* pAIPawn = (pAIController != nullptr) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;

  if (pAIPawn && pAIPawn->GetCurrentRoom())
  {
    pAIPawn->GetCurrentRoom()->ReturnDustPooken(pAIPawn);
  }
}
