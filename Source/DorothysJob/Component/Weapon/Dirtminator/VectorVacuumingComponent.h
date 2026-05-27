/************************************************************************
 * @description: Represents the cleaning of the Dirtminator
 * @date: 10/05/2025
 * @edited_by: Aurora Maria Fernandez - 21/08/25
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseWeaponComponent.h"
#include "VectorVacuumingComponent.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class UDirtminatorWeaponStatsDataAsset;

/**
 * @brief Component of the Dirtminator in charge of the cleaning.
 */
UCLASS()
class DOROTHYSJOB_API UVectorVacuumingComponent : public UBaseWeaponComponent
{
  GENERATED_BODY()

public:
  /**
   * @brief Constructor of the component.
   */
  UVectorVacuumingComponent();

  /**
   * @brief Function that happends when the game starts.
   */
  virtual void BeginPlay() override;

  /**
   * @brief Use the dirtminator cleaning
   */
  virtual void Use() override;

  /**
   * @brief What happends when the cleaning is release
   */
  virtual void Release() override;


protected:
  /**
   * @brief Detect if Dust Poops are overlapping in the collider
   * @param _lOutDust list with the dust poops detected
   * @return true if some dust poop has been detected, false otherwise
   */
  UFUNCTION()
  bool DetectCleanCollision(TArray<FHitResult>& _lOutDust);

private:
  /**
   *  Current clean length in cm.
   */
  UPROPERTY(VisibleAnywhere, Category = "Bola13|Cleaning Collider", meta = (DisplayName = "Current Length"))
  float m_fCurrentLength = 100.0f;

  /**
   * Show the debug or not.
   */
  UPROPERTY(EditAnywhere, Category = "Bola13|Cleaning Debug", meta = (DisplayName = "Show Debug Length"))
  bool m_bShowDebug = false;

  /**
   * Show the debug or not.
   */
  UPROPERTY(EditAnywhere, Category = "Bola13|Cleaning VFX", meta = (DisplayName = "Whirlwind VFX Material"))
  TObjectPtr<UMaterial> m_pWhirlwindtMaterial;

#pragma region Components
  UPROPERTY(EditAnywhere, Category = "Bola13|Cleaning VFX", meta = (DisplayName = "Whirlwind VFX DinamciMaterial"))
  TObjectPtr<UMaterialInstanceDynamic> m_pDynamicMaterial;

  /* Clean tornado efect */
  UPROPERTY()
  TObjectPtr<UNiagaraComponent> m_pWhirlwindComponent;
#pragma endregion

#pragma region Variables
  /* The niagara effect of the cleaning */
  UPROPERTY(EditAnywhere, Category = "Bola13|Cleaning VFX", meta = (DisplayName = "Whirlwind VFX"))
  TObjectPtr<UNiagaraSystem> m_pWhirlwindEffect;

  UPROPERTY(EditAnywhere, Category = "Bola13")
  TArray<TEnumAsByte<EObjectTypeQuery>>  m_lObjectQueryParamsClean;

  UPROPERTY(EditAnywhere, Category = "Bola13")
  TArray<AActor*>  m_lActorsToIgnore;

  UPROPERTY(EditAnywhere, Category = "Bola13")
  TArray<TEnumAsByte<EObjectTypeQuery>>  m_lObjectQueryParamsRaycast;

  float m_fAttackRange;

  /* Is cleaning or no*/
  bool m_bIsVacuuming = false;

  /* Stop vacuum Lenght*/
  bool m_bStopLength = false;

  UPROPERTY()
  TWeakObjectPtr<ABaseWeapon> m_pOwnerWeapon;

  UPROPERTY()
  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

  UPROPERTY()
  TObjectPtr<USceneComponent> m_pMuzzle;

  UPROPERTY()
  TObjectPtr<USkeletalMeshComponent> m_pPlayerSkeletalMesh;

  UPROPERTY()
  TObjectPtr<UDirtminatorWeaponStatsDataAsset> m_pDataAsset;
#pragma endregion

#pragma region Functions
  /**
   * @brief Starts the VFX.
   */
  void StartCleaningVFX();

  /**
   * @brief Update the VFX.
   */
  void UpdateCleaningVFX();

  /**
   * @brief Stops the VFX.
   */
  void StopCleaningVFX();

  void SetPlayerVelocity(bool _bActivate);
#pragma endregion
};
