/************************************************************************
 * @description: Represents the Mrs Clean consumable actor.
 * @author: Jaime Paramo
 * @date: 01/07/2025
 * @edited_by: Raul Fernandez
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Consumables/BaseConsumableActor.h"
#include "DorothysJob/Interface/Damageable.h"
#include "MrsCleanActor.generated.h"

class UNiagaraComponent;
class UNiagaraSystem;
class UFMODEvent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDecoyDestroyed, AMrsCleanActor*);

UCLASS()
class DOROTHYSJOB_API AMrsCleanActor : public ABaseConsumableActor, public IDamageable
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief Default constructor for the decoy.
	 */
	AMrsCleanActor();

#pragma region | Stats
  /**
   * @brief It returns the decoy current health.
   * @return Decoy's Current Health.
   */
  virtual int32 GetCurrentHealth() const override;

  /**
   * @brief It returns the character current health.
   * @return Character's Current Health.
   */
  virtual void SetCurrentHealth(int32 _iDamage) override;

  /**
   * @brief It returns the decoy total health.
   */
  virtual int32 GetMaxHealth() const override;

  /**
   * @brief It is called when the decoy receives a damage hit.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   * @param _eAttackType -> Type to calculate the hit effectiveness.
   * @param _vHitLocation -> Hit impact location.
   * @param _vHitRotation -> Hit impact rotation.
   * @param _bIsCritical -> Is Critical hit.
   * @return The real damage after apply bonus and mitigation effects.
   */
  virtual int32 Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation = FVector::ZeroVector, const FRotator& _vHitRotation = FRotator::ZeroRotator, bool _bIsCritical = false) override;

  /**
   * @brief It is called when the decoy is healed.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   */
  virtual void Heal(int32 _iModifyHealth) override;
#pragma endregion

	FOnDecoyDestroyed OnDecoyDestroyed;

protected:

  /**
   * @brief It is called when the decoy is spawned.
   */
	virtual void BeginPlay() override;

private:

#pragma region | Private Functions
  /**
   * @brief It plays the FMOD Event _pFMODEvent.
   * @param _pFMODEvent -> Event to play.
   */
  void PlayFMODEvent(TObjectPtr<UFMODEvent> _pFMODEvent);

  /**
   * @brief It notifies the death of the decoy in the room.
   */
  void Death();

  /**
   * @brief It destroys the actor after the animation is ended.
   */
  UFUNCTION()
  void DestroyActor();
#pragma endregion

  /**
   * Niagara component to show MrsClean effects.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Niagara Effects", AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> m_pVFXEffectsComponent;

  /**
   * Action animation for the decoy
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Animations", meta = (DisplayName = "Action Montage", AllowPrivateAccess = "true"))
  TObjectPtr<UAnimMontage> m_pActionMontage;

	/**
	 * The decoy's life duration.
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Lifetime", meta = (DisplayName = "Duration"))
	float m_fLifetime;

	/**
	 * The decoy's max health.
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Health", meta = (DisplayName = "Max Health"))
	int32 m_iMaxHealth;

#pragma region | VFX
  /**
   * It will be used when decoy dies.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Dead VFX"))
  TObjectPtr<UNiagaraSystem> m_pDeadEffect;
#pragma endregion

#pragma region | Audio
  /**
   * It will be executed when the decoy spawns.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Spawn"))
  TObjectPtr<UFMODEvent> m_pSpawnEvent;

  /**
   * It will be executed when the decoy is damaged.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Damage"))
  TObjectPtr<UFMODEvent> m_pDamageEvent;

  /**
   * It will be executed when the decoy dies.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Death"))
  TObjectPtr<UFMODEvent> m_pDeathEvent;

  /**
   * The decoy's spawn sound enabled.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio Event", meta = (DisplayName = "Skelly Mode Enabled"))
  bool m_bSkellyModeEnabled;
#pragma endregion

	/**
	 * The decoy's current health.
	 */
	int32 m_iCurrentHealth;

	/**
	 * The timer to destroy the decoy if the enemies were not able to defeat it.
	 */
	FTimerHandle m_tTimerHandler;
};