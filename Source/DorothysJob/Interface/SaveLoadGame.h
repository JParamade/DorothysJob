/************************************************************************
 * @description: Represents the SaveGame Interface. This interface identifies who want to save data.
 * @author: Jorge Duart
 * @date: 05/03/2025
 * @edited_by:
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveLoadGame.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveLoadGame : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DOROTHYSJOB_API ISaveLoadGame
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * @brief Called after load actor data. Use it if you need custom changes on actor after load.
	 */
	UFUNCTION(BlueprintNativeEvent)
	void OnActorDataLoaded();

};
