/************************************************************************
 * @description: Player animation instance class
 * @author: David Gonzalez
 * @date: 12/06/2025
 * @edited_by: [Other Contributors]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Animations/BaseAnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class ABasePlayer;
class UBlendSpace;
class UBaseWeaponStatsDataAsset;
class UWeaponAnimInstance;
enum class EDirtType : uint8;
enum class EWeaponAttackType : uint8;

UCLASS()
class DOROTHYSJOB_API UPlayerAnimInstance : public UBaseAnimInstance
{
	GENERATED_BODY()

public:

  /**
   * @brief It plays the current montage section if exists.
   * @param _sSectionName -> Section name to play.
   * @param _fPlayRate -> Play rate to reproduce the action montage.
   * @return True if it could be played.
   */
  virtual bool PlayActionMontage(FName _sSectionName, float _fPlayRate = 1.f) override;

  /**
   * @brief It stops action animation.
   */
  virtual void StopActionMontage() override;

  /**
   * @brief It jumps to the section _sSectionName in the current montage.
   * @param _sSectionName -> Section name to play.
   * @return True if it can jump to the section.
   */
  bool JumpSectionActionMontage(FName _sSectionName);
	
protected:

  /**
   * @brief It initializes the animation.
   */
  virtual void NativeInitializeAnimation() override;

  /**
   * @brief It updates the current animation parameters.
   * @param _fDeltaSeconds -> Senconds since last time executed.
   */
  virtual void NativeUpdateAnimation(float _fDeltaSeconds) override;

  /**
   * @brief This function is executed when the montage ends.
   * @param _pMontage -> reference to the eneded montage.
   * @param _bInterrupted -> It shows if the monrage ends by an interruption or not.
   */
  virtual void OnActionMontageEnded(UAnimMontage* _pMontage, bool _bInterrupted) override;

  /**
   * @brief It jumps to the next section in the combo and returns the section FName.
   */
  virtual FName JumptoNextSection() override;

private:

  /**
   * It updates the related weapon animations when the current weapon changes.
   * @param _eAttackType -> Weapon dirt clean type.
   * @param _eWeaponAttackType -> Weapon attack type (distance, melee).
   */
  void UpdateWeaponAnimations(EDirtType _eAttackType, EWeaponAttackType _eWeaponAttackType);

  /**
   * Blend Weight.
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Anim Montage", meta = (DisplayName = "Blend Weigth", AllowPrivateAccess = "true"))
  float m_fBlendWeigth;

  /**
   * Lateral Speed.
   */
  UPROPERTY(BlueprintReadOnly, Category = "Bola 13|Movement", meta = (DisplayName = "Lateral Speed", AllowPrivateAccess = "true"))
  float m_fLateralSpeed;

  /**
   * Current weapon data asset.
   */
  TSoftObjectPtr<UBaseWeaponStatsDataAsset> m_pWeaponDataAsset;

  /**
   * Current weapon mesh component.
   */
  TSoftObjectPtr<USkeletalMeshComponent> m_pCurrentWeaponMesh;

  /**
   * Current weapon animation reference.
   */
  TSoftObjectPtr<UWeaponAnimInstance> m_pCurrentWeaponAnimInstance;

  /**
   * Animation owner player reference.
   */
  TSoftObjectPtr<ABasePlayer> m_pPlayer;

  /**
   * When the montage is currently playing.
   */
  bool m_bMontagePlaying;
};