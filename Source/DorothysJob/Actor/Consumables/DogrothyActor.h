/************************************************************************
 * @description: Represents the Dogrothy consumable actor.
 * @author: Jaime Paramo
 * @date: 01/07/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Consumables/BaseConsumableActor.h"
#include "DogrothyActor.generated.h"

class UDrawing;
class UFMODEvent;
class UNiagaraSystem;

UCLASS()
class DOROTHYSJOB_API ADogrothyActor : public ABaseConsumableActor
{
	GENERATED_BODY()
	
public:
	/**
	 * @brief 
	 */
	ADogrothyActor();

	/**
	 * @brief 
	 */
	virtual void BeginPlay() override;
	/**
	 * @brief 
	 * @param _fDeltaTime 
	 */
	virtual void Tick(float _fDeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Start FMOD Event"))
	TObjectPtr<UFMODEvent> m_pStartEvent;
	UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Movement FMOD Event"))
	TObjectPtr<UFMODEvent> m_pMovementEvent;
	UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Destroy FMOD Event"))
	TObjectPtr<UFMODEvent> m_pDestroyEvent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Aux Audio Component"))
	TObjectPtr<UFMODAudioComponent> m_pAuxAudioComponent;

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
	bool m_bStartPaused = false;
	/**
   * @brief
   */
	bool m_bCleanPaused = false;

	/**
	 * @brief
	 */
	FDelegateHandle m_oPauseHandle;
	/**
	 * @brief
	 */
	FDelegateHandle m_oUnpauseHandle;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Deteccion", meta = (DisplayName = "Dust destruccion collision"))
  TObjectPtr<UShapeComponent> m_pDustDeteccion;

protected:
	/**
	 * @brief 
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Dirt", meta = (DisplayName = "Drawing Component"))
	TObjectPtr<UDrawing> m_pDrawingComponent;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Size"))
	FVector2D m_vDirtSize;
	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Clean Texture"))
	TObjectPtr<UTexture2D> m_pCleanTexture;

	/**
	 * @brief 
	 * @param _vNewDirection 
	 */
	virtual void ChangeDirection(FVector _vNewDirection);

	/**
	 * @brief 
	 * @param _pHitComponent 
	 * @param _pOtherActor 
	 * @param _pOtherComp 
	 * @param _vNormalImpulse 
	 * @param _rHit 
	 */
	UFUNCTION(BlueprintCallable)
	virtual void OnHit(UPrimitiveComponent* _pHitComponent, AActor* _pOtherActor, UPrimitiveComponent* _pOtherComp, FVector _vNormalImpulse, const FHitResult& _rHit);
	UFUNCTION(BlueprintCallable)
	virtual void OnBeginOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor, UPrimitiveComponent* _pOtherComp, int32 _iOtherBodyIndex, bool _bFromSweep, const FHitResult& _rSweepResult);

	void ToDirt();

	/**
	 * @brief 
	 * @param _fDamage 
	 */
	virtual void ApplyDamage(float _fDamage);
	/**
	 * @brief 
	 */
	UFUNCTION(BlueprintCallable)
	virtual void Die();

private:
	/**
	 * @brief 
	 */
	void SetupDogrothy();

	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Movement Speed"))
	float m_fMovementSpeed = 400.f;
	FVector m_vMovementDirection;

	/**
	 * @brief
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Movement", meta = (DisplayName = "Movement Speed"))
	float m_fMaxRotationAngle = 90.f;

	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Lifetime", meta = (DisplayName = "Max Health"))
	int32 m_fMaxHealth = 100.f;
	/**
	 * @brief 
	 */
	UPROPERTY(VisibleAnywhere, Category = "Bola 13|Lifetime", meta = (DisplayName = "Current Health"))
	int32 m_fCurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Lifetime", meta = (DisplayName = "Hit damage"))
	int32 m_fHitDamage = 20;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Dust", meta = (DisplayName = "Dust Damage"))
	int32 m_iDustDamage;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Lifetime", meta = (DisplayName = "Life time"))
	float m_fLifeTime = 30;

#pragma region | VFX
  /**
   * It will be used when decoy dies.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|VFX", meta = (DisplayName = "Dead VFX"))
  TObjectPtr<UNiagaraSystem> m_pDeadEffect;
#pragma endregion


	/**
	 * @brief 
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Combat", meta = (DisplayName = "Invulnerability Time"))
	float m_fInvulnerabilityTime = .1f;
	/**
	 * @brief 
	 */
	UPROPERTY()
	FTimerHandle m_oInvulnerabilityTimer;

	float m_fStartDelay = .8f;
	FTimerHandle m_oStartDelayTimer;
	UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Time"))
	float m_fCleaningTime;
	FTimerHandle m_oDirtingTimer;
	FTimerHandle m_oLifeTimeTimer;
	/**
	 * @brief 
	 */
	void ResetHit();

	/**
	 * @brief 
	 */
	void PlayMovementSound();
};
