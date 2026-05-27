#include "DamageCollider.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
// UTILS
#include "Logging/StructuredLog.h"														          // for the loggin
#include "Components/BoxComponent.h"													          // for box collision detection
#include "Components/SphereComponent.h"																	// for sphere collision detection
#include "DrawDebugHelpers.h"                                           // for the draw debugs

/**
 * @brief Defining the Log category
 * @param The Log category
 */
DEFINE_LOG_CATEGORY(ColliderLog);

ADamageCollider::ADamageCollider()
{
	m_pRoot = nullptr;
	m_lColliders.Empty();
}

void ADamageCollider::DrawDebug(FColor _color)
{
#if WITH_EDITOR
	for (FGenericCollider& collider : m_lColliders)
	{
		switch (collider.m_eColliderShape)
		{
		case EColliderShape::None:
			break;

		case EColliderShape::Box:
		{
			DrawDebugBox(GetWorld(), collider.m_pCollider->GetComponentLocation(), collider.m_pSize, collider.m_pCollider->GetComponentQuat(), _color, false, 0.05f, 0.0f, 5.0f);
		}
		break;

		case EColliderShape::Sphere:
		{
			DrawDebugSphere(GetWorld(), collider.m_pCollider->GetComponentLocation(), collider.m_fRadius, 20, _color, false, 0.1f, 0.0f, 5.0f);
		}
		break;

		default:
			break;
		}

		
	}
#endif
}

void ADamageCollider::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* RootComp = GetRootComponent();
	if (!RootComp)
	{
		UE_LOG(ColliderLog, Error, TEXT("No root component found."));
		return;
	}

	Init(RootComp);
}

void ADamageCollider::Init(USceneComponent* _root)
{
	if (m_lColliders.Num() == 0)
	{
		UE_LOG(ColliderLog, Warning, TEXT("[BeginPlay]\tNo colliders configured."));
		return;
	}

	for (FGenericCollider& ColliderData : m_lColliders)
	{
		UShapeComponent* collider = nullptr;

		switch (ColliderData.m_eColliderShape)
		{
		case EColliderShape::None:
			break;

		case EColliderShape::Box:
		{
			UBoxComponent* box = NewObject<UBoxComponent>(this);
			if (box) box->SetBoxExtent(ColliderData.m_pSize);
			collider = box;
		}
		break;

		case EColliderShape::Sphere:
		{
			USphereComponent* sphere = NewObject<USphereComponent>(this);
			if (sphere) sphere->InitSphereRadius(ColliderData.m_fRadius);
			collider = sphere;
		}
		break;

		default:
			break;
		}

		if (!collider)
		{
			continue;
		}

		collider->RegisterComponent();
		collider->AttachToComponent(_root, FAttachmentTransformRules::KeepRelativeTransform);
		collider->SetCollisionProfileName(TEXT("Weapon")); // this refers to the checks on the default colliders channels
		collider->SetGenerateOverlapEvents(true);
		collider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		collider->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		collider->Deactivate(); // disable until needed

		ColliderData.m_pCollider = collider;
	}
}

void ADamageCollider::UpdateRotation(FRotator _rotator)
{
	for (FGenericCollider& collider : m_lColliders)
	{
		collider.m_pCollider->SetRelativeRotation(_rotator);
	}
}

void ADamageCollider::UpdateBoxRotations(FRotator _rotator)
{
	for (FGenericCollider& collider : m_lColliders)
	{
		if (collider.m_eColliderShape == EColliderShape::Box)
		{
			collider.m_pCollider->SetRelativeRotation(_rotator);
		}
	}
}

void ADamageCollider::UpdateLocation(FVector _location)
{
	for (FGenericCollider& collider : m_lColliders)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Damage Location = %i, %i, %i"), static_cast<int>(newPos.X), static_cast<int>(newPos.Y), static_cast<int>(newPos.Z)));
		collider.m_pCollider->SetRelativeLocation(collider.m_vPosOffset + _location);
	}
}

void ADamageCollider::SetSphereRadius(float _radius)
{
	for (FGenericCollider& collider : m_lColliders)
	{
		if (collider.m_eColliderShape == EColliderShape::Sphere)
		{
			if (USphereComponent* sphere = Cast<USphereComponent>(collider.m_pCollider))
			{
				sphere->SetSphereRadius(_radius);
			}
		}
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, FString::Printf(TEXT("Damage Location = %i, %i, %i"), static_cast<int>(newPos.X), static_cast<int>(newPos.Y), static_cast<int>(newPos.Z)));
	}
}

void ADamageCollider::SetCollisionResponse(ECollisionChannel _channel, ECollisionResponse _response)
{
	for (FGenericCollider& collider : m_lColliders)
	{
		collider.m_pCollider->SetCollisionResponseToChannel(_channel, _response);
	}
}

void ADamageCollider::SetBoxCollisionResponse(ECollisionChannel _channel, ECollisionResponse _response)
{
	for (FGenericCollider& collider : m_lColliders)
	{
		if (collider.m_eColliderShape == EColliderShape::Box)
		{
		  collider.m_pCollider->SetCollisionResponseToChannel(_channel, _response);
		}
	}
}

void ADamageCollider::SetSphereCollisionResponse(ECollisionChannel _channel, ECollisionResponse _response)
{
	for (FGenericCollider& collider : m_lColliders)
	{
		if (collider.m_eColliderShape == EColliderShape::Sphere)
		{
			collider.m_pCollider->SetCollisionResponseToChannel(_channel, _response);
		}
	}
}

UShapeComponent* ADamageCollider::GetCollider(int _index)
{
	if (m_lColliders.IsValidIndex(_index))
	{
		return m_lColliders[_index].m_pCollider;
	}
	return nullptr;
}

TArray<UShapeComponent*> ADamageCollider::GetColliders()
{
	TArray<UShapeComponent*> lColliders_;

	for (FGenericCollider& coll : m_lColliders)
	{
		if (coll.m_pCollider)
		{
			lColliders_.Add(coll.m_pCollider);
		}
	}
	return lColliders_;
}

TArray<UShapeComponent*> ADamageCollider::GetBoxColliders()
{
	TArray<UShapeComponent*> lColliders_;

	for (FGenericCollider& coll : m_lColliders)
	{
		if (coll.m_eColliderShape == EColliderShape::Box && coll.m_pCollider)
		{
			lColliders_.Add(coll.m_pCollider);
		}
	}
	return lColliders_;
}

TArray<UShapeComponent*> ADamageCollider::GetSphereColliders()
{
	TArray<UShapeComponent*> lColliders_;

	for (FGenericCollider& coll : m_lColliders)
	{
		if (coll.m_eColliderShape == EColliderShape::Sphere && coll.m_pCollider)
		{
			lColliders_.Add(coll.m_pCollider);
		}
	}
	return lColliders_;
}


bool ADamageCollider::OnActive(bool _flag)
{
	if (_flag)
	{
		SetCollisionResponse(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Overlap);
		// Colliders
		for (FGenericCollider& collider : m_lColliders)
		{
			collider.m_pCollider->Activate();
		}
	}
	else
	{
		SetCollisionResponse(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
		// Colliders
		for (FGenericCollider& collider : m_lColliders)
		{
			collider.m_pCollider->Deactivate();
		}
	}

	return true;
}

void ADamageCollider::OnOverlap(TArray<AActor*>& _lActors)
{
	TSet<AActor*> lUniqueActors; // using a set to prevent duplicates
	TArray<AActor*> lActors; // using a temporary array to get all the overlapped actors

	for (FGenericCollider& collider : m_lColliders)
	{
		lActors.Empty(); // we clean the temp array
		collider.m_pCollider->GetOverlappingActors(lActors); // we get all the actors of class BaseEnemy
		
		for (AActor* actor : lActors)
		{
			// if the actor is a duplicate it won't be added here
			lUniqueActors.Add(actor);
		}
	}

	_lActors = lUniqueActors.Array(); // we copy the resulting array into the receiving one
}

void ADamageCollider::OnOverlap(TArray<AActor*>& _lActors, int _index)
{
	if (m_lColliders.IsValidIndex(_index))
	{
		m_lColliders[_index].m_pCollider->GetOverlappingActors(_lActors);
		UE_LOG(ColliderLog, Warning, TEXT("Got %d actors from the overlap."), _lActors.Num());
	}
}

