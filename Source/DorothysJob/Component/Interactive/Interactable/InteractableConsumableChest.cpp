#include "InteractableConsumableChest.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/UI/Menus/SelectConsumablesMenu/SelectConsumablesMenu.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "Kismet/GameplayStatics.h"

UInteractableConsumableChest::UInteractableConsumableChest():
  m_pConsumablesMenuInstance(nullptr),
  m_pGameInstance(nullptr)
{
}

void UInteractableConsumableChest::BeginPlay()
{
  Super::BeginPlay();
  if (GetWorld()) m_pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

}

void UInteractableConsumableChest::Interact()
{
  if (m_pGameInstance && !m_pGameInstance->m_lConsumables.IsEmpty())
  {
    GetWorld()->GetSubsystem<ULevelManager>()->SetCurrentGameState(EBaseGameState::SELECTCONSUMABLE);
  }
}

void UInteractableConsumableChest::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (m_pGameInstance && !m_pGameInstance->m_lConsumables.IsEmpty())
  {
    Super::OnInteractableBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
  }
}

void UInteractableConsumableChest::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (m_pGameInstance && !m_pGameInstance->m_lConsumables.IsEmpty())
  {
    Super::OnInteractableEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
  }
}
