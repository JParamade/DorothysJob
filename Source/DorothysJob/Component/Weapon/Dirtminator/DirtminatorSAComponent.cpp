#include "DirtminatorSAComponent.h"
#include "DorothysJob/Interface/Damageable.h"
#include "DorothysJob/Data/DataAssets/Weapon/DirtminatorWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Weapon/Dirtminator/Dirtminator.h"
#include "DorothysJob/Actor/Camera/MainLegacyCameraShake.h"
#include "DorothysJob/Component/Action/Attack/DamageCollider.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "Components/SphereComponent.h"	
#include "Engine/OverlapResult.h"
#include "Engine/World.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/Actor.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Dust/DustPoop.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "FMODBlueprintStatics.h"

// Sets default values for this component's properties
UDirtminatorSAComponent::UDirtminatorSAComponent()
{
  PrimaryComponentTick.bCanEverTick = false;
  m_pDirtminatorDamageColliderClass = nullptr;
}

// Called when the game starts
void UDirtminatorSAComponent::BeginPlay()
{
  Super::BeginPlay();

  SetSpecialAttackVariables();
}

void UDirtminatorSAComponent::InitSpecialAttack()
{
  if (!m_pOwnerWeapon.IsValid() || !IsValid(m_pDataAsset) || !IsValid(m_pSpecialDamageCollider))
  {
    return;
  }

  m_pOwnerWeapon->OnSpecialActive.Broadcast(true);
  m_pOwnerWeapon->OnSpecialStart.Broadcast();

  m_fIsSpecialAttackActive = true;
  SetPlayerVelocity(true);

  if (IsValid(m_pDataAsset->m_oSpecialAttackCameraShake))
  {
    m_pOwnerWeapon->SetCameraShake(m_pDataAsset->m_oSpecialAttackCameraShake);
  }

  m_pOwnerWeapon->StartCameraShake();
  m_pOwnerWeapon->StartControllerRumble(
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_fIntensity,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_fDuration,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftLarge,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsLeftSmall,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightLarge,
    m_pDataAsset->m_oSpecialAttackVibrationParams.m_bAffectsRightSmall);

  m_pSpecialDamageCollider->OnActive(true);

  if (UWorld* CachedWorld = GetWorld())
  {
    CachedWorld->GetTimerManager().SetTimer(SpecialAttackTimerHandle, this, &UDirtminatorSAComponent::SpecialAttackAction, TickInterval, true);
    // Timer for when the duratioon of the attack reach it limit.
    CachedWorld->GetTimerManager().SetTimer(SpecialAttackEndTimerHandle, this, &UDirtminatorSAComponent::EndSpecialAttack, m_pDataAsset->m_fSpecialAttackTime, false);
  }

  if (m_pOwnerPlayer.IsValid() && m_pOwnerPlayer->GetMuzzle() && IsValid(m_pDataAsset->m_pSpecialAttackVFX))
  {
    m_pSpecialAttackVFX = UNiagaraFunctionLibrary::SpawnSystemAttached(
      m_pDataAsset->m_pSpecialAttackVFX,
      m_pOwnerPlayer->GetMuzzle(),
      NAME_None,
      FVector::ZeroVector,
      FRotator::ZeroRotator,
      EAttachLocation::KeepRelativeOffset,
      true
    );
  }
}

void UDirtminatorSAComponent::SetSpecialAttackVariables()
{
  ADirtminator* pOwner = Cast<ADirtminator>(GetOwner());
  if (IsValid(pOwner))
  {
    m_pOwnerWeapon = pOwner;

    if (ABasePlayer* Player = Cast<ABasePlayer>(m_pOwnerWeapon->GetOwner()))
    {
      m_pOwnerPlayer = Player;
    }

    if (pOwner->GetDataAsset().IsValid())
    {
      if (UDirtminatorWeaponStatsDataAsset* oDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(pOwner->GetDataAsset().Get()))
      {
        m_pDataAsset = oDataAsset;
      }
    }
  }

  if (!m_pOwnerWeapon.IsValid() || !m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset))
  {
    return;
  }

  // Else create the collision sphere:
  FActorSpawnParameters params;
  params.Owner = m_pOwnerWeapon.Get();

  FVector Location = m_pOwnerWeapon->GetActorLocation();
  FRotator Rotator = m_pOwnerWeapon->GetActorRotation();

  // Spawn the collider:
  if (IsValid(m_pDirtminatorDamageColliderClass))
  {
    m_pSpecialDamageCollider = GetWorld()->SpawnActor<ADamageCollider>(m_pDirtminatorDamageColliderClass, Location, Rotator, params);
  }

  // If there is no collider, don't do a thing, just return.
  if (!IsValid(m_pSpecialDamageCollider))
  {
    return;
  }

  // Check if the player has a valid Muzzle component
  USceneComponent* Muzzle = m_pOwnerPlayer->GetMuzzle();
  if (!IsValid(Muzzle))
  {
    UE_LOG(LogTemp, Warning, TEXT("Player has no valid Muzzle component"));
    return;
  }

  // Now it's safe to attach and initialize the collider
  m_pSpecialDamageCollider->AttachToActor(m_pOwnerWeapon.Get(), FAttachmentTransformRules::KeepWorldTransform);
  m_pSpecialDamageCollider->Init(Muzzle);

  if (IsValid(m_pSpecialDamageCollider) && !m_pSpecialDamageCollider->m_lColliders.IsEmpty())
  {
    UShapeComponent* Collider = m_pSpecialDamageCollider->m_lColliders[0].m_pCollider;
    if (USphereComponent* Sphere = Cast<USphereComponent>(Collider))
    {
      // Change the collider collision so it also takes the dust actor. 
      Sphere->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
      Sphere->SetSphereRadius(m_pDataAsset->m_fSpecialAttackDistance);
    }
    m_pSpecialDamageCollider->OnActive(false);
  }
}

void UDirtminatorSAComponent::SpecialAttackAction()
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset))
  {
    UE_LOG(LogTemp, Warning, TEXT("[Dirtminator Special Attack Component] Player or Data Asset not found"));
    return;
  }

  // See if the muzzle is correct
  USceneComponent* Muzzle = m_pOwnerPlayer->GetMuzzle();
  if (!IsValid(Muzzle))
  {
    UE_LOG(LogTemp, Warning, TEXT("[Dirtminator Special Attack Component] Player does not have muzzle"));
    return;
  }

  // Check player SkeletalMesh
  USkeletalMeshComponent* Mesh = m_pOwnerPlayer->GetSkeletalMesh();
  if (!IsValid(Mesh))
  {
    UE_LOG(LogTemp, Warning, TEXT("[Dirtminator Special Attack Component] Player doesn't have SkeletalMesh"));
    return;
  }

  // Now chake the cone start and forward direction
  FVector Start = Muzzle->GetComponentLocation();
  FVector Forward = Mesh->GetForwardVector();

  TArray<AActor*> OverlappedActors;
  m_pSpecialDamageCollider->OnOverlap(OverlappedActors);

  float fStartWidth = m_pDataAsset->m_fSpecialAttackStartWidth;
  float fStartRadius = fStartWidth / 2.0f;
  float fConeAngle = m_pDataAsset->m_fSpecialAttackAngle;
  float fConeDistance = m_pDataAsset->m_fSpecialAttackDistance;

  if (fConeDistance == 0.0f)
  {
    return;
  }

  // Max radius of the cone at the top.
  float fMaxRadius = FMath::Tan(FMath::DegreesToRadians(fConeAngle / 2.0f)) * fConeDistance;

  for (AActor* pOverlappedActor : OverlappedActors)
  {
    if (!IsValid(pOverlappedActor))
    {
      continue;
    }

    FVector ActorLocation = pOverlappedActor->GetActorLocation();
    FVector ToActor = ActorLocation - Start;
    float ForwardDist = FVector::DotProduct(ToActor, Forward);

    // If it snot in the cone range get out
    if (ForwardDist < 0.0f || ForwardDist > fConeDistance)
    {
      continue;
    }

    // perpendicular distance from the cone.
    FVector ClosestPointOnAxis = Start + Forward * ForwardDist;
    float PerpendicularDist = (ActorLocation - ClosestPointOnAxis).Size();

    // Ratio permited in that position 
    float AllowedRadius = fStartRadius + ((fMaxRadius - fStartRadius) * (ForwardDist / fConeDistance));

    if (PerpendicularDist <= AllowedRadius)
    {
      ApplyDamageToActor(pOverlappedActor);
    }
  }
}

void UDirtminatorSAComponent::EndSpecialAttack()
{
  if (!m_pOwnerWeapon.IsValid())
  {
    return;
  }

  m_pOwnerWeapon->OnSpecialActive.Broadcast(false);
  m_pOwnerWeapon->OnSpecialEnd.Broadcast();

  if (IsValid(m_pSpecialAttackVFX))
  {
    m_pSpecialAttackVFX->DeactivateImmediate();
    m_pSpecialAttackVFX = nullptr;
  }

  SetPlayerVelocity(false);

  //Stop the camera shake and the controller vibration:
  m_pOwnerWeapon->StopCameraShake();
  m_pOwnerWeapon->StopControllerRumble();

  //Clear the timers:
  if (UWorld* CachedWorld = GetWorld())
  {
    CachedWorld->GetTimerManager().ClearTimer(SpecialAttackTimerHandle);
    CachedWorld->GetTimerManager().ClearTimer(SpecialAttackEndTimerHandle);
  }

  //Set the special attack inactive:
  m_fIsSpecialAttackActive = false;
}

void UDirtminatorSAComponent::SetPlayerVelocity(bool _bActivate)
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset))
  {
    return;
  }

  float MovementMultiplier = m_pDataAsset->m_fSpecialMovementMultiplicator;
  float RotationMultiplier = m_pDataAsset->m_fSpecialRotatorMultiplicator;

  if (MovementMultiplier == 0.0f || RotationMultiplier == 0.0f)
  {
    return;
  }

  MovementMultiplier = _bActivate ? MovementMultiplier : 1.0f / MovementMultiplier;
  RotationMultiplier = _bActivate ? RotationMultiplier : 1.0f / RotationMultiplier;

  m_pOwnerPlayer->ChangeMovementVelocity(MovementMultiplier, RotationMultiplier);
}

void UDirtminatorSAComponent::ApplyDamageToActor(AActor* _pOverlappedActor)
{
  if (!IsValid(_pOverlappedActor) && !IsValid(m_pDataAsset))
  {
    return;
  }

  // Check if the actor it's an enemy or a dust poop and take de correct damage to apply:
  ADustPoop* pDustPoop = Cast<ADustPoop>(_pOverlappedActor);
  IDamageable* pDamageable = Cast<IDamageable>(_pOverlappedActor);
  int32 iDamage = IsValid(pDustPoop) ? m_pDataAsset->m_fSpecialAttackDustDamage : m_pDataAsset->m_fSpecialAttackEnemyDamage;
  m_pOwnerWeapon->ApplyDamage(pDamageable, iDamage, m_pDataAsset->m_eWeaponDirtType, false, _pOverlappedActor->GetActorLocation());

  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/Characters/Enemies/DustBunnies/SFX_ENE_dustBunny_dustSpeckDamaged", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }
}

bool UDirtminatorSAComponent::GetSpeciaAttackIsActive()
{
  return m_fIsSpecialAttackActive;
}

