// Fill out your copyright notice in the Description page of Project Settings.
#include "BaseFloor.h"

#include "TimerManager.h"
#include "Kismet/KismetSystemLibrary.h"

void ABaseFloor::BeginPlay()
{
  Super::BeginPlay();
  if (m_iX == 0 && m_iY == 0)
  {
    //InitLaunch();
  }
}

bool ABaseFloor::InitTile()
{
  if (m_bHasBeenInicialize == true)
  {
    return false;
  }

  if (!Super::InitTile())
  {
    UE_LOG(LogTemp, Error, TEXT("Something fail inicializating the ABaseTile"));
    return false;
  }

  FVector location[m_uiArraySize];
  FVector2f newGridLocation[m_uiArraySize];
  m_bHasBeenInicialize = true;

  TArray<FIntPoint> Offsets = {
      {  0,  1 }, {  1,  1 }, {  1,  0 }, {  1, -1 },
      {  0, -1 }, { -1, -1 }, { -1,  0 }, { -1,  1 },

      {  2,  0 }, {  2,  1 }, {  2,  2 }, {  2, -1 }, {  2, -2 },
      {  0, -2 }, {  1, -2 }, { -1, -2 }, { -2, -2 },
      { -2,  0 }, { -2, -1 }, { -2,  1 }, { -2,  2 },
      {  0,  2 }, {  1,  2 }, { -1,  2 }
  };

  for (size_t i = 0; i < m_uiArraySize; ++i)
  {
    const FIntPoint& Offset = Offsets[i];

    newGridLocation[i] = GetGridOffset(Offset);
    location[i] = GetWorldLocation(Offset);

    m_pTileArray[i] = GetTileAtLocation(location[i]);
    if (m_pTileArray[i] != nullptr)
    {
      m_pTileArray[i]->SetGridLocation(static_cast<int>(newGridLocation[i].X), static_cast<int>(newGridLocation[i].Y));
      m_pTileArray[i]->InitTile();
    }
  }

  InitEnd();
  return true;
}

ABaseFloor* ABaseFloor::GetTileAtLocation(FVector _location)
{
  TArray<TEnumAsByte<EObjectTypeQuery>> objects;
  TArray<AActor*> ignore;
  TArray<AActor*> Out;

  ignore.Add(this);

  objects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldStatic));
  objects.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));

  UKismetSystemLibrary::SphereOverlapActors(GetWorld(),
    _location,
    50,
    objects,
    nullptr,
    ignore,
    Out
  );

  for (size_t i = 0; i < Out.Num(); i++)
  {
    AActor* Actor = Out[i];
    if (Cast<ABaseFloor>(Actor) != nullptr)
    {
      return Cast<ABaseFloor>(Actor);
    }
  }
  return nullptr;
}

FVector ABaseFloor::GetSize() const
{
  return m_vTileSize;
}

ABaseFloor* ABaseFloor::GetAroundTiles(int i) const
{
  return m_pTileArray[i];
}

void ABaseFloor::GetGridLocation(int& x_, int& y_) const
{
  x_ = m_iX;
  y_ = m_iY;
}

FString ABaseFloor::GetId() const
{
  return m_sId;
}

void ABaseFloor::SetGridLocation(int _x, int _y)
{
  // add Id of the room
  m_iX = _x;
  m_iY = _y;
}


FVector2f ABaseFloor::GetGridOffset(FIntPoint _Offset) const
{
  return FVector2f(m_iX + _Offset.X, m_iY + _Offset.Y);
}

FVector ABaseFloor::GetWorldLocation(FIntPoint _Offset) const
{
  FVector actorLocation = GetActorLocation();
  FVector tileSize = GetSize();

  return FVector(
    actorLocation.X + _Offset.X * tileSize.X,
    actorLocation.Y + _Offset.Y * tileSize.Y,
    actorLocation.Z
  );
}