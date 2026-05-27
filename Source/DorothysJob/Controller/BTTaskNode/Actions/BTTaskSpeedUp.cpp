#include "BTTaskSpeedUp.h"

UBTTaskSpeedUp::UBTTaskSpeedUp()
{
  NodeName = "Speed Up";
}

FString UBTTaskSpeedUp::GetStaticDescription() const
{
  return TEXT("It performs speed up action");
}

void UBTTaskSpeedUp::StartEnemyAction(ABaseEnemy* _pEnemy)
{
  _pEnemy->SpeedUp();
}

void UBTTaskSpeedUp::CancelEnemyAction(ABaseEnemy* _pEnemy)
{
  
}

FOnEnemyActionComplete& UBTTaskSpeedUp::GetCompletionDelegate(ABaseEnemy* _pEnemy)
{
  return _pEnemy->OnSpeedUpComplete;
}