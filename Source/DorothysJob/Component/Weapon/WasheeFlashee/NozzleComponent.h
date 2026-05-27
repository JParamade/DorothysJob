/************************************************************************
 * @description: Represents the Nozzle Component of Washee & Flashee.
 *							 This class determines the Type of damage and cleaning.
 * @author: Pablo Velasco
 * @date: 11/03/2025
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseWeaponComponent.h"
#include "NozzleComponent.generated.h"

class UDrawing;
class UWFWeaponStatsDataAsset;
class AWasheeFlashee;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCleanAction, FVector, position, FRotator, rotation);

UCLASS()
class DOROTHYSJOB_API UNozzleComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()

public:
  UNozzleComponent();

  virtual void Use() override;

  void CleanSprite();
  
  void SetDrawingComp(UDrawing* _comp);

  UPROPERTY(BlueprintAssignable)
  FOnCleanAction OnCleanAction;

protected:
  virtual void BeginPlay() override;

private:
  UPROPERTY()
  FTimerHandle m_CleanTimerHandle;

  UPROPERTY(EditAnywhere, Category = "Cleaning", meta = (DisplayName = "Drawing Component"))
  TObjectPtr<UDrawing> m_pDrawing;
  
  int32 m_iCurrentCleanTexture;

  UPROPERTY()
  TWeakObjectPtr<AWasheeFlashee> m_pOwnerWeapon;

  UPROPERTY()
  TWeakObjectPtr<ABasePlayer> m_pOwnerPlayer;

  UPROPERTY()
  TObjectPtr<UWFWeaponStatsDataAsset> m_pDataAsset;

  UPROPERTY()
  UWorld* CachedWorld;
};
