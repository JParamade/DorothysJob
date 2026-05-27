#include "BaseConsumableActor.h"
#include "Components/CapsuleComponent.h"
#include "FMODAudioComponent.h"

ABaseConsumableActor::ABaseConsumableActor()
{
  // Collision Component
  m_pCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
  if (IsValid(m_pCollisionComponent))
  {
    RootComponent = m_pCollisionComponent;
    m_pCollisionComponent->InitCapsuleSize(70.f, 105.f);
    m_pCollisionComponent->SetGenerateOverlapEvents(true);
    m_pCollisionComponent->SetNotifyRigidBodyCollision(true);
    m_pCollisionComponent->CanCharacterStepUpOn = ECB_No;
    m_pCollisionComponent->SetCollisionProfileName(TEXT("Consumable"));
  }

  // Mesh Component
  m_pMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetupAttachment(m_pCollisionComponent);
    m_pMeshComponent->SetCollisionProfileName(TEXT("CharacterMesh"));
    m_pMeshComponent->SetGenerateOverlapEvents(false);
    m_pMeshComponent->SetCanEverAffectNavigation(false);
    m_pMeshComponent->SetRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(0, 0, -105.f)));
  }

  // Audio Component
  m_pAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("AudioComponent"));
  if (IsValid(m_pAudioComponent))
  {
    m_pAudioComponent->SetupAttachment(m_pCollisionComponent);
  }
}