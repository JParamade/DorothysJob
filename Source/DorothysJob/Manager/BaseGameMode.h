/************************************************************************
 * @description: Represents the Base Game Mode.
 * @author: David Gonzalez
 * @date: 12/03/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BaseGameMode.generated.h"

UCLASS()
class DOROTHYSJOB_API ABaseGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	/**
	 * @brief Default Constructor of the Game Mode
	 */
	ABaseGameMode();
};
