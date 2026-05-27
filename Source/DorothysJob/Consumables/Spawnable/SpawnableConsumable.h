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
class DOROTHYSJOB_API USpawnableConsumable : public UBaseConsumable {
  GENERATED_BODY()

public:
  /**
   * @brief Uses the consumable item.
   * @param _pPlayer The player using the consumable.
   */
  virtual bool Use(ABasePlayer* _pPlayer) override;

  /**
   * @brief The class of the actor to spawn when the consumable is used.
   */
  TSubclassOf<ABaseConsumableActor> m_oActorToSpawn;

private:

  /**
   * @brief Gets a random location to spawn the consumable.
   * @param _pPlayer The player attempting to spawn the consumable.
   * @param vLocationToSpawn The location to spawn the consumable.
   * @return True if a location was found, false otherwise.
   */
  bool GetRandomLocationToSpawn(const ABasePlayer* _pPlayer, FVector& vLocationToSpawn);
};