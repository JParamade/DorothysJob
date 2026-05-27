/************************************************************************
 * @description: Represents the Slow Effect.
 *							 This class makes the Character move Slower
 * @author: Pablo Velasco
 * @date: 05/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "../BaseStatus.h"
#include "SlowEffect.generated.h"

class UFloatingPawnMovement;

UCLASS()
class DOROTHYSJOB_API USlowEffect : public UBaseStatus
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief Default Constructor of SlowEffect.
	 */
	USlowEffect();

protected:
	/**
	 * @brief Makes the Target move Slower.
	 */
	virtual void StatusEffect() override;

	/**
	 * @brief Sets the Taget's Speed to its original value.
	 */
	virtual void RevertEffect() override;

private:
	/**
	 * @brief Original Speed of the Target
	 */
	float m_fOriginalSpeed;
	
	/**
	 * @brief The amount of Slow that will be applied. 
	 *				If equals to 0 the target won't be able to move.
	 *				If equals to 1 the taget's speed won't change.
	 *				If higher than 0 the target's speed will raise.
	 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Slow Factor", ClampMin = 0.0f))
	float m_fSlowFactor = 0.5f;

	/**
	 * @brief Movement component of the Target.
	 */
	UFloatingPawnMovement* m_pMovementComponent;
};
