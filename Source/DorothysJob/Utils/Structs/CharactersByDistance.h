/************************************************************************
 * @description: Represents the data table for the rooms
 * @author: Pablo Velasco Crespo
 * @date: 19/08/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "CharactersByDistance.generated.h"

class ABasePlayer;
class ABaseEnemy;

USTRUCT(BlueprintType)
struct FCharactersByDistance
{
  GENERATED_BODY()

  FCharactersByDistance() 
    : m_pPlayer(nullptr)
  {}

  TObjectPtr<ABasePlayer> m_pPlayer;

  TArray<TWeakObjectPtr<ABaseEnemy>> m_lCloseEnemies;

  TArray<TWeakObjectPtr<ABaseEnemy>> m_lFarEnemies;
};