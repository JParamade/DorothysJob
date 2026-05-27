/************************************************************************
 * @description: Represents the Weapon Mopressor
 *							 This class uses the components to Attack and Clean
 * @author: Josephine
 * @date: 12/03/2025
 * @edited_by: Jorge Duart - 21/04/2025
 *						 Josephine   - 03/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "Mopressor.generated.h"

#pragma region | Forward Declarations
class UMopHandleComponent;  // mop handle
class UMopHeadComponent;	  // mop head
class UMopSpecialComponent; //mop special
class ADamageCollider;		  // custom damage collider
class UDrawing;						  // cleaning and dirt
class UNiagaraSystem;			  // VFX
class UNiagaraComponent;	  // VFX
class UMoppresorWeaponStatsDataAsset; //data asset
class ABasePlayer;
#pragma endregion

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMopressorAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMopressorAttackRelease);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMopressorCleanUse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMopressorCleanRelease);

/**
 * Represents the Weapon Mopressor
 */
UCLASS()
class DOROTHYSJOB_API AMopressor : public ABaseWeapon
{
  GENERATED_BODY()

public:
  /**
  * @brief Default Constructor of Mopressor
  */
  AMopressor();

#pragma region | GETTERS

  UFUNCTION(BlueprintPure)
  ADamageCollider* GetDamageCollider() const;

  UFUNCTION(BlueprintPure)
  ADamageCollider* GetSpecialDamageCollider() const;

  UFUNCTION(BlueprintPure)
  UDrawing* GetDrawingComponent() const;

  UMoppresorWeaponStatsDataAsset* GetMoppressorDataAsset() const;

  ADamageCollider* GetCleanDashCollider() const;
#pragma endregion

#pragma region | DELEGATES and EVENTS

  //Temporal, used for the improvised animation
  UPROPERTY(BlueprintAssignable)
  FOnMopressorAttack OnMopressorAttack;

  UPROPERTY(BlueprintAssignable)
  FOnMopressorAttackRelease OnMopressorAttackRelease;

  UPROPERTY(BlueprintAssignable)
  FOnMopressorCleanUse OnMopressorCleanUse;

  UPROPERTY(BlueprintAssignable)
  FOnMopressorCleanRelease OnMopressorCleanRelease;

#pragma endregion

  /**
   * @brief This will create the basic attack visual effect
   * @param _index The index of the array of visual effects for the basic attack
   * @return A Niagara Component of the spawned system
   */
  TObjectPtr<UNiagaraComponent> CreateBasicAttackParticle(int32 _index);

  /**
   * @brief Sets player speed depending on boolean parameter.
   */
  UFUNCTION()
  void SetPlayerVelocity(bool _bActivate, float _fMovementMultiplier = 1.f, float _fRotationMultiplier = 1.f);


#pragma region | Effects
  /**
   * @brief This will activate the cleaning VFX
   */
  void StartAttachedVFX(UNiagaraSystem* _pNiagaraEffect, FVector _offset);

  /**
   * @brief This will deactivate the cleaning VFX
   */
  void StopAttachedVFX(bool _immediate);
  /**
   * @brief Applies the Wave visual effect
   */
  UFUNCTION(BlueprintCallable)
  void CreateWaveVFX(float _damageRadius, float _cleanRadius, float _alpha);
#pragma endregion

  bool m_bSpecialOngoing = false;

  virtual void ComboStateChanged(bool _bStarts) override;

protected:

  /**
   * @brief The overriden BeginPlay of the weapon
   */
  virtual void BeginPlay() override;

#pragma region | ACTIONS

  /**
   * @brief This will be called when realising the attack input
   */
  UFUNCTION(BlueprintCallable)
  virtual void ReleaseAttack() override;

  virtual void MeleeAttackHitStart(int32 _iCurrentCombo) override;

  virtual void MeleeAttackHitEnd() override;

  /**
  * @brief This will be called when using the special attack input
  */
  UFUNCTION(BlueprintCallable)
  virtual void SpecialAttack() override;

  UFUNCTION(BlueprintCallable)
  virtual void ReleaseSpecialAttack() override;
  /**
  * @brief This will be called when using the clean input
  */
  UFUNCTION(BlueprintCallable)
  virtual void Clean() override;

  /**
   * @brief This will be called when releasing the clean input
   */
  UFUNCTION(BlueprintCallable)
  virtual void ReleaseClean() override;

#pragma endregion

private:

  UFUNCTION()
  void AttachToPlayer();

  void InitColliderComponent(ADamageCollider* _DamageColliderComponent, USceneComponent* _RootComponent);

#pragma region | Variables
  /**
   * @brief The damage collider class
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Basic Attack", meta = (DisplayName = "Damage Collider Class"))
  TSubclassOf<ADamageCollider> m_pDamageColliderClass;

  /**
   * @brief The last hit collider class
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Basic Attack", meta = (DisplayName = "Last Hit Collider Class"))
  TSubclassOf<ADamageCollider> m_pLastHitColliderClass;

  /**
   * @brief The special collider class
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Special Attack", meta = (DisplayName = "Special Damage Collider Class"))
  TSubclassOf<ADamageCollider> m_pSpecialDamageColliderClass;

  /**
   * @brief The clean dash collider class
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Clean", meta = (DisplayName = "Clean Dash Collider Class"))
  TSubclassOf<ADamageCollider> m_pCleanDashColliderClass;

  /**
   * @brief The Base attack damage collider object for the slash hits
   */
  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pDamageCollider = nullptr;

  /**
   * @brief The Last Hit attack damage collider object for the slash hits
   */
  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pLastHitCollider;

  /**
   * @brief The Special attack damage collider object
   */
  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pSpecialDamageCollider = nullptr;

  /**
   * @brief The Clean Dash status collider object
   */
  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pCleanDashCollider = nullptr;

  /**
   * @brief The component to use for the visual effects for the basic attacks
   */
  UPROPERTY()
  TObjectPtr<UNiagaraComponent> m_BasicAttackComponent;

  /**
   * @brief Component used to clean or draw
   */
  UPROPERTY(VisibleAnywhere, Category = "Bola 13|Cleaning", meta = (DisplayName = "Drawing component"))
  TObjectPtr<UDrawing> m_pDrawing;

  /**
   * @brief The base Niagara Component for the clean effect
   */
  UPROPERTY()
  TObjectPtr<UNiagaraComponent> m_pVFXNiagaraComponent = nullptr;

  /**
  * @brief Handles the moppresor cleaning
  */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Components", meta = (DisplayName = "Head"))
  TObjectPtr<UMopHeadComponent> m_pMopHead;

  /**
  * @brief Handles the moppresor attack
  */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Components", meta = (DisplayName = "Handle"))
  TObjectPtr<UMopHandleComponent> m_pHandle;

  /**
  * @brief Handles the moppresor special
  */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Components", meta = (DisplayName = "Special"))
  TObjectPtr<UMopSpecialComponent> m_pSpecial;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
  TObjectPtr<USceneComponent> m_pBasicAttackPivot;

  UPROPERTY();
  TObjectPtr<UMoppresorWeaponStatsDataAsset> m_pMopressorDataAsset = nullptr;

  UPROPERTY();
  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer = nullptr;

  UPROPERTY()
  bool m_bCanUseSpecial = true;
#pragma endregion
};