// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LegacyCameraShake.h"
#include "MainLegacyCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UMainLegacyCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()
	
public:

  UMainLegacyCameraShake();

	/** Método para aplicar configuración desde ShakeConfig */
	void ApplySettingsFromDataAsset();
private:

};
