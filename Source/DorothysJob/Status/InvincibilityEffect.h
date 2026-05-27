/************************************************************************
 * @description: Represents the Invincibility Effect.
 *							 This Status makes the Target not
 *							 beeing able to be damaged and polluted.
 * @author: Lucia Fernandez
 * @date: 05/07/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BaseStatus.h"
#include "InvincibilityEffect.generated.h"

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UInvincibilityEffect : public UBaseStatus
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default constructor of StunEffect
	 */
	UInvincibilityEffect();

protected:
	/**
	 * @brief Makes the Target not being able to move or using actions (shoot, attack, clean, dirt...).
	 */
	virtual void StatusEffect() override;

	/**
	 * @brief Makes the Target being able to move and use actions again.
	 */
	virtual void RevertEffect() override;
};
