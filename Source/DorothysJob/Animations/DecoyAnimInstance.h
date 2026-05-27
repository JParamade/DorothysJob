/************************************************************************
 * @description: Decoy animation instance class
 * @author: David Gonzalez
 * @date: 16/09/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DecoyAnimInstance.generated.h"

DECLARE_DELEGATE(FOnActionMontageEnds);

UCLASS()
class DOROTHYSJOB_API UDecoyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

  /**
   * @brief It plays the inflate animation.
   */
  void PlayInflateAnimation();

  /**
   * @brief It plays the hit animation.
   */
  void PlayHitAnimation();

  /**
   * @brief It plays the death animation.
   */
  void PlayDeathAnimation();

  /**
   * @brief It sets the action montage to play in the decoy.
   * @param _pActionMontage 
   */
  void SetActionMontage(TObjectPtr<UAnimMontage> _pActionMontage);

  /**
   * Delegate to notify when the action montage ends.
   */
  FOnActionMontageEnds OnActionMontageEnds;
	
private:

  /**
   * Action animation for the decoy
   */
  TObjectPtr<UAnimMontage> m_pActionMontage;
};