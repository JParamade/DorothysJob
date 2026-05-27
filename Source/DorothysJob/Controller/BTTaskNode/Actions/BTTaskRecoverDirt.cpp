#include "BTTaskRecoverDirt.h"

UBTTaskRecoverDirt::UBTTaskRecoverDirt()
{
  NodeName = "Recover Dirt";
}

FString UBTTaskRecoverDirt::GetStaticDescription() const
{
  return TEXT("It performs the recover dirt action");
}

void UBTTaskRecoverDirt::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->RecoverDirt();
}

void UBTTaskRecoverDirt::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->CancelAction();
}

FOnEnemyActionComplete& UBTTaskRecoverDirt::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnRecoverDirtComplete;
}