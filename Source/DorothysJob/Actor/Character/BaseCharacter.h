/************************************************************************
 * @description: Represents the Base Character in the Game.
 *							 This class uses the Damageable Interface
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: David Gonzalez
 * @edited_by: Jaime Paramo
 * @edited_by: Josephine Esposito
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DorothysJob/Interface/Damageable.h"
#include "BaseCharacter.generated.h"

class UCapsuleComponent;
class UFloatingPawnMovement;
class UCharacterPropertiesDataAsset;
class UBaseStatus;
class UWidgetComponent;
class UFMODAudioComponent;
class UFMODEvent;
class UBaseRoom;
class UBaseStatus;
enum class EModifiableStats : uint8;

DECLARE_DELEGATE_TwoParams(FOnCharacterHealthChanged, int32 /* NewHealth */, int32 /* MaxHealth */);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusChanged, UBaseStatus*, Status, bool, bApplied);

UCLASS()
class DOROTHYSJOB_API ABaseCharacter : public APawn, public IDamageable
{
  GENERATED_BODY()

public:

  UPROPERTY(BlueprintAssignable)
  FOnStatusChanged OnStateChanged;
  /**
   * @brief Default constructor of the character.
   */
  ABaseCharacter();

  /**
   * @brief It is called when game starts or when spawned.
   */
  virtual void BeginPlay() override;

#pragma region | Stats
  /**
   * @brief It returns the character current health.
   * @return Character's Current Health.
   */
  virtual int32 GetCurrentHealth() const override;

  /**
   * @brief It returns the character current health.
   * @return Character's Current Health.
   */
  virtual void SetCurrentHealth(int32 _iDamage) override;

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
   * @brief It is called when the character is healed.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   */
  virtual void Heal(int32 _iModifyHealth) override;

  /**
   * @brief It returns the character total health.
   */
  virtual int32 GetMaxHealth() const override;

  /**
   * @brief It returns the actual mitigation value (Between 0 and 1).
   */
  float GetMitigation() const;

  /**
   * @brief It sets a new mitigation value.
   * @param _fNewMitigation -> New mitigation value (Should be between 0 and 1).
   */
  virtual void SetMitigation(float _fNewMitigation);

  /**
   * @brief It returns the actual bonus damage multiplier value.
   */
  float GetBonusDamageMultiplier() const;

  /**
  * @brief It sets a new bonus damage multiplier value.
  * @param _fBonusMultiplier -> New bonus damage multiplier value (Should be greater than 0).
  */
  void SetBonusDamageMultiplier(float _fBonusMultiplier);

  /**
   * @brief The base method that will modify the stats of the character.
   * @param _mStats -> Map with the values to add or multiply to the stats.
   */
  virtual void ModifyStats(const TMap<EModifiableStats, float> _mStats) PURE_VIRTUAL(ABaseCharacter::ModifyStats, );

  /**
   * @brief The base method that will modify the stats of the character
   * @param _mStats -> Map with the values to rest or divide from the stats.
   */
  virtual void RevertStats(const TMap<EModifiableStats, float> _mStats) PURE_VIRTUAL(ABaseCharacter::RevertStats, );

  /**
   * @brief Delegate to execute when the character current health changes.
   */
  FOnCharacterHealthChanged OnCharacterHealthChanged;
#pragma endregion

#pragma region | Status Effect
  /**
   * @brief It applies the Grimeling pollution efects to the character.
   */
  virtual void BePolluted();

  /**
   * @brief It applies the status _oStatus to the character.
   * @param _oStatus -> Status to apply. 
   */
  void ApplyStatus(TSubclassOf<UBaseStatus> _oStatus);

  /**
   * @brief It removes the status _oStatus to the character.
   * @param _oStatus -> Status to remove.
   */
  void RemoveStatus(TSubclassOf<UBaseStatus> _oStatus);

  /**
   * @brief It will apply speed up status.
   */
  virtual void SpeedUp();

  /**
   * @brief It returns if the character can do actions or not.
   */
  bool CanDoActions() const;

  /**
   * @brief It sets if the character can do actions according to _bCanDoActions.
   * @param _bCanDoActions -> Can do actions or not.
   */
  virtual void SetCanDoActions(bool _bCanDoActions);

  /**
   * @brief It returns if the character is invulnerable or not. 
   */
  bool IsInvulnerable();

  /**
   * @brief It change the vharacter invulnerability.
   * @param _bIsInvulnerable -> True if invulnerable.
   */
  void SetIsInvulnerable(bool _bIsInvulnerable);

  /**
   * @brief Clear all status effects from the Character
   */
  void ClearAllStatus();
#pragma endregion

#pragma region | Getters
  /**
   * @briefIt returns the character data asset properties.
   */
  UCharacterPropertiesDataAsset* GetDataAsset() const;

  /**
   * @brief It returns the character movement component.
   */
  virtual UPawnMovementComponent* GetMovementComponent() const override;

  /**
   * @brief It returns the character skeletal mesh.
   */
  USkeletalMeshComponent* GetSkeletalMesh() const;

  /**
   * @brief It returns the projectile SpawnPoint.
   */
  UFUNCTION(BlueprintCallable)
  USceneComponent* GetMuzzle() const;
#pragma endregion

#pragma region | Current Room
  /**
   * @brief It returns the room where is the enemy.
   */
  virtual UBaseRoom* GetCurrentRoom();

  /**
   * @brief It sets the room where is the enemy
   * @param _pNewCurrentRoom -> The room in the level.
   */
  void SetCurrentRoom(UBaseRoom* _pNewCurrentRoom);
#pragma endregion

  /**
 * Character current health.
 */
  int32 m_iCurrentHealth;

protected:

#pragma region | Game Over
  /**
   * @brief It is callen when the current health is less than 0.
   */
  virtual void GameOver();
#pragma endregion

#pragma region | Skin
  /**
   * @brief It sets the base material to Base Character.
   */
  virtual void SetBaseMaterial();

  /**
   * @brief It sets the damaged material to Base Character according to _fEffectiveness.
   * @param _fEffectiveness -> Impact effectiveness.
   */
  virtual void SetDamagedMaterial(float _fEffectiveness);
#pragma endregion

#pragma region | Audio
  /**
   * @brief It plays the FMOD Event _pFMODEvent.
   * @param _pFMODEvent -> Event to play.
   */
  void PlayFMODEvent(TObjectPtr<UFMODEvent> _pFMODEvent);
#pragma endregion

#pragma region | Feedback
  /**
   * @brief It shows feedback when the character receives a hit impact.
   * @param _vHitLocation -> hit impact location.
   * @param _vHitRotation -> hit impact rotation.
   * @param _fEffectiveness -> effectivemess to addapt the feedback.
   */
  virtual void ShowHitImpact(const FVector& _vHitLocation, const FRotator& _vHitRotation, float _fEffectiveness);

  /**
   * @brief It blinks between invulnerable and base materials.
   */
  void InvulnerableBlink();
#pragma endregion

#pragma region | Components
  /**
   * Character properties data asset.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Data", meta = (DisplayName = "Data Asset"))
  TObjectPtr<UCharacterPropertiesDataAsset> m_pCharacterDataAsset;

  /**
   * Character movement component.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Movement", AllowPrivateAccess = "true"))
  TObjectPtr<UFloatingPawnMovement> m_pMovementComponent;

  /**
   * Character collision component.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Collision", AllowPrivateAccess = "true"))
  TObjectPtr<UCapsuleComponent> m_pCollisionComponent;

  /**
   * Character mesh component.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Mesh", AllowPrivateAccess = "true"))
  TObjectPtr<USkeletalMeshComponent> m_pMeshComponent;

  /**
   * Character audio component.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Audio", AllowPrivateAccess = "true"))
  TObjectPtr<UFMODAudioComponent> m_pAudioComponent;

  /**
   * It is used to spwan the weapons projectile.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Projectile Spawn Point", AllowPrivateAccess = "true"))
  TObjectPtr<USceneComponent> m_pProjectileSpawnPoint;
#pragma endregion

#pragma region Character Parameters
  /**
   * It resets material timer when the Base character gets hurt.
   */
  FTimerHandle m_tTimerResetMaterial;

  /**
   * It resets blinksbetween base and invulnerable materials.
   */
  UPROPERTY()
  FTimerHandle m_tTimerBlinkMaterial;

  /**
   * It is the blink count.
   */
  int32 m_iBlinkCount;

  /**
   * True if the blink effect when the character is invulnerable is active.
   */
  bool m_bBlinkActive;

  /**
   * It shows if the character can perform actions or not.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Stats", meta = (DisplayName = "Can Attack"))
  bool m_bCanDoActions;

  /**
   * It shows the current mitigation in the received damage.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Stats", meta = (DisplayName = "Mitigation multiplier"))
  float m_fMitigation;

  /**
   * It shows the current bonus multiplier.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Stats", meta = (DisplayName = "Bonus Multiplier"))
  float m_fBonusMultiplier;

  /**
   * It shows the character active status. 
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Status", meta = (DisplayName = "Active Status"))
  TMap<TSubclassOf<UBaseStatus>, UBaseStatus*> m_mActiveStatus;

  /**
   * It controls if the character is invulnerable.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Stats", meta = (DisplayName = "Is Invulnerable"))
  bool m_bIsInvulnerable;
#pragma endregion

#pragma region | Capsule Parameters
  /**
   * It is the enemy capsule radius.
   */
  float m_fCapsuleRadius;

  /**
   * It is the enemy capsule half height.
   */
  float m_fCapsuleHalfHeigth;
#pragma endregion

#pragma region | Current Room Parameters
  /**
   * It shows the room where the enemy is located.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Current Room", meta = (DisplayName = "Current Room"))
  TObjectPtr<UBaseRoom> m_pCurrentRoom;
#pragma endregion

#pragma region | Debug Mode
  /**
   * @brief It updates the damage widget adding _iDamageValue.
   * @param _iDamageValue -> Value to add.
   */
  void UpdateDamageWidget(int32 _iDamageValue);

  /**
   * @brief It resets the DPS Damage.
   */
  void ResetDPS();

  /**
   * It shows the widget component in the enemy.
   */
  UPROPERTY(VisibleAnywhere, Category = "Bola 13|UI", meta = (DisplayName = "Health Widget Component"))
  UWidgetComponent* m_pHealthWidgetComponent;

  /**
   * It sets the damage debug mode.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Damage", meta = (DisplayName = "Debug Mode"))
  bool m_bDamageDebug;

  /**
   * When the damage debug mode is enabled, it will show a especial widget.
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|UI", meta = (DisplayName = "Debug Damage Widget Class"))
  TSubclassOf<UUserWidget> m_wDamageClass;

  /**
   * Debug Damage properites
   */
  FTimerHandle m_tTimerDPS;
  int32 m_iDPSDamage;
  int32 m_iTotalDamage;
#pragma endregion
};