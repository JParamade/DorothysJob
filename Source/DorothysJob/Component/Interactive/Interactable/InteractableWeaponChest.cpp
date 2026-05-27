#include "InteractableWeaponChest.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"

UInteractableWeaponChest::UInteractableWeaponChest()
{

}

void UInteractableWeaponChest::BeginPlay()
{
  Super::BeginPlay();
}

void UInteractableWeaponChest::Interact()
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return;
  }

  pLevelManager->SetCurrentGameState(EBaseGameState::SELECTWEAPON);
}