// Fill out your copyright notice in the Description page of Project Settings.


#include "SporeBulbsGimmick.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Interface/Damageable.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"

ASporeBulbsGimmick::ASporeBulbsGimmick()
{

  // Crear el componente de colisión
  m_pChargeAnimation = nullptr;
  m_pExplodeAnimation = nullptr;
  m_pRestoreAnimation = nullptr;
  m_pParticles = nullptr;

  m_fChargeTime = 4.f;
  m_fRestoreTime = 4.f;

  m_fSporeDuration = 2.0f;
  m_pSporeRadius = 300.f;
  m_eSporeEffect = ESporeEffectType::StinkEffect;

  m_pDetectionRadius = 300.f;
  m_bCanBeTriggered = true;

  m_pDetectionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectionSphere"));
  m_pDetectionSphere->SetupAttachment(RootComponent);
  m_pDetectionSphere->SetSphereRadius(m_pDetectionRadius);
  m_pDetectionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
  m_pDetectionSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  m_pDetectionSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap); // Solo jugadores, cambiar a canal específico.
  m_pDetectionSphere->SetGenerateOverlapEvents(true);

  m_pSporeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
  m_pSporeSphere->SetupAttachment(RootComponent);
  m_pSporeSphere->SetSphereRadius(m_pSporeRadius);
  m_pSporeSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  m_pSporeSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
  m_pSporeSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void ASporeBulbsGimmick::BeginPlay()
{
  Super::BeginPlay();

  m_pDetectionSphere->SetSphereRadius(m_pDetectionRadius);
  m_pSporeSphere->SetSphereRadius(m_pSporeRadius);
}

void ASporeBulbsGimmick::GimmickActivation()
{
  m_pDetectionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASporeBulbsGimmick::OnDetectionOverlapBegin);
  m_pSporeSphere->OnComponentBeginOverlap.AddDynamic(this, &ASporeBulbsGimmick::OnSporeOverlapBegin);
}

void ASporeBulbsGimmick::ActivateSporeSphere()
{
  // Play Animation:
  if (m_pExplodeAnimation)
  {
    AActor* SelfAsActor = this;
    if (USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>())
    {
      if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
      {
        AnimInstance->Montage_Play(m_pExplodeAnimation);
      }
    }
  }

  // Play Particles
  if (m_pParticles)
  {
    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_pParticles, GetActorLocation());
  }

  m_pSporeSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

  // Desactivar después del tiempo de vida
  FTimerHandle DeactivationTimer;
  GetWorldTimerManager().SetTimer(DeactivationTimer, this, &ASporeBulbsGimmick::DeactivateSporeSphere, m_fSporeDuration, false);

  DrawDebugSphere(
    GetWorld(),
    m_pSporeSphere->GetComponentLocation(),
    m_pSporeSphere->GetScaledSphereRadius(),
    32,                      // Segmentos (resolución)
    FColor::Green,
    false,                   // Si es persistente
    m_fSporeDuration,        // Tiempo que permanece visible
    0,                       // Profundidad
    2.0f                     // Grosor de línea
  );
}

void ASporeBulbsGimmick::DeactivateSporeSphere()
{
  m_pSporeSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);

  // Play Animation:
  if (m_pRestoreAnimation)
  {
    AActor* SelfAsActor = this;
    if (USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>())
    {
      if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
      {
        AnimInstance->Montage_Play(m_pRestoreAnimation);
      }
    }
  }

  DrawDebugSphere(
    GetWorld(),
    m_pDetectionSphere->GetComponentLocation(),
    m_pDetectionSphere->GetScaledSphereRadius(),
    32,                      // Segmentos (resolución)
    FColor::Yellow,
    false,                   // Si es persistente
    m_fRestoreTime,        // Tiempo que permanece visible
    0,                       // Profundidad
    2.0f                     // Grosor de línea
  );

  FTimerHandle ResetTimer;
  GetWorldTimerManager().SetTimer(ResetTimer, this, &ASporeBulbsGimmick::ResetBulb, m_fRestoreTime, false);
}

void ASporeBulbsGimmick::ResetBulb()
{
  m_bCanBeTriggered = true;
}

void ASporeBulbsGimmick::OnDetectionOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (!m_bCanBeTriggered || !Cast<ABasePlayer>(OtherActor)) return;

    UE_LOG(LogTemp, Warning, TEXT("Jugador Detectado: %s"), *OtherActor->GetName());

    m_pActorDetected = OtherActor;
    m_bCanBeTriggered = false;

    // Play Animation:
    if (m_pChargeAnimation)
    {
      AActor* SelfAsActor = this;
      if (USkeletalMeshComponent* Mesh = FindComponentByClass<USkeletalMeshComponent>())
      {
        if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
        {
          AnimInstance->Montage_Play(m_pChargeAnimation);
        }
      }
    }

    DrawDebugSphere(
      GetWorld(),
      m_pDetectionSphere->GetComponentLocation(),
      m_pDetectionSphere->GetScaledSphereRadius(),
      32,                      // Segmentos (resolución)
      FColor::Red,
      false,                   // Si es persistente
      m_fChargeTime,        // Tiempo que permanece visible
      0,                       // Profundidad
      2.0f                     // Grosor de línea
    );

    // Desactivar después del tiempo de vida
    GetWorldTimerManager().SetTimer(m_tSporeActivationTimerHandle, this, &ASporeBulbsGimmick::ActivateSporeSphere, m_fChargeTime, false);
}

void ASporeBulbsGimmick::OnSporeOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

  if (!OtherActor || OtherActor == this || !Cast<ABasePlayer>(OtherActor)) return;

  switch (m_eSporeEffect)
  {
  case ESporeEffectType::StinkEffect:
    if (OtherActor->Implements<UDamageable>())
    {
      IDamageable* DamageableActor = Cast<IDamageable>(OtherActor);
      if (DamageableActor)
      {
        int iMaxHealth = DamageableActor->GetMaxHealth();
        UE_LOG(LogTemp, Warning, TEXT("VIDA: %i"), iMaxHealth);
        DamageableActor->Damage(iMaxHealth, EDirtType::Neutral);
      }
    }
    break;
  default:
    break;
  }
}
