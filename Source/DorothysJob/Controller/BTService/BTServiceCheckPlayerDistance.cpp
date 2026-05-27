#include "BTServiceCheckPlayerDistance.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"

UBTServiceCheckPlayerDistance::UBTServiceCheckPlayerDistance()
{
  NodeName = "CheckPlayerDistance";
  bNotifyTick = true;
}

FString UBTServiceCheckPlayerDistance::GetStaticDescription() const
{
  return TEXT("Service to check the distance from the player and decide to attack or flee");
}

void UBTServiceCheckPlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

  AAIController* pAIController = OwnerComp.GetAIOwner();
  ABaseEnemy* pEnemy = IsValid(pAIController) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();

  if (IsValid(pAIController) && IsValid(pEnemy) && IsValid(pBlackboardComp))
  {
    AActor* pPlayerPawn = IsValid(pEnemy->GetCurrentRoom()) ? pEnemy->GetCurrentRoom()->GetTarget(pEnemy) : UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (IsValid(pPlayerPawn))
    {
      FVector vActorLocation = pEnemy->GetActorLocation();
      float fDistance = FVector::Dist(vActorLocation, pPlayerPawn->GetActorLocation());
      float fAttackRadius = pBlackboardComp->GetValueAsFloat(m_tAttackRadiusKey.SelectedKeyName);
      float fFleeRadius = pBlackboardComp->GetValueAsFloat(m_tFleeRadiusKey.SelectedKeyName);
      float fLastFleeDate = pBlackboardComp->GetValueAsFloat(m_tLastFleeDateKey.SelectedKeyName);
      float fBraveDuration = pBlackboardComp->GetValueAsFloat(m_tBraveDurationKey.SelectedKeyName);

      bool bCanFlee = (fLastFleeDate < 0.f) || ((pEnemy->GetWorld()->GetTimeSeconds() - fLastFleeDate) > fBraveDuration);

      pBlackboardComp->SetValueAsBool(m_tShouldAttackKey.SelectedKeyName, fDistance <= fAttackRadius);
      pBlackboardComp->SetValueAsBool(m_tShouldFleeKey.SelectedKeyName, bCanFlee && (fDistance <= fFleeRadius));
      pBlackboardComp->SetValueAsObject(m_tTargetToAttackKey.SelectedKeyName, pPlayerPawn);
    }
  }
}
