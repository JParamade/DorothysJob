/************************************************************************
 * @description: Base animation instance class
 * @author: David Gonzalez
 * @date: 12/06/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

DECLARE_DELEGATE(FOnActionChanges);
DECLARE_DELEGATE_OneParam(FOnActionStarts, int32 /* _iCurrentCombo */);
DECLARE_DELEGATE_OneParam(FOnComboStateChanges, bool /* _bStarts */);

UCLASS()
class DOROTHYSJOB_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

  /**
   * @brief It plays action animation. It returns if the montage can be reproduced.
   * @param _sSectionName -> Section name to play.
   * @param _fPlayRate -> Play rate to reproduce the action montage.
   * @return True if it could be played.
   */
  UFUNCTION(BlueprintCallable)
  virtual bool PlayActionMontage(FName _sSectionName, float _fPlayRate = 1.f);

  /**
   * @brief It stops action animation.
   */
  UFUNCTION(BlueprintCallable)
  virtual void StopActionMontage();

  /**
   * @brief It returns if the action montage animation is currently playing.
   */
  UFUNCTION(BlueprintCallable)
  virtual bool IsActionMontagePlaying();

  /**
   * @brief It opens/closes the combo window.
   */
  UFUNCTION(BlueprintCallable)
  virtual void SetComboWindowOpen(bool _bOpen);

  /**
   * @brief It sets if the combo will continue or not.
   * @param _bActive -> Is combo active.
   */
  UFUNCTION(BlueprintCallable)
  void SetComboActive(bool _bActive);

  /**
   * @brief It returns the current combo index. 
   */
  int32 GetCurrentComboIndex();

  /**
   * Delegate to notify when the action starts.
   */
  FOnActionStarts OnActionStarts;

  /**
   * Delegate to notify when the action ends.
   */
  FOnActionChanges OnActionEnds;

  /**
   * Delegate to notify when the action montage ends. 
   */
  FOnActionChanges OnActionMontageEnds;

  /**
   * Delegate to notify when the combo starts/ends.
   */
  FOnComboStateChanges OnComboStateChanges;
	
protected:

  /**
   * @brief It initializes the animation.
   */
  virtual void NativeInitializeAnimation() override;

  /**
   * @brief It updates the current animation parameters.
   * @param _fDeltaSeconds -> Senconds since last time executed.
   */
  virtual void NativeUpdateAnimation(float DeltaSeconds) override;

  /**
   * @brief This function is executed when the montage ends.
   * @param _pMontage -> reference to the eneded montage.
   * @param _bInterrupted -> It shows if the monrage ends by an interruption or not.
   */
  UFUNCTION()
  virtual void OnActionMontageEnded(UAnimMontage* _pMontage, bool _bInterrupted);

  /**
   * @brief It jumps to the next section in the combo and returns the section FName.
   */
  virtual FName JumptoNextSection();

  /**
   * Movement Speed.
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Movement", meta = (DisplayName = "Speed"))
  float m_fSpeed;

  /**
   * Movement animation for the current weapon.
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Animations", meta = (DisplayName = "Movement Blend Space"))
  UBlendSpace* m_pMovementBlendSpace;

  /**
   * Attack animation for the current weapon
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Animations", meta = (DisplayName = "Action Montage"))
  UAnimMontage* m_pActionMontage;

  /**
   * First como section mane tag.
   */
  FName m_sComboSectionName;

  /**
   * Current combo index.
   */
  int32 m_iCurrentCombo;

  /**
   * Action combo window open/close.
   */
  bool m_bComboWindowOpen;

  /**
   * @brief Action combo input queued for the next step.
   */
  bool m_bComboActive;
};