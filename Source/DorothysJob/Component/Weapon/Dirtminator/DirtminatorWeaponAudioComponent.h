// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BaseWeaponAudioComponent.h"
#include "DirtminatorWeaponAudioComponent.generated.h"

UCLASS()
class DOROTHYSJOB_API UDirtminatorWeaponAudioComponent : public UBaseWeaponAudioComponent
{
	GENERATED_BODY()

  /**
   * @brief 
   * @param _eType 
   */
  virtual void AttackSound(EAttackType _eType) override;

  /**
   * @brief 
   * @param _eType 
   */
  virtual void ReleaseAttack(EAttackType _eType) override;

  /**
   * @brief 
   */
  virtual void CleanSound() override;

  /**
   * @brief 
   */
  virtual void ReleaseClean() override;
	
  /**
   * @brief 
   */
  virtual void SpecialAttack() override;

  /**
   * @brief 
   */
  virtual void ReleaseSpecialAttack() override;

  /**
   * @brief
   */
  virtual void PauseEvents() override;
  /**
   * @brief
   */
  virtual void UnpauseEvents() override;

  /**
   * @brief
   */
  bool m_bSpecialPaused = false;
};
