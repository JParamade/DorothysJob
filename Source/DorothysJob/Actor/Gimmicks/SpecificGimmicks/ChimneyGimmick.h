/************************************************************************
 * @description: Represents the Chimney Gimmick
 * @author: AuroraFernández
 * @date: 30/05/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "../BaseGimmick.h"
#include "DorothysJob/Component/Spawner/SpawnEnemyComponent.h"
#include "DorothysJob/Interface/Damageable.h"
#include "ChimneyGimmick.generated.h"

class UDustHealthComponent;

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API AChimneyGimmick : public ABaseGimmick, public IDamageable
{
	GENERATED_BODY()
	
public:

	AChimneyGimmick();

	virtual void Tick(float _DeltaTime) override;

	/**
	 * @brief Gets the Current Health
	 * @return Current Health
	 */
	virtual int GetCurrentHealth() const override;

	/**
	 * @brief It returns the character current health.
	 * @return Character's Current Health.
	 */
	virtual void SetCurrentHealth(int32 _iDamage) override;

	/**
	 * @brief Gets the Total Health
	 * @return Total Health
	 */
	virtual int GetMaxHealth() const override;

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
  virtual void Heal(int _iModifyHealth) override;

protected:

	virtual void BeginPlay() override;

	virtual void GimmickActivation() override;

private:

	UPROPERTY()
	TObjectPtr<USpawnEnemyComponent> m_pSpawnEnemyComponent;

	UPROPERTY()
	TObjectPtr<UDustHealthComponent> m_pDustHealthComponent;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spawner", meta = (DisplayName = "Spawn Cooldown"))
	float m_fSpawnCooldown;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spawner", meta = (DisplayName = "Spawn Enemy Number"))
	int m_iSpawnEnemyNumber;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spawner", meta = (DisplayName = "Spawn Min Radius"))
	float m_fSpawnMinRadius;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spawner", meta = (DisplayName = "Spawn Max Radius"))
	float m_fSpawnMaxRadius;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Spawner", meta = (DisplayName = "Spawn Angle"))
	float m_fSpawnAngle;
	
	UPROPERTY(EditAnywhere, Category = "Bola13 | Health", meta = (DisplayName = "Max Health"))
	int m_iMaxHealth;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Health", meta = (DisplayName = "Max Health"))
	int m_iCurrentHealth;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Health", meta = (DisplayName = "Restore Time"))
	float m_fRestoreCooldown;

	UPROPERTY(EditAnywhere, Category = "Bola13 | Health", meta = (DisplayName = "Is Off"))
	bool m_bIsOff;

	/**
	 * @brief Cooldown manager
	 */
	FTimerHandle m_oSpawnTimer;

	void SpawnLoop();

	FVector GetRandomSpawnLocationAroundActor(float MinRadius, float MaxRadius);

	UFUNCTION()
	void TurnOffActor();

};
