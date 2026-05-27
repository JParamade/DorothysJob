#include "BTTaskRecoverAttack.h"

UBTTaskRecoverAttack::UBTTaskRecoverAttack()
{
  NodeName = "Recover Attack";
}

FString UBTTaskRecoverAttack::GetStaticDescription() const
{
  return TEXT("It performs the recover attack action");
}

void UBTTaskRecoverAttack::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->RecoverAttack();
}

void UBTTaskRecoverAttack::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->CancelAction();
}

FOnEnemyActionComplete& UBTTaskRecoverAttack::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnRecoverAttackComplete;
}