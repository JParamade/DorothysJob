/************************************************************************
 * @description: Represents the Exposure Effect.
 *							 This Status modify the bonuses of the target (the value of damage that recives)
 * @author: Lucia Fernadez
 * @date: 05/07/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BaseStatus.h"
#include "ExposureEffect.generated.h"

UCLASS()
class DOROTHYSJOB_API UExposureEffect : public UBaseStatus
{
	GENERATED_BODY()
public:

	/**
	 * @brief Default constructor of StunEffect
	 */
	UExposureEffect();

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
	
	UPROPERTY(EditAnywhere, Category = "Exposure", meta = (DisplayName = "Exposure Value"))
	float m_fExposure;

	float m_fOriginalBonuses;
};