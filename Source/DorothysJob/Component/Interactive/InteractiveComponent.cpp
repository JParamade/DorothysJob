
#include "InteractiveComponent.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"

UInteractiveComponent::UInteractiveComponent()
{
  PrimaryComponentTick.bCanEverTick = true;
  
  SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  SetCollisionResponseToAllChannels(ECR_Ignore);
  SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

  BoxExtent = FVector(50, 50, 50);
}


