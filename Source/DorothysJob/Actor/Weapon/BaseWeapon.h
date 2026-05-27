/************************************************************************
 * @description: Represents the Base Weapon in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: David Gonzalez
 *             Josephine
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "BaseWeapon.generated.h"

enum class EDirtType : uint8;
enum class EWeaponType : uint8;
class UBaseWeaponStatsDataAsset;
class UBlendSpace;
class UFMODAudioComponent;
class ABaseCharacter;
class UMainLegacyCameraShake;
class IDamageable;
class UBaseWeaponAudioComponent;
class ABasePlayer;
class UAudioManager;
class UPlayerPropertiesDataAsset;

UENUM(BlueprintType)
enum class EAttackType : uint8
{
  BASE_ATTACK     = 0,
  COMBO_ATTACK_1  = 1,
  COMBO_ATTACK_2  = 2,
  MAX             UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EMaterialFeedBack : uint8
{
  NO_FEEDBACK,
  ATTACKING,
  KILLING
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttackSignature, EAttackType, AttackType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponActionSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponHitSignature, float /* MITIGATION */, fMittigation, bool /* THIRD ATTACK */, bThirdAttack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponSpecialSignature, bool, HasShot);

// TODO: delgate with location y rotation


UCLASS()
class DOROTHYSJOB_API ABaseWeapon : public ABaseActor
{
  GENERATED_BODY()
public:

  /**
   * @brief Default Constructor of Base Weapon
   */
  ABaseWeapon();

#pragma region Events

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnAttackSignature OnAttackStart;
  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnAttackSignature OnAttackEnd;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponActionSignature OnCleanStart;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponActionSignature OnCleanEnd;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponActionSignature OnSpecialStart;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponActionSignature OnSpecialEnd;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponActionSignature OnSpecialCharge;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponActionSignature HasAlreadyShot;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponHitSignature OnMeleeHit;

  FOnWeaponSpecialSignature OnSpecialActive;

  FOnWeaponActionSignature OnHasSpecialAlreadyShot;

  UPROPERTY(BlueprintAssignable, Category = "Events delegates")
  FOnWeaponActionSignature OnWeaponSwicht;

#pragma endregion


  void ChangeWeapon();

#pragma region Use Actions

  /**
   * @brief This method uses the virtual method Attack, this method manages the cooldown of the Attack
   */
  UFUNCTION(BlueprintCallable)
  virtual bool UseWeaponAttack();

  UFUNCTION(BlueprintCallable)
  virtual void MeleeAttackHitStart(int32 _iCurrentCombo);

  UFUNCTION(BlueprintCallable)
  virtual void MeleeAttackHitEnd();

  /**
   * @brief This method uses the virtual method Attack, this method manages the cooldown of the Attack
   */
  UFUNCTION(BlueprintCallable)
  void UseWeaponReleaseAttack();

  /**
   * @brief This method uses the virtual method Clean, this method manages the cooldown of the Clean
   */
  UFUNCTION(BlueprintCallable)
  void UseWeaponClean();

  /**
 * @brief This method uses the virtual method Clean, this method manages the cooldown of the Clean
 */
  UFUNCTION(BlueprintCallable)
  void UseWeaponReleaseClean();

  /**
 * @brief This method uses the virtual method Special, this method manages the cooldown of the Special
 */
  UFUNCTION(BlueprintCallable)
  void UseWeaponSpecial();

  /**
   * @brief This method uses the virtual method Special,
   */
  UFUNCTION(BlueprintCallable)
  void UseWeaponSpecialRelease();

  /**
   * @brief This method uses the virtual method Special,
   */
  UFUNCTION(BlueprintCallable)
  void UseWeaponSpecialHold();

#pragma endregion

#pragma region Combo

  /**
   * This method updates if the weapon is currently in combo and the combo hits counter.
   */
  UFUNCTION(BlueprintCallable)
  virtual void ComboStateChanged(bool _bStarts);

  /**
   * It returns if the weapon is currently in combo.
   */
  bool IsInCombo();

  /**
   * It returns the current combo hit.
   */
  int CurrentComboHit();

#pragma endregion

#pragma region Cooldown

  virtual void SetCooldownMultiplier(float _fCooldownMultiplier);

  virtual void ResetCooldownMultiplier();

  float GetAttackingCooldown() const;

#pragma endregion

#pragma region Skeletom

  /**
  * It returns the primera mesh component for this weapon (Washee)
  */
  USkeletalMeshComponent* GetMeshComponent() const;

  /**
   * It returns the secondary mesh component for this weapon (Washee)
   */
  virtual USkeletalMeshComponent* GetSecondaryMeshComponent() const;

#pragma endregion

#pragma region Material Feedback

  virtual void ApplyFeedback(EMaterialFeedBack _feedBack, float _time);

  virtual void DeactivateFeedback(EMaterialFeedBack _feedback);

  virtual void ApplyFeedbackSound(IDamageable* damage) {};

#pragma endregion

#pragma region Damage

  void ApplyDamage(IDamageable* _enemy, int32 _damage, EDirtType _eAttackType, bool _bCanChargeSpecial = true, const FVector& _vHitLocation = FVector::ZeroVector, const FRotator& _vHitRotation = FRotator::ZeroRotator);

  void ApplyDustPoopDamage(IDamageable* _enemy, int32 _damage, EDirtType _eAttackType, const FVector& _vHitLocation = FVector::ZeroVector, const FRotator& _vHitRotation = FRotator::ZeroRotator, bool _critical = false);

  EDirtType GetDirtType() const;

#pragma endregion

  TSoftObjectPtr<UBaseWeaponStatsDataAsset> GetDataAsset() const;

#pragma region Ultimate

  /**
  * @brief its call when the weapon make damage
  * @param damage The damage have made
  */
  void NotifyDamage(float damage);

  /**
   * @brief Its call when the dust is clean
   */
  void NotifyCleaningDust();

  /**
   * @brief its call when cleaning liquid
   * @param _floorClean The dirt that has been clean
   */
  void NotifyCleaningLiquid(float _floorClean);

  float GetUltimatePercentage();

  void SetUltimatePercentage(float _fValue);
#pragma endregion

  EAttackType m_EAttackType;
  
#pragma region Status Effecs

  virtual void ApplyAttackStatusEffecs(ABaseCharacter* _character);

  virtual void ApplyCleanStatusEffects(ABaseCharacter* _characterr) const;

  virtual void ApplySpecialStatusEffects(ABaseCharacter* _character);

#pragma endregion

#pragma region Special

  /**
   * @brief Changes the boolean m_bCanUseSpecial to true so this weapon can Use Special again
   */
  void ChargeSpecial(float _charge);

  /**
   * @brief Changes the boolean m_bReleaseSpecialAttack, that indicates if the special have a release
   */
  void SetReleaseSpecial(bool _release);

  /**
   * @brief It returns if the special attack could be executed.
   */
  bool IsAvailableSpecialAttack();

#pragma endregion

#pragma region CameraShake
  virtual void StartCameraShake();
  virtual void StopCameraShake();
  virtual void SetCameraShake(TSubclassOf<UMainLegacyCameraShake> _ShakeClass);
#pragma endregion

#pragma region ControllerRumble
  FDynamicForceFeedbackHandle FeedbackHandle;

  virtual void StartControllerRumble(float _fIntensity, float _fDuration, bool _bAffectsLeftLarge, bool _bAffectsLeftSmall, bool _bAffectsRightLarge, bool _bAffectsRightSmall);
  virtual void UpdateControllerRumble(float _fIntensity, float _fDuration, bool _bAffectsLeftLarge, bool _bAffectsLeftSmall, bool _bAffectsRightLarge, bool _bAffectsRightSmall);
  virtual void StopControllerRumble();
#pragma endregion

  virtual UFMODAudioComponent* GetCleanAudioComponent();
  virtual UFMODAudioComponent* GetSpecialAudioComponent();
protected:

#pragma region Base funtion

  virtual void BeginPlay();

#pragma endregion

#pragma region Actions

  /**
  * @brief Virtual Attack method that will be overwritten by the Weapons
  */
  virtual void Attack() {};

  /**
   * @brief Virtual Release Attack method that will be overwritten by the Weapons
   */
  virtual void ReleaseAttack() {};

  /**
   * @brief Virtual Clean method that will be overwritten by the Weapons
   */
  virtual void Clean() {};

  /**
  * @brief Virtual Release Clean method that will be overwritten by the Weapons
  */
  virtual void ReleaseClean() {};

  /**
   * @brief Virtual Special Attack method that will be overwritten by the Weapons
   */
  virtual void SpecialAttack() {};

  /**
   * @brief Virtual Release Special Attack method that will be overwritten by the Weapons
   */
  virtual void ReleaseSpecialAttack() {};

  /**
   * @brief Virtual Hold Special Attack method that will be overwritten by the Weapons
   */
  virtual void HoldSpecialAttack() {};

#pragma endregion

#pragma region Components

  /**
   * @brief Skeletal mesh component of the Weapon
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Weapon", meta = (DisplayName = "Mesh"))
  USkeletalMeshComponent* m_pWeaponMesh;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Audio Component"))
  TObjectPtr<UFMODAudioComponent> m_pCleanAudioComponent;

  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Audio Component"))
  TObjectPtr<UFMODAudioComponent> m_pSpecialAudioComponent;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Weapon", meta = (DisplayName = "Data Asset", AllowedClasses = "/Script/DorothysJob.BaseWeaponStatsDataAsset"))
  TObjectPtr<UBaseWeaponStatsDataAsset> m_pWeaponDataAsset;

  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola 13|Component")
  TObjectPtr<UBaseWeaponAudioComponent> m_pWeaponAudioComponent;


#pragma endregion

#pragma region Stats


  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats", meta = (DisplayName = "Cooldown Multiplier"))
  float m_fCooldownMultiplier = 1;

  /**
   * @brief Seconds that must pass until using the Special again
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Stats", meta = (DisplayName = "Special Charge per attack"))
  float m_fCharge;

#pragma endregion

#pragma region Combo

  /**
 * It indicates if the weapon is currently in combo animation
 */
  bool m_bInCombo;

  /**
   * The current combo hit.
   */
  int m_iCurrentCombo;

#pragma endregion

  /**
 * @brief marks if the release atack of the special attack is activated
 */
  bool m_bReleasSpecialAttack;

private:

#pragma region Reset Actions

  /**
 * @brief Changes the boolean m_bCanAttack to true so this weapon can Attack again
 */
  void ResetAttack();

  /**
   * @brief Changes the boolean m_bCanClean to true so this weapon can Clean again
   */
  void ResetClean();
#pragma endregion

#pragma region cooldown

  /**
   * @brief Timer that Manages the Attack Cooldown
   */
  UPROPERTY()
  FTimerHandle m_oAttackRate;

  /**
   * @brief Timer that Manages the Clean Cooldown
   */
  UPROPERTY()
  FTimerHandle m_oCleanRate;

#pragma endregion

#pragma region Action booleans

  /**
   * @brief True if the weapon can Attack, false if it can't
   */
  UPROPERTY()
  bool m_bCanAttack;

  /**
   * @brief True if the weapon can Clean, false if it can't
   */
  UPROPERTY()
  bool m_bCanClean;

  /**
   * @brief Charge of the Special Attack
   */
  float m_fSpecialCharge;
  bool m_bSpecialHasCharged = false;

#pragma endregion

#pragma region Material Feedback

  EMaterialFeedBack m_eMaterialFeedback;

  UPROPERTY(EditAnywhere, Category = "Bola13|Material feedback")
  float m_fAttackMaterialTime;

  UPROPERTY(EditAnywhere, Category = "Bola13|Material feedback")
  float m_fKillMaterialTime;

  UPROPERTY()
  FTimerHandle m_TimeHandle;

  UPROPERTY(EditAnywhere, Category = "Bola13|Material feedback")
  TObjectPtr<UMaterialInterface> m_pBaseMaterial;

  UPROPERTY(EditAnywhere, Category = "Bola13|Material feedback")
  TObjectPtr<UMaterialInterface> m_pAttackMaterial;

  UPROPERTY(EditAnywhere, Category = "Bola13|Material feedback")
  TObjectPtr<UMaterialInterface> m_pKillMaterial;

  UPROPERTY()
  UWorld* CachedWorld;

  UPROPERTY()
  TWeakObjectPtr<ABasePlayer> m_pPlayerOwner;

  UPROPERTY()
  TWeakObjectPtr<UAudioManager> m_pAudioManager;

  UPROPERTY()
  TWeakObjectPtr<UPlayerPropertiesDataAsset> m_pPlayerDataAsset;
#pragma endregion
};