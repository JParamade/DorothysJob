/************************************************************************
 * @description: Represents the Head Component of Mopressor.
 *							 This class determines the Type of damage and cleaning.
 * @author: Josephine Esposito
 * @date: 13/03/2025
 * @edited_by: Josephine - 21/07/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseAttackComponent.h"
#include "MopHeadComponent.generated.h"

class ADamageCollider;
class AMopressor;
class UMoppresorWeaponStatsDataAsset;
class UDrawing;

UCLASS(Blueprintable, BlueprintType, ClassGroup = (Drawing), meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UMopHeadComponent : public UBaseWeaponComponent
{
  GENERATED_BODY()

public:
  UMopHeadComponent();

  virtual void Use() override;

  virtual void Release() override;

  void DashStatus(bool _bActivate);

  void SetCollider(ADamageCollider* Collider);

protected:
  virtual void BeginPlay() override;

  UFUNCTION()
  virtual void OnDashColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
  void ActivateCollider(bool _bActivate);

  FVector CalculateCleaningLocation() const;

  bool m_bIsCleaning = false;

  bool m_bIsDashing = false;

  float m_fAngle = 0.0f;

  UPROPERTY()
  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

  UPROPERTY()
  TWeakObjectPtr<AMopressor> m_pOwnerWeapon;

  UPROPERTY()
  TObjectPtr<UMoppresorWeaponStatsDataAsset> m_pDataAsset;

  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pDashCollider; 
  
  UPROPERTY()
  TObjectPtr<UDrawing> m_pDrawingComponent;
};