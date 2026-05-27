// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraShakeHandlerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Actor/Camera/MainLegacyCameraShake.h"

// Sets default values for this component's properties
UCameraShakeHandlerComponent::UCameraShakeHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UCameraShakeHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

  if (!ShakeInstance && ShakeClass)
  {
    ShakeInstance = NewObject<UMainLegacyCameraShake>(this, ShakeClass);
  }
}


// Called every frame
void UCameraShakeHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCameraShakeHandlerComponent::PlayCameraShake(float Scale)
{
  APlayerController* PC = UGameplayStatics::GetPlayerController(GetOwner(), 0);
  if (!PC) return;

  if (ShakeClass)
  {
    UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(ShakeClass, 1.f);
  }
}

void UCameraShakeHandlerComponent::StopCameraShake()
{
  APlayerController* PC = UGameplayStatics::GetPlayerController(GetOwner(), 0);
  if (PC)
  {
    PC->PlayerCameraManager->StopAllCameraShakes(false);
  }
}

void UCameraShakeHandlerComponent::SetCameraShake(TSubclassOf<UMainLegacyCameraShake> _ShakeClass)
{
  ShakeClass = _ShakeClass;

  if (ShakeInstance)
  {
    ShakeInstance->ConditionalBeginDestroy();
    ShakeInstance = nullptr;
  }

  if (ShakeClass)
  {
    ShakeInstance = NewObject<UMainLegacyCameraShake>(this, ShakeClass);
  }
}

UMainLegacyCameraShake* UCameraShakeHandlerComponent::GetShakeIstance()
{
  return ShakeInstance;
}

