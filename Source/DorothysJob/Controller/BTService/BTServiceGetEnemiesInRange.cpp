#include "BTServiceGetEnemiesInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DorothysJob/Actor/Character/Enemy/Goblin.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Utils/Structs/CharactersByDistance.h"

UBTServiceGetEnemiesInRange::UBTServiceGetEnemiesInRange()
{
  NodeName = "GetEnemiesInRange";
  bNotifyTick = true;
}

FString UBTServiceGetEnemiesInRange::GetStaticDescription() const
{
  return TEXT("Service to check if there are enemies in attack range");
}

void UBTServiceGetEnemiesInRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
  Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

  AAIController* pAIController = OwnerComp.GetAIOwner();
  APawn* pAIPawn = IsValid(pAIController) ? pAIController->GetPawn() : nullptr;
  AGoblin* pGoblin = Cast<AGoblin>(pAIPawn);
  UBlackboardComponent* pBlackboardComp = OwnerComp.GetBlackboardComponent();

  if (IsValid(pAIController) && IsValid(pGoblin) && IsValid(pBlackboardComp) && IsValid(pGoblin->GetCurrentRoom()))
  {
    float fDetectRadius = pBlackboardComp->GetValueAsFloat(m_tDetectRadiusKey.SelectedKeyName);
    float fFleeRadius = pBlackboardComp->GetValueAsFloat(m_tFleeRadiusKey.SelectedKeyName);
    float fLastFleeDate = pBlackboardComp->GetValueAsFloat(m_tLastFleeDateKey.SelectedKeyName);
    bool bBraveMode = pBlackboardComp->GetValueAsBool(m_tBraveModeKey.SelectedKeyName);
    float fBraveDuration = pBlackboardComp->GetValueAsFloat(m_tBraveDurationKey.SelectedKeyName);

    FCharactersByDistance tCharacters = pGoblin->GetCurrentRoom()->GetCharactersByDistance(pGoblin, fDetectRadius, { AGoblin::StaticClass() });
    // Characters in range including the player. It is necessary 2 or more characters to attack or when there is only 1 enemy in the room
    int32 iCharactersInRange = tCharacters.m_lCloseEnemies.Num() + (IsValid(tCharacters.m_pPlayer) ? 1 : 0);
    bool bCharactersInRange = (iCharactersInRange >= 2) || ((iCharactersInRange == 1) && tCharacters.m_lFarEnemies.IsEmpty());
    // The enemy to chase will be always the first enemy, if exists, that isn't in range.
    ABaseEnemy* pEnemyToChase = nullptr;
    if (bBraveMode)
    {
      pEnemyToChase = Cast<ABaseEnemy>(pBlackboardComp->GetValueAsObject(m_tEnemyToChaseKey.SelectedKeyName));
      if (!IsValid(pEnemyToChase) || (FVector::Dist(pGoblin->GetActorLocation(), pEnemyToChase->GetActorLocation()) <= fFleeRadius))
      {
        pEnemyToChase = (!tCharacters.m_lFarEnemies.IsEmpty()) ? tCharacters.m_lFarEnemies[0].Get() : nullptr;
      }
    }
    // The enemy can flee from the player
    bool bCanFlee = (fLastFleeDate < 0.f) || ((pAIPawn->GetWorld()->GetTimeSeconds() - fLastFleeDate) > fBraveDuration);
    // If the player is in the detect range, it could be in flee radius.
    float bIsInFleeRadius = IsValid(tCharacters.m_pPlayer) && (FVector::Dist(pGoblin->GetActorLocation(), tCharacters.m_pPlayer->GetActorLocation()) <= fFleeRadius);
      
    pBlackboardComp->SetValueAsBool(m_tShouldAttackKey.SelectedKeyName, bCharactersInRange || IsValid(pEnemyToChase));
    pBlackboardComp->SetValueAsBool(m_tShouldFleeKey.SelectedKeyName, bCanFlee && bIsInFleeRadius);
    pBlackboardComp->SetValueAsBool(m_tCharactersInRangeKey.SelectedKeyName, bCharactersInRange);
    pBlackboardComp->SetValueAsObject(m_tEnemyToChaseKey.SelectedKeyName, pEnemyToChase);
  }
}