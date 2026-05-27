/************************************************************************
 * @description: Represents the Base Projectile
 * @author: Pablo Velasco
 * @date: 12/03/2025
 * @edited_by: [Aurora Maria Fernandez Basanta] - [15/06/25]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "BaseProjectile.generated.h"

class USphereComponent;
class UBaseProjectileMovementComponent;
class ABaseWeapon;
class UNiagaraSystem;
class UNiagaraComponent;
class UFMODEvent;
class UFMODAudioComponent;
class UAudioManager;
class UBaseWeaponStatsDataAsset;
class UEnemyPropertiesDataAsset;

DECLARE_DYNAMIC_DELEGATE(FProjectileDestroy);

UCLASS()
class DOROTHYSJOB_API ABaseProjectile : public ABaseActor
{
  GENERATED_BODY()

public:
  ABaseProjectile();

  FProjectileDestroy OnProjectileFinished;

protected:
  virtual void BeginPlay() override;

  UPROPERTY(EditAnywhere, Category = "Bola13", meta = (DisplayName = "Damage Index"))
  int32 m_iDamageIndex = 0;

protected:
#pragma region | Helpers
  UPROPERTY()
  TWeakObjectPtr<ABaseWeapon> m_pWeaponOwner;

  UPROPERTY()
  TWeakObjectPtr<UBaseWeaponStatsDataAsset> m_pWeaponDataAsset;

  UPROPERTY()
  TWeakObjectPtr<UEnemyPropertiesDataAsset> m_pEnemyDataAsset;

  UPROPERTY()
  UWorld* CachedWorld;

  UPROPERTY()
  UAudioManager* m_pAudioManager;

  UFUNCTION()
  virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

  void InitVelocity(FVector ShootDirection);
#pragma endregion

#pragma region | Variables
  UPROPERTY(EditAnywhere, Category = "Bola13|Stats", meta = (DisplayName = "Damage"))
  int32 m_iDamage = 10; // @TODO: delete this after taking the damage from data asset

  UPROPERTY(EditAnywhere, Category = "Bola13|Stats", meta = (DisplayName = "Speed"))
  float m_fSpeed;
#pragma endregion

#pragma region | Components
  UPROPERTY(VisibleAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Collision"))
  TObjectPtr<USphereComponent> m_pCollisionComponent;

  UPROPERTY(EditAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Movement"))
  TObjectPtr<UBaseProjectileMovementComponent> m_pMovementComponent;

  UPROPERTY(VisibleAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Mesh"))
  TObjectPtr<UStaticMeshComponent> m_pMeshComponent;

  UPROPERTY(VisibleAnywhere, Category = "Bola13|Components", meta = (DisplayName = "FMOD Audio"))
  TObjectPtr<UFMODAudioComponent> m_pAudioComponent;
#pragma endregion

#pragma region | VFX
  UPROPERTY(EditDefaultsOnly, Category = "Bola13|VFX", meta = (DisplayName = "Projectile Impact VFX"))
  TObjectPtr<UNiagaraSystem> m_pImpactVFX = nullptr;

  UPROPERTY(EditAnywhere, Category = "Bola13|VFX", meta = (DisplayName = "Projectile VFX"))
  TObjectPtr<UNiagaraComponent> m_pProjectileVFX;
#pragma endregion

#pragma region | Audio
  /**
   * @brief
   * @param _pEvent
   */
  virtual void PlayFMODSound(UFMODEvent* _pEvent, float _fMitigation);

  /**
   * @brief
   */
  UPROPERTY(EditAnywhere, Category = "Bola13|Audio", meta = (DisplayName = "Impact FMOD Event"))
  TObjectPtr<UFMODEvent> m_pImpactFMODEvent;
#pragma endregion

  virtual void PreDestroy();
};
