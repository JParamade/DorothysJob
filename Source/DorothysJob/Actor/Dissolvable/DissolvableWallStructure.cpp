#include "DissolvableWallStructure.h"

ADissolvableWallStructure::ADissolvableWallStructure()
{
  PrimaryActorTick.bCanEverTick = false;

  BasicWalls = CreateDefaultSubobject<USceneComponent>(TEXT("BasicWalls"));
  BasicWalls->SetupAttachment(VisibleParentMesh);

  Columns = CreateDefaultSubobject<USceneComponent>(TEXT("Columns"));
  Columns->SetupAttachment(VisibleParentMesh);

  BasicWallPlinths = CreateDefaultSubobject<USceneComponent>(TEXT("BasicWallPlinths"));
  BasicWallPlinths->SetupAttachment(VisibleParentMesh);

  ColumnPlinths = CreateDefaultSubobject<USceneComponent>(TEXT("ColumnPlinths"));
  ColumnPlinths->SetupAttachment(VisibleParentMesh);

  Splits01 = CreateDefaultSubobject<USceneComponent>(TEXT("SplitWalls01"));
  Splits01->SetupAttachment(VisibleParentMesh);

  Split01Plinths = CreateDefaultSubobject<USceneComponent>(TEXT("SplitWall01Plinths"));
  Split01Plinths->SetupAttachment(VisibleParentMesh);

  Splits02 = CreateDefaultSubobject<USceneComponent>(TEXT("SplitWalls02"));
  Splits02->SetupAttachment(VisibleParentMesh);

  Split02Plinths = CreateDefaultSubobject<USceneComponent>(TEXT("SplitWall02Plinths"));
  Split02Plinths->SetupAttachment(VisibleParentMesh);

  Outcorners = CreateDefaultSubobject<USceneComponent>(TEXT("OutcornerWalls"));
  Outcorners->SetupAttachment(VisibleParentMesh);

  OutcornerPlinths = CreateDefaultSubobject<USceneComponent>(TEXT("OutcornerWallPlinths"));
  OutcornerPlinths->SetupAttachment(VisibleParentMesh);

  HideableProps = CreateDefaultSubobject<USceneComponent>(TEXT("HideableProps"));
  HideableProps->SetupAttachment(VisibleParentMesh);

  UnhideableProps = CreateDefaultSubobject<USceneComponent>(TEXT("UnhideableProps"));
  UnhideableProps->SetupAttachment(VisibleParentMesh);
}

void ADissolvableWallStructure::Hide()
{
  bUsingOriginalMesh = !bUsingOriginalMesh;

  HideProps();        // HideableProps
  ChangeOutcorners(); // Outcorners & OutcornerPlinths
  ChangeSplits();     // Splits01, Splits02, SplitPlinths01 & SplitPlinths02
  ChangeColumns();    // Columns & ColumnPlinths
  ChangeBasicWalls(); // Basic Walls & Basic Wall Plinths
}

void ADissolvableWallStructure::BeginPlay()
{
  Super::BeginPlay();
  OriginalOutcorners = GetMeshFromFolder(Outcorners);
  OriginalOutcornerPlinths = GetMeshFromFolder(OutcornerPlinths);
  OriginalSplits01 = GetMeshFromFolder(Splits01);
  OriginalSplits02 = GetMeshFromFolder(Splits02);
  OriginalSplitPlinths01 = GetMeshFromFolder(Split01Plinths);
  OriginalSplitPlinths02 = GetMeshFromFolder(Split02Plinths);
  OriginalColumns = GetMeshFromFolder(Columns);
  OriginalColumnPlinths = GetMeshFromFolder(ColumnPlinths);
  OriginalBasicWalls = GetMeshFromFolder(BasicWalls);
  OriginalBasicWallPlinths = GetMeshFromFolder(BasicWallPlinths);
}

TObjectPtr<UStaticMesh> ADissolvableWallStructure::GetMeshFromFolder(TObjectPtr<USceneComponent> Folder) const
{
  TArray<USceneComponent*> VisibleMeshChildren;
  Folder->GetChildrenComponents(true, VisibleMeshChildren);
  for (USceneComponent* Child : VisibleMeshChildren)
  {
    if (Child != Folder)
    {
      TObjectPtr<UStaticMeshComponent> StaticMeshChild = Cast<UStaticMeshComponent>(Child);
      if (StaticMeshChild)
      {
        return StaticMeshChild->GetStaticMesh();
      }
    }
  }
  return nullptr;
}

void ADissolvableWallStructure::HideProps()
{
  Change(HideableProps);
}

void ADissolvableWallStructure::ChangeOutcorners()
{
  Change(Outcorners, ChangedOutcorners, OriginalOutcorners);
  Change(OutcornerPlinths, ChangedOutcornerPlinths, OriginalOutcornerPlinths);
}

void ADissolvableWallStructure::ChangeSplits()
{
  Change(Splits01, ChangedSplits01, OriginalSplits01);
  Change(Splits02, ChangedSplits02, OriginalSplits02);
  Change(Split01Plinths, ChangedSplitsPlinths01, OriginalSplitPlinths01);
  Change(Split02Plinths, ChangedSplits02Plinths, OriginalSplitPlinths02);
}

void ADissolvableWallStructure::ChangeColumns()
{
  Change(Columns, ChangedColumns, OriginalColumns);
  Change(ColumnPlinths, ChangedColumnPlinths, OriginalColumnPlinths);
}

void ADissolvableWallStructure::ChangeBasicWalls()
{
  Change(BasicWalls, ChangedBasicWalls, OriginalBasicWalls);
  Change(BasicWallPlinths, ChangedBasicWallPlinths, OriginalBasicWallPlinths);
}

void ADissolvableWallStructure::Change(TObjectPtr<USceneComponent> Folder, TObjectPtr<UStaticMesh> ChangedSM, TObjectPtr<UStaticMesh> OriginalSM)
{
  TArray<USceneComponent*> VisibleMeshChildren;
  Folder->GetChildrenComponents(true, VisibleMeshChildren);
  for (USceneComponent* Child : VisibleMeshChildren)
  {
    if (Child != Folder)
    {
      UStaticMeshComponent* StaticMeshChild = Cast<UStaticMeshComponent>(Child);
      if (StaticMeshChild)
      {
        if (IsValid(ChangedSM) && StaticMeshChild->GetComponentLocation().Z < 200.0f)
        {
          UStaticMesh* MeshToChange = bUsingOriginalMesh ? OriginalSM : ChangedSM;
          StaticMeshChild->SetStaticMesh(MeshToChange);
        }
        else
        {
          StaticMeshChild->SetVisibility(bUsingOriginalMesh, true);
          StaticMeshChild->SetHiddenInGame(!bUsingOriginalMesh, true);
        }
      }
    }
  }
}
