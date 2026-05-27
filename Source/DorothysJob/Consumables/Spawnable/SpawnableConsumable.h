/************************************************************************
 * @description: Represents the spawnable consumable.
 * @author: Jaime Paramo
 * @date: 11/06/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Consumables/BaseConsumable.h"
#include "SpawnableConsumable.generated.h"

class ABasePlayer;
class ABaseConsumableActor;

UCLASS()
class DOROTHYSJOB_API USpawnableConsumable : public UBaseConsumable
{
	GENERATED_BODY()

public:
	/**
	 * @brief
	 * @param _pPlayer
	 */
	virtual bool Use(ABasePlayer* _pPlayer) override;

	/**
	 * @brief 
	 */
	TSubclassOf<ABaseConsumableActor> m_oActorToSpawn;

private:

  /**
   * @brief It returns a location to spawn the consumable. If It can't find a location to spawn in the max tries, it will return false.
   * @param _pPlayer -> Player that is traying to spawn the consumable.
   * @param vLocationToSapwn_ -> Consumable location to spawn. Out parameter.
   * @return True if it had been able to found a location to spawn.
   */
  bool GetRandomLocationToSpawn(const ABasePlayer* _pPlayer, FVector& vLocationToSapwn_);
};