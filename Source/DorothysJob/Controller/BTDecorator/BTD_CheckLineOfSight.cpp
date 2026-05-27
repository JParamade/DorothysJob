#include "BTD_CheckLineOfSight.h"
#include "AIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Dust/DustPoop.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"

UBTD_CheckLineOfSight::UBTD_CheckLineOfSight()
{
  NodeName = "Check Line Of Sight to Player";
}

FString UBTD_CheckLineOfSight::GetStaticDescription() const
{
  Super::GetStaticDescription();

  return TEXT("Checks if there is some obstacle between enemy and player using a line trace");
}

bool UBTD_CheckLineOfSight::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
  AAIController* pAIController = OwnerComp.GetAIOwner();
  ABaseEnemy* pAIPawn = (pAIController != nullptr) ? Cast<ABaseEnemy>(pAIController->GetPawn()) : nullptr;
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();
  bool bHit = false;

  AActor* pPlayerPawn = nullptr;
  if (IsValid(pBlackboardComp))
  {
    pPlayerPawn = Cast<AActor>(pBlackboardComp->GetValueAsObject(m_tEnemyToSee.SelectedKeyName));
  }

  if (IsValid(pAIPawn) && IsValid(pPlayerPawn))
  {
    FVector vStart = pAIPawn->GetActorLocation();
    FVector vEnd = pPlayerPawn->GetActorLocation();
    TArray<FHitResult> lHits;
    FCollisionQueryParams CollParams;
    CollParams.AddIgnoredActor(pAIPawn);
    CollParams.AddIgnoredActor(pPlayerPawn);
    pAIPawn->GetWorld()->LineTraceMultiByChannel(lHits, vStart, vEnd, ECollisionChannel::ECC_Visibility, CollParams);
    int32 iHitsSize = lHits.Num();
    for (FHitResult tHit : lHits)
    {
      if (tHit.GetActor() && tHit.GetActor()->IsA(ADustPoop::StaticClass()))
      {
        --iHitsSize;
      }
    }
    bHit = (iHitsSize == 0);
  }
  else
  {
    UE_LOG(LogBehaviorTree, Error, TEXT("Enemy(self) or Player is null"))
  }

  return bHit;
}