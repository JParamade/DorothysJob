/************************************************************************
 * @description: Isometric Camera Zoom Handler Component
 * @author: Aurora
 * @date: [01/07/2025]
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/
// FOR THE MOMNET, THERE WONT BE A ZOOM IN THE GAME, THIS CLASS WONT BE USE.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraZoomControllerComponent.generated.h"

 /**
	* @brief Component in charge of handling teh camera zoom.
	*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API UCameraZoomControllerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraZoomControllerComponent();

	/**
	 * @brief Adjust de zoom depending of the distance to the door.
	 * @param DistanceToHit The distance to the door.
	 */
	void AdjustCameraZoom(float DistanceToHit);

private:

	float LastDistanceToDoor = -1.0f;

	/** Camera orthographic width */
	UPROPERTY(EditAnywhere, Category = "Bola 13 | Zoom Controller", meta = (DisplayName = "Ortho Width"))
	float m_fOrthoWidth;


public:

#pragma region Getters&Setters
	float GetOrthoWidth();
	void SetOrthoWidth(float _OrthoWidth);
#pragma endregion
};
