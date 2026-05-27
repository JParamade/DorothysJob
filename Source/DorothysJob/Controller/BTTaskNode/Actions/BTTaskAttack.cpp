#include "BTTaskAttack.h"

UBTTaskAttack::UBTTaskAttack()
{
  NodeName = "Attack";
}

FString UBTTaskAttack::GetStaticDescription() const
{
  return TEXT("It performs attack action");
}

void UBTTaskAttack::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->Attack();
}

void UBTTaskAttack::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->CancelAction();
}

FOnEnemyActionComplete& UBTTaskAttack::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnAttackComplete;
}