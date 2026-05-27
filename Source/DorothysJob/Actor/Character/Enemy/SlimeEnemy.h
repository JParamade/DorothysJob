/************************************************************************
 * @description: Slime Enemy Class
 * @author: Pablo Velasco & Jorge Duart
 * @edited_by: Aurora
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BaseEnemy.h"
#include "SlimeEnemy.generated.h"

class UDrawing;
class UNiagaraComponent;
class UOozerPropertiesDataAsset;
struct FAIRequestID;
struct FPathFollowingResult;

UCLASS()
class DOROTHYSJOB_API ASlimeEnemy : public ABaseEnemy
{
	GENERATED_BODY()

public:
	/**
	 * @brief Default Constructor of Slime Basic
	 */
	ASlimeEnemy();

  /**
   * @brief It is called when game starts or when spawned
   */
  virtual void BeginPlay() override;

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
   * @brief 
   */
  virtual void GameOver() override;
#pragma endregion

#pragma region | Attack
  /**
   * @brief Generic enemy AI attack function.
   */
  virtual void Attack() override;
#pragma endregion

#pragma region | To Dirt
  /**
   * @brief Generic enemy AI to dirt function.
   */
  virtual void ToDirt() override;

  /**
   * @brief It resets can dirt action after the cooldown.
   */
  void ResetToDirt();
#pragma endregion

protected:

#pragma region | Dash
  /**
   * @brief It will be called when the dash attack finishes.
   * @param _tRequestID -> Request ID for the AI Controller.
   * @param _tResult -> Path following result.
   */
  void FinishDash(FAIRequestID _tRequestID, const FPathFollowingResult& _tResult);
#pragma endregion

#pragma region | Feedback
  /**
   * @brief It shows the walk feedback VFX.
   */
  virtual void ShowWalkFeedback() override;
#pragma endregion

#pragma region | Components
	/**
	 * Drawing component used for Oozer to dirt action.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Drawing", AllowPrivateAccess = "true"))
	TObjectPtr<UDrawing> m_pDrawingComponent;

  /**
   * Niagara component to show Oozer attack feedback.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Niagara", AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> m_pVFXNiagaraComponent;

  /**
   * Oozer data asset to avoid cast.
   */
  TObjectPtr<UOozerPropertiesDataAsset> m_pOozerPropertiesDA;
#pragma endregion

private:

#pragma region Collision
	/**
	 * @brief It hurts _pActor if the Oozer overlaps.
	 * @param _pComponent -> Oozer actor coollision component.
	 * @param _pActor -> The other overlapped actor.
	 * @param _pActorComp -> The other actor collision component.
	 * @param _iActorBodyIndex -> The other actor collision index.
	 * @param _bFromSweep -> True if sweep movement is detected.
	 * @param _tSweepResult -> Hit result when sweep movement is detected.
	 */
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* _pComponent, AActor* _pActor, UPrimitiveComponent* _pActorComp, int32 _iActorBodyIndex, bool _bFromSweep, const FHitResult& _tSweepResult);
#pragma endregion

  /**
   * Parameter to control the cooldown in to dirt action.
   */
  bool m_bCanDirt;

  /**
   * Parameter to control when the Oozer is dashing.
   */
  bool m_bIsDashing;

  /**
   * Param to control the rotation when the Oozer dirts.
   */
  float m_fDirtRotation;

  /**
   * It resets the brave mode.
   */
  FTimerHandle m_tBraveTimerHandle;

  /**
   * It resets the dirt action.
   */
  FTimerHandle m_tDirtCooldownTimerHandle;

#pragma region | Audio
  /**
   * @brief
   */
  UFUNCTION()
  virtual void PauseFMODEvents();
  /**
   * @brief
   */
  UFUNCTION()
  virtual void UnpauseFMODEvents();

  /**
   * @brief
   */
  bool m_bSpecialPaused = false;

  /**
   * @brief
   */
  FDelegateHandle m_oPauseHandle;
  /**
   * @brief
   */
  FDelegateHandle m_oUnpauseHandle;
#pragma endregion
};