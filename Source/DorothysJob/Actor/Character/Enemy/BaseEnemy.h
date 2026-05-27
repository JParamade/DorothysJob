/************************************************************************
 * @description: Represents the Base Enemy in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: Aurora
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "BaseEnemy.generated.h"

UENUM()
enum class EWeaponsTypes : uint8
{
  WF    UMETA(DisplayName = "Melee"),
  DUSTY  UMETA(DisplayName = "Distance"),
  MOPPRESSOR  UMETA(DisplayName = "Distance"),
  DIRTMINATOR  UMETA(DisplayName = "Distance"),
  BOMB  UMETA(DisplayName = "Melee"),
  NONE
};

class UBehaviorTree;
class UWidgetComponent;
class UBaseRoom;
class UEnemyPropertiesDataAsset;
class RandomNumberGenerator;
class UNiagaraComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyActionComplete, ABaseEnemy*, _pPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyEliminate, ABaseEnemy*, _pPawn); 

UCLASS()
class DOROTHYSJOB_API ABaseEnemy : public ABaseCharacter
{
	GENERATED_BODY()
public:

  /**
   * @brief Delegate to execute when the character is eliminated.
   */
  UPROPERTY(BlueprintAssignable)
  FOnEnemyEliminate OnEnemyEliminate;

	/**
	 * @brief Default Constructor of Base Enemy
	 */
	ABaseEnemy();

  /**
   * @brief It is called when game starts or when spawned
   */
  virtual void BeginPlay() override;

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
   * @brief It is called when the game ends.
   */
  virtual void EndPlay(const EEndPlayReason::Type _eEndPlayReason) override;

  /**
   * @brief It is called every frame
   * @param _fDeltaTime -> Elapsed time since last frame
   */
  virtual void Tick(float _fDeltaTime) override;

#pragma region | Stats
  /**
   * @brief It sets a new mitigation value.
   * @param _fNewMitigation -> New mitigation value (Should be between 0 and 1).
   */
  virtual void SetMitigation(float _fNewMitigation) override;

  void SetMitigationVFXVisibility(bool _bVisible);

  /**
   * @brief The base method that will modify the stats of the enemy.
   * @param _mStats -> Map with the values to add or multiply to the stats.
   */
  virtual void ModifyStats(const TMap<EModifiableStats, float> _mStats) override;

  /**
   * @brief The base method that will modify the stats of the enemy.
   * @param _mStats -> Map with the values to rest or divide from the stats.
   */
  virtual void RevertStats(const TMap<EModifiableStats, float> _mStats) override;
#pragma endregion

#pragma region | Status Effect
  /**
   * @brief It applies the Grimeling pollution efects to the enemy.
   */
  virtual void BePolluted() override;
#pragma endregion

#pragma region | AI
	/**
	 * @brief It returns the enemy behavior tree.
	 */
	UBehaviorTree* GetBehaviorTree();

  /**
   * @brief It restarts the enemy behavior tree.
   */
  void RestartBehaviorTree();

  /**
   * @brief It stops the enemy behavior tree.
   * @param _sReason -> Reason to stop the behavior tree.
   */
  void StopBehaviorTree(FString _sReason = TEXT("Stopped by user"));
#pragma endregion

#pragma region | Movement
  /**
   * @brief It returns if the enemy is in movement or not.
   */
  virtual bool IsInMovement() const;
#pragma endregion

#pragma region | Attack
  /**
   * @brief Generic enemy AI prepare attack function.
   */
  virtual void PrepareAttack();

  /**
   * @brief Generic enemy AI attack function.
   */
  virtual void Attack();

  /**
   * @brief Generic enemy AI recover attack function.
   */
  virtual void RecoverAttack();

  /**
   * @brief Delegate to execute when the prepare attack is finished.
   */
  FOnEnemyActionComplete OnPrepareAttackComplete;

  /**
   * @brief Delegate to execute when the attack is finished.
   */
  FOnEnemyActionComplete OnAttackComplete;

  /**
   * @brief Delegate to execute when the recover attack is finished.
   */
  FOnEnemyActionComplete OnRecoverAttackComplete;
#pragma endregion

#pragma region | Dirt
  /**
   * @brief Generic enemy AI prepare dirt function.
   */
  virtual void PrepareDirt();

  /**
   * @brief Generic enemy AI to dirt function.
   */
  virtual void ToDirt();

  /**
   * @brief Generic enemy AI recover dirt function.
   */
  virtual void RecoverDirt();

  /**
   * @brief Delegate to execute when the prepare dirt is finished.
   */
  FOnEnemyActionComplete OnPrepareDirtComplete;

  /**
   * @brief Delegate to execute when to dirt action is finished.
   */
  FOnEnemyActionComplete OnToDirtComplete;

  /**
   * @brief Delegate to execute when the prepare dirt is finished.
   */
  FOnEnemyActionComplete OnRecoverDirtComplete;
#pragma endregion

#pragma region | Hide
  /**
   * @brief Generic enemy AI to hide function.
   */
  virtual void ToHide();

  /**
   * @brief It returns if the Enemy has been hidden any time in the past.
   */
  bool HasEnemyBeenHiddenAnyTime() const;

  /**
   * @brief Delegate to execute when the prepare dirt is finished.
   */
  FOnEnemyActionComplete OnToHideComplete;
#pragma endregion

#pragma region | Cancel Action
  /**
   * @brief Generic enemy AI cancel action function.
   */
  virtual void CancelAction();
#pragma endregion

#pragma region | Status Effect
  /**
   * @brief It sets if the character can do actions according to _bCanDoActions.
   * @param _bCanDoActions -> Can do actions or not.
   */
  virtual void SetCanDoActions(bool _bCanDoActions) override;

  /**
   * @brief It will apply speed up status
   */
  virtual void SpeedUp() override;

  /**
   * @brief Delegate to execute when the speed up state is applied.
   */
  FOnEnemyActionComplete OnSpeedUpComplete;
#pragma endregion

#pragma region | Enemy Manager
  /**
   * @brief It returns if there are available tokens to attack in the current room.
   */
  virtual bool AreAvailableTokens();

  /**
   * @brief It request a token to attack in the current room.
   */
  virtual bool RequestToken();

  /**
   * @brief It returns the token to the pool.
   */
  virtual bool ReturnToken();

#pragma endregion

  UEnemyPropertiesDataAsset* GetEnemyDataAsset() const;

protected:

#pragma region | Capsule
  /**
   * @brief It updates enemy capsule.
   * @param _fCapsuleRadius -> Capsule radius to set.
   */
  void UpdateCapsule(float _fCapsuleRadius);
#pragma endregion

#pragma region | Game Over
  /**
   * @brief It is callen when the current health is less than 0.
   */
  virtual void GameOver() override;
#pragma endregion

#pragma region | Animations
  /**
   * @brief It plays the section in the animation montage.
   * @param _sSection -> Section to play.
   * @param _pDelegate -> Pointer to a delegate to callback.
   */
  void PlayAnimMontage(const FName& _sSection, const FOnEnemyActionComplete* _pDelegate = nullptr);

  /**
   * @brief It stops the animation montage.
   */
  void StopAnimMontage();
#pragma endregion

#pragma region | Actions
  /**
   * @brief The enemy performs the attack when the notification arrives according to the animation.
   * @param _iCurrentCombo -> Current action combo index.
   */
  virtual void PerformAttack(int32 _iCurrentCombo);

  /**
   * @brief The enemy performs to dirt action when the notification arrives according to the animation.
   */
  virtual void PerformToDirt();
#pragma endregion

#pragma region | Feedback
  /**
   * @brief It shows the walk feedback VFX.
   */
  virtual void ShowWalkFeedback();
#pragma endregion

#pragma region | Audio
  /**
   * @brief It plays the enemy idle sound.
   */
  void PlayIdleSound();
#pragma endregion

#pragma region | Movement Parameters
  /**
   * It allows to draw the max and min radius and the selected target.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Wander", meta = (DisplayName = "Debug Mode"))
  bool m_bWanderDebug;
#pragma endregion

#pragma region | Attack Parameters
  /**
   * It allows to draw attack.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Attack", meta = (DisplayName = "Debug Mode"))
  bool m_bAttackDebug;

  /**
   * Detect radius for Enemy attack.
   */
  float m_fDetectRadius;

  /**
   * DangerRadius for Enemy flee.
   */
  float m_fDangerRadius;
#pragma endregion

#pragma region | To Hide Parameters
  /**
 * It shows if the Enemy is hidden or not.
 */
  bool m_bIsHidden;

  /**
   * It shows if the Enemy has been hidden any time or not.
   */
  bool m_bHasBeenHiddenAnyTime;
#pragma endregion

#pragma region | Status Effect Parameters
  /**
   * It shows the widget component in the enemy.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|UI", meta = (DisplayName = "Status Widget Component"))
  TObjectPtr<UWidgetComponent> m_pStatusWidgetComponent;

  /**
   * Niagara component to show buff status effect in the enemy.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Niagara Status", AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> m_pVFXStatusComponent;

  /**
   * Niagara component to show polluted status effect in the enemy.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Polluted Status", AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> m_pVFXPollutedComponent;

  /**
   * Niagara component to show stun status effect in the enemy.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Stun Status", AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> m_pVFXStunComponent;
#pragma endregion

#pragma region | Audio Parameters
  /**
   * Timer to play again the enemy idle sound.
   */
  FTimerHandle m_tIdleSoundTimer;
#pragma endregion
  /**
 * The weapon of the last attack the enemy receive
 */
  EWeaponsTypes m_eWeaponLastAttackReceive;
private:

  /**
   * Enemy data asset to avoid cast.
   */
  TObjectPtr<UEnemyPropertiesDataAsset> m_pEnemyPropertiesDA;

  /**
   * It is the random number generator.
   */
  TSharedPtr<RandomNumberGenerator> m_pRandom;


};