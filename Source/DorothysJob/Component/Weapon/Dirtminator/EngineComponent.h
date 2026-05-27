/************************************************************************
 * @description: Represents the Base Engine Component of Dirtminator Weapon
 * @author: Pablo Velasco
 * @date: 06/05/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeaponComponent.h"
#include "EngineComponent.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UEngineComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()
	
public:
	//virtual void Release() {};
	virtual void ResetTimers() {};
};
