/************************************************************************
 * @description: Isometric Camera Component
 * @author: Aurora
 * @date: [01/07/2025]
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CameraLookAheadComponent.generated.h"


/**
 * @brief Component in charge of the camera Look Ahead or Around.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API UCameraLookAheadComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	/* Sets default values for this component's properties */
	UCameraLookAheadComponent();

protected:

	/* Called when the game starts */
	virtual void BeginPlay() override;

public:

	/**
	 * @brief Calculate look aheado offset
	 */
	void CalculateLookAheadOffset();

//--------This may be not necessary in the future----------------- 
	/**
	 * @brief Stop look ahead in the direction of the look ahead
	 * @param Direction 
	 */
	void StopLookAheadIfAligned(const FVector& Direction);

	/**
	 * @brief Step look ahead complete
	 */
	void StopLookAhead();
//-----------------------------------------------------------------
private:

#pragma region Edit Variables
	/* Look Ahead Max distance */
	UPROPERTY(EditAnywhere, Category = "Bola13|Look Ahead", meta = (DisplayName = "Look Ahead Max Distance"))
	float m_fLookAheadMaxDistance;

	/* Look Ahead for 4 directions */
	UPROPERTY(EditAnywhere, Category = "Bola13|Look Ahead", meta = (DisplayName = "Activate Look Ahead In 4 Direction"))
	bool m_bLookIn4Directions = false;
#pragma endregion

#pragma region Variables
	/* Mouse position for look ahead function */
	FVector m_pRelativeMousePos;

	/* Direction of the mouse */
	FVector2D m_vMouseDirection;

	/* Look Ahead Offset result */
	FVector m_vLookAheadOffset;
#pragma endregion
	/**
	 * @brief Function to calculate the look ahead offset only in 4 directions
	 */
	void FourDirectionsLookAt();

	/**
	 * @brief Function to calculate the look ahead offset in all directions
	 */
	void NormalLookAt();

public:

#pragma region Getters&Setters
	/*Get the look ahead offset */
	FVector GetLookAheadOffset();

	/* Set the target position */
	void SetTargetPosition(const FVector& RelativeMousePos);
#pragma endregion
		
};
