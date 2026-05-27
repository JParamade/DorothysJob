/************************************************************************
 * @description: Represents the Clean 4 consumable actor.
 * @author: Jaime Paramo
 * @date: 01/07/2025
 * @edited_by: Josephine - 21/08/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Consumables/BaseConsumableActor.h"
#include "Clean4Actor.generated.h"

#pragma region | Forward Declaration
class UDrawing;
class UNiagaraSystem;
class UNiagaraComponent;
class USphereComponent;
class UFMODEvent;
#pragma endregion

/**
 * @brief The actor that will be spawned on use
 */
UCLASS()
class DOROTHYSJOB_API AClean4Actor : public ABaseConsumableActor
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief The default constructor
	 */
	AClean4Actor();

protected:

	/**
	 * @brief Called on begin
	 */
	virtual void BeginPlay() override;

  UFUNCTION()
  void DetectNearbyEnemies(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  void ActivateOtherBombs();

	void DamageInArea(TArray<TEnumAsByte<EObjectTypeQuery>> _oChannel, float _fDamage);

  void CheckNearbyEnemies();

  void TimingExplosion();

  void Explode();

	/**
	 * @brief Called after the timer ends
	 */
	UFUNCTION()
	void OnActivate();

	/**
	 * @brief This will handle the cleaning
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bola 13|Dirt", meta = (DisplayName = "Component"))
	TObjectPtr<UDrawing> m_pDrawingComponent;

	/**
	 * @brief The size of the cleaning area
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Size"))
	FVector2D m_vCleanSize;

	/**
	 * @brief The texture to use for the cleaning
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Dirt", meta = (DisplayName = "Texture"))
	TObjectPtr<UTexture2D> m_pCleanTexture;

	/**
	 * @brief This holds the system for the area to spawn
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Visual Effects", meta = (DisplayName = "Area"))
	TObjectPtr<UNiagaraSystem> m_pAreaVFXSystem;

	/**
	 * @brief This holds the system for the explosion to spawn
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Visual Effects", meta = (DisplayName = "Explosion"))
	TObjectPtr<UNiagaraSystem> m_pExplosionVFXSystem;


	UPROPERTY(EditAnywhere, Category = "Bola 13|Combat", meta = (DisplayName = "Damage collisios presets"))
	TArray<TEnumAsByte<EObjectTypeQuery>> m_oDamageCollisionSets;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Dust", meta = (DisplayName = "Clean collisios presets"))

	TArray<TEnumAsByte<EObjectTypeQuery>> m_oDustCollisionSets;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Explosions", meta = (DisplayName = "Other Bombs collisios presets"))

  TArray<TEnumAsByte<EObjectTypeQuery>> m_oBombsCollisionSets;

	UPROPERTY(EditAnywhere, Category = "Bola 13|Detection", meta = (DisplayName = "Detection collisios presets"))

	TArray<TEnumAsByte<EObjectTypeQuery>> m_oDetectionCollisionSets;

	/**
	 * @brief The damage to apply on explosion
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Combat", meta = (DisplayName = "Explosion Damage"))
	float m_fExplosionDamage;

	/**
	* @brief The damage to apply on explosion
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Dust", meta = (DisplayName = "Clean Damage"))
	float m_fCleanDamage;

	/**
	 * @brief The radius of the explosion
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Combat", meta = (DisplayName = "Explosion Radius"))
	float m_fExplosionRadius;


	/**
	 * @brief The seconds before activation
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Combat", meta = (DisplayName = "Activation Time"))
	float m_fActivationTime;

  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Combat", meta = (DisplayName = "Other bombs activation time"))
  float m_fOtherBombActivation = 0.3f;

  bool m_bHasDetone = false;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Debug", meta = (DisplayName = "Debug mode"))
  bool m_bDebugMode;

  FTimerHandle m_oTimeHandle;


	UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Audio", meta = (DisplayName = "Explosion FMOD Event"))
	TObjectPtr<UFMODEvent> m_pExplosionEvent;

private:

	/**
	 * @brief This will spawn the receiving niagara system using its related component
	 * @param _pVFXSystem The system to spawn
	 * @param _pVFXComponent The component that will handle it
	 */
	void SpawnVFX(TObjectPtr<UNiagaraSystem> _pVFXSystem, TObjectPtr<UNiagaraComponent> _pVFXComponent);

	/**
	 * @brief The component of the visual effect for the area
	 */
	TObjectPtr<UNiagaraComponent> m_pAreaVFXComponent;

	/**
	 * @brief The component of the visual effect for the explosion
	 */
	TObjectPtr<UNiagaraComponent> m_pExplosionVFXComponent;

	UPROPERTY(EditAnywhere)
	/**
	 * @brief The collider component
	 */
	TObjectPtr<USphereComponent> m_pDetectionCollider;
};
//EOF