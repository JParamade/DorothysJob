/************************************************************************
 * @description: Represents the interactable cleaning station
 * @author: Jorge
 * @date: -/-/2025
 * @edited_by: Josephine
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "InteractableComponent.h"
#include "InteractableCleaningCupboard.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteraction);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRegenerateHealth, int, health);

UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UInteractableCleaningCupboard : public UInteractableComponent
{
	GENERATED_BODY()

public:

	UInteractableCleaningCupboard();

	void AddTime(float _TimeToAdd);

	/**
	 * @brief Represents the first phase timer
	 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "First phase timer"))
	float m_fFirstPhaseTimer;

	/**
	 * @brief Represents the second phase timer
	 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Second phase timer"))
	float m_fSecondPhaseTimer;

	/**
	 * @brief Represents the first phase hp to regenerate on interaction
	 */
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Hit points to regenerate"))
	int32 m_iRegenerationPoints;

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Heals Dorothy when this actor is interacted and activate cooldown.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

	virtual void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	/**
	 * @brief Delegate for the interaction
	 */
	UPROPERTY(BlueprintAssignable)
	FOnInteraction OnInteraction;

	/**
	 * @brief Delegate for the regeneration timer
	 */
	UPROPERTY(BlueprintAssignable)
	FOnRegenerateHealth OnRegenerateHealth;

	/**
	 * @brief Seconds interval in which it is regenerating
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Health", meta = (DisplayName = "Healing Rate"))
	float m_fHealingRate;

	/**
	 * @brief Delay to start regenerating when it has been used
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Health", meta = (DisplayName = "Healing Delay"))
	float m_fHealingDelay;

	/**
	 * @brief The rate at witch it regenerates
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Health", meta = (DisplayName = "Healing Rate"))
	int m_iRegenerateAmount;

	/**
	 * @brief The particle system to spawn
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Health", meta = (DisplayName = "Visual Effect"))
	TObjectPtr<UNiagaraSystem> m_pVFXNiagaraSystem;

	/**
	 * @brief The component of the particle system to spawn
	 */
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> m_pVFXNiagaraComponent;
	
private:

	/**
	 * @brief Modify the health to add on interaction based on the current phase
	 */
	UFUNCTION()
	void OnRegenerate();

	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Health", meta = (DisplayName = "Max Health Points", ClampMin = "0", AllowPrivateAccess = true))
	int m_iMaxHealth;

	/**
	 * @brief The current health to add
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Health", meta = (DisplayName = "Current Health Points", ClampMin = "0", AllowPrivateAccess = true))
	int m_iCurrentHealth;

	/**
	 * @brief Holds the current phase
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Health", meta = (DisplayName = "Current Phase", ClampMin = "0", AllowPrivateAccess = true))
	int32 m_iCurrentPhase;

	/**
	 * @brief The timer handler for the current phase
	 */
	UPROPERTY()
	FTimerHandle m_fthRechargeTimer;

	bool bItsfirstTimeUsing = true;
};
