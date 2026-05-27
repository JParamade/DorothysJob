#include "BTD_OrCondition.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTD_OrCondition::UBTD_OrCondition()
{
  NodeName = "Or Condition";
}

FString UBTD_OrCondition::GetStaticDescription() const
{
  Super::GetStaticDescription();

  return TEXT("It checks if any parameter is true or not");
}

bool UBTD_OrCondition::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
  bool bFirstCondition = false, bSecondCOndition = false;
  if (UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent())
  {
    bFirstCondition = pBlackboardComp->GetValueAsBool(m_tFirstConditionKey.SelectedKeyName);
    bSecondCOndition = pBlackboardComp->GetValueAsBool(m_tSecondConditionKey.SelectedKeyName);
  }

  return (bFirstCondition || bSecondCOndition);
}