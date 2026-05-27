#include "BTTaskToDirt.h"

UBTTaskToDirt::UBTTaskToDirt()
{
  NodeName = "To Dirt";
}

FString UBTTaskToDirt::GetStaticDescription() const
{
  return TEXT("It performs to dirt action");
}

void UBTTaskToDirt::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->ToDirt();
}

void UBTTaskToDirt::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->CancelAction();
}

FOnEnemyActionComplete& UBTTaskToDirt::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnToDirtComplete;
}