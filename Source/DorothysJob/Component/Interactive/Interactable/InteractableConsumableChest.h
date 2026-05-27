/************************************************************************
 * @description: Represents the interactable consumable chest
 * @author: Josephine Esposito
 * @date: 18/08/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Interactive/Interactable/InteractableComponent.h"
#include "InteractableConsumableChest.generated.h"

class USelectConsumablesMenu;
class UBaseGameInstance;

UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UInteractableConsumableChest : public UInteractableComponent
{
	GENERATED_BODY()
	
public:
	UInteractableConsumableChest();

protected:

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;

	virtual void OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
		const FHitResult& SweepResult) override;

	virtual void OnInteractableEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13", meta = (DisplayName = "Consumables Change Menu"))
	TSubclassOf<USelectConsumablesMenu> SelectConsumablesMenuWidget;

private:

	TObjectPtr<USelectConsumablesMenu> m_pConsumablesMenuInstance;

	TObjectPtr<UBaseGameInstance> m_pGameInstance;
};
