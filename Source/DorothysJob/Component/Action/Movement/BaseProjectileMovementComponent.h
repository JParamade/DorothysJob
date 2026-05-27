/************************************************************************
 * @description: Represents the Projectile movement component.
 * @author: Aurora María Fernández Basanta
 * @date: 15/06/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Component/BaseActorComponent.h"
#include "BaseProjectileMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Custom, meta = (BlueprintSpawnableComponent))
class DOROTHYSJOB_API UBaseProjectileMovementComponent : public UBaseActorComponent
{
	GENERATED_BODY()
	
public:

  /**
   * @brief Default Constructor of the Projectile Movement Component
   */
  UBaseProjectileMovementComponent();

  /**
   * @brief Called every frame.
   *
   * @param DeltaTime, Time elapsed since the last frame.
   * @param TickType, Type of tick this is, indicating how the tick is being called (e.g., during gameplay or paused).
   * @param ThisTickFunction, Tick function used for this component, allowing control over tick order and behavior.
   */
  virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

#pragma region Getters&Setters
  /**
   * @brief Get movement velocity.
   */
  FVector GetVelocity();

  /**
   * @brief Get movement velocity.
   *
   * @param _vVelocity, vector to set the velocity.
   */
  void SetVelocity(FVector _vVelocity);

  /**
   * @brief Set movement "Can Push".
   * 
   * @param _bCanPush, boolean to set the variable.
   */
  void SetCanPush(bool _bCanPush);
#pragma endregion

protected:

  /**
   * @brief Called when the game starts.
   */
  virtual void BeginPlay() override;

private:

  /**
   * @brief Projectile Life Time.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Projectile Movement", meta = (DisplayName = "Life Time"))
  float m_fLifeTime = 100.0f;

  /**
   * @brief Projectile velocity.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Projectile Movement", meta = (DisplayName = "Velocity"))
  FVector m_vVelocity;

  /**
   * @brief Projectile push strength.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Projectile Movement", meta = (DisplayName = "Push Strength"))
  float PushStrength = 1500.0f;

  /**
   * @brief Projectile can push enemies.
   */
  UPROPERTY(EditAnywhere, Category = "Bola 13|Projectile Movement", meta = (DisplayName = "Can Push"))
  bool m_bCanPush;

  /**
   * @brief time that acumulates to see when the projectil gets to an end.
   */
  float m_fElapsedTime = 0.0f;

  /**
   * @brief Function to push the enemies actors with the projectile.
   * 
   * @param Hit, hit result of the collision with the enemies.
   * @param DeltaTime, Time elapsed since the last frame.
   */
  void PushActor(FHitResult Hit, float DeltaTime);


};
