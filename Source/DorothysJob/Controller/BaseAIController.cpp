#include "BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"

ABaseAIController::ABaseAIController()
{
  // Blackboard Component
  m_pBlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
}

void ABaseAIController::BeginPlay()
{
  Super::BeginPlay();
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
  Super::OnPossess(InPawn);

  ABaseEnemy* pEnemy = Cast<ABaseEnemy>(InPawn);
  if (IsValid(pEnemy) && IsValid(pEnemy->GetBehaviorTree()))
  {
    if (UseBlackboard(pEnemy->GetBehaviorTree()->BlackboardAsset, m_pBlackboardComponent))
    {
      RunBehaviorTree(pEnemy->GetBehaviorTree());
    }
  }
}