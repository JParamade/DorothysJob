#include "BaseActorComponent.h"
#include "Logging/StructuredLog.h"    // for logging

DEFINE_LOG_CATEGORY(ComponentLog);

UBaseActorComponent::UBaseActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickEnabled(true);

}


// Called when the game starts
void UBaseActorComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UBaseActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}
