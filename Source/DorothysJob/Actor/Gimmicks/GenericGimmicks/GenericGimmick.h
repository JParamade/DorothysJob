// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseGimmick.h"
#include "GenericGimmick.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API AGenericGimmick : public ABaseGimmick
{
	GENERATED_BODY()
	
public:

	AGenericGimmick();

protected:

	UPROPERTY()
	float m_fEffectRadius;
};
