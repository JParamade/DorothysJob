/************************************************************************
 * @description: Represents the Dizzyness Effect.
 *							 This State stacks, when full Stuns the target
 * @author: Pablo Velasco
 * @date: 05/04/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "../BaseStatus.h"
#include "DizzyEffect.generated.h"

class USlowEffect;
class UStunEffect;

UCLASS()
class DOROTHYSJOB_API UDizzyEffect : public UBaseStatus
{
	GENERATED_BODY()
public:
	/**
	 * @brief Default constructor of DizzyEffect.
	 */
	UDizzyEffect();

	int GetStacks() const;

protected:
	/**
	 * @brief Adds one Stack to the Target, if the Target has more stacks
	 *				than the maximum amount it can have will be Stunned.
	 */
	virtual void StatusEffect() override;

	/**
	 * @brief Sets the number of Stacks to 0
	 */
	virtual void RevertEffect() override;

private:
	/**
	 * @brief Current amount of Stacks
	 */
	int m_iStacks = 0;

	/**
	 * @brief Maximum amount of Stacks the Character can have.
	 *				Default value: 5, it can be edited in the Editor.
	 */
	UPROPERTY(EditAnywhere, Category = "Dizzy", meta = (DisplayName = "Max Stacks"))
	int m_iMaxStacks = 3;

	UPROPERTY(EditAnywhere, Category = "Dizzy")
	TSubclassOf<USlowEffect> _SlowEffectE2;

	UPROPERTY(EditAnywhere, Category = "Dizzy")
	TSubclassOf<USlowEffect> _SlowEffectE1;

	UPROPERTY(EditAnywhere, Category = "Dizzy")
	TSubclassOf<UStunEffect> _StunEffect;

};
