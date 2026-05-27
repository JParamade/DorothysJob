/************************************************************************
 * @description: Represents the Base Actor in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseActor.generated.h"




UCLASS()
class DOROTHYSJOB_API ABaseActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default Constructor of Base Actor
	 */
	ABaseActor();

  /**
 * @brief It is called every frame
 * @param DeltaTime Time elapsed since last frame
 */
  virtual void Tick(float DeltaTime) override;

protected:
	/**
	 * @brief It is called when game starts or when spawned
	 */
	virtual void BeginPlay() override;



};
