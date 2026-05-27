/************************************************************************
 * @description: Represents the Special Attack projectil of the WasheeFlashee
 * @author: Aurora María Fernández Basanta
 * @date: 12/06/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "BaseProjectile.h"
#include "SpecialAttackProjectile.generated.h"

class ABaseCharacter;
class UDrawing;
class UWFWeaponStatsDataAsset;

UCLASS()
class DOROTHYSJOB_API ASpecialAttackProjectile : public ABaseProjectile
{
  GENERATED_BODY()

public:
  ASpecialAttackProjectile();

protected:
  virtual void BeginPlay() override;

  virtual void OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

  UFUNCTION()
  void OnComponentPopingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

#pragma region | Components
  UPROPERTY(VisibleAnywhere, Category = "Bola13|Components", meta = (DisplayName = "Popping Collision"))
  TObjectPtr<USphereComponent> m_pPoppingCollision;
#pragma endregion

private:
#pragma region | Helpers
  float m_fHalfSpeed = 0.0f;

  FVector2D m_v2CurrentCleaningSize = FVector2D::ZeroVector;

  UPROPERTY()
  FTimerHandle m_oUpdateProjectileTimer;
  
  UPROPERTY()
  FTimerHandle m_oInitCheckTimer;

  UPROPERTY(VisibleAnywhere, Category = "Bola 13|Projectile Cleaning", meta = (DisplayName = "Drawing Component"))
  TObjectPtr<UDrawing> m_pDrawing;

  TObjectPtr<UWFWeaponStatsDataAsset> m_pWFDataAsset;

  void UpdateProjectile();

  UFUNCTION()
  void ValidateShootData();
#pragma endregion

#pragma region | VFX
  UPROPERTY(EditDefaultsOnly, Category = "Bola13|VFX", meta = (DisplayName = "Projectile Destroy VFX"))
  TObjectPtr<UNiagaraSystem> m_pDestroyVFX = nullptr;
#pragma endregion

#pragma region | Audio
  /**
   * @brief
   */
  UPROPERTY(EditAnywhere, Category = "Bola13|Audio", meta = (DisplayName = "Special Loop FMOD Event"))
  TObjectPtr<UFMODEvent> m_pSpecialLoopFMODEvent;

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
  bool m_bSpecialPaused = false;

  /**
   * @brief 
   */
  FDelegateHandle m_oPauseHandle;
  /**
   * @brief 
   */
  FDelegateHandle m_oUnpauseHandle;
#pragma endregion
};