#include "InteractableDoor.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "FMODBlueprintStatics.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/Events/EventManager.h"

UInteractableDoor::UInteractableDoor()
{

}

void UInteractableDoor::BeginPlay()
{
  Super::BeginPlay();
}

void UInteractableDoor::Interact()
{
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pGameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameInstance not found"));
    return;
  }

  UEventManager* pEventManager = pGameInstance->m_pEventManager;
  if (!pEventManager)
  {
    return;
  }

  if (!pEventManager->CanStartLevel(pGameInstance->m_iCurrentLevel))
  {
    pEventManager->SetIsEventActive(TEXT("HUB_MISS_TUT"), false);
    pEventManager->TriggerEvent(TEXT("HUB_MISS_TUT"));
    return;
  }

  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    UE_LOG(LogTemp, Error, TEXT("UInteractableDoor::Interact > LevelManager not found."));
    return;
  }

  pLevelManager->SetCurrentGameState(EBaseGameState::PRELEVEL);
}