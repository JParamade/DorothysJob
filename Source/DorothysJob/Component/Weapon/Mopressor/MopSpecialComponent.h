#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseWeaponComponent.h"
#include "MopSpecialComponent.generated.h"

class AMopressor;
class UMoppresorWeaponStatsDataAsset;
class UDrawing;
class ADamageCollider;

UCLASS()
class DOROTHYSJOB_API UMopSpecialComponent : public UBaseWeaponComponent
{
  GENERATED_BODY()

public:
  virtual void Use() override;

  void SpecialExplosion();

  void SetHasShoot(bool _shot);

  void SetDamageCollider(ADamageCollider* _DamageCollider);

protected:
  virtual void BeginPlay() override;

private:
  UPROPERTY()
  float m_fCurrentWhirlCharge = 0.0f;

  UPROPERTY()
  bool m_bHasShoot = false;

  UPROPERTY()
  TWeakObjectPtr<ABasePlayer> m_pPlayerOwner = nullptr;

  UPROPERTY()
  TWeakObjectPtr<AMopressor> m_pWeaponOwner = nullptr;

  UPROPERTY()
  TObjectPtr<UMoppresorWeaponStatsDataAsset> m_pDataAsset = nullptr;

  UPROPERTY()
  TObjectPtr<UDrawing> m_pDrawingComponent = nullptr;

  UPROPERTY()
  TObjectPtr<ADamageCollider> m_pDamageCollider = nullptr;
};
