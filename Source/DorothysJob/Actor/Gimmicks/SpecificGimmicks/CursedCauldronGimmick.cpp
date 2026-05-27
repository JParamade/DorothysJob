// Fill out your copyright notice in the Description page of Project Settings.


#include "CursedCauldronGimmick.h"
#include "DorothysJob/Component/Spawner/SpawnEnemyComponent.h"
#include "NavigationSystem.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"



ACursedCauldronGimmick::ACursedCauldronGimmick()
{
  m_pSpawnEnemyComponent = CreateDefaultSubobject<USpawnEnemyComponent>(TEXT("SpawnEnemyComponent"));
}

void ACursedCauldronGimmick::BeginPlay()
{
  //we go through the array of types of enemies for the ritual and prepare the pool
  for (TSubclassOf<ABaseEnemy> Enemies : EnemyRitual)
  {
    m_pSpawnEnemyComponent->SetEnemyPool(Enemies, 100.f);
  }
  //Add all the ritual enemies in an other array and spawn them
	for (int i = 0; i < enemiesToSpawnRitual; ++i)
	{
		AActor* aux = m_pSpawnEnemyComponent->SpawnEnemy(GetRandomPosition());

    TObjectPtr<ABaseEnemy> enemy = Cast<ABaseEnemy>(aux);
    EnemiesSpawned.Add(enemy);
    
    //we subscribe those enemies to the enemies' delegate of death
    enemy->OnEnemyEliminate.AddDynamic(this, &ACursedCauldronGimmick::OnEnemyEliminate);
		
	}
  SpawnEnemies();


}

/**
 * each time a ritual enemy is killed one is added,
 * when all ritual enemies are killed, normal enemies are spawned and spawning stops.
 */
void ACursedCauldronGimmick::OnEnemyEliminate(ABaseEnemy* _pPawn)
{
  DeathRitualEnemies++;

  if (DeathRitualEnemies >= enemiesToSpawnRitual)
  {
    bCanSpawn = false;
    UE_LOG(LogTemp, Warning, TEXT("RITUAL TERMINADO"));
    
  }
}
FVector ACursedCauldronGimmick::GetRandomPosition()
{

  UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
  if (!NavSys)
  {
    UE_LOG(LogTemp, Warning, TEXT("Nav mesh invalido"));
    return FVector::ZeroVector;
  }
  FVector SafeOrigin = GetActorLocation() + GetActorForwardVector() * 200.f;
  DrawDebugCircle(
    GetWorld(),
    SafeOrigin,
    1000.f,
    64,
    FColor::Green,
    true,
    -1.f,
    0,
    5.f,
    FVector(1, 0, 0),
    FVector(0, 1, 0),
    false
  );

  FNavLocation RandomNavLocation;
  

  bool bFound = NavSys->GetRandomReachablePointInRadius(SafeOrigin, 1000.f, RandomNavLocation);
  if (bFound)
  {
    
    return RandomNavLocation.Location;
  }
  else 
  {
    UE_LOG(LogTemp, Warning, TEXT("No se ha encontrado punto"));
    return FVector::ZeroVector;
  }

  

}

void ACursedCauldronGimmick::SpawnEnemies()
{
  if (bCanSpawn)
  {
    m_pSpawnEnemyComponent->SpawnEnemy(GetRandomPosition());
    GetWorldTimerManager().SetTimer(m_oSpawnTimer, this, &ACursedCauldronGimmick::SpawnEnemies, iCooldownSpawn, false);
  }
 
}

void ACursedCauldronGimmick::Tick(float _DeltaTime)
{
  
}


