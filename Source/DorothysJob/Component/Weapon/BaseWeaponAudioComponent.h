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
   * @brief 
   */
  virtual void BeginPlay() override;

  /**
   * @brief 
   * @param _eType 
   */
  UFUNCTION()
  virtual void AttackSound(EAttackType _eType);

  /**
   * @brief 
   * @param _eType 
   */
  UFUNCTION()
  virtual void ReleaseAttack(EAttackType _eType);

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void CleanSound();

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void ReleaseClean();

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void SwitchWeapon();

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void SpecialCharge();

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void SpecialAttack();

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void ReleaseSpecialAttack();

  /**
   * @brief
   */
  UFUNCTION()
  virtual void OnEnemyHit(float _fMitigation, bool _bThirdAttack);

  /**
   * @brief 
   */
  UFUNCTION()
  virtual void PauseEvents();
  /**
   * @brief 
   */
  UFUNCTION()
  virtual void UnpauseEvents();

  /**
   * @brief 
   */
  TObjectPtr<UAudioManager> m_pAudioManager;
  /**
   * @brief 
   */
  TObjectPtr<UFMODAudioComponent> m_pCleanAudioComponent;
  /**
   * @brief
   */
  TObjectPtr<UFMODAudioComponent> m_pSpecialAudioComponent;
};
