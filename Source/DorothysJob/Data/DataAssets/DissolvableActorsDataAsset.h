/************************************************************************
 * @description: Data Asset for the furniture tah has to be sometimes in half.
 * @author: Aurora
 * @date: [02/07/2025]
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DissolvableActorsDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FDissolvablePair
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> FullActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> HalfActor;
};

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UDissolvableActorsDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dissolvable")
	TArray<FDissolvablePair> DissolvableActors;

	// ? Runtime-efficient map: nombre ? struct con ambos actores
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dissolvable")
	TMap<FName, FDissolvablePair> DissolvableMap;
};
