// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnEnemyComponent.h"
#include "DorothysJob/Utils/Random/RandomNumberGenerator.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "NavigationSystem.h"

// Sets default values for this component's properties
USpawnEnemyComponent::USpawnEnemyComponent()
{
}

void USpawnEnemyComponent::SetEnemyPool(TSubclassOf<ABaseEnemy> _enemyType, float _weight)
{
  m_mEnemyPool.Add(_enemyType, _weight);
}

AActor* USpawnEnemyComponent::SpawnEnemy(const FVector& _SpawnLocation)
{

  if (m_mEnemyPool.Num() == 0)
  {
    return nullptr;
  }

  UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(GetWorld());  //TODO: When separated navmesh per room, choose the correct navmesh.
  FNavLocation tNavLocation;

  FVector vInFloorLocation = _SpawnLocation;

  if ((pNavSys != nullptr) && pNavSys->ProjectPointToNavigation(_SpawnLocation, tNavLocation))
  {
    vInFloorLocation = tNavLocation.Location + FVector(0.f, 0.f, 200.f); //Enemies Capsule Half Height (For now, all the enemies have the same capsule height)
  }
  else
  {
    UE_LOG(LogTemp, Error, TEXT("SpawnEnemy: Not NavMesh Point"));
    return nullptr;
  }

  float totalWeigth = 0.0f;
  RandomNumberGenerator rng;
  for (const auto& Pair : m_mEnemyPool)
  {
    totalWeigth += Pair.Value;
  }

  float randomWeight = rng.GetRandomInRange(0.0f, totalWeigth);
  UWorld* World = GetWorld();
  if (!World)
  {
    UE_LOG(LogTemp, Error, TEXT("SpawnEnemy: GetWorld() returned nullptr"));
    return nullptr;
  }

  for (auto& Pair : m_mEnemyPool)
  {
    if (!Pair.Key)
    {
      UE_LOG(LogTemp, Warning, TEXT("SpawnEnemy: Found null enemy class in pool"));
      continue;
    }

    randomWeight -= Pair.Value;
    if (randomWeight <= 0.0f)
    {
      FActorSpawnParameters spawnInfo;
      spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;  //AdjustIfPossible can modify Z value when spawning
      
      AActor* SpawnedActor_ = World->SpawnActor<AActor>(Pair.Key, vInFloorLocation, FRotator::ZeroRotator, spawnInfo);
      if (SpawnedActor_)
      {
        return SpawnedActor_;
      }
      UE_LOG(LogTemp, Error, TEXT("SpawnEnemy: Failed to spawn actor"));
    }
  }

  return nullptr;
}

