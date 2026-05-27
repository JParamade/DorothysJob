#include "DissolvableBookcase.h"

void ADissolvableBookcase::Hide()
{
  Super::Hide();
  HideChildren();
}

void ADissolvableBookcase::BeginPlay()
{
  Super::BeginPlay();
}

void ADissolvableBookcase::HideChildren()
{
  TArray<USceneComponent*> VisibleMeshChildren;
  VisibleParentMesh->GetChildrenComponents(true, VisibleMeshChildren);
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
