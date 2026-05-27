/************************************************************************
 * @description: Represents the Base Dust Poop
 * @author: Jorge Duart
 * @date: 14/05/2025
 * @edited_by: David Gonzalez
 ************************************************************************/

#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "DorothysJob/Interface/Damageable.h"
#include "DustPoop.generated.h"

class UDustHealthComponent;
class USphereComponent;
class UDrawing;
class ADustBunny;
class RandomNumberGenerator;
class UNiagaraComponent;
class UFMODEvent;
class ABaseEnemy;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDustPoopEliminate, ADustPoop*, _pDustPoopEliminated);

UENUM(BlueprintType)
enum class EDirWeakPoint : uint8
{
  NONE,
  NORTH,
  SOUTH,
  EAST,
  WEST
};

UCLASS()
class DOROTHYSJOB_API ADustPoop : public ABaseActor, public IDamageable
{
	GENERATED_BODY()

public:

  ADustPoop();

  /**
   * @brief It is called every frame
   * @param DeltaTime Time elapsed since last frame
   */
  virtual void Tick(float DeltaTime) override;

#pragma region | Stats
  /**
   * @brief Gets the Current Health
   * @return Current Health 
   */
  virtual int GetCurrentHealth() const override;

  /**
   * @brief It returns the character current health.
   * @return Character's Current Health.
   */
  virtual void SetCurrentHealth(int32 _iDamage) override;

  /**
   * @brief Gets the Total Health 
   * @return Total Health 
   */
  virtual int GetMaxHealth() const override;

  /**
   * @brief It is called when the character receives a damage hit.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   * @param _eAttackType -> Type to calculate the hit effectiveness.
   * @param _vHitLocation -> Hit impact location.
   * @param _vHitRotation -> Hit impact rotation.
   * @param _bIsCritical -> Is Critical hit.
   * @return The real damage after apply bonus and mitigation effects.
   */
  virtual int32 Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation = FVector::ZeroVector, const FRotator& _vHitRotation = FRotator::ZeroRotator, bool _bIsCritical = false) override;

  /**
   * @brief It is called when the character is healed.
   * @param _iModifyHealth -> The value to add to the current health. It should be positive.
   */
  virtual void Heal(int32 _iModifyHealth) override;
#pragma endregion

#pragma region | Getters
  /**
   * @brief It returns the Dust Poop static mesh.
   */
  TObjectPtr<UStaticMeshComponent> GetStaticMesh() const;

  /**
   * @brief It returns if a Dust Bunny is hidden in the dust poop.
   */
  bool IsDustBunnyHidden() const;
#pragma endregion

  /**
   * @brief It sets the current Dust Bunny hidden.
   * @param _pDustBunny -> Dust Bunny hidden.
   */
  void SetDustBunnyHidden(TWeakObjectPtr<ADustBunny> _pDustBunny);

  /**
   * @brief Function to create the dust dirt area.
   */
  UFUNCTION()
  void FloorDirt();

  /**
   * @brief Delegate to execute when the dust poop health changes.
   */
  UPROPERTY(BlueprintAssignable)
  FOnHealthChanged OnHealthChanged;

  /**
   * @brief Delegate to execute when the dust poop shoul be removed.
   */
  UPROPERTY(BlueprintAssignable)
  FOnDustPoopEliminate OnDustPoopEliminate;

protected:

  /**
   * @brief It is called when game starts or when spawned
   */
  virtual void BeginPlay() override;

private:

#pragma region | Private Functions
  /**
   * @brief It returns the damage multiplier if the hit is critical.
   */
  int CheckCriticalHit();

  /**
   * @brief It generates the weak point direction.
   */
  EDirWeakPoint GenerateWeakPoint() const;

  /**
   * @brief Rescale the mesh of the poop depending of the health.
   */
  void RescaleMeshHealth();

  /**
   * @brief Function to remove the dust poop.
   */
  UFUNCTION()
  void PoopCleaned();

  /**
   * @brief Function to modify the poop material.
   */
  UFUNCTION()
  void PoopMaterialModify(ABaseEnemy* _pEnemy);
#pragma endregion

#pragma region | Components
  /**
   * Dust poop health component.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Dust Health", AllowPrivateAccess = "true"))
  TObjectPtr<UDustHealthComponent> m_pDustHealthComponent;

  /**
   * Dust poop collision component.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Collision", AllowPrivateAccess = "true"))
  TObjectPtr<USphereComponent> m_pCollisionComponent;

  /**
   * Mesh asociated with the dust poop.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Mesh", AllowPrivateAccess = "true"))
  TObjectPtr<UStaticMeshComponent> m_pMeshComponent;

  /**
   * Component to draw in the floor the dust area.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Dirt", AllowPrivateAccess = "true"))
  TObjectPtr<UDrawing> m_pDrawingComponent;
  
  /**
   * Component to show the dust poop weak point VFX.
   */
  UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bola 13|Component", meta = (DisplayName = "Weak Point", AllowPrivateAccess = "true"))
  TObjectPtr<UNiagaraComponent> m_pWeakPointComponent;
#pragma endregion

#pragma region | Dust Poop Parameters
  /**
   * Dust poop dirt size.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Size", AllowPrivateAccess = "true"))
  FVector2D m_vDirtSize;

  /**
   * Dust poop dirt texture.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Dirt", meta = (DisplayName = "Dirt Texture", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pDirtTexture;

  /**
   * Dust poop clean texture.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Dirt", meta = (DisplayName = "Clean Texture", AllowPrivateAccess = "true"))
  TObjectPtr<UTexture2D> m_pCleanTexture;

  /**
   * Dust poop clean texture.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Skin", meta = (DisplayName = "Powered Material", AllowPrivateAccess = "true"))
  TObjectPtr<UMaterialInterface> m_pPoweredMaterial;

  /**
   * Dust poop max health.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Health", meta = (DisplayName = "Max Health", ClampMin = "0", AllowPrivateAccess = "true"))
  int m_iMaxHealth;

  /**
   * Multiplier when it is a critical hit.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Health", meta = (DisplayName = "Critical Damage Multiplier", AllowPrivateAccess = "true"))
  int m_iDamageMultiplier;

  /**
   * Audio for impact.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Audio", meta = (DisplayName = "Impact FMOD Event", AllowPrivateAccess = "true"))
  TObjectPtr<UFMODEvent> m_pImpactEvent;
  
  /**
   * Audio for critical impact.
   */
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Bola 13|Audio", meta = (DisplayName = "Critic FMOD Event", AllowPrivateAccess = "true"))
  TObjectPtr<UFMODEvent> m_pCriticEvent;

  /**
   * Dust poop weak point direction.
   */
  EDirWeakPoint m_eDirWS;

  /**
   * It is the dust poop where the dust bunny is hidden.
   */
  TWeakObjectPtr<ADustBunny> m_pDustBunnyHidden;

  /**
   * It is the random number generator.
   */
  TSharedPtr<RandomNumberGenerator> m_pRandom;
#pragma endregion

#pragma region | Debug
  /**
   * @brief Function to draw debug indicators in the dust poop.
   */
  void DrawDebug();

  /**
   * It shows/hide the debug information.
   */
  bool m_bShowDebug;
#pragma endregion
};