/************************************************************************
 * @description: Represents the Dirtminator Weapon
 * @author: Pablo Velasco
 * @date: 06/05/2025
 * @edited_by: Aurora Maria Fernández - 21/08/2025
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "Dirtminator.generated.h"

class URocketLauncherComponent;
class UVectorVacuumingComponent;
class UDirtminatorSAComponent;
class UHoseComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class UFMODEvent;
class ABasePlayer;

/**
 * @brief Class of the Dirminator weapon.
 */
UCLASS()
class DOROTHYSJOB_API ADirtminator : public ABaseWeapon
{
	GENERATED_BODY()

public:

	/**
	 * @brief Default Dirtminator's Constructor
	 */
	ADirtminator();

protected:

#pragma region Components
	/** The cleaning component*/
  UPROPERTY(VisibleAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Hose Module Component"))
  TObjectPtr<UVectorVacuumingComponent> m_pVacumCleanComponent;

	/** The attack component */
	UPROPERTY(VisibleAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Engine Module Component"))
	TObjectPtr<URocketLauncherComponent> m_pRocketLauncherComponent;

	/** The special attack component */
	UPROPERTY(VisibleAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Special Attack Component"))
	TObjectPtr<UDirtminatorSAComponent> m_pSpecialAttackComponent;

  /** The aim VFX */
  UPROPERTY(EditAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Aim VFX"))
  TObjectPtr<UNiagaraComponent> m_pAimVFX;

	/** The charge VFX */
  UPROPERTY(EditAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Charge VFX"))
  TObjectPtr<UNiagaraComponent> m_pChargeVFX;

	TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;
#pragma endregion

	virtual void BeginPlay() override;

#pragma region Base Actions
	/**
	 * @brief Attacks.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Attack() override;

	/**
	 * @brief Release attack action.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ReleaseAttack() override;

	/**
	 * @brief Cleans.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Clean() override;

	/**
	 * @brief Release cleaning action.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void ReleaseClean() override;

	/**
	 * @brief Special attack action.
	 */
	UFUNCTION(BlueprintCallable)
	virtual void SpecialAttack() override;
#pragma endregion

public:

#pragma region Getters & Setters
	/**
	 * @brief Sets a new VFX
	 * @param _pNewVFX New VFX
	 */
	void SetChargeVFX(UNiagaraSystem* _pNewVFX);

	/**
	 * @brief Activate aim VFX
	 * @param _pNewVFX Activate VFX
	 */
	void SetActiveAimVFX(bool _bIsActive, UNiagaraSystem* _niagara);

	virtual void SetCooldownMultiplier(float _fCooldownMultiplier) override;

	virtual void ResetCooldownMultiplier() override;

#pragma endregion

};
