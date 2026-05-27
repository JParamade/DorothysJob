/************************************************************************
 * @description: Represents the Base Interactive Component
 * @author: Jorge Duart & Pablo Velasco
 * @date: 05/03/2025
 * @edited_by: 
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractiveComponent.generated.h"

/**
 * 
 */
class ABasePlayer;

UCLASS()

class DOROTHYSJOB_API UInteractiveComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	 /**
	  * @brief UInteractiveComponent constructor. Sets collision and trigger size.
	  */
	 UInteractiveComponent();

protected:

	/**
	 * @brief Base interaction funcionality. Children of this class should override it.
	 */
	virtual void Interact(){};
	
private:
	
};
