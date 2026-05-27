/************************************************************************
 * @description: Represents the Base Player in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: David Gonzalez
 * @edited_by: Jaime Paramo
 * @edited_by: Raul
 * @edited_by: Josephine Esposito
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "BasePlayer.generated.h"

class ABaseWeapon;
class ABaseCamera;
class UBaseDash;
class UDrawing;
class UWidgetComponent;
class UBaseConsumable;
class UPlayerPropertiesDataAsset;
class UNiagaraSystem;
struct FInputActionValue;
enum class EWeaponAttackType : uint8;
class UTimeManager;
class ULevelManager;

DECLARE_MULTICAST_DELEGATE(FOnAction);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDashModeChange, bool /* _bIsDashing */);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponChange, EDirtType /* _eAttackType */, EWeaponAttackType /* _eWeaponAttackType */)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnConsumableUpdated, int32 /* _iConsumableStack */, int32 /* _iConsumibleID */);

UCLASS()
class DOROTHYSJOB_API ABasePlayer : public ABaseCharacter
{
  GENERATED_BODY()

public:

  /**
   * @brief Default constructor of Base Player.
   */
  ABasePlayer();

  /**
   * @brief It is called when game starts or when spawned.
   */
  virtual void BeginPlay() override;

  /**
   * @brief It is called every tick.
   * @param _fDeltaTime -> Time since the last function call.
   */
  virtual void Tick(float _fDeltaTime) override;

  /**
   * @brief It is called when the game ends.
   */
  virtual void EndPlay(const EEndPlayReason::Type _eEndPlayReason) override;

  /**
   * @brief Called to bind functionality to input.
   * @param _pPlayerInputComponent -> Player input component.
   */
  virtual void SetupPlayerInputComponent(UInputComponent* _pPlayerInputComponent) override;

#pragma region | Stats
  /**
   * @brief It is called when the character receives a damage hit.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   * @param _eAttackType -> Type to calculate the hit effectiveness.
   * @param _vHitLocation -> Hit impact location.
   * @param _vHitRotation -> Hit impact rotation.
   * @param _bIsCritical -> Is Critical hit.
   * @return The real damage after apply bonus and mitigation effects.
   */
  virtual int32 Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation = FVector::ZeroVector, const FRotator& _vHitRotation = FRotator::ZeroRotator, bool _bIsCritical = false) override;

  /**
   * @brief It is called when the player is healed. It also restores the stinky state.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   */
  virtual void Heal(int32 _iModifyHealth) override;

  /**
   * @brief Applies Pristine Time VFX
   */
  void PristineTimeVFX() const;

  /**
   * @brief It returns the current weapon special charge.
   */
  float GetCurrentWeaponSpecialCharge();

  /**
   * @brief It changes the charge percentage in the current weapon special charge.
   * @param _fValue -> New percentage.
   */
  void SetCurrentWeaponSpecialCharge(float _fValue);

  /**
   * @brief The base method that will modify the stats of the character.
   * @param _mStats The value to add or multiply to the stat
   */
  virtual void ModifyStats(const TMap<EModifiableStats, float> _mStats) override;

  /**
   * @brief The base method that will modify the stats of the character
   * @param _mStats The value to rest or divide from the stat
   */
  virtual void RevertStats(const TMap<EModifiableStats, float> _mStats) override;
#pragma endregion

#pragma region | Getters
  /**
   * @brief It returns the current attached weapon.
   */
  UFUNCTION(BlueprintCallable)
  ABaseWeapon* GetCurrentWeapon() const;

  /**
   * @brief It returns a reference to the current selected weapons.
   */
  TArray<TObjectPtr<ABaseWeapon>>& GetCurrentWeapons();

  /**
   * @brief It returns the character current rotation.
   */
  FRotator GetLookAtRotation() const;

  /**
   * @brief It returns the player active camera.
   */
  ABaseCamera* GetCameraComponent() const;

  /**
   * @brief It Returns the player's current room
   */
  virtual UBaseRoom* GetCurrentRoom() override;

  /**
   * @brief Returns the attack speed multiplier
   */
  float GetAttackSpeedMultiplier() const;

  /**
   * @brief Get if the player is stinky
   * @return A bool of if is stinky or not
   */
  bool GetIsPlayerStinky() const;

  /**
   * @brief Get if the player is in B13 state.
   * @return A bool of if is with B13 or no.
   */
  bool GetIsPlayerWithB13() const;

  int32 GetPrimaryConsumableAmount() const;

  int32 GetSecondaryConsumableAmount() const;
#pragma endregion

#pragma region | Camera
  /**
   * @brief It changes the active camera to the player.
   */
  UFUNCTION(BlueprintCallable)
  void SetControllerCamera();
#pragma endregion

#pragma region | Weapon
  /**
   * @brief It spawns the current selected weapons and removes the old ones.
   */
  void SpawnSelectedWeapons();

  /**
   * @brief Delegate to execute when the player changes between the available weapons.
   */
  FOnWeaponChange OnWeaponChange;
#pragma endregion

#pragma region | Consumables
  /**
   * @brief It spawns the current selected consumables and removes the old ones.
   */
  void SetSelectedConsumables();

  /**
   * @brief It adds the reward consumable to the stack.
   * @param _iConsumableID -> Consumable stack ID.
   */
  void AddConsumableCharge(int32 _iConsumableID);

  /**
   * @brief It starts the consumable effects.
   */
  void OnConsumableEffectStart(float _fMovementSpeedMultiplier, float _fAttackSpeedMultipliere);

  /**
   * @brief It ends the consumable effects.
   */
  void OnConsumableEffectEnd();

  /**
   * @brief It updates the available consumable info.
   */
  UFUNCTION(BlueprintCallable)
  void UpdateConsumableInfo();

  void SetPrimaryConsumableAmount(uint32 _NewAmount);

  void SetSecondaryConsumableAmount(uint32 _NewAmount);

  /**
   * @brief Delegate to execute when the available consumable number changes.
   */
  FOnConsumableUpdated OnConsumableUpdated;
  FOnConsumableUpdated OnConsumableUnavailable;
#pragma endregion

#pragma region | Movement
  /**
   * @brief It changes the player current movement velocity.
   * @param _fVelocityMultiplicator -> Velocity multiplicator.
   * @param _fRotationMultiplicator -> Rotator multiplicator.
   */
  void ChangeMovementVelocity(float _fVelocityMultiplicator, float _fRotationMultiplicator = 1.f);

  /**
   * @brief It performs a dash.
   * @param _vDirection -> Dash direction.
   * @param _fDistance -> Dash distance.
   * @param _fTime -> Dash time.
   */
  void PerformDash(const FVector& _vDirection, float _fDistance, float _fTime);

  /**
   * @brief Delegate to execute when the player dash mode changes.
   */
  FOnDashModeChange OnDashModeChange;
#pragma endregion

#pragma region | Interaction
  /**
   * @brief Delegate to execute when the player interacts with the environment.
   */
  FOnAction OnInteract;
#pragma endregion

  UFUNCTION(BlueprintImplementableEvent, Category = "Bola 13|Aim Assist")
  void CalculateAimAssist();

  /**
   * @brief 
   */
  bool GetCanPlayAudio() const;
  /**
   * @brief 
   */
  void SetCanPlayAudio(bool _bValue);

protected:

#pragma region | Game Over
  /**
   * @brief It is callen when the current health is less than 0.
   */
  virtual void GameOver() override;
#pragma endregion

#pragma region | Skin
  /**
   * @brief It sets the base material to Base Player.
   */
  virtual void SetBaseMaterial() override;
#pragma endregion

private:
#pragma region | Input Action Functions
  /**
   * @brief It's called for movement input.
   * @param _oIAValue -> Value generated for the input.
   */
  void Move(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for looking input.
   * @param _oIAValue -> Value generated for the input.
   */
  void LookAt(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called when looking input is completed.
   * @param _oIAValue -> Value generated for the input.
   */
  void LookAtComplete(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for attack input.
   * @param _oIAValue -> Value generated for the input.
   */
  void Attack(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called when released attack input.
   * @param _oIAValue -> Value generated for the input.
   */
  void ReleaseAttack(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for clean input.
   * @param _oIAValue -> Value generated for the input.
   */
  void Clean(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called when released clean input.
   * @param _oIAValue -> Value generated for the input.
   */
  void ReleaseClean(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for dash input.
   * @param _oIAValue -> Value generated for the input.
   */
  void Dash(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for interact input.
   * @param _oIAValue -> Value generated for the input.
   */
  void Interact(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for special attack input.
   * @param _oIAValue -> Value generated for the input.
   */
  void SpecialAttack(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called when released special attack input.
   * @param _oIAValue -> Value generated for the input.
   */
  void ReleaseSpecialAttack(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for pause input.
   * @param _oIAValue -> Value generated for the input.
   */
  void Pause(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for change weapon input.
   * @param _oIAValue -> Value generated for the input.
   */
  void WeaponChange(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for primary consumable use input.
   * @param _oIAValue -> Value generated for the input.
   */
  void UsePrimaryConsumable(const FInputActionValue& _oIAValue);

  /**
   * @brief It's called for secondary consumable use input.
   * @param _oIAValue -> Value generated for the input.
   */
  void UseSecondaryConsumable(const FInputActionValue& _oIAValue);
#pragma endregion

#pragma region | Private Functions
  /**
   * @brief It selects the current weapon
   * @param _iCurrentWeaponIndex -> Weapon index to select.
   */
  void SelectWeapon(int32 _iCurrentWeaponIndex);

  /**
   * @brief It is called when the Base Player dash ends.
   */
  void StopDash();

  /**
   * @brief It resets the dash cooldown.
   */
  void ResetDash();

  /**
   * @brief It schedules the timer to dirt.
   */
  void ScheduleToDirt();

  /**
   * @brief Itdirts the floor when the Player is stinky.
   */
  void ToDirt();

  /**
   * @brief It uses an available consumable _iConsumableID.
   * @param _iConsumableID -> consumable ID used.
   */
  void UseConsumable(int32 _iConsumableID);

  /**
   * @brief It plays the animation montage.
   * @param _sSection -> Section to play.
   * @param _fPlayRate -> Play rate to reproduce the action montage.
   */
  void PlayAnimMontage(const FName& _sSection, float _fPlayRate = 1.0f);

  /**
   * @brief It jumps to the section in the animation montage.
   * @param _sSection -> Section to play.
   */
  void JumpSectionAnimMontage(const FName& _sSection);

  /**
   * @brief It is executed when the weapon shot ends.
   */
  UFUNCTION()
  void WeaponAlreadyShot();

  /**
   * @brief It is executed when the special attack is active.
   */
  UFUNCTION()
  void OnSpecialActive(bool _bISpecialActive);

  /**
   * @brief It is executed when the special weapon attack shot ends.
   */
  UFUNCTION()
  void WeaponSpecialAlreadyShot();

  /**
   * @brief It resets the action block parameters.
   */
  UFUNCTION()
  void ResetActionState();
#pragma endregion

#pragma region | Components
  /**
   * Player camera.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Camera Component", AllowPrivateAccess = "true"))
  TObjectPtr<ABaseCamera> m_pCurrentCamera;

  /**
   * Drawing component used to clean action or dirt when the player is stinky.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Drawing Component", AllowPrivateAccess = "true"))
  TObjectPtr<UDrawing> m_pDrawingComponent;
#pragma endregion

#pragma region | Skin Parameters
  /**
   * It's the material when Dorothy is Dirty.
   */
  TArray<TObjectPtr<UMaterialInterface>> m_lDirtyMaterial;

  /**
   * It's true when Dorothy is Dirty.
   */
  bool m_bStinky;
#pragma endregion

#pragma region | Player Parameters
  /**
   * Character current rotation.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Aim", meta = (DisplayName = "Player Look At", AllowPrivateAccess = "true"))
  FRotator m_rLookAtRotation;

  /**
   * It's the list with the current selected consumables.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Consumables", meta = (DisplayName = "Selected Consumables", AllowPrivateAccess = "true"))
  TArray<TObjectPtr<UBaseConsumable>> m_lSelectedConsumables;

  /**
   * Player data asset to avoid cast.
   */
  TObjectPtr<UPlayerPropertiesDataAsset> m_pPlayerPropertiesDA;

  /**
   * It's the list with the current selected weapons.
   */
  TArray<TObjectPtr<ABaseWeapon>> m_lSpawnedWeapon;

  /**
   * It's the current player weapon.
   */
  TObjectPtr<ABaseWeapon> m_pCurrentWeapon;

  /**
   * It's the weapon list size.
   */
  int32 m_iWeaponsSize;

  /**
   * It's the current weapon index in the list.
   */
  int32 m_iCurrentWeaponIndex;

  /**
   * It is the rotation speed for the player.
   */
  float m_fRotInterpSpeed;

  /**
   * Param to control the rotation when the Player dirts.
   */
  float m_fDirtRotation;

  /**
   * Param to reset the speed multiplier after B13 effect expires.
   */
  float m_fB13SpeedMultiplier = 1.0f;

  /**
   * Param to reset the speed multiplier after B13 effect expires.
   */
  float m_fAttackSpeedMultiplier = 1.0f;

  /**
   * It shows when the player is attacking.
   */
  bool m_bIsAttacking;

  /**
   * It shows when the player is cleaning.
   */
  bool m_bIsCleaning;

  /**
   * It shows if the player is dashing.
   */
  bool m_bIsDashing;

  /**
   * It shows if the player can dash or not.
   */
  bool m_bCanDash;

  /**
   * It shows if the player can shoot or not.
   */
  bool m_bCanShoot;

  /**
   * It shows if the player can special shoot or not.
   */
  bool m_bCanSpecialShoot;

  /**
   * It shows if the special attack is active or not.
   */
  bool m_bIsSpecialActive;

  /**
   * It indicate if the player is using the LookAt behavior or not.
   */
  bool m_bIsUsingLookAt;

  /**
   * It is the timer to control invulnerable time after damage.
   */
  FTimerHandle m_tInvulnerableTimerHandle;

  /**
   * It is the timer to stop the dash.
   */
  FTimerHandle m_tDashStopTimerHandle;

  /**
   * It resets the dash action.
   */
  FTimerHandle m_tDashCooldownTimerHandle;

  /**
   * It resets the dirt action.
   */
  FTimerHandle m_tDirtCooldownTimerHandle;

  /**
   * Feedback Handle
   */
  FDynamicForceFeedbackHandle m_oFeedbackHandle;
#pragma endregion

#pragma region | Audio Interpolation Parameters
  /**
   * @brief
   */
  UPROPERTY()
  TObjectPtr<UTimeManager> m_pTimeManager;

  /**
 * @brief
 */
  UPROPERTY()
  TObjectPtr<ULevelManager> m_pLevelManager;

  /**
   * @brief Current FMOD Audio Parameter value.
   */
  float m_fCurrentAudioValue = 0.f;
  /**
   * @brief Target FMOD Audio Parameter value.
   */
  float m_fTargetAudioValue = 0.f;
  /**
   * @brief Interpolation speed between current and target FMOD Audio Parameter values.
   */
  float m_fInterpAudioSpeed = 2.f;

  /**
   * @brief Flag that handles when to start interpolating.
   */
  bool m_bStartAudioInterp = false;

  /**
   * @brief 
   * @param _sParameterName 
   * @param _fDeltaTime 
   */
  void InterpolateAudio(FName _sParameterName, bool& _bStartInterpolating, float& _fCurrentValue, float _fTargetValue, float _fInterpolationSpeed, float _fDeltaTime);

  /**
   * @brief
   */
  bool m_bCanPlayUnavailableSound = true;
#pragma endregion 
};