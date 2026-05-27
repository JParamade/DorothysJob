#include "DissolvableActor.h"
#include "DorothysJob/Manager/ProceduralGeneration.h"

ADissolvableActor::ADissolvableActor()
{
  PrimaryActorTick.bCanEverTick = false;

  Rotator = CreateDefaultSubobject<USceneComponent>(TEXT("Rotator"));
  RootComponent = Rotator;

  VisibleParentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleParentMesh"));
  VisibleParentMesh->SetupAttachment(Rotator);

  bUsingOriginalMesh = true;
  OriginalParentMesh = nullptr;
  ChangedParentMesh = nullptr;


  if (!HasAnyFlags(RF_ClassDefaultObject))
    UProceduralGeneration::OnLevelReady.AddUObject(this, &ADissolvableActor::TryRegisterInRoom);
}

void ADissolvableActor::Hide()
{
  ChangeRootMesh();
}

void ADissolvableActor::BeginPlay()
{
  Super::BeginPlay();

  if (VisibleParentMesh)
  {
    OriginalParentMesh = VisibleParentMesh->GetStaticMesh();
  }
}

void ADissolvableActor::ChangeRootMesh()
{
  if (IsValid(ChangedParentMesh))
  {
    UStaticMesh* MeshToChange = bUsingOriginalMesh ? ChangedParentMesh : OriginalParentMesh;
    VisibleParentMesh->SetStaticMesh(MeshToChange);
  }
  else
  {
    VisibleParentMesh->SetVisibility(!bUsingOriginalMesh, true);
    VisibleParentMesh->SetHiddenInGame(bUsingOriginalMesh, true);
  }
  bUsingOriginalMesh = !bUsingOriginalMesh;
}

void ADissolvableActor::TryRegisterInRoom(UBaseRoom* Room)
{
  if (!IsValid(Room))
  {
    return;
  }

  const ULevel* MyLevel = GetLevel();
  const ULevel* RoomLevel = Room->GetLevel();

  if (!IsValid(MyLevel) || !IsValid(RoomLevel))
  {
    return;
  }

  if (RoomLevel == MyLevel)
  {
    if (UProceduralGeneration::OnLevelReady.IsBound())
    {
      UProceduralGeneration::OnLevelReady.RemoveAll(this);
    }

    if (IsValid(Room))
    {
      Room->RegisterDissolvableActor(this);
    }
  }
}
