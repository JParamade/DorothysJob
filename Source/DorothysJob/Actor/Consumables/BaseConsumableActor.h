/************************************************************************
 * @description: Represents the base consumable actor.
 * @author: Jaime Paramo
 * @date: 01/07/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "BaseConsumableActor.generated.h"

class UCapsuleComponent;
class UFMODAudioComponent;

UCLASS()
class DOROTHYSJOB_API ABaseConsumableActor : public ABaseActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief -> Default constructor for base consumables.
	 */
	ABaseConsumableActor();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Collision Component"))
	TObjectPtr<UCapsuleComponent> m_pCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Skeletal Mesh"))
	TObjectPtr<USkeletalMeshComponent> m_pMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Audio Component"))
	TObjectPtr<UFMODAudioComponent> m_pAudioComponent;
};