/************************************************************************
 * @description: Represents the GameMode for Procedural Generation
 * @author: Pablo Velasco
 * @date: 04/07/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "BaseGameMode.h"
#include "ProceduralGameMode.generated.h"

/**
 *
 */
UCLASS()
class DOROTHYSJOB_API AProceduralGameMode : public ABaseGameMode
{
  GENERATED_BODY()
public:
  AProceduralGameMode();

  void SpawnPlayerAt(const FVector& Location, const FRotator& Rotation);

};
