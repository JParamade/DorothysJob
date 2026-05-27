/************************************************************************
 * @description: Represents the Base Spawn Point in the Game.
 * @author: Pablo Velasco
 * @date: 25/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "BaseSpawnPoint.generated.h"

class UBaseRoom;

UCLASS()
class DOROTHYSJOB_API ABaseSpawnPoint : public ABaseActor
{
	GENERATED_BODY()
public:
	ABaseSpawnPoint();
protected:

private:
	void TryRegisterInRoom(UBaseRoom* Room);
};
