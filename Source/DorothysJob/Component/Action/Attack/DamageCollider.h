/************************************************************************
 * @description: Represents a Damage Collider with VFX
 * @author: Josephine
 * @date: 24/07/2025
 * @edited_by: 
 ************************************************************************/
#pragma once

#include "CoreMinimal.h"
#include "DorothysJob/Actor/BaseActor.h"
#include "DamageCollider.generated.h"

#pragma region | Forward Declarations
class UBoxComponent;
class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class ABaseEnemy;
#pragma endregion

/**
* @brief Declaration of the Category for Logs
*/
DECLARE_LOG_CATEGORY_EXTERN(ColliderLog, Log, All);

UENUM(BlueprintType)
enum class EColliderShape : uint8
{
	None		UMETA(DisplayName = "None"),
	Box     UMETA(DisplayName = "Box"),
	Sphere  UMETA(DisplayName = "Sphere")
};

USTRUCT()
struct FGenericCollider
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Shape"))
	EColliderShape m_eColliderShape = EColliderShape::None;

	UPROPERTY()
	TObjectPtr<UShapeComponent> m_pCollider = nullptr;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "m_eColliderShape == EColliderShape::Box", DisplayName = "Size", EditConditionHides))
	FVector m_pSize = FVector::OneVector;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "m_eColliderShape == EColliderShape::Sphere", DisplayName = "Radius", EditConditionHides))
	float m_fRadius = 1.f;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "m_eColliderShape != EColliderShape::None", DisplayName = "Offset", EditConditionHides))
	FVector m_vPosOffset = FVector::ZeroVector;
};



/**
 * @brief This handles a list of colliders with damage and visual effects
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DOROTHYSJOB_API ADamageCollider : public ABaseActor
{
	GENERATED_BODY()

public:

	/**
	 * @brief Sets default values for this component's properties
	 */
	ADamageCollider();

	/**
	 * @brief This will activate or deactivate the colliders and visual effects. You can get the overlapping actors with the OnOverlap method
	 * @return A flag stating if the effect was successfull or not
	 */
	UFUNCTION()
	bool OnActive(bool _flag);

	/**
	 * @brief This will return the array of actors that are overlapping with all the registered colliders
	 * @return 
	 */
	void OnOverlap(TArray<AActor*>& _lActors);

	/**
	 * @brief This will return the array of actors that are overlapping with the given saved collider
	 * @param _index The collider saved into the array
	 * @return 
	 */
	void OnOverlap(TArray<AActor*>& _lActors, int _index);

	/**
	 * @brief [EDITOR_ONLY] This will draw a debug box for each collider.
	 * @param _color the color of the collider
	 */
	void DrawDebug(FColor _color);

	/**
	 * @brief Called when the game starts
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief This will initialize the collider
	 * @param _root 
	 */
	void Init(USceneComponent* _root);

	/**
	 * @brief This will update the rotation of all the colliders
	 * @param _rotator 
	 */
	void UpdateRotation(FRotator _rotator);

	void UpdateBoxRotations(FRotator _rotator);

	void UpdateLocation(FVector _location);

	void SetSphereRadius(float _radius);

	void SetCollisionResponse(ECollisionChannel _channel, ECollisionResponse _response);

	void SetBoxCollisionResponse(ECollisionChannel _channel, ECollisionResponse _response);

	void SetSphereCollisionResponse(ECollisionChannel _channel, ECollisionResponse _response);

	UShapeComponent* GetCollider(int _index);

	TArray<UShapeComponent*> GetColliders();

	TArray<UShapeComponent*> GetBoxColliders();

	TArray<UShapeComponent*> GetSphereColliders();

	/**
	 * @brief Holds the box colliders with their relative damage and size
	 */
	UPROPERTY(EditAnywhere, Category = "Bola 13|Collider", meta = (DisplayName = "Colliders' sizes"))
	TArray<FGenericCollider> m_lColliders;

	UPROPERTY()
	USceneComponent* m_pRoot;
};
//EOF