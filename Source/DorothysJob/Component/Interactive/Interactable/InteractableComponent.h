/************************************************************************
 * @description: Represents the Base Interactable Component
 * @author: Jorge Duart & Pablo Velasco
 * @date: 05/03/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "../InteractiveComponent.h"
#include "InteractableComponent.generated.h"

// Widget
class UBaseWidget;
class UWidgetComponent;

UCLASS()
class DOROTHYSJOB_API UInteractableComponent : public UInteractiveComponent
{
	GENERATED_BODY()

public:
	/**
	 * @brief UInteractableComponent constructor
	 */
	UInteractableComponent();

	/**
	 * @brief Variable m_bIsOverlapping getter
	 * @return True if the player is overlapping, False otherwise.
	 */
	bool IsOverlap() const;


protected:
	/**
	 * @brief @TOFILL.
	 */
	virtual void OnRegister() override;

	/**
	 * @brief Register custom functions Begin/End Overlap to their respectives BoxComponent delegates.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief Unregister custom functions Begin/End Overlap to their respectives BoxComponent delegates.
	 * @param EndPlayReason 
	 */
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	 * @brief Custom Base BeginOverlap function. 
	 * Sets isOverlapping to True, stores player and binds to player delegate so it can interact pressing a key.
	 * 
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 * @param bFromSweep 
	 * @param SweepResult 
	 */
	UFUNCTION()
	virtual void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult);

	/**
	 * @brief Custom Base EndOverlap function.
	 * Sets isOverlapping to False, sets player variable to null and unbind to player delegate.
	 * @param OverlappedComponent 
	 * @param OtherActor 
	 * @param OtherComp 
	 * @param OtherBodyIndex 
	 */
	UFUNCTION()
	virtual void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/**
	 * @brief Base interactable funcionality. Children of this class should override it.
	 */
	virtual void Interact() override;

	bool m_bIsOverlapping;
	TObjectPtr<ABasePlayer> m_pPlayer; 

	UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Interact Prompt Class"))
	TSubclassOf<UBaseWidget> m_oInteractPromptClass;
	UPROPERTY(EditAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Relative Position"))
	FVector m_vRelativePosition;

	UWidgetComponent* m_pInteractPromptWidget;
};
