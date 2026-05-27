// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/Weapon/BaseAttackComponent.h"
#include "DustyHummingbirdFeathers.generated.h"

class IDamageable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDustyClean);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDustyCleanEnd);

/**
 * 
 */
UCLASS()
class DOROTHYSJOB_API UDustyHummingbirdFeathers : public UBaseAttackComponent
{
	GENERATED_BODY()
	
public:

  virtual void Use() override;

  virtual void Release() override; 

  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

  void CleanAttack();

  TArray<IDamageable*> GetCleaningObjects();

#pragma region Getters
  /**
   * @brief Gets the clean range of the colision
   * @return The float of the range
   */
  float GetCleanRange() const;

  /**
   * @brief Gets the size of the clean collision
   * @return Clean Area Radius
   */
  float GetCleanRadius() const;

#pragma endregion

#pragma region Setters

  /**
   * @brief Sets the clean range
   * @param _range
   */
  void SetCleanRange(float  _range);

  /**
   * @brief Set Clean Area Radius
   * @param _fRadius Clean Area Radius
   */
  void SetCleanRadius(float _fRadius);

  UPROPERTY(BlueprintAssignable)
  FOnDustyClean OnDustyClean;

  UPROPERTY(BlueprintAssignable)
  FOnDustyCleanEnd OnDustyCleanEnd;

#pragma endregion

private:

  /**
   * @brief Pointer to the Animation montage
   */
  TUniquePtr<UAnimMontage> m_pAnimMontage;
   
  UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Bola13|Stats", meta = (DisplayName = "Clean Range", AllowPrivateAccess = true))
  float m_fCleanRange;

  UPROPERTY(EditAnywhere, Category = "Bola13|Stats", meta = (DisplayName = "Clean Radius"))
  float m_fRadius;

  bool m_bIsCleaning = false;
};
