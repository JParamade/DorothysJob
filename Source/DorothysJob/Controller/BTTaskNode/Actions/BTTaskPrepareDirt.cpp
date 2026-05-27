#include "BTTaskPrepareDirt.h"

UBTTaskPrepareDirt::UBTTaskPrepareDirt()
{
  NodeName = "Prepare Dirt";
}

FString UBTTaskPrepareDirt::GetStaticDescription() const
{
  return TEXT("It performs the prepare dirt action");
}

void UBTTaskPrepareDirt::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->PrepareDirt();
}

void UBTTaskPrepareDirt::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->CancelAction();
}

FOnEnemyActionComplete& UBTTaskPrepareDirt::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnPrepareDirtComplete;
}