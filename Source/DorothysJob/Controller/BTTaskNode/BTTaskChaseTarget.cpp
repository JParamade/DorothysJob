#include "BTTaskChaseTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DorothysJob/Utils/GeneralUtils.h"

UBTTaskChaseTarget::UBTTaskChaseTarget()
{
  NodeName = "ChaseTarget";
}

FString UBTTaskChaseTarget::GetStaticDescription() const
{
  return TEXT("It searches a point to move to chase the target");
}

EBTNodeResult::Type UBTTaskChaseTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  EBTNodeResult::Type eReturn = EBTNodeResult::Failed;
  AAIController* pAIController = OwnerComp.GetAIOwner();
  APawn* pAIPawn = IsValid(pAIController) ? pAIController->GetPawn() : nullptr;

  if (IsValid(pAIController) && IsValid(pAIPawn))
  {
    // Blackboard properties
    float fMinRadius = 200.f, fMaxRadius = 350.f;
    bool bShowDebug = false;
    AActor* pCharacter = nullptr;
    if (UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent())
    {
      fMinRadius = pBlackboardComp->GetValueAsFloat(m_tMinRadiusKey.SelectedKeyName);
      fMaxRadius = pBlackboardComp->GetValueAsFloat(m_tMaxRadiusKey.SelectedKeyName);
      pCharacter = Cast<AActor>(pBlackboardComp->GetValueAsObject(m_tEnemyToChase.SelectedKeyName));
      bShowDebug = pBlackboardComp->GetValueAsBool(m_tShowDebugKey.SelectedKeyName);
    }

    FVector vLocationToReach = FVector::ZeroVector;
    bool bResult = false;
    if (IsValid(pCharacter))
    {
      bResult = GeneralUtils::GetStepPathPoint(pAIPawn, fMinRadius, fMaxRadius, pCharacter->GetActorLocation(), vLocationToReach);
    }

    if (bResult)
    {
      OwnerComp.GetBlackboardComponent()->SetValueAsVector(m_tTargetLocationKey.SelectedKeyName, vLocationToReach);
      eReturn = EBTNodeResult::Succeeded;
    }

    // Debug -> It draws min and max radius and location to reach
    if (bShowDebug)
    {
      GeneralUtils::DrawDebugCone(pAIPawn, pAIPawn->GetActorRotation(), fMinRadius, fMaxRadius, 1.f, FColor::Yellow, FColor::Green, 1.5f);
      // Location to reach
      if (eReturn == EBTNodeResult::Succeeded)
      {
        DrawDebugSphere(GetWorld(), vLocationToReach, 25.f, 12, FColor::Blue, false, 1.5f);
      }
    }
  }

  return eReturn;
}