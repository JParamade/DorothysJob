#include "OneFlowerSlime.h"
#include "DorothysJob/Data/DataAssets/CharacterPropertiesDataAsset.h"
#include <DorothysJob/Consumables/BaseConsumable.h>
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

AOneFlowerSlime::AOneFlowerSlime() :
  ASlimeEnemy()
{
  // Capsule
  UpdateCapsule(80.f);
}

void AOneFlowerSlime::BeginPlay()
{
  Super::BeginPlay();

  if (IsValid(m_pCharacterDataAsset))
  {
    SetIsInvulnerable(true);
    TWeakObjectPtr<ABaseCharacter> WeakThis(this);
    GetWorldTimerManager().SetTimer(m_tInvulnerableTimerHandle,
      [WeakThis]() {
        if (WeakThis.IsValid())
        {
          WeakThis->SetIsInvulnerable(false);
        }
      },
      m_pCharacterDataAsset->m_fInvulnerableTime, false);
  }
}

void AOneFlowerSlime::GameOver()
{
  UAchievementSubsystem* AchSubsystem = nullptr;
  if (UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    AchSubsystem = GameInstance->GetSubsystem<UAchievementSubsystem>();
  }

  if (AchSubsystem)
  {
    AchSubsystem->AddProgress("SludgeSlayer", 1);

    AchSubsystem->ReportSlimeKill(m_eWeaponLastAttackReceive);
  }


  Super::GameOver();



}
