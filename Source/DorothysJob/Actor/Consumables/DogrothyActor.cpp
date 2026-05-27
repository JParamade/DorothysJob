/************************************************************************
 * @description: Represents the Dogrothy consumable actor.
 * @author: Jaime Paramo
 * @date: 01/07/2025
 * @edited_by: [Other Contributors] - [Edit Date]
 ************************************************************************/

#include "DogrothyActor.h"

 // Component
#include "Components/CapsuleComponent.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"

// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"

// FMOD
#include "FMODEvent.h"
#include "FMODBlueprintStatics.h"
#include "FMODAudioComponent.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "NiagaraFunctionLibrary.h"

ADogrothyActor::ADogrothyActor() {
  SetupDogrothy();
}

void ADogrothyActor::BeginPlay() {
  Super::BeginPlay();

  if (IsValid(m_pCollisionComponent)) {
    m_pCollisionComponent->OnComponentHit.AddDynamic(this, &ADogrothyActor::OnHit);
    m_pDustDeteccion->OnComponentBeginOverlap.AddDynamic(this, &ADogrothyActor::OnBeginOverlap);
  }

  // Bind Pause Events.
  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      m_oPauseHandle = pLevelManager->OnGamePaused.AddUObject(this, &ADogrothyActor::PauseFMODEvents);
      m_oUnpauseHandle = pLevelManager->OnGameUnpaused.AddUObject(this, &ADogrothyActor::UnpauseFMODEvents);
    }
  }

  float fRandomAngle = FMath::FRandRange(-75., 75.);
  FRotator oRandomRotator = GetActorRotation() + FRotator(0., fRandomAngle, 0.);
  m_vMovementDirection = oRandomRotator.Vector().GetSafeNormal();

  if (IsValid(m_pAudioComponent) && IsValid(m_pStartEvent)) {
    m_pAudioComponent->SetEvent(m_pStartEvent);
    m_pAudioComponent->Play();

    TWeakObjectPtr<ADogrothyActor> WeakThis(this);
    GetWorld()->GetTimerManager().SetTimer(
      m_oStartDelayTimer,
      [WeakThis]() {
        if (WeakThis.IsValid())
        {
          WeakThis->PlayMovementSound();
        }
      },
      m_fStartDelay,
      false
    );
  }
  GetWorld()->GetTimerManager().SetTimer(m_oDirtingTimer, this, &ADogrothyActor::ToDirt, m_fCleaningTime, true);
  GetWorld()->GetTimerManager().SetTimer(m_oLifeTimeTimer, this, &ADogrothyActor::Die, m_fLifeTime);

}

void ADogrothyActor::Tick(float _fDeltaTime) {
  Super::Tick(_fDeltaTime);

  if (!m_vMovementDirection.IsNearlyZero()) {
    FVector vNormalizedDirection = m_vMovementDirection.GetSafeNormal();
    AddActorWorldOffset(vNormalizedDirection * m_fMovementSpeed * _fDeltaTime, true);
    SetActorRotation(vNormalizedDirection.Rotation());
  }
}


void ADogrothyActor::PauseFMODEvents() {
  if (IsValid(m_pAudioComponent) && m_pAudioComponent->IsPlaying()) {
    m_pAudioComponent->SetPaused(true);
    m_bStartPaused = true;
  }

  if (IsValid(m_pAuxAudioComponent) && m_pAuxAudioComponent->IsPlaying()) {
    m_pAuxAudioComponent->SetPaused(true);
    m_bCleanPaused = true;
  }
}

void ADogrothyActor::UnpauseFMODEvents() {
  if (IsValid(m_pAudioComponent) && m_bStartPaused) {
    m_pAudioComponent->SetPaused(false);
    m_bStartPaused = false;
  }

  if (IsValid(m_pAuxAudioComponent) && m_bCleanPaused) {
    m_pAuxAudioComponent->SetPaused(false);
    m_bCleanPaused = false;
  }
}

void ADogrothyActor::ChangeDirection(FVector _vNewDirection) {
  m_vMovementDirection = _vNewDirection.GetSafeNormal();
}

void ADogrothyActor::OnHit(UPrimitiveComponent* _pHitComponent, AActor* _pOtherActor, UPrimitiveComponent* _pOtherComp, FVector _vNormalImpulse, const FHitResult& _rHit) {
  if (!GetWorld()->GetTimerManager().IsTimerActive(m_oInvulnerabilityTimer)) {
    FVector vBaseDirection = _rHit.ImpactNormal;
    vBaseDirection.Z = 0.f;

    FVector vNewDirection;

    if (vBaseDirection.IsNearlyZero()) {
      // If the hit normal is invalid or close to zero, generate a fully random 2D direction.
      FVector2D vRandomPoint = FMath::RandPointInCircle(1.f);
      vNewDirection = FVector(vRandomPoint.X, vRandomPoint.Y, 0.f);
    }
    else {
      // Apply a random deviation to the hit normal within a configurable angle range.
      float fRandomAngle = FMath::FRandRange(-m_fMaxRotationAngle, m_fMaxRotationAngle);
      vNewDirection = vBaseDirection.RotateAngleAxis(fRandomAngle, FVector::UpVector);
    }

    if (_pOtherActor)
    {
      // Comprobar si el actor es de una clase específica, por ejemplo, AMyTargetActor
      if (ABaseEnemy* TargetActor = Cast<ABaseEnemy>(_pOtherActor))
      {

        // Puedes ahora acceder a métodos o variables del actor
        ApplyDamage(m_fHitDamage);
      }
    }

    // Normalize and apply the new direction.
    ChangeDirection(vNewDirection.GetSafeNormal());

    GetWorld()->GetTimerManager().SetTimer(m_oInvulnerabilityTimer, this, &ADogrothyActor::ResetHit, m_fInvulnerabilityTime);
  }
}

void ADogrothyActor::OnBeginOverlap(UPrimitiveComponent* _pOverlappedComponent, AActor* _pOtherActor, UPrimitiveComponent* _pOtherComp, int32 _iOtherBodyIndex, bool _bFromSweep, const FHitResult& _rSweepResult)
{
  if (IDamageable* pDamageable = Cast<IDamageable>(_pOtherActor))
  {
    pDamageable->Damage(m_iDustDamage, EDirtType::Neutral);
  }
}

void ADogrothyActor::ToDirt()
{
  if (!IsValid(m_pDrawingComponent))
  {
    return;
  }

  m_pDrawingComponent->Clean(m_pCleanTexture, m_vDirtSize, 90.f, FVector2D(.5f));
}

void ADogrothyActor::ApplyDamage(float _fDamage) {
  m_fCurrentHealth -= _fDamage;

  if (m_fCurrentHealth <= 0) {
    Die();
  }
}

void ADogrothyActor::Die()
{
  if (IsValid(m_pDeadEffect))
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pDeadEffect, GetActorLocation());
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySoundAtLocation(this, m_pDestroyEvent, GetActorLocation());
    }
  }

  if (IsValid(m_pAuxAudioComponent)) m_pAuxAudioComponent->Stop();

  if (m_oDirtingTimer.IsValid())
  {
    GetWorld()->GetTimerManager().ClearTimer(m_oDirtingTimer);
  }
  if (m_oLifeTimeTimer.IsValid())
  {
    GetWorld()->GetTimerManager().ClearTimer(m_oLifeTimeTimer);
  }

  // Bind Pause Events.
  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      pLevelManager->OnGamePaused.Remove(m_oPauseHandle);
      pLevelManager->OnGameUnpaused.Remove(m_oUnpauseHandle);
    }
  }

  Destroy();
}

void ADogrothyActor::SetupDogrothy() {
  PrimaryActorTick.bCanEverTick = true;

  m_fCurrentHealth = m_fMaxHealth;
  
  // Drawing Component
  m_pDrawingComponent = CreateDefaultSubobject<UDrawing>(TEXT("DrawingComponent"));
  checkf(m_pDrawingComponent, TEXT("Dogrothy: m_pDrawingComponent not initialized"));

  m_pDustDeteccion = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Dust Collision Component"));
  m_pDustDeteccion->SetupAttachment(m_pCollisionComponent);

  m_vDirtSize = FVector2D(150.f);

  // Audio Component
  m_pAuxAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("AuxAudioComponent"));
  if (IsValid(m_pAuxAudioComponent))
  {
    m_pAuxAudioComponent->SetupAttachment(m_pCollisionComponent);
  }
}

void ADogrothyActor::ResetHit() {
  GetWorld()->GetTimerManager().ClearTimer(m_oInvulnerabilityTimer);
}

void ADogrothyActor::PlayMovementSound() {
  if (IsValid(m_pAuxAudioComponent)) {
    m_pAuxAudioComponent->SetEvent(m_pMovementEvent);
    m_pAuxAudioComponent->Play();
  }
}