#include "DissolvablePropStructure.h"

ADissolvablePropStructure::ADissolvablePropStructure()
{
  HideableProps = CreateDefaultSubobject<USceneComponent>(TEXT("HideableProps"));
  HideableProps->SetupAttachment(VisibleParentMesh);

  UnhideableProps = CreateDefaultSubobject<USceneComponent>(TEXT("UnhideableProps"));
  UnhideableProps->SetupAttachment(VisibleParentMesh);
}

void ADissolvablePropStructure::Hide()
{
  bUsingOriginalMesh = !bUsingOriginalMesh;
  TArray<USceneComponent*> VisibleMeshChildren;
  HideableProps->GetChildrenComponents(true, VisibleMeshChildren);
  for (USceneComponent* Child : VisibleMeshChildren)
  {
    if (Child != VisibleParentMesh)
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

void ADissolvablePropStructure::BeginPlay()
{
  Super::BeginPlay();
}
