/************************************************************************
 * @description: Represents the SpecialAttack Component of Washee & Flashee.
 * 
 * @author: Pablo Velasco
 * @date: 11/03/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseWeaponComponent.h"
#include "SpecialProjectileComponent.generated.h"

class UWFWeaponStatsDataAsset;
class AWasheeFlashee;

UCLASS()
class DOROTHYSJOB_API USpecialProjectileComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()
	
public:

  USpecialProjectileComponent();

  UFUNCTION()
  virtual void Use() override;

protected:
  virtual void BeginPlay() override;

private:

  UFUNCTION()
  void StopSpecialShake();

  TObjectPtr<UWFWeaponStatsDataAsset> m_pWFDataAsset;

  bool ValidateShootData();

  void Shoot() const;

  FActorSpawnParameters m_oSpawnParams;

  TWeakObjectPtr<AWasheeFlashee> m_pOwnerWeapon;

  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

  UWorld* CachedWorld;
};
