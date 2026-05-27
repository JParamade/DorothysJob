// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingsSaveGame.generated.h"


USTRUCT()
struct FSettingsData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	TArray<uint8> m_lSettingsManagerData;
};

UCLASS()
class DOROTHYSJOB_API USettingsSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TMap<FName, FSettingsData> m_lSettingsData;

	UPROPERTY()
	FString m_sGameLanguage;
};
