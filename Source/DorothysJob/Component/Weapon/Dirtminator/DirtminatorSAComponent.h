/************************************************************************
 * @description: Represents the Special Attack of the Dirtminator
 * @author: Aurora Maria Fernandez
 * @date: 21/08/2025
 * @edited_by: 
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DirtminatorSAComponent.generated.h"

class ADirtminator;
class UDirtminatorWeaponStatsDataAsset;
class ADamageCollider;
class UNiagaraComponent;
class ABasePlayer;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API UDirtminatorSAComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDirtminatorSAComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief The damage collider class
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Special Attack", meta = (DisplayName = "Special Damage Collider Class"))
	TSubclassOf<ADamageCollider> m_pDirtminatorDamageColliderClass;

	void InitSpecialAttack();

private:
	UPROPERTY()
	TObjectPtr<ADamageCollider> m_pSpecialDamageCollider;

	UPROPERTY()
	TWeakObjectPtr<ADirtminator> m_pOwnerWeapon;

	UPROPERTY()
	TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

	UPROPERTY()
	TObjectPtr<UDirtminatorWeaponStatsDataAsset> m_pDataAsset;

	UPROPERTY()
	FTimerHandle SpecialAttackTimerHandle;
	
	UPROPERTY()
	FTimerHandle SpecialAttackEndTimerHandle;

	bool m_fIsSpecialAttackActive = false;
	float TickInterval = 0.1f;

	UFUNCTION()
	void SetSpecialAttackVariables();

	void SpecialAttackAction();

  void EndSpecialAttack();

	/**
	 * @brief Set the player velocity.
	 */
	void SetPlayerVelocity(bool _bActivate);

	void ApplyDamageToActor(AActor* _pOverlappedActor);

	TObjectPtr<UNiagaraComponent> m_pSpecialAttackVFX;
public:

	bool GetSpeciaAttackIsActive();
};
