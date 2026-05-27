/************************************************************************
 * @description: This class is dusty attack component.
 * @author: Raul
 * @date: -/-/2025
 * @edited_by: Aurora - 24/08/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseAttackComponent.h"
#include "DustyTradicionalHandle.generated.h"

class ABasePlayer;
class ADamageCollider;
class ADusty;
class UDustyWeaponStatsDataAsset;

UCLASS()
class DOROTHYSJOB_API UDustyTradicionalHandle : public UBaseAttackComponent
{
  GENERATED_BODY()

public:
#pragma region | BP_Variable
  /**
   * @brief The mini hits damage collider class
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Attack", meta = (DisplayName = "Mini Attacks Damage Collider Class"))
  TSubclassOf<ADamageCollider> m_pMiniDustyDamageColliderClass;

  /**
   * @brief The mega hit damage collider class
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Attack", meta = (DisplayName = "Mega Attack Damage Collider Class"))
  TSubclassOf<ADamageCollider> m_pMegaDustyDamageColliderClass;
#pragma endregion

#pragma region | Virtual Functions
  /**
   * @brief Use the attack component
   */
  virtual void Use() override;

  /**
   * @brief What happends when it ends
   */
  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
#pragma endregion

#pragma region | Own Functions
  /**
   * @brief Restor when attack finish.
   */
  void AttackFinish();
#pragma endregion

protected:

  virtual void BeginPlay() override;

  void DashStatus(bool _bActivate);

private:
#pragma region | Variables
  /** The dusty */
  TWeakObjectPtr<ADusty> m_pOwner;

  /** The dusty data asset */
  TObjectPtr<UDustyWeaponStatsDataAsset> m_pStatsDataAsset;

  /** The player */
  TWeakObjectPtr<ABasePlayer> m_pPlayer;

  /** The mini hit damage collider */
  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pMiniHitDamageCollider;

  /** The mega hit damage collider */
  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pMegaHitDamageCollider;

  /** The time to atack post dash */
  UPROPERTY(EditAnywhere, Category = "Stats")
  float m_fDelayPunchAtack;

  TArray<AActor*> m_lEnemiesToDamage;
#pragma endregion

#pragma region | Private Functions
  UFUNCTION()
  void OnBeginOverlap(UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult);

  UFUNCTION()
  void InitDustyComponents();

  UFUNCTION()
  void AttackApplyDamage(int _iDamage, AActor* _damageActor, FVector _impactPoint);

  UFUNCTION()
  void MakeDash();
#pragma endregion
};
