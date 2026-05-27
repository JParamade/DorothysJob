
#include "PickUpComponent.h"


void UPickUpComponent::Interact()
{
  UE_LOG(LogTemp, Warning, TEXT("IT'S ME, A PICK UP!!"))
}

void UPickUpComponent::BeginPlay()
{
  Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UPickUpComponent::OnPickUpBeginOverlap);
}

void UPickUpComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);

  OnComponentBeginOverlap.RemoveAll(this);
}

void UPickUpComponent::OnPickUpBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (ABasePlayer* player = Cast<ABasePlayer>(OtherActor))
  {
    OnComponentBeginOverlap.RemoveAll(this);

    Interact();

    //DESTROY?
    GetOwner()->Destroy();
  }
}
