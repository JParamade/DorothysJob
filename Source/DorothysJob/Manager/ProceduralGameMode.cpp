#include "ProceduralGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"

AProceduralGameMode::AProceduralGameMode()
{
  bStartPlayersAsSpectators = true;
}

void AProceduralGameMode::SpawnPlayerAt(const FVector& Location, const FRotator& Rotation)
{
  bStartPlayersAsSpectators = false;

  AController* Controller = UGameplayStatics::GetPlayerController(this, 0);
  if (!Controller)
  {
    UE_LOG(LogTemp, Warning, TEXT("No player controller found"));
    return;
  }

  FTransform SpawnTransform(Rotation, Location);

  RestartPlayerAtTransform(Controller, SpawnTransform);

  ABasePlayer* player = Cast<ABasePlayer>(GetWorld()->GetPlayerControllerIterator()->Get()->GetPawn());
  player->SetControllerCamera(); //@TODO: find player when camera needs it, not in begin play
}
