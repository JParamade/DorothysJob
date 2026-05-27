/************************************************************************
 * @description: Represents the Base Actor in the Game.
 * @author: Pablo Velasco & Jorge Duart
 * @date: 05/03/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseWeaponAudioComponent.h"
#include "MoppressorWeaponAudioComponent.generated.h"

UCLASS()
class DOROTHYSJOB_API UMoppressorWeaponAudioComponent : public UBaseWeaponAudioComponent
{
	GENERATED_BODY()

  /**
   * @brief
   * @param _eType
   */
  virtual void AttackSound(EAttackType _eType) override;

  /**
   * @brief
   */
  virtual void CleanSound() override;

  /**
   * @brief
   */
  virtual void ReleaseClean() override;
};
