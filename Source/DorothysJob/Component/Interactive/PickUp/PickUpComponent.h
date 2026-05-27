/************************************************************************
 * @description: Represents the Base Pick Up Component
 * @author: Jorge Duart & Pablo Velasco
 * @date: 05/03/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "../InteractiveComponent.h"
#include "PickUpComponent.generated.h"


/**
 * 
 */
UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UPickUpComponent : public UInteractiveComponent
{
	GENERATED_BODY()
	

public:

protected:
	/**
	 * @brief Register custom function BeginOverlap to respective BoxComponent delegate.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Unregister custom function BeginOverlap to respective BoxComponent delegate.
	 * @param EndPlayReason
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Custom Base BeginOverlap function.
	 * Unregister overlap event, calls interact function and destroy actor at the end.
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 * @param bFromSweep 
	 * @param SweepResult 
	 */
	UFUNCTION()
	virtual void OnPickUpBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	/**
	 * @brief Base PickUp funcionality. Children of this class should override it.
	 */
	virtual void Interact() override;

private:
};
