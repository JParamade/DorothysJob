
/************************************************************************
 * @description: class that implements the behavior of the cauldron gimmick, 
 spawns enemies until the ritual enemies are killed.
 * @author: Lucía Fernández
 * @date: 18/06/25
 * @edited_by:
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "../BaseGimmick.h"
#include "CursedCauldronGimmick.generated.h"

class USpawnEnemyComponent;
class ABaseEnemy;

 /**
	* represents the gimmicks of the cursed cauldron
	* inherits from BaseGimmick and takes care of all logical behavior
	*/
UCLASS()
class DOROTHYSJOB_API ACursedCauldronGimmick : public ABaseGimmick
{
	GENERATED_BODY()
public:

	/**
 * class constructor
 */
	ACursedCauldronGimmick();
	virtual void Tick(float _DeltaTime) override;

	//Cooldown spawn normal enemies
	UPROPERTY(EditAnywhere, Category = "Bola 13|Skins", meta = (DisplayName = "Cooldown Enemies"))
	int32 iCooldownSpawn=5;

	//number of ritual enemies 
	UPROPERTY(EditAnywhere, Category = "Bola13|Enemies", meta = (DisplayName = "Ritual Enemies Count"))
	int enemiesToSpawnRitual = 1;

protected:

	virtual void BeginPlay() override;
	//delegate subscription
	UFUNCTION()
	void OnEnemyEliminate(ABaseEnemy* _pPawn);
	//auxiliary variable to know the dead ritual enemies
	int32 DeathRitualEnemies = 0;


private:

	//Pointer to SpawnEnemyComponent
	UPROPERTY()
	TObjectPtr<USpawnEnemyComponent> m_pSpawnEnemyComponent;

	//Array of type ritual enemies
	UPROPERTY(EditAnywhere, Category = "Bola13|Enemies", meta = (DisplayName = "Array Enemies"))
	TArray<TSubclassOf<ABaseEnemy>> EnemyRitual;
	//Array of ritual enemies
	UPROPERTY(EditAnywhere, Category = "Bola13|Enemies", meta = (DisplayName = "Array Enemies"))
	TArray<TObjectPtr<AActor>> EnemiesSpawned;

	FTimerHandle m_oSpawnTimer;

	//auxiliary variable to know if the normal enemies can spawn
	bool bCanSpawn = true;

	/**
 * calculates a navigable random point in a area
 * @return a FVector (random position in a area)
 */
	FVector GetRandomPosition();

	/**
 * Spawner normal enemies
 */
	void SpawnEnemies();
};
