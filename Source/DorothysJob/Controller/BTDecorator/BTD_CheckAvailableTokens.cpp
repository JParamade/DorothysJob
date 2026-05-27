#include "BTD_CheckAvailableTokens.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_CheckAvailableTokens::UBTD_CheckAvailableTokens()
{
  NodeName = "CheckAvailableTokens";
  INIT_DECORATOR_NODE_NOTIFY_FLAGS();
  bNotifyDeactivation = true;
}

FString UBTD_CheckAvailableTokens::GetStaticDescription() const
{
  Super::GetStaticDescription();
  return TEXT("Checks if there is any available token");
}

bool UBTD_CheckAvailableTokens::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
  Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

  AAIController* pAIController = OwnerComp.GetAIOwner();
  ABaseEnemy* pAIPawn = IsValid(pAIController) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;

  bool bCanAttack = false, bIsPolluted = false;
  if (UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent())
  {
    bCanAttack = pBlackboardComp->GetValueAsBool(m_tCanAttackKey.SelectedKeyName);
    bIsPolluted = pBlackboardComp->GetValueAsBool(m_tIsPollutedKey.SelectedKeyName);
  }

  return (pAIPawn && bCanAttack && (bIsPolluted || pAIPawn->AreAvailableTokens()));
}

void UBTD_CheckAvailableTokens::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
  Super::OnNodeActivation(SearchData);

  AAIController* pAIController = SearchData.OwnerComp.GetAIOwner();
  ABaseEnemy* pAIPawn = IsValid(pAIController) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;

  bool bIsPolluted = false;
  if (UBlackboardComponent* pBlackboardComp = SearchData.OwnerComp.GetBlackboardComponent())
  {
    bIsPolluted = pBlackboardComp->GetValueAsBool(m_tIsPollutedKey.SelectedKeyName);
  }

  if (pAIPawn && !bIsPolluted)
  {
    pAIPawn->RequestToken();
  }
}

void UBTD_CheckAvailableTokens::OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult)
{
  Super::OnNodeDeactivation(SearchData, NodeResult);

  AAIController* pAIController = SearchData.OwnerComp.GetAIOwner();
  ABaseEnemy* pAIPawn = IsValid(pAIController) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;
  if (pAIPawn)
  {
    pAIPawn->ReturnToken();
  }
}