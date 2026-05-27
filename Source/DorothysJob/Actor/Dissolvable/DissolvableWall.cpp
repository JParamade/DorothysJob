#include "DissolvableWall.h"

ADissolvableWall::ADissolvableWall()
{
  PrimaryActorTick.bCanEverTick = false;

  VisiblePlinthMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisiblePlinthMesh"));
  VisiblePlinthMesh->SetupAttachment(VisibleParentMesh);

  OriginalParentMesh = nullptr;
  OriginalPlinthMesh = nullptr;
}

void ADissolvableWall::Hide()
{
  ChangeRootMesh();
  ChangePlinth();
  HideChildren();
}

void ADissolvableWall::BeginPlay()
{
  Super::BeginPlay();

  if (VisiblePlinthMesh)
  {
    if (GetActorLocation().Z >= 100)
    {
      ChangedParentMesh = nullptr;
    }
    OriginalPlinthMesh = VisiblePlinthMesh->GetStaticMesh();
  }
}

void ADissolvableWall::HideChildren()
{
  TArray<USceneComponent*> VisibleMeshChildren;
  VisibleParentMesh->GetChildrenComponents(true, VisibleMeshChildren);
  for (USceneComponent* Child : VisibleMeshChildren)
  {
    if (Child != VisibleParentMesh && Child != VisiblePlinthMesh)
    {
      UStaticMeshComponent* StaticMeshChild = Cast<UStaticMeshComponent>(Child);
      if (StaticMeshChild)
      {
        StaticMeshChild->SetVisibility(bUsingOriginalMesh, true);
        StaticMeshChild->SetHiddenInGame(!bUsingOriginalMesh, true);
      }
    }
  }
}

void ADissolvableWall::ChangePlinth()
{
  if (IsValid(ChangedPlinthMesh))
  {
    UStaticMesh* MeshToChange = !bUsingOriginalMesh ? ChangedPlinthMesh : OriginalPlinthMesh;
    VisiblePlinthMesh->SetStaticMesh(MeshToChange);
  }
  else
  {
    VisiblePlinthMesh->SetVisibility(bUsingOriginalMesh, false);
    VisiblePlinthMesh->SetHiddenInGame(!bUsingOriginalMesh, false);
  }
}
