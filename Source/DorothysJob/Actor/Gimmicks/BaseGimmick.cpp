// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseGimmick.h"
#include "Components/CapsuleComponent.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"

// Sets default values
ABaseGimmick::ABaseGimmick()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_pCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = m_pCapsuleComponent;

	m_pMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	m_pMeshComponent->SetupAttachment(RootComponent);
}

void ABaseGimmick::GimmickActivation()
{
}

void ABaseGimmick::TryRegisterInRoom(UBaseRoom* Room)
{
  const ULevel* MyLevel = GetLevel();
  const ULevel* RoomLevel = Room->GetLevel();

  FVector MyLevelLocation = GetLevelTransform().GetLocation();
  FVector RoomLevelLocation = Room->GetLocation();

  //if (!UProceduralGenerator::OnLevelReady.IsBound())
  //{
  //  UProceduralGenerator::OnGimmicksReady.Broadcast();
  //}
}

// Called every frame
void ABaseGimmick::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseGimmick::Activate()
{
  // for now we use this structure in case we might need to add some common logic between all gimmicks
  GimmickActivation();
}

void ABaseGimmick::GetHit()
{
}

