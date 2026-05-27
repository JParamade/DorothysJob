#include "BaseSpawnPoint.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Manager/ProceduralGeneration.h"

ABaseSpawnPoint::ABaseSpawnPoint()
{
  if (!HasAnyFlags(RF_ClassDefaultObject))
    UProceduralGeneration::OnLevelReady.AddUObject(this, &ABaseSpawnPoint::TryRegisterInRoom);
}

void ABaseSpawnPoint::TryRegisterInRoom(UBaseRoom* Room)
{
  const ULevel* MyLevel = GetLevel();
  const ULevel* RoomLevel = Room->GetLevel();

  if (RoomLevel == MyLevel)
  {
    UProceduralGeneration::OnLevelReady.RemoveAll(this);
    Room->RegisterSpawner(this);
  }
}
