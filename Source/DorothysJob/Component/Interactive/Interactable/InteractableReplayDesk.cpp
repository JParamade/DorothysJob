
#include "InteractableReplayDesk.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "Kismet/GameplayStatics.h"

void UInteractableReplayDesk::BeginPlay()
{
  Super::BeginPlay();
  if (GetWorld()) m_pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
}

void UInteractableReplayDesk::Interact()
{
  if (GetWorld())
  {
    if (ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>())
    {
      pLevelManager->SetCurrentGameState(EBaseGameState::REPLAY);
    }
  }
}

void UInteractableReplayDesk::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (m_pGameInstance && m_pGameInstance->m_iHighestLevelAchieved > 1)
  {
    Super::OnInteractableBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
  }
}

void UInteractableReplayDesk::OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (m_pGameInstance && m_pGameInstance->m_iHighestLevelAchieved > 1)
  {
    Super::OnInteractableEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
  }
}
