/************************************************************************
 * @description: Represents the Weapon Dusty
 *							 This class uses the components to Attack and Clean
 * @author: Raul
 * @date: -/-/2025
 * @edited_by: Josephine - 03/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "Dusty.generated.h"

class UDustyTradicionalHandle;
class UDustyWeaponStatsDataAsset;
class UDustyHummingbirdFeathers;
class USpecialAttackDustyComponent;
class UBaseDash;
class UNiagaraSystem;
class UBaseStatus;
class UFMODEvent;
class UNiagaraComponent;
class ABasePlayer;

/**
 * Represents the Weapon Dusty
 */
UCLASS()
class DOROTHYSJOB_API ADusty : public ABaseWeapon
{
  GENERATED_BODY()

public:

  ADusty();

  bool isCleaningVelocity = false;

#pragma region | Effects
  UPROPERTY()
  TObjectPtr<UNiagaraComponent> pEffectComponentCleaningDust;

  UPROPERTY()
  TObjectPtr<UNiagaraComponent> pEffectComponentCleaningCircle;

  UFUNCTION(BlueprintCallable)
  void ShowVFXCleanig();
#pragma endregion

#pragma region | Default Special Attack
  /**
   * @brief The total charge timer for the whirl. This sets also the max timer to hold the action
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Special Attack", meta = (DisplayName = "Whirl Charge"))
  float m_fWhirlCharge = 0.2f;

  /**
   * @brief The stun effect to apply with the special attack
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Status", meta = (DisplayName = "Special Attack Stun Status"))
  TSubclassOf<UBaseStatus> m_pStunEffect;

  /**
   * @brief The invincibility effect to apply during the final attack of the combo
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Status", meta = (DisplayName = "Final Attack Invincibility Status"))
  TSubclassOf<UBaseStatus> m_pAttackInvincibilityEffect;
#pragma endregion

#pragma region | Audio
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Clean FMOD Event"))
  TObjectPtr<UFMODEvent> m_pCleanEvent;
#pragma endregion

protected:

  virtual void BeginPlay() override;

  virtual void ComboStateChanged(bool _bStarts) override;

  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

  TObjectPtr<UDustyWeaponStatsDataAsset> m_pDataAsset;

private:

#pragma region | Components
  UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Handle (Attack) Component", AllowPrivateAccess = true))
  TObjectPtr<UDustyTradicionalHandle> m_pHandle;

  UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, meta = (DisplayName = "Feathers (Clean) Component", AllowPrivateAccess = true))
  TObjectPtr<UDustyHummingbirdFeathers> m_pFeathers;

  UPROPERTY(VisibleAnywhere, Category = "Special Attack", meta = (DisplayName = "Special Attack Component"))
  TObjectPtr<USpecialAttackDustyComponent> m_pSpecialAttack;
#pragma endregion

#pragma region | Action Functions
  virtual void MeleeAttackHitEnd() override;

  virtual void Attack() override;

  virtual void ReleaseAttack() override;

  virtual void SpecialAttack() override;

  virtual void Clean() override;

  virtual void ReleaseClean() override;
#pragma endregion

  bool auxDashCombo = false;

  UPROPERTY()
  float m_fCurrentWhirlCharge = 0.0f;

  const float kDashComboVelocityScale = 0.1f;

  const float kDashComboVelocityScaleInversed = 1.0f /0.1f;
};
