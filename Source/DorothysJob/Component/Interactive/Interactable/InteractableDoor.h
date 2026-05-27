/************************************************************************
 * @description: Represents the interactable door of the HUB
 * @author: Josephine Esposito
 * @date: 09/09/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Interactive/Interactable/InteractableComponent.h"
#include "InteractableDoor.generated.h"

UCLASS(ClassGroup = Interaction, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UInteractableDoor : public UInteractableComponent
{
	GENERATED_BODY()
	
public:
	UInteractableDoor();

protected:

	virtual void BeginPlay() override;

	/**
	 * @brief Called on interaction
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Interact() override;
};