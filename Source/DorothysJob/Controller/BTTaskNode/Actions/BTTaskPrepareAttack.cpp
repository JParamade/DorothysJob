#include "BTTaskPrepareAttack.h"

UBTTaskPrepareAttack::UBTTaskPrepareAttack()
{
  NodeName = "Prepare Attack";
}

FString UBTTaskPrepareAttack::GetStaticDescription() const
{
  return TEXT("It performs the prepare attack action");
}

void UBTTaskPrepareAttack::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->PrepareAttack();
}

void UBTTaskPrepareAttack::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->CancelAction();
}

FOnEnemyActionComplete& UBTTaskPrepareAttack::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnPrepareAttackComplete;
}