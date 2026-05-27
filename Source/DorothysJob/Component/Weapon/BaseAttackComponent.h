/************************************************************************
 * @description: Represents the Base Weapon Attack Component
 * @author: Josephine Esposito
 * @date: 21/07/2025
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseWeaponComponent.h"
#include "BaseAttackComponent.generated.h"

/**
 * The base weapon attack component
 */
UCLASS()
class DOROTHYSJOB_API UBaseAttackComponent : public UBaseWeaponComponent
{
	GENERATED_BODY()
	
protected:

  void DeactivateAttackFeedback();

  void ActivateAttackFeedback();

  void ActivateKillFeedback();

  UPROPERTY(EditAnywhere)
  float m_fFeedbackAttackTiming;

  UPROPERTY(EditAnywhere)
  float m_fFeedbackKillTiming;
};
