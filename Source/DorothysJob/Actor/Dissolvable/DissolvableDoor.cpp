#include "DissolvableDoor.h"
#include "Components/BoxComponent.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Manager/ProceduralGeneration.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"

ADissolvableDoor::ADissolvableDoor()
{
  PrimaryActorTick.bCanEverTick = false;

  OpenDoorEmission = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenDoorEmission"));
  OpenDoorEmission->SetupAttachment(VisibleParentMesh);

  /*OpenDoorIcon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OpenDoorIcon"));
  OpenDoorIcon->SetupAttachment(VisibleParentMesh);
  OpenDoorIcon->SetMaterial(0, nullptr);*/

  TapeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TapeMesh"));
  TapeMesh->SetupAttachment(VisibleParentMesh);

  Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Plane"));
  Plane->SetupAttachment(VisibleParentMesh);

  TriggerZone = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerZone"));
  TriggerZone->SetupAttachment(VisibleParentMesh);

  if (!HasAnyFlags(RF_ClassDefaultObject))
    UProceduralGeneration::OnLevelReady.AddUObject(this, &ADissolvableDoor::TryRegisterInRoom);
}

void ADissolvableDoor::BeginPlay()
{
  Super::BeginPlay();

  VisibleParentMeshOpen = VisibleParentMesh->GetStaticMesh();
  ChangedParentMeshOpen = ChangedParentMesh;

  TriggerZone->OnComponentBeginOverlap.AddDynamic(this, &ADissolvableDoor::OnBeginOverlap);
  TriggerZone->OnComponentEndOverlap.AddDynamic(this, &ADissolvableDoor::OnEndOverlap);

  ULevelManager* MyLevelManager = GetWorld()->GetSubsystem<ULevelManager>();

  m_pLevelManager = MyLevelManager;
}

void ADissolvableDoor::Hide()
{
  ChangeRootMesh();
  ChangePlinth();
  HideChildren();
}

void ADissolvableDoor::CloseDoor()
{
  bIsDoorClosed = true;
  CloseRoot();
  HideEmission();
  TapeMesh->SetStaticMesh(TapeMeshComponent);
}

void ADissolvableDoor::OpenDoor()
{
  bIsDoorClosed = false;
  OpenRoot();
  ShowEmission();
  TapeMesh->SetStaticMesh(nullptr);
}

void ADissolvableDoor::HideChildren()
{
  TArray<USceneComponent*> VisibleMeshChildren;
  VisibleParentMesh->GetChildrenComponents(true, VisibleMeshChildren);
  for (USceneComponent* Child : VisibleMeshChildren)
  {
    if (Child != VisibleParentMesh && Child != VisiblePlinthMesh && Child && Child != OpenDoorEmission)
    {
      UStaticMeshComponent* StaticMeshChild = Cast<UStaticMeshComponent>(Child);
      if (StaticMeshChild)
      {
        StaticMeshChild->SetVisibility(bUsingOriginalMesh, true);
        StaticMeshChild->SetHiddenInGame(!bUsingOriginalMesh, true);

        if (StaticMeshChild->GetName() == TEXT("OpenDoorIcon"))
        {
          if (m_bHideIcon)
          {
            StaticMeshChild->SetVisibility(false, true);
            StaticMeshChild->SetHiddenInGame(true, true);
          }
        }
      }
    }
  }
}

void ADissolvableDoor::TryRegisterInRoom(UBaseRoom* Room)
{
  Super::TryRegisterInRoom(Room);

  const ULevel* MyLevel = GetLevel();
  const ULevel* RoomLevel = Room->GetLevel();

  if (RoomLevel == MyLevel)
  {
    UProceduralGeneration::OnLevelReady.RemoveAll(this);
    Room->RegisterDoor(this);
  }
}

void ADissolvableDoor::CloseRoot()
{
  if (!IsValid(VisibleParentMeshClosed) || !IsValid(VisibleParentMesh) || !IsValid(ChangedParentMeshClosed))
  {
    return;
  }
  VisibleParentMesh->SetStaticMesh(VisibleParentMeshClosed);
  ChangedParentMesh = ChangedParentMeshClosed;
  OriginalParentMesh = VisibleParentMeshClosed;
  Plane->SetVisibility(false);
  Plane->SetHiddenInGame(true);
}

void ADissolvableDoor::OpenRoot()
{
  if (!IsValid(VisibleParentMeshClosed) || !IsValid(VisibleParentMesh) || !IsValid(ChangedParentMeshClosed))
  {
    return;
  }
  VisibleParentMesh->SetStaticMesh(VisibleParentMeshOpen);
  ChangedParentMesh = ChangedParentMeshOpen;
  OriginalParentMesh = VisibleParentMeshOpen;
  Plane->SetVisibility(false);
  Plane->SetHiddenInGame(true);
}

void ADissolvableDoor::HideEmission()
{
  if (!IsValid(OpenDoorEmission))
  {
    return;
  }
  OpenDoorEmission->SetVisibility(false, false);
  OpenDoorEmission->SetHiddenInGame(true, false);

  //OpenDoorIcon->SetVisibility(false, false);
  //OpenDoorIcon->SetHiddenInGame(true, false);
  //OpenDoorIcon->SetMaterial(0, nullptr);
}

void ADissolvableDoor::ShowEmission()
{
  if (!IsValid(OpenDoorEmission))
  {
    return;
  }
  OpenDoorEmission->SetVisibility(true, false);
  OpenDoorEmission->SetHiddenInGame(false, false);

  //if (!m_bHideIcon)
  //{
  //  OpenDoorIcon->SetVisibility(true, false);
  //  OpenDoorIcon->SetHiddenInGame(false, false);
  //  OpenDoorIcon->SetMaterial(0, m_pIconMaterial);
  //}
  //else
  //{
  //  OpenDoorIcon->SetVisibility(false, false);
  //  OpenDoorIcon->SetHiddenInGame(true, false);
  //  OpenDoorIcon->SetMaterial(0, nullptr);
  //}
}

#pragma region | Overlap Functions
void ADissolvableDoor::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (OtherActor->IsA(ABasePlayer::StaticClass()))
  {
    bPlayerWasInside = true;
  }
}

void ADissolvableDoor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
  if (!bPlayerWasInside || !OtherActor->IsA(ABasePlayer::StaticClass()))
  {
    return;
  }

  bPlayerWasInside = false;

  ABasePlayer* Player = Cast<ABasePlayer>(OtherActor);

  FVector2D PlayerLocation = FVector2D(Player->GetActorLocation());
  FVector2D ColliderPosition = FVector2D(TriggerZone->GetComponentLocation());
  FVector2D DoorPosition = FVector2D(GetActorLocation());

  if (FVector2D::Distance(PlayerLocation, DoorPosition) > FVector2D::Distance(PlayerLocation, ColliderPosition))
  {
    m_pLevelManager->SetCurrentRoom(MyRoom);
  }
}
#pragma endregion

#pragma region | Getters&Setters
void ADissolvableDoor::SetRoom(TObjectPtr<UBaseRoom> NewRoom)
{
  MyRoom = NewRoom;
}

const TObjectPtr<UBaseRoom>& ADissolvableDoor::GetRoom() const
{
  return MyRoom;
}

bool ADissolvableDoor::IsDoorClosed() const
{
  return bIsDoorClosed;
}

void ADissolvableDoor::SetIconMaterial(ERoomType _RoomType, bool _ShowIcon)
{
  //if (!_ShowIcon)
  //{
  //  OpenDoorIcon->SetVisibility(false, false);
  //  OpenDoorIcon->SetHiddenInGame(true, false);
  //  m_bHideIcon = true;
  //  OpenDoorIcon->SetMaterial(0, nullptr);
  //  return;
  //}

  //if (TObjectPtr<UMaterialInterface>* FoundMaterial = m_pIconMaterialList.Find(_RoomType))
  //{
  //  m_pIconMaterial = *FoundMaterial; // desreferenciamos el puntero para obtener el material
  //  OpenDoorIcon->SetMaterial(0, m_pIconMaterial);
  //}
  //else
  //{
  //  UE_LOG(LogTemp, Warning, TEXT("SetIconMaterial: Material not found for RoomType %d"), (int32)_RoomType);
  //  m_pIconMaterial = nullptr;
  //}
}
#pragma endregion
//EOF
