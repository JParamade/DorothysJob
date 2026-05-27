// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AchievementsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UAchievementsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<uint8> m_lAchievementManagerData;
	
};
