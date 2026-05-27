/************************************************************************
 * @description: Isometric Camera  Border Handler Component
 * @author: Aurora
 * @date: [21/07/2025]
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraBorderHandlerComponent.generated.h"

class ABaseCamera;

/**
 * @brief Component in charge of the camera out of bounds.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API UCameraBorderHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraBorderHandlerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief Get the collision offset for when the camera quare is out of the room square.
	 * @return The offset.
	 */
	FVector GetBorderCollisionOffset();

	/**
	 * @brief Get the room square corners
	 */
	void GetAndSetRoomSize(int32 _iCurrentLevel);

	/**
 * @brief Get camera square corners.
 */
	void GetCameraSize();

private:

	UPROPERTY(EditAnywhere, Category = "Bola13 | Border Component");
	bool m_bPlayDebug = false;

	ABaseCamera* m_pCameraOwner;

	/** The actor position taht teh camara is following */
	FVector m_vTargetPosition;

	/** Room border square corners */
	TArray<FVector> m_vBorderPos;

	/** Camera border square corners */
	TArray<FVector> m_vCameraBorderPos;

	FVector m_vLastCollisionOffset;


};
