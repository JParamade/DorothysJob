#include "BTTaskRotateToFaceTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"

UBTTaskRotateToFaceTarget::UBTTaskRotateToFaceTarget()
{
  NodeName = "RotateToFaceTarget";
  bNotifyTick = true;
}

FString UBTTaskRotateToFaceTarget::GetStaticDescription() const
{
  return TEXT("It rotates the AI Controlled Pawn to face the target");
}

EBTNodeResult::Type UBTTaskRotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  return EBTNodeResult::InProgress;
}

void UBTTaskRotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  AAIController* pAIController = OwnerComp.GetAIOwner();
  APawn* pAIPawn = (pAIController != nullptr) ? pAIController->GetPawn() : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();
  if (!IsValid(pAIController) || !IsValid(pAIPawn) || !IsValid(pBlackboardComp))
  {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    return;
  }

  float fRotationInterpSpeed = pBlackboardComp->GetValueAsFloat(m_tRotationInterpSpeedKey.SelectedKeyName);
  float fAcceptableRadius = pBlackboardComp->GetValueAsFloat(m_tAcceptableRadiusKey.SelectedKeyName);
  AActor* pTargetToFace = Cast<AActor>(pBlackboardComp->GetValueAsObject(m_tTargetToFaceKey.SelectedKeyName));
  if (!IsValid(pTargetToFace))
  {
    FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    return;
  }

  FVector vTargetLocation = pTargetToFace->GetActorLocation();
  FRotator CurrentRotation = pAIPawn->GetActorRotation();
  FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(pAIPawn->GetActorLocation(), vTargetLocation);
  FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, DeltaSeconds, fRotationInterpSpeed);
  pAIPawn->SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));
  if (FMath::Abs((NewRotation - DesiredRotation).Yaw) < fAcceptableRadius)
  {
    FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
  }
}