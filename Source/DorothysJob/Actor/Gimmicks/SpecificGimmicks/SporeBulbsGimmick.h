// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseGimmick.h"
#include "Components/SphereComponent.h"
#include "SporeBulbsGimmick.generated.h"


UENUM(BlueprintType)
enum class ESporeEffectType : uint8
{
	None        UMETA(DisplayName = "None"),
	StinkEffect    UMETA(DisplayName = "Stink Effect")
};

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API ASporeBulbsGimmick : public ABaseGimmick
{
	GENERATED_BODY()
	
public:

	ASporeBulbsGimmick();

protected:

	virtual void BeginPlay() override;

	virtual void GimmickActivation() override;

private:

#pragma region Animation&Particles
	// Animación del actor (ataque, carga, etc.)
	UPROPERTY(EditAnywhere, Category = "Bola13 | Animation", meta = (DisplayName = "Charge Animation"))
	TObjectPtr<UAnimMontage> m_pChargeAnimation;

	// Animación del actor (ataque, carga, etc.)
	UPROPERTY(EditAnywhere, Category = "Bola13 | Animation", meta = (DisplayName = "Explode Animation"))
	TObjectPtr<UAnimMontage> m_pExplodeAnimation;

	// Animación del actor (ataque, carga, etc.)
	UPROPERTY(EditAnywhere, Category = "Bola13 | Animation", meta = (DisplayName = "Restore Animation"))
	TObjectPtr<UAnimMontage> m_pRestoreAnimation;

	// Sistema de partículas que se activan al atacar
	UPROPERTY(EditAnywhere, Category = "Bola13 | Particles", meta = (DisplayName = "Particle System"))
	TObjectPtr<UParticleSystem> m_pParticles;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Animation", meta = (DisplayName = "Animation Charge Time"))
	float m_fChargeTime;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Animation", meta = (DisplayName = "Animation Restore Time"))
	float m_fRestoreTime;
#pragma endregion

#pragma region SporeSphere
	UPROPERTY(VisibleAnywhere, Category = "Bola13 | Spore Sphere", meta = (DisplayName = "Spore Sphere Component"))
	TObjectPtr<USphereComponent> m_pSporeSphere;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spore Sphere", meta = (DisplayName = "Spore Range"));
	float m_pSporeRadius;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spore Sphere", meta = (DisplayName = "Spore Duration"))
	float m_fSporeDuration;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spore Sphere", meta = (DisplayName = "Spore Effect"))
	ESporeEffectType m_eSporeEffect;
#pragma endregion

#pragma region DetectionSphere
	UPROPERTY(VisibleAnywhere, Category = "Bola13 | Detection Sphere", meta = (DisplayName = "Detection Sphere Component"))
	TObjectPtr<USphereComponent> m_pDetectionSphere;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Detection Sphere", meta = (DisplayName = "Detection Range"));
	float m_pDetectionRadius;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Detection Sphere", meta = (DisplayName = "Detection Can Be Trigger"))
	bool m_bCanBeTriggered;
#pragma endregion

	TObjectPtr<AActor> m_pActorDetected;
	FTimerHandle  m_tSporeActivationTimerHandle;
	
#pragma region SporeSphereImplementation
	UFUNCTION()
	void ActivateSporeSphere();

	UFUNCTION()
	void DeactivateSporeSphere();

	UFUNCTION()
	void OnSporeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
#pragma endregion

#pragma region DetectionSphereImplementation
	UFUNCTION()
	void ResetBulb();

	UFUNCTION()
	void OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);
#pragma endregion

};
