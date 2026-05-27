// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseActorComponent.h"
#include "BaseWeaponAudioComponent.generated.h"

class UAudioManager;
class UFMODAudioComponent;

UCLASS()
class DOROTHYSJOB_API UBaseWeaponAudioComponent : public UBaseActorComponent
{
  GENERATED_BODY()

protected:
  /**
   * @brief Initializes the component and binds to the weapon's delegates to handle audio events when the game starts.
   */
  virtual void BeginPlay() override;

  /**
   * @brief Plays the attack sound based on the attack type.
   * @param _eType The type of attack being performed, used to determine which sound to play.
   */
  UFUNCTION()
  virtual void AttackSound(EAttackType _eType);

  /**
   * @brief Plays the appropriate sound when an attack is released, based on the attack type.
   * @param _eType The type of attack being released, used to determine which sound to play.
   */
  UFUNCTION()
  virtual void ReleaseAttack(EAttackType _eType);

  /**
   * @brief Plays the appropriate sound when a clean action is initiated.
   */
  UFUNCTION()
  virtual void CleanSound();

  /**
   * @brief Plays the appropriate sound when a clean action is released.
   */
  UFUNCTION()
  virtual void ReleaseClean();

  /**
   * @brief Plays the appropriate sound when the weapon is switched.
   */
  UFUNCTION()
  virtual void SwitchWeapon();

  /**
   * @brief Plays the appropriate sound when a special charge action is initiated.
   */
  UFUNCTION()
  virtual void SpecialCharge();

  /**
   * @brief Plays the appropriate sound when a special attack is initiated.
   */
  UFUNCTION()
  virtual void SpecialAttack();

  /**
   * @brief Plays the appropriate sound when a special attack is released.
   */
  UFUNCTION()
  virtual void ReleaseSpecialAttack();

  /**
   * @brief Plays the appropriate sound when an enemy is hit, based on the mitigation and whether it's a third attack.
   */
  UFUNCTION()
  virtual void OnEnemyHit(float _fMitigation, bool _bThirdAttack);

  /**
   * @brief Pauses all currently playing audio events associated with this weapon.
   */
  UFUNCTION()
  virtual void PauseEvents();
  /**
   * @brief Unpauses all currently paused audio events associated with this weapon.
   */
  UFUNCTION()
  virtual void UnpauseEvents();

  /**
   * @brief Reference to the Audio Manager, used to play and manage audio events for this weapon.
   */
  TObjectPtr<UAudioManager> m_pAudioManager;
  /**
   * @brief Reference to the Clean Audio Component, used to play clean-related audio events for this weapon.
   */
  TObjectPtr<UFMODAudioComponent> m_pCleanAudioComponent;
  /**
   * @brief Reference to the Special Audio Component, used to play special-related audio events for this weapon.
   */
  TObjectPtr<UFMODAudioComponent> m_pSpecialAudioComponent;
};
