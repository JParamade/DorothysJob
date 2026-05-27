/************************************************************************
 * @description: Represents the Handle Component of Mopressor
 *							 This class determines the Type of damage and cleaning.
 * @author: Josephine Esposito
 * @date: 13/03/2025
 * @edited_by: Josephine - 21/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseAttackComponent.h"
#include "MopHandleComponent.generated.h"


class ADamageCollider;
class AMopressor;
class UNiagaraComponent;
class UMoppresorWeaponStatsDataAsset;

/**
 * Represents the handle of the weapon Mopressor
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Drawing), meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UMopHandleComponent : public UBaseAttackComponent
{
	GENERATED_BODY()

public:
	/**
	* @brief The Default Constructor
	*/
	UMopHandleComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);

	virtual void Use() override;

	void DeactivateCollider();

	void Attack(int32 _iInnerDamage, int32 _iOuterDamage, TArray<AActor*> _lInnerHitEnemies, AActor* _enemy, FVector _impactPoint);

	void Attack(int32 _iInnerDamage, int32 _iOuterDamage, TArray<AActor*> _lInnerHitEnemies, TArray<AActor*> _lOuterHitEnemies);

	void SetDamageCollider(ADamageCollider* _pDamageCollider, ADamageCollider* _pLastHitCollider);

	void SetAttackPivot(USceneComponent* _pivot);

	void SetReductionMovility(bool _hasReduction);
	
	void SetAttackSpeedModifier(float _fSpeedModifier);

	bool HasReductionMovility() const;

	void SetReductionRotation(bool _hasReduction);

	bool HasReductionRotation() const;

	bool IsSlashing() const;

	void ChangedWeapon();

protected:

	UFUNCTION()
	virtual void OnDamageColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnLastHitColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	void ResetTimeDilation();

	void SetFreezeFrameCooldown();

	UPROPERTY()
	bool m_bIsSlashing = false;

	UPROPERTY()
	bool m_bReductionMove = false;

	UPROPERTY()
	bool m_bReductionRotation = false;

	UPROPERTY()
	bool m_bCooldownFreezeFrame = false;

	UPROPERTY()
	FRotator m_rEndAngle = FRotator::ZeroRotator;

	UPROPERTY()
	FRotator m_rCurrentAngle = FRotator::ZeroRotator;

	UPROPERTY()
	float m_fRotationSpeed = 0.f;

	UPROPERTY()
	float m_fSpeedModifier = 1.0f;

	UPROPERTY()
	TObjectPtr<ADamageCollider> m_pDamageCollider = nullptr;

	UPROPERTY()
	TObjectPtr<ADamageCollider> m_pLastHitCollider = nullptr;

	UPROPERTY()
	TObjectPtr<USceneComponent> m_pAttackPivot = nullptr;

	UPROPERTY()
	TObjectPtr<UMoppresorWeaponStatsDataAsset> m_pDataAsset = nullptr;

	UPROPERTY()
	TWeakObjectPtr<ABasePlayer> m_pPlayerOwner = nullptr;

	UPROPERTY()
	TWeakObjectPtr<AMopressor> m_pWeaponOwner = nullptr;

	UPROPERTY()
	TObjectPtr<UNiagaraComponent> m_pSlashVFX = nullptr;

	UPROPERTY()
	UWorld* CachedWorld = nullptr;

	UPROPERTY()
	TArray<AActor*> m_lEnemiesToDamage;

	UPROPERTY()
	FTimerHandle m_thGlobalTimeDilation;

	UPROPERTY()
	FTimerHandle m_thCooldownTimeDilation;
};
