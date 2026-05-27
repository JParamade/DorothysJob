// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraBorderHandlerComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Actor/Camera/BaseCamera.h"

// Sets default values for this component's properties
UCameraBorderHandlerComponent::UCameraBorderHandlerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCameraBorderHandlerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (!Owner) return;

	m_pCameraOwner = Cast<ABaseCamera>(Owner);
	if (!m_pCameraOwner) return;
	
}


// Called every frame
void UCameraBorderHandlerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GetCameraSize();

  if (!m_bPlayDebug)
  {
    return;
  }

  // Dibuja el borde del cuarto en verde
  for (int32 i = 0; i < m_vBorderPos.Num(); ++i)
  {
    const FVector& Start = m_vBorderPos[i];
    const FVector& End = m_vBorderPos[(i + 1) % m_vBorderPos.Num()]; // Cierra el cuadrado
    DrawDebugLine(
      GetWorld(),
      Start,
      End,
      FColor::Green,
      false,         // No persistente
      10.0f,         // Duración (en segundos)
      0,
      5.0f           // Grosor
    );
  }

  // Dibuja el borde de la cámara en azul
  for (int32 i = 0; i < m_vCameraBorderPos.Num(); ++i)
  {
    const FVector& Start = m_vCameraBorderPos[i];
    const FVector& End = m_vCameraBorderPos[(i + 1) % m_vCameraBorderPos.Num()]; // Cierra el cuadrado

    DrawDebugLine(
      GetWorld(),
      Start,
      End,
      FColor::Blue,
      false, // No persistente
      10.0f, // Duración
      0,
      5.0f   // Grosor
    );
  }
}

FVector UCameraBorderHandlerComponent::GetBorderCollisionOffset()
{
  AActor* Owner = GetOwner();
  if (!Owner) return FVector::ZeroVector;

  ABaseCamera* CameraOwner = Cast<ABaseCamera>(Owner);
  if (!CameraOwner) return FVector::ZeroVector;

  if (m_vBorderPos.Num() != 4 || m_vCameraBorderPos.Num() != 4)
    return FVector::ZeroVector;

  // Convert 3D -> 2D
  TArray<FVector2D> RoomPoly;
  for (const FVector& Vec : m_vBorderPos)
    RoomPoly.Add(FVector2D(Vec.X, Vec.Y));

  TArray<FVector2D> CameraPoly;
  for (const FVector& Vec : m_vCameraBorderPos)
    CameraPoly.Add(FVector2D(Vec.X, Vec.Y));

  FVector2D TotalOffset2D = FVector2D::ZeroVector;

  // Flags para saber qué borde se violó (visual)
  bool bVioloDerecha = false;
  bool bVioloIzquierda = false;
  bool bVioloArriba = false;
  bool bVioloAbajo = false;

  // Direcciones visuales (puedes ajustar si cambia perspectiva)
  const FVector2D VisualRight = FVector2D(1, 1).GetSafeNormal();
  const FVector2D VisualLeft = -VisualRight;
  const FVector2D VisualUp = FVector2D(-1, 1).GetSafeNormal();
  const FVector2D VisualDown = -VisualUp;

  for (int32 i = 0; i < RoomPoly.Num(); ++i)
  {
    const FVector2D& A = RoomPoly[i];
    const FVector2D& B = RoomPoly[(i + 1) % RoomPoly.Num()];

    FVector2D EdgeDir = (B - A).GetSafeNormal();
    FVector2D EdgeNormal(-EdgeDir.Y, EdgeDir.X);

    FVector2D EdgeTotalOffset = FVector2D::ZeroVector;
    bool bEdgeViolated = false;

    for (const FVector2D& Point : CameraPoly)
    {
      float Penetration = FVector2D::DotProduct(Point - A, EdgeNormal);
      if (Penetration < 0.f)
      {
        EdgeTotalOffset += EdgeNormal * (-Penetration);
        bEdgeViolated = true;
      }
    }

    if (bEdgeViolated)
    {
      TotalOffset2D += EdgeTotalOffset;

      // Clasificación por dirección visual
      float DotRight = FVector2D::DotProduct(EdgeNormal, VisualRight);
      float DotLeft = FVector2D::DotProduct(EdgeNormal, VisualLeft);
      float DotUp = FVector2D::DotProduct(EdgeNormal, VisualUp);
      float DotDown = FVector2D::DotProduct(EdgeNormal, VisualDown);

      float MaxDot = FMath::Max(
        FMath::Max(FMath::Abs(DotRight), FMath::Abs(DotLeft)),
        FMath::Max(FMath::Abs(DotUp), FMath::Abs(DotDown))
      );

      if (FMath::Abs(DotRight) == MaxDot) bVioloDerecha = true;
      else if (FMath::Abs(DotLeft) == MaxDot) bVioloIzquierda = true;
      else if (FMath::Abs(DotUp) == MaxDot) bVioloArriba = true;
      else if (FMath::Abs(DotDown) == MaxDot) bVioloAbajo = true;
    }
  }

  FVector FinalOffset(TotalOffset2D.X, TotalOffset2D.Y, 0.f);

  // Debug logs
  //if (bVioloDerecha) UE_LOG(LogTemp, Warning, TEXT("Se salió por DERECHA"));
  //if (bVioloIzquierda) UE_LOG(LogTemp, Warning, TEXT("Se salió por IZQUIERDA"));
  //if (bVioloArriba) UE_LOG(LogTemp, Warning, TEXT("Se salió por ARRIBA"));
  //if (bVioloAbajo) UE_LOG(LogTemp, Warning, TEXT("Se salió por ABAJO"));
  /*UE_LOG(LogTemp, Warning, TEXT("Offset final aplicado: X=%.2f, Y=%.2f, Z=%.2f"), FinalOffset.X, FinalOffset.Y, FinalOffset.Z);*/
  return FinalOffset * 2.0f;
}

void UCameraBorderHandlerComponent::GetAndSetRoomSize(int32 _iCurrentLevel)
{
  AActor* Owner = GetOwner();
  if (!Owner) return;

  // It takes the level and see what are the borders so it's save in order:
  if (_iCurrentLevel != 0)
  {
    // Construir el tag completo que deben tener los actores del borde, e.g., "BoxBorder_1"
    FString BorderTagStr = FString::Printf(TEXT("BoxBorder_%d"), _iCurrentLevel);
    FName BorderTag(*BorderTagStr);

    // Obtener el mundo y todos los actores
    UWorld* World = GetWorld();
    if (!World) return;

    TArray<AActor*> AllActors;
    UGameplayStatics::GetAllActorsWithTag(World, BorderTag, AllActors);

    TMap<FName, FVector> OrderedCorners;

    for (AActor* Actor : AllActors)
    {
      FVector Pos = Actor->GetActorLocation();

      if (Actor->Tags.Contains("TopLeft"))
      {
        OrderedCorners.Add("TopLeft", Pos);
      }
      else if (Actor->Tags.Contains("TopRight"))
      {
        OrderedCorners.Add("TopRight", Pos);
      }
      else if (Actor->Tags.Contains("BottomLeft"))
      {
        OrderedCorners.Add("BottomLeft", Pos);
      }
      else if (Actor->Tags.Contains("BottomRight"))
      {
        OrderedCorners.Add("BottomRight", Pos);
      }
    }
    m_vBorderPos.Empty();

    if (OrderedCorners.Num() == 4)
    {

      m_vBorderPos.Add(OrderedCorners["TopLeft"]);
      m_vBorderPos.Add(OrderedCorners["TopRight"]);
      m_vBorderPos.Add(OrderedCorners["BottomRight"]);
      m_vBorderPos.Add(OrderedCorners["BottomLeft"]);

    }
  }
}

void UCameraBorderHandlerComponent::GetCameraSize()
{
  APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
  if (!PC) return;

  // Take the viewport size:
  FVector2D ViewportSize;
  GEngine->GameViewport->GetViewportSize(ViewportSize);

  // Sme order as the GetRoomSize:
  TArray<FVector2D> ScreenCorners = {
      FVector2D(0, 0),                             // Top-Left
      FVector2D(ViewportSize.X, 0),               // Top-Right
      FVector2D(ViewportSize.X, ViewportSize.Y),   // Bottom-Right
      FVector2D(0, ViewportSize.Y)               // Bottom-Left
  };

  TArray<FVector> WorldCorners;

  // For each corner, take the projected pos in the world:
  for (FVector2D ScreenPos : ScreenCorners)
  {
    FVector WorldOrigin, WorldDirection;
    if (PC->DeprojectScreenPositionToWorld(ScreenPos.X, ScreenPos.Y, WorldOrigin, WorldDirection))
    {
      // Intersect with Z = 0 plane
      if (FMath::Abs(WorldDirection.Z) > KINDA_SMALL_NUMBER)
      {
        float t = -WorldOrigin.Z / WorldDirection.Z;
        FVector Intersection = WorldOrigin + t * WorldDirection;
        WorldCorners.Add(Intersection);

        // For debug
        /*DrawDebugSphere(GetWorld(), Intersection, 20, 12, FColor::Green, false, 0.05f);*/
      }
    }
  }

  m_vCameraBorderPos = WorldCorners;
}

