/************************************************************************
 * @description: Isometric Camera Collision Handler Component
 * @author: Aurora
 * @date: [01/07/2025]
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraCollisionHandler.generated.h"

class ABaseCamera;

/**
 * @brief Component in charge of the camera hits and collision with raycast.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API UCameraCollisionHandler : public UActorComponent
{
	GENERATED_BODY()

public:	

	/** Default constructor */
	UCameraCollisionHandler();

protected:

	/** Execute at the begining of the game */
	virtual void BeginPlay() override;

public:

	/**
	 * @brief Function execute every frame.
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool GetDoorHit();

	/**
	 * @brief Calculate when the player is near a door an when it's going to cross the door for the zoom.
	 */
	void CalculateDoorCollision();

private:

	//-------------------------------WARNING! IS THIS NEEDED?-----------------------------------------------//
	/**
	 * @brief If a door is detected, handle the result of that detection, if it crossing, if it has to do a zoom, etc...
	 * @param Hit: The hit result.
	 * @param Direction: The direction of the door from the target.
	 */
	void HandleDoorHit(const FHitResult& Hit, const FVector& Direction);
	//------------------------------------------------------------------------------------------------------//

#pragma region Edit Variables
	/** Camera margin distance */
	UPROPERTY(EditAnywhere, Category = "Bola 13 | Door Collision", meta = (DisplayName = "Door Detection Distance"))
	float m_fMarginDistance = 200.f;

	/** Camera door detection distance for the zoom */
	UPROPERTY(EditAnywhere, Category = "Bola 13 | Door Collision", meta = (DisplayName = "Door Distance"))
	float m_fDoorDistance = 0.0f;
#pragma endregion

#pragma region Varibales
	/** The owner of the component*/
	ABaseCamera* m_pCameraOwner;

	/** The actor position taht teh camara is following */
	FVector m_vTargetPosition;

	/** */
	float m_fCurrentDot = 0.f;

	/** */
	float m_fDoorZoomDistance = 0.f;

	/** */
	bool m_bIsDoorHit;
#pragma endregion

public:

#pragma region Getters&Setters
	/** */
	float GetCurrentDot();

	/** */
	float GetDoorZoomDistance();
#pragma endregion
};
