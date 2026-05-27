// Fill out your copyright notice in the Description page of Project Settings.
#include "Drawing.h"

#include "../../Actor/Room/Tile/BaseFloor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "Kismet/GameplayStatics.h"

void UDrawing::BeginPlay()
{
  Super::BeginPlay();
  FindFloor();

}

void UDrawing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  if (m_currentFloor != nullptr)
  {
    DrawDebugSphere(GetWorld(), m_currentFloor->GetActorLocation(), 60, 8, FColor::Green);
  }
  if (m_currentFloor == nullptr)
  {
    FindFloor();
  }
  else
  {
    if (!CheckFloor())
    {
      FindFloor();
    }
  }
}

void UDrawing::Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot)
{
  Clean(_texture, _size, _angle, _pivot, GetOwner()->GetActorLocation());
}

void UDrawing::Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot, EDrawTier _tier)
{
  Clean(_texture, _size, _angle, _pivot, GetOwner()->GetActorLocation(), _tier);
}

void UDrawing::Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot, FVector _position)
{
  Clean(_texture, _size, _angle, _pivot, _position, EDrawTier::Liquid);
}

void UDrawing::Clean(UTexture* _texture, FVector2D _size, float _angle, FVector2D _pivot, FVector _position, EDrawTier _tier)
{
  if (!IsValid(m_currentFloor))
  {
    UE_LOG(LogTemp, Error, TEXT("No tiene un suelo"))
      return;
  }

  if (IsValid(m_currentFloor->GetStaticMesh()))
  {
    DrawOnFloor(m_currentFloor, nullptr, _texture, _size, _angle, _pivot, EDrawColor::EBase, _tier, true, _position);
    for (size_t i = 0; i < m_currentFloor->m_uiArraySize; i++)
    {
      if (IsValid(m_currentFloor->GetAroundTiles(i)))
      {
        if (IsValid((m_currentFloor->GetAroundTiles(i))->GetStaticMesh()))
        {
          DrawOnFloor(m_currentFloor->GetAroundTiles(i), nullptr, _texture, _size, _angle, _pivot, EDrawColor::EBase, _tier, true, _position);
        }
      }
    }
  }
}

void UDrawing::Dirt(UTexture* _texture, UTexture* _textureCleaning, FVector2D _size, float _angle, FVector2D _pivot, EDrawColor _colorDraw, EDrawTier _tierDraw)
{
  if (m_currentFloor == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("No tiene un suelo"))
    return;
  }
  if (m_currentFloor->GetStaticMesh() == nullptr)
  {
    UE_LOG(LogTemp, Error, TEXT("There isn't any mesh"))
    return;
  }
  DrawOnFloor(m_currentFloor, _texture, _textureCleaning, _size, _angle, _pivot, _colorDraw, _tierDraw, false, GetOwner()->GetActorLocation());
  for (size_t i = 0; i < m_currentFloor->m_uiArraySize; i++)
  {
    if (m_currentFloor->GetAroundTiles(i) != nullptr)
    {
      float CleaningRadiusX = (_size.X * m_currentFloor->GetAroundTiles(i)->GetSize().X) / 256;
      CleaningRadiusX = CleaningRadiusX / 2 + m_currentFloor->GetAroundTiles(i)->GetSize().X / 2;

      float CleaningRadiusY = (_size.Y * m_currentFloor->GetAroundTiles(i)->GetSize().Y) / 256;
      CleaningRadiusY = CleaningRadiusY / 2 + m_currentFloor->GetAroundTiles(i)->GetSize().Y / 2;

      FVector EntityDistance = (m_currentFloor->GetAroundTiles(i)->GetActorLocation() - GetOwner()->GetActorLocation()).GetAbs();
      //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Radius x %f, Radius y %f Entity x %f, Entity y %f"), CleaningRadiusX, CleaningRadiusY, EntityDistance.X, EntityDistance.Y));
      FString thisId = m_currentFloor->GetId();
      FString otherId = m_currentFloor->GetAroundTiles(i)->GetId();

      if (otherId == thisId  && (m_currentFloor->GetAroundTiles(i))->GetStaticMesh() != nullptr && CleaningRadiusX > EntityDistance.X && CleaningRadiusY > EntityDistance.Y)
      {
        DrawOnFloor(m_currentFloor->GetAroundTiles(i), _texture, _textureCleaning, _size, _angle, _pivot, _colorDraw, _tierDraw, false, GetOwner()->GetActorLocation());
      }
    }
  }
}


void UDrawing::DrawOnFloor(ABaseFloor* _floor, UTexture* _texture, UTexture* _textureCleaning, FVector2D _size, float _angle, FVector2D _pivot, EDrawColor _colorDraw, EDrawTier _tierDraw, bool _isCleaning, FVector _position)
{
  FVector2D vector;
  FHitResult hit = UGameplayStatics::MakeHitResult(
    false,
    false,
    0.0F,
    0.0F,
    _position,
    _position,
    FVector(0, 0, 0),
    FVector(0, 0, 0),
    nullptr,
    _floor,
    _floor->GetStaticMesh(),
    FName(TEXT("None")),
    FName(TEXT("None")),
    0,
    0,
    0,
    FVector(0, 0, 0),
    FVector(0, 0, 0)
  );
  UGameplayStatics::FindCollisionUV(hit, 0, vector);
  vector.X = 1 - vector.X; //Si el pintar falla es por UV hay que modificar este coso
  _floor->Draw(_texture, _textureCleaning, vector, _size, _angle, _pivot, _colorDraw, _tierDraw, _isCleaning);
}

bool UDrawing::FindFloor()
{
  FVector location = GetOwner()->GetActorLocation();
  FVector locationToHit = location + (-1 * FVector::UpVector * 1000);

  TArray<AActor*> actorToIngone;
  actorToIngone.Add(GetOwner());
  actorToIngone.Add(UGameplayStatics::GetPlayerPawn(this, 0));
  TArray<FHitResult> hitResults;
 

  if (UKismetSystemLibrary::LineTraceMulti(
    GetWorld(),
    location,
    locationToHit,
    UEngineTypes::ConvertToTraceType(ECC_Camera),
    true,
    actorToIngone,
    EDrawDebugTrace::ForDuration,
    hitResults,
    true,
    FLinearColor(1, 0, 0, 1),
    FLinearColor::Green,
    10.0f
  ))
  {
    for (const FHitResult& hit : hitResults)
    {
      ABaseFloor* floor = Cast<ABaseFloor>(hit.GetActor());
      if (floor)
      {
        //GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Red, FString::Printf(TEXT("UV Shell: %i"), hit.FaceIndex));
        m_currentFloor = floor;
        OnFloorFound.Broadcast();
        return true;
      }
    }
  }
  return false;
}

bool UDrawing::CheckFloor()
{
  float x = 0;
  float y = 0;


  x = GetOwner()->GetActorLocation().X - m_currentFloor->GetActorLocation().X;
  y = GetOwner()->GetActorLocation().Y - m_currentFloor->GetActorLocation().Y;

  x = abs(x);
  y = abs(y);

  if(x > m_currentFloor->GetSize().X/2 || y > m_currentFloor->GetSize().Y/2)
  {
    for (size_t i = 0; i < m_currentFloor->m_uiArraySize; i++)
    {
      if (m_currentFloor->GetAroundTiles(i) != nullptr)
      {
        x = m_currentFloor->GetAroundTiles(i)->GetActorLocation().X - GetOwner()->GetActorLocation().X;
        y = m_currentFloor->GetAroundTiles(i)->GetActorLocation().Y - GetOwner()->GetActorLocation().Y;

        x = abs(x);
        y = abs(y);
        if (x < m_currentFloor->GetAroundTiles(i)->GetSize().X/2 && y < m_currentFloor->GetAroundTiles(i)->GetSize().Y/2)
        {
          // This is commented for FPS issue
          //UpdatePercentageInFloors();
          m_currentFloor = m_currentFloor->GetAroundTiles(i);
          return true;
        }
      }
    }
  }
  else
  {
    return true;
  }
  return false;
}

void UDrawing::UpdatePercentageInFloors()
{
  for (size_t i = 0; i < m_currentFloor->m_uiArraySize; i++)
  {
    if (m_currentFloor->GetAroundTiles(i) != nullptr)
    {
      m_currentFloor->GetAroundTiles(i)->UpdateCleaningPercentage();
    }
  }
}

ABaseFloor* UDrawing::GetFloor() const
{
  return m_currentFloor;
}

float UDrawing::GetNotCleaningPercentage()
{
  float percentage = 0;
  if(GetFloor() != nullptr)
  {
    percentage = GetFloor()->GetPercetage();
  }
  return percentage;
}
