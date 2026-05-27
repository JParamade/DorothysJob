/************************************************************************
 * @description: Represents the Stunned Effect.
 *							 This Status makes the Target not 
 *							 beeing able to move or do actions.
 * @author: Pablo Velasco
 * @date: 05/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "../BaseStatus.h"
#include "StunEffect.generated.h"

class UFloatingPawnMovement;

UCLASS()
class DOROTHYSJOB_API UStunEffect : public UBaseStatus
{
	GENERATED_BODY()
public:
	/**
	 * @brief Default constructor of StunEffect
	 */
	UStunEffect();

protected:
	/**
	 * @brief Makes the Target not being able to move or using actions (shoot, attack, clean, dirt...).
	 */
	virtual void StatusEffect() override;

	/**
	 * @brief Makes the Target being able to move and use actions again.
	 */
	virtual void RevertEffect() override;

private:
	/**
	 * @brief Original Speed of the Target
	 */
	float m_fOriginalSpeed;

	/**
	 * @brief Movement component of the Target
	 */
	UFloatingPawnMovement* m_pMovementComponent;
};
