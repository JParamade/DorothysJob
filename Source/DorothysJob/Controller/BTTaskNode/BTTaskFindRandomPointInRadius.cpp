#include "BTTaskFindRandomPointInRadius.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Utils/GeneralUtils.h"

UBTTaskFindRandomPointInRadius::UBTTaskFindRandomPointInRadius()
{
  NodeName = "FindRandomPointInRadius";
}

FString UBTTaskFindRandomPointInRadius::GetStaticDescription() const
{
  return TEXT("It finds a random reachable point between min and max radius");
}

EBTNodeResult::Type UBTTaskFindRandomPointInRadius::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  EBTNodeResult::Type eReturn = EBTNodeResult::Failed;
  AAIController* pAIController = OwnerComp.GetAIOwner();
  APawn* pAIPawn = IsValid(pAIController) ? pAIController->GetPawn() : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();

  if ((!IsValid(pAIController)) || (!IsValid(pAIPawn)) || (!IsValid(pBlackboardComp)))
  {
    return eReturn;
  }

  // Blackboard properties
  float fMinRadius = pBlackboardComp->GetValueAsFloat(m_tMinRadiusKey.SelectedKeyName);
  float fMaxRadius = pBlackboardComp->GetValueAsFloat(m_tMaxRadiusKey.SelectedKeyName);
  bool bShowDebug = pBlackboardComp->GetValueAsBool(m_tShowDebugKey.SelectedKeyName);
  float fWanderAngle = pBlackboardComp->GetValueAsFloat(m_tWanderAngleKey.SelectedKeyName);
  float fWanderBounceAngle = pBlackboardComp->GetValueAsFloat(m_tWanderBounceAngleKey.SelectedKeyName);
  int32 iMaxTries = pBlackboardComp->GetValueAsInt(m_tMaxTriesKey.SelectedKeyName);
  int32 iBounceMaxTries = pBlackboardComp->GetValueAsInt(m_tBounceMaxTriesKey.SelectedKeyName);

  FVector vLocationToReach = FVector::ZeroVector;
  bool bResult = GeneralUtils::GetRandomPointInCone(pAIPawn, fMinRadius, fMaxRadius, fWanderAngle, iMaxTries, pAIPawn->GetActorRotation(), vLocationToReach);
  // If it fails, it will try the with the bounce angle
  if (!bResult)
  {
    bResult = GeneralUtils::GetRandomPointInCone(pAIPawn, fMinRadius, fMaxRadius, fWanderBounceAngle, iBounceMaxTries, pAIPawn->GetActorRotation() + FRotator(0.f, 180, 0.f), vLocationToReach);
  }

  if (bResult)
  {
    pBlackboardComp->SetValueAsVector(m_tTargetLocationKey.SelectedKeyName, vLocationToReach);
    eReturn = EBTNodeResult::Succeeded;
  }

  // Debug -> It draws min and max radius and location to reach
  if (bShowDebug)
  {
    GeneralUtils::DrawDebugCone(pAIPawn, pAIPawn->GetActorRotation(), fMinRadius, fMaxRadius, fWanderAngle, FColor::Yellow, FColor::Green, 1.5f);
    GeneralUtils::DrawDebugCone(pAIPawn, pAIPawn->GetActorRotation() + FRotator(0.f, 180, 0.f), fMinRadius, fMaxRadius, fWanderBounceAngle, FColor::Yellow, FColor::Green, 1.5f);

    // Location to reach
    if (eReturn == EBTNodeResult::Succeeded)
    {
      DrawDebugSphere(GetWorld(), vLocationToReach, 25.f, 12, FColor::Blue, false, 1.5f);
    }
  }

  return eReturn;
}