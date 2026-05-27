/************************************************************************
 * @description: Weapon animation instance class
 * @author: David Gonzalez
 * @date: 01/08/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "WeaponAnimInstance.generated.h"

UCLASS()
class DOROTHYSJOB_API UWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

  /**
   * @brief It plays the current montage section if exists.
   * @param _sSectionName -> Section name to play.
   * @param _fPlayRate -> Play rate to reproduce the action montage.
   * @return True if it could be played.
   */
  bool PlayActionMontage(FName _sSectionName, float _fPlayRate = 1.f);

  /**
   * @brief It stops action animation.
   */
  void StopActionMontage();

  /**
   * @brief It sets the left hand position.
   * @param _vHandSocketPosition -> left hand position.
   */
  void SetLeftHandSocketPosition(FVector _vHandSocketPosition);

  /**
   * @brief It sets the speed movement for the weapon.
   * @param _fForwardSpeed -> Forward speed.
   * @param _fLateralSpeed -> Lateral speed.
   */
  void SetSpeed(float _fForwardSpeed, float _fLateralSpeed);

private:

  /**
   * Action animation for the weapon
   */
  UPROPERTY(EditDefaultsOnly, Category = "Bola 13|Animation", meta = (DisplayName = "Action Montage", AllowPrivateAccess = "true"))
  TObjectPtr<UAnimMontage> m_pActionMontage;

  /**
   * Left hand socket.
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Animation", meta = (DisplayName = "Left Hand Socket", AllowPrivateAccess = "true"))
  FVector m_vLeftHandSocket;

  /**
   * Movement Speed.
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Movement", meta = (DisplayName = "Forward Speed", AllowPrivateAccess = "true"))
  float m_fForwardSpeed;

  /**
   * Lateral Speed.
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Movement", meta = (DisplayName = "Lateral Speed", AllowPrivateAccess = "true"))
  float m_fLateralSpeed;
};