/************************************************************************
 * @description: Represents the Liquid Component of Washee & Flashee.
 *							 This class determines the status applied to the enemies.
 * @author: Pablo Velasco
 * @date: 11/03/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "../BaseWeaponComponent.h"
#include "LiquidComponent.generated.h"

class UWFWeaponStatsDataAsset;
class AWasheeFlashee;

UCLASS()
class DOROTHYSJOB_API ULiquidComponent : public UBaseWeaponComponent
{
  GENERATED_BODY()

public:

  ULiquidComponent();

  UFUNCTION()
  virtual void Use() override;

protected:
  virtual void BeginPlay() override;

  virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
  TObjectPtr<UWFWeaponStatsDataAsset> m_pWFDataAsset;

  bool ValidateShootData();

  void Shoot();

  void FireSingleBullet();

  UPROPERTY()
  FTimerHandle BurstTimerHandle;

  int32 m_iBurstShotsFired = 0;

  FActorSpawnParameters m_oSpawnParams;

  TWeakObjectPtr<AWasheeFlashee> m_pOwnerWeapon;

  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

  UWorld* CachedWorld;
};
