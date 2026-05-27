
#include "BaseProjectile.h"
#include "Components/SphereComponent.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Component/Action/Movement/BaseProjectileMovementComponent.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Data/DataAssets/Enemy/EnemyPropertiesDataAsset.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Interface/Damageable.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "FMODAudioComponent.h"

ABaseProjectile::ABaseProjectile()
{
  // For now we don't need it to use Tick, but if in the future this class or a subclass needs it this must be TRUE
  PrimaryActorTick.bCanEverTick = false;

  // Collision Component
  m_pCollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
    m_pCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnComponentOverlap);

    RootComponent = m_pCollisionComponent;
    m_pCollisionComponent->SetCanEverAffectNavigation(false);
  }

  // Mesh Component
  m_pMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetupAttachment(RootComponent);
    m_pMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_pMeshComponent->SetCanEverAffectNavigation(false);
  }

  // VFX
  m_pProjectileVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileVFX"));
  if(IsValid(m_pProjectileVFX))
  {
    m_pProjectileVFX->SetupAttachment(RootComponent);
    m_pProjectileVFX->SetAutoActivate(true);
  }

  // Movement Component
  m_pMovementComponent = CreateDefaultSubobject<UBaseProjectileMovementComponent>(TEXT("BaseProjectileMovementComponent"));
  if(IsValid(m_pMovementComponent))
  {
    m_pMovementComponent->SetCanPush(false);
  }
  m_fSpeed = 3000.f;

  // FMOD Audio Component
  m_pAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("FMODAudioComponent"));
  if(IsValid(m_pAudioComponent))
  {
    m_pAudioComponent->SetupAttachment(RootComponent);
  }
}

void ABaseProjectile::OnComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!OtherActor || OtherActor == this || !IsValid(CachedWorld))
  {
    return;
  }

  FVector SpawnLocation;
  FRotator SpawnRotation;

  if (SweepResult.bBlockingHit && !SweepResult.bStartPenetrating)
  {
    SpawnLocation = SweepResult.ImpactPoint;
    SpawnRotation = SweepResult.ImpactNormal.Rotation();
  }
  else
  {
    SpawnLocation = GetActorLocation();
    SpawnRotation = GetActorForwardVector().Rotation();
  }

  if (m_pImpactVFX)
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(CachedWorld, m_pImpactVFX, SpawnLocation, SpawnRotation);
  }

  if (!OtherActor->Implements<UDamageable>())
  {
    OnProjectileFinished.ExecuteIfBound();
    PreDestroy(); // @TODO: add to a pool in order to now spawn them all the time
    return;
  }

  IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
  if (!DamageableActor)
  {
    OnProjectileFinished.ExecuteIfBound();
    PreDestroy(); // @TODO: add to a pool in order to now spawn them all the time
    return;
  }

  if (m_pWeaponOwner.IsValid() && m_pWeaponDataAsset.IsValid())
  {
    if (m_pWeaponDataAsset->m_oDamage.IsValidIndex(m_iDamageIndex))
    {
      int32 iDamage = m_pWeaponDataAsset->m_oDamage[m_iDamageIndex];
      m_pWeaponOwner->ApplyDamage(DamageableActor, iDamage, m_pWeaponDataAsset->m_eWeaponDirtType, true, GetActorLocation(), GetActorRotation());
      if (ABaseCharacter* pCharacter = Cast<ABaseCharacter>(DamageableActor)) PlayFMODSound(m_pImpactFMODEvent, pCharacter->GetMitigation());
    }
  }
  else if (m_pEnemyDataAsset.IsValid())
  {
    DamageableActor->Damage(m_pEnemyDataAsset->m_iDamage, EDirtType::Neutral, GetActorLocation(), GetActorRotation());

  }
  else
  {
    DamageableActor->Damage(m_iDamage, EDirtType::Neutral, GetActorLocation(), GetActorRotation());
  }

  if (m_pImpactVFX)
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(CachedWorld, m_pImpactVFX, GetActorLocation(), GetActorRotation(), FVector(1, 1, 1));
  }

  PreDestroy();
}

void ABaseProjectile::InitVelocity(FVector ShootDirection)
{
  if (IsValid(m_pMovementComponent))
  { // @TODO: maybe get here the speed from the DataAsset
    FVector NewVelocity = ShootDirection.GetSafeNormal() * m_fSpeed;
    m_pMovementComponent->SetVelocity(NewVelocity);
  }
}

void ABaseProjectile::PlayFMODSound(UFMODEvent* _pEvent, float _fMitigation)
{
  if (IsValid(m_pAudioManager) && _pEvent && IsValid(m_pAudioComponent))
  {
    m_pAudioManager->PlayEventWithParameters(
      m_pAudioComponent,
      _pEvent,
      { {"isArmored", _fMitigation > 0.f ? 1.f : 0.f} }
    );
  }
}

void ABaseProjectile::PreDestroy()
{
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  }

  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetHiddenInGame(true);
  }

  if (IsValid(m_pProjectileVFX))
  {
    m_pProjectileVFX->SetHiddenInGame(true);
  }

  FTimerHandle oHandle;
  TWeakObjectPtr<ABaseProjectile> WeakThis(this);
  CachedWorld->GetTimerManager().SetTimer(
    oHandle,
    [WeakThis]() {
      if (WeakThis.IsValid())
      {
        WeakThis->OnProjectileFinished.ExecuteIfBound();
        WeakThis->Destroy(); // @TODO: add to a pool in order to now spawn them all the time
      }
    },
    1.f,
    false
  );
}

void ABaseProjectile::BeginPlay()
{
  Super::BeginPlay();

  if (UWorld* pCachedWorld = GetWorld())
  {
    CachedWorld = pCachedWorld;
  }

  if (ABaseWeapon* Weapon = Cast<ABaseWeapon>(GetOwner()))
  {
    m_pWeaponOwner = Weapon;

    if(m_pWeaponOwner->GetDataAsset().IsValid())
    {
      if (UBaseWeaponStatsDataAsset* WeaponDataAsset = Cast<UBaseWeaponStatsDataAsset>(m_pWeaponOwner->GetDataAsset().Get()))
      {
        m_pWeaponDataAsset = WeaponDataAsset;
      }
    }
  }
  else if (ABaseEnemy* Enemy = Cast<ABaseEnemy>(GetOwner()))
  {
    if (UEnemyPropertiesDataAsset* EnemyDataAsset = Cast<UEnemyPropertiesDataAsset>(Enemy->GetDataAsset()))
    {
      m_pEnemyDataAsset = EnemyDataAsset;
    }
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>())
    {
      m_pAudioManager = pAudioManager;
    }
  }

  InitVelocity(GetActorForwardVector());
}