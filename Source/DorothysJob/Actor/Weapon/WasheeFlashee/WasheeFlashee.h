/************************************************************************
 * @description: Weapon "Washee & Flashee" - Combines a cleaning nozzle and liquid attack.
 *               Handles attack, clean, and special attack functionalities.
 * @author: Pablo Velasco
 * @date: 11/03/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "WasheeFlashee.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBarrageShot);

class UNozzleComponent;
class ULiquidComponent;
class USpecialProjectileComponent;
class UDrawing;
class UExposureEffect;
class UFMODEvent;
class UWFWeaponStatsDataAsset;

UCLASS()
class DOROTHYSJOB_API AWasheeFlashee : public ABaseWeapon
{
  GENERATED_BODY()

public:
  AWasheeFlashee();

#pragma region | Getters
  virtual USkeletalMeshComponent* GetSecondaryMeshComponent() const override { return m_pSecondaryWeaponMesh; }
  TObjectPtr<UWFWeaponStatsDataAsset> GetDataAsset();
#pragma endregion

#pragma region | Audio
  // @TODO: Removee when integrated with Jaime's Audio System
  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Attack FMOD Event"))
  TObjectPtr<UFMODEvent> m_pAttackEvent;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Clean FMOD Event"))
  TObjectPtr<UFMODEvent> m_pCleanEvent;

  UPROPERTY(EditAnywhere, Category = "Bola 13|Audio", meta = (DisplayName = "Special FMOD Event"))
  TObjectPtr<UFMODEvent> m_pSpecialEvent;
#pragma endregion
  UPROPERTY(BlueprintAssignable, Category = "Bola13")
  FOnBarrageShot OnBarrageShot;
protected:
#pragma region | Actions
  UFUNCTION(BlueprintCallable)
  virtual void Attack() override;

  UFUNCTION(BlueprintCallable)
  virtual void Clean() override;

  UFUNCTION(BlueprintCallable)
  virtual void SpecialAttack() override;
#pragma endregion

private:

  TObjectPtr<UWFWeaponStatsDataAsset> m_pWFDataAsset;

#pragma region | Components
  /**
   * @brief Skeletal mesh component of the Weapon
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Weapon", meta = (DisplayName = "Secondary Mesh"))
  TObjectPtr<USkeletalMeshComponent> m_pSecondaryWeaponMesh;

  /**
   * @brief Decides the area that is going to clean this weapon
   */
  UPROPERTY(VisibleAnywhere, Category = "Washee", meta = (DisplayName = "Nozzle"))
  TObjectPtr<UNozzleComponent> m_pWasheeNozzle;

  /**
   * @brief Decides the effect that the enemies will get after getting attacked
   */
  UPROPERTY(VisibleAnywhere, Category = "Flashee", meta = (DisplayName = "Liquid"))
  TObjectPtr<ULiquidComponent> m_pFlasheeLiquid;

  /**
   * @brief Decides the effect that the enemies will get after getting attacked
   */
  UPROPERTY(VisibleAnywhere, Category = "Flashee", meta = (DisplayName = "Liquid"))
  TObjectPtr<USpecialProjectileComponent> m_pSpecialProjectile;



  /**
   * @brief Component used to clean or draw
   */
  UPROPERTY(VisibleAnywhere, Category = "Washee", meta = (DisplayName = "Drawing"))
  TObjectPtr<UDrawing> m_pDrawing;
#pragma endregion

#pragma region | Helpers
  float m_fCosHalfAngle = 0.0f;
  void ConeStatusApplication();
#pragma endregion
};