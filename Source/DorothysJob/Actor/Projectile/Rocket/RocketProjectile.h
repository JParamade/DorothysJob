/************************************************************************
 * @description: Represents the Rocket Projectile
 * @author: Pablo Velasco
 * @date: 11/05/2025
 * @edited_by: Aurora Maria Fernandez - 23/07/2025
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/Projectile/BaseProjectile.h"
#include "RocketProjectile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRocketExplode, float, ExplosionRange);

class ADirtminator;
class ABasePlayer;
class UDirtminatorWeaponStatsDataAsset;

UCLASS()
class DOROTHYSJOB_API ARocketProjectile : public ABaseProjectile
{
  GENERATED_BODY()

public:

  /* Constructor */
  ARocketProjectile();

  /* Delegate for when the projectile explode */
  UPROPERTY(BlueprintAssignable)
  FOnRocketExplode OnRocketExplode;

protected:

  /**
   * @brief Function when the game starts.
   */
  virtual void BeginPlay() override;

  /**
   * @brief What happends when the projectile collide with something.
   */
  virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

  /**
   * @brief What happends when the projectile explodes.
   */
  void Explode(AActor* _OtherActor);

  /**
   * @brief In the editor, when a variable change, do this. In this case is for the debug.
   */
  void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);

  UPROPERTY()
  TWeakObjectPtr<ADirtminator> m_pDirtminatorOwner;
  
  UPROPERTY()
  TWeakObjectPtr<UDirtminatorWeaponStatsDataAsset> m_pDirtminatorDataAsset;

private:
#pragma region Edit Variables
  /* The explosion radious. */
  UPROPERTY(EditAnywhere, Category = "Bola13|Explosion", meta = (DisplayName = "Explosion Ray Detection"))
  float m_fExplosionRayDetection;

  /* The explosion radious. */
  UPROPERTY(EditAnywhere, Category = "Bola13|Explosion", meta = (DisplayName = "Explosion Radius"))
  float m_fExplosionRadius;

  /* The explosion damage. */
  UPROPERTY(EditAnywhere, Category = "Bola13|Explosion", meta = (DisplayName = "Explosion Damage"))
  int m_iExplosionDamage;

  /* The channels the projetile hurt, pwan, visibility, etc. */
  UPROPERTY(EditAnywhere, Category = "Bola13|Explosion", meta = (DisplayName = "Collision params"))
  TArray<TEnumAsByte<EObjectTypeQuery>> m_oChannels;

  /* The explosion VFX. */
  UPROPERTY(EditAnywhere, Category = "Bola13|Explosion VFX", meta = (DisplayName = "Explosion VFX"))
  UNiagaraSystem* m_oExplosionVFX;

  /* The explosion VFX radius. */
  UPROPERTY(EditAnywhere, Category = "Bola13|Explosion VFX", meta = (DisplayName = "Explosion VFX Radius"))
  FVector m_vExplosionVFXSRadius;

  /* The radius preview of the explosion. This is for the debug only. */
  UPROPERTY(VisibleAnywhere, Category = "Bola13|Visual", meta = (DisplayName = "Explosion Radius Preview Debug"))
  USphereComponent* m_pExplosionRadiusPreview;

  UPROPERTY(EditAnywhere, Category = "Bola13|Visual", meta = (DisplayName = "Line trace detection for explosion"))
  TArray<TEnumAsByte<EObjectTypeQuery>> m_oParams;

  UPROPERTY(EditAnywhere, Category = "Bola13|Tier", meta = (DisplayName = "Bullet Tier"))
  int m_iBulletTier = 1;

  UPROPERTY()
  TArray<AActor*> ActorsToIgnore;
#pragma endregion

#pragma region | Audio
  /**
   * @brief
   * @param _pEvent
   */
  virtual void PlayFMODSound(UFMODEvent* _pEvent, float _fMitigation) override;
#pragma endregion
};
