/************************************************************************
 * @description: Represents the Dirtminator Weapon
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/BaseActorComponent.h"
#include "BaseWeaponComponent.generated.h"

enum class EStatus : uint8;
class ABaseWeapon;
class ABasePlayer;

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UBaseWeaponComponent : public UBaseActorComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	/**
	 * @brief Virtual Use method that will be overwritten by implemented Weapon Component
	 */
	virtual void Use() {};

	/**
	 * @brief Virtual Use method that will be overwritten by implemented Weapon Component
	 * @param _target, A target actor for the action
	 */
	virtual void Use(AActor* _target) {};

	/**
	 * @brief Virtual Release method taht will be overwritten by implemented Weapon Component
	 */
	virtual void Release() {};


#pragma region Gets


	ABasePlayer* GetPlayerOwner();
#pragma endregion

protected:

	/**
	 * @brief If this component Attacks it may apply a Status Condition
	 */
	EStatus m_eAppliedStatus;


private:
	UPROPERTY();
	TObjectPtr<ABasePlayer> m_pPlayer;
};