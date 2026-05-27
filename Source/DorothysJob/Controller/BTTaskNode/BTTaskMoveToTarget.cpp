#include "BTTaskMoveToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Navigation/PathFollowingComponent.h"

UBTTaskMoveToTarget::UBTTaskMoveToTarget()
{
  NodeName = "MoveToTarget";
  bNotifyTick = true;
  bNotifyTaskFinished = true;
}

FString UBTTaskMoveToTarget::GetStaticDescription() const
{
  return TEXT("It moves the AI Controlled Pawn to the target location");
}

EBTNodeResult::Type UBTTaskMoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  EBTNodeResult::Type eReturn = EBTNodeResult::Failed;
  AAIController* pAIController = OwnerComp.GetAIOwner();
  APawn* pAIPawn = (pAIController != nullptr) ? pAIController->GetPawn() : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();

  if (IsValid(pAIController) && IsValid(pAIPawn) && IsValid(pBlackboardComp))
  {
    FVector vTargetLocation = pBlackboardComp->GetValueAsVector(m_tTargetLocationKey.SelectedKeyName);
    float fAcceptableRadius = pBlackboardComp->GetValueAsFloat(m_tAcceptableRadiusKey.SelectedKeyName);

    FAIMoveRequest tMoveReq(vTargetLocation);
    tMoveReq.SetUsePathfinding(true);
    tMoveReq.SetAllowPartialPath(true);
    tMoveReq.SetProjectGoalLocation(true);
    tMoveReq.SetNavigationFilter(pAIController->GetDefaultNavigationFilterClass());
    tMoveReq.SetAcceptanceRadius(fAcceptableRadius);
    tMoveReq.SetReachTestIncludesAgentRadius(false);
    tMoveReq.SetReachTestIncludesGoalRadius(false);
    tMoveReq.SetCanStrafe(true);

    EPathFollowingRequestResult::Type MoveResult =  pAIController->MoveTo(tMoveReq);
    eReturn = (MoveResult != EPathFollowingRequestResult::Failed) ? EBTNodeResult::InProgress : EBTNodeResult::Failed;
  }

  return eReturn;
}

void UBTTaskMoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  AAIController* pAIController = OwnerComp.GetAIOwner();
  APawn* pAIPawn = (pAIController != nullptr) ? pAIController->GetPawn() : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();

  if (IsValid(pAIController) && IsValid(pAIPawn) && IsValid(pBlackboardComp))
  {
    if (UPawnMovementComponent* oMovementComponent = pAIPawn->GetMovementComponent())
    {
      FVector vVelocity = oMovementComponent->Velocity;
      if (!vVelocity.IsNearlyZero())
      {
        float fRotationInterpSpeed = pBlackboardComp->GetValueAsFloat(m_tRotationInterpSpeedKey.SelectedKeyName);
        FRotator NewRotation = FMath::RInterpTo(pAIPawn->GetActorRotation(), vVelocity.Rotation(), DeltaSeconds, fRotationInterpSpeed);
        pAIPawn->SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
      }
    }

    EPathFollowingStatus::Type Status = pAIController->GetPathFollowingComponent()->GetStatus();

    if (Status == EPathFollowingStatus::Idle)
    {
      if (pAIController->GetMoveStatus() == EPathFollowingStatus::Idle && pAIController->GetPathFollowingComponent()->DidMoveReachGoal())
      {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
      }
      else
      {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
      }
    }
  }
  else
  {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
  }
}

void UBTTaskMoveToTarget::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
  Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);

  AAIController* pAIController = OwnerComp.GetAIOwner();
  if (IsValid(pAIController))
  {
    pAIController->StopMovement();
  }
}
