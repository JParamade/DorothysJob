#include "BTTaskFlee.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Utils/GeneralUtils.h"

UBTTaskFlee::UBTTaskFlee()
{
  NodeName = "Flee";
}

FString UBTTaskFlee::GetStaticDescription() const
{
  return TEXT("It flees from the player to a safe distance");
}

EBTNodeResult::Type UBTTaskFlee::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
  EBTNodeResult::Type eReturn = EBTNodeResult::Failed;
  AAIController* pAIController = OwnerComp.GetAIOwner();
  APawn* pAIPawn = (pAIController != nullptr) ? pAIController->GetPawn() : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();
  if (!IsValid(pAIController) || !IsValid(pAIPawn) || !IsValid(pBlackboardComp))
  {
    return eReturn;
  }

  // Blackboard properties
  bool bShowDebug = pBlackboardComp->GetValueAsBool(m_tShowDebugKey.SelectedKeyName);
  float fFleeDistance = pBlackboardComp->GetValueAsFloat(m_tFleeDistanceKey.SelectedKeyName);
  float fFleeAngle = pBlackboardComp->GetValueAsFloat(m_tFleeAngleKey.SelectedKeyName);
  float fFleeBounceAngle = pBlackboardComp->GetValueAsFloat(m_tFleeBounceAngleKey.SelectedKeyName);
  float fFleeForbiddenBounceAngle = pBlackboardComp->GetValueAsFloat(m_tFleeForbiddenBounceAngleKey.SelectedKeyName);
  int32 iMaxTries = pBlackboardComp->GetValueAsInt(m_tMaxTriesKey.SelectedKeyName);
  int32 iBounceMaxTries = pBlackboardComp->GetValueAsInt(m_tBounceMaxTriesKey.SelectedKeyName);
  AActor* pTargetFromFlee = Cast<AActor>(pBlackboardComp->GetValueAsObject(m_tTargetFromFleeKey.SelectedKeyName));
  if (!IsValid(pTargetFromFlee))
  {
    return eReturn;
  }

  FVector vActorLocation = pAIPawn->GetActorLocation();
  FVector vDirection = vActorLocation - pTargetFromFlee->GetActorLocation();
  vDirection.Z = 0.f;
  vDirection = vDirection.GetSafeNormal();
  FVector vLocationToReach = FVector::ZeroVector;
  bool bResult = GeneralUtils::GetRandomPointInCone(pAIPawn, fFleeDistance, fFleeDistance, fFleeAngle, iMaxTries, vDirection.Rotation(), vLocationToReach);
  // If it fails, it will try the with the bounce angle
  if (!bResult)
  {
    bResult = GeneralUtils::GetRandomPointInCone(pAIPawn, fFleeDistance, fFleeDistance, fFleeBounceAngle, iBounceMaxTries, vDirection.Rotation() + FRotator(0.f, 180, 0.f), vLocationToReach, fFleeForbiddenBounceAngle);
    if (bResult)
    {
      // Flee cooldown init
      pBlackboardComp->SetValueAsFloat(m_tLastFleeDateKey.SelectedKeyName, pAIPawn->GetWorld()->GetTimeSeconds());
    }
  }

  if (bResult)
  {
    pBlackboardComp->SetValueAsVector(m_tTargetLocationKey.SelectedKeyName, vLocationToReach);
    eReturn = EBTNodeResult::Succeeded;
  }

  // Debug -> It draws min and max radius and location to reach
  if (bShowDebug)
  {
    GeneralUtils::DrawDebugCone(pAIPawn, vDirection.Rotation(), fFleeDistance, fFleeDistance, fFleeAngle, FColor::Magenta, FColor::Magenta, 1.5f);
    GeneralUtils::DrawDebugCone(pAIPawn, vDirection.Rotation() + FRotator(0.f, 180, 0.f), fFleeDistance, fFleeDistance, fFleeBounceAngle, FColor::Magenta, FColor::Magenta, 1.5f, fFleeForbiddenBounceAngle);

    // Location to reach
    if (eReturn == EBTNodeResult::Succeeded)
    {
      DrawDebugSphere(GetWorld(), vLocationToReach, 25.f, 12, FColor::Magenta, false, 1.5f);
    }
  }
  return eReturn;
}