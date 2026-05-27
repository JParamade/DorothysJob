// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnEnemyComponent.generated.h"

class ABaseEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API USpawnEnemyComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnEnemyComponent();

	void SetEnemyPool(TSubclassOf<ABaseEnemy> _enemyType, float _weight);
protected:

	/**
	 * @brief This map has the pool of enemies that can be spawned and it's weight, the higher the weigth the higher the
	 * chances of spawning that enemy
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (DisplayName = "Enemy Pool"))
	TMap<TSubclassOf<ABaseEnemy>, float> m_mEnemyPool;

public:	

	/**
	 * @brief Spawns a single Enemy in the given position
	 * @param _SpawnLocation World location where spawn
	 * @return The spawned actor or nullptr if it failed
	 */
	AActor* SpawnEnemy(const FVector& _SpawnLocation);
};
