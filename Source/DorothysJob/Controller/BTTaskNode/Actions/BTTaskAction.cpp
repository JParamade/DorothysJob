#include "BTTaskAction.h"
#include "AIController.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"

UBTTaskAction::UBTTaskAction()
{
  NodeName = "Action";
}

FString UBTTaskAction::GetStaticDescription() const
{
  return TEXT("It performs an action");
}

EBTNodeResult::Type UBTTaskAction::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  EBTNodeResult::Type eReturn = EBTNodeResult::Failed;
  AAIController* pAIController = OwnerComp.GetAIOwner();
  ABaseEnemy* pAIEnemy = IsValid(pAIController) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;

  if (IsValid(pAIController) && IsValid(pAIEnemy))
  {
    GetCompletionDelegate(pAIEnemy).AddDynamic(this, &UBTTaskAction::OnActionCompleted);
    StartEnemyAction(pAIEnemy);
    eReturn = EBTNodeResult::InProgress;
  }

  return eReturn;
}

EBTNodeResult::Type UBTTaskAction::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  AAIController* pAIController = OwnerComp.GetAIOwner();
  ABaseEnemy* pAIEnemy = IsValid(pAIController) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;

  if (IsValid(pAIEnemy))
  {
    GetCompletionDelegate(pAIEnemy).RemoveDynamic(this, &UBTTaskAction::OnActionCompleted);
    CancelEnemyAction(pAIEnemy);
  }

  return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTaskAction::OnActionCompleted(ABaseEnemy* _pEnemy)
{
  AAIController* pAIController = IsValid(_pEnemy) ? Cast<AAIController>(_pEnemy->GetController()) : nullptr;
  UBehaviorTreeComponent* pBehaviorComp = IsValid(pAIController) ? Cast<UBehaviorTreeComponent>(pAIController->BrainComponent) : nullptr;

  if (IsValid(_pEnemy) && IsValid(pBehaviorComp))
  {
    GetCompletionDelegate(_pEnemy).RemoveDynamic(this, &UBTTaskAction::OnActionCompleted);
    FinishLatentTask(*pBehaviorComp, EBTNodeResult::Succeeded);
  }
}