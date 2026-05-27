#include "BaseActor.h"

ABaseActor::ABaseActor()
{
	/*
		bCanEverTick has to be true so its children can use Tick(), if the child does not Tick change it to false.
	*/
	PrimaryActorTick.bCanEverTick = true;

}

void ABaseActor::BeginPlay()
{
	Super::BeginPlay();

}

void ABaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}