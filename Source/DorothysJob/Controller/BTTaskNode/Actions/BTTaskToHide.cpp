#include "BTTaskToHide.h"

UBTTaskToHide::UBTTaskToHide()
{
  NodeName = "Hide";
}

FString UBTTaskToHide::GetStaticDescription() const
{
  return TEXT("It performs hide action");
}

void UBTTaskToHide::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->ToHide();
}

void UBTTaskToHide::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->CancelAction();
}

FOnEnemyActionComplete& UBTTaskToHide::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnToHideComplete;
}