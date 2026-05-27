/************************************************************************
<<<<<<<< HEAD:Source/DorothysJob/Actor/Consumables/Clean4Actor.cpp
 * @description: Represents the Clean 4 consumable actor.
 * @author: Jaime Paramo
 * @date: 01/07/2025
 * @edited_by: Josephine - 21/08/2025
 ************************************************************************/
#include "Clean4Actor.h"
// Audio
#include "FMODBlueprintStatics.h"
#include "FMODEvent.h"
// VFX
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
// Utils
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"									    // for the cleaning
#include "Kismet/KismetSystemLibrary.h"
// Damage
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DrawDebugHelpers.h"                                           // for the draw debugs
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
// Manager
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"
#include <Kismet/GameplayStatics.h>

AClean4Actor::AClean4Actor() :
  m_vCleanSize(FVector2D::One()),
  m_pCleanTexture(nullptr),
  m_fExplosionDamage(1.0f),
  m_fExplosionRadius(1.0f),
  m_fActivationTime(1.0f)
{
  // drawing component setup
  m_pDrawingComponent = CreateDefaultSubobject<UDrawing>(TEXT("Drawing component"));

  // collider creation
  m_pDetectionCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Damage collider"));
  if (m_pDetectionCollider)
  {
     m_pDetectionCollider->SetupAttachment(m_pCollisionComponent);
  }
}

void AClean4Actor::BeginPlay()
{
  Super::BeginPlay();
  
  // timer setup
  FTimerDelegate oTimerDelegate;
  FTimerHandle oTimerHandle;
  oTimerDelegate.BindUObject(this, &AClean4Actor::OnActivate);
  GetWorld()->GetTimerManager().SetTimer(oTimerHandle, oTimerDelegate, m_fActivationTime, false);

  // Play audio through FMOD System.
  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/CombatTools/Consumables/Clean4/SFX_CONS_C4_deploy", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }
}

void AClean4Actor::DetectNearbyEnemies(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  if (Cast<IDamageable>(OtherActor))
  {
    Explode();
  }
}

void AClean4Actor::ActivateOtherBombs()
{
  TArray<AActor*> aActorsToIgnore;
  TArray<AActor*> aOutActors;

  aActorsToIgnore.Add(this);

  UKismetSystemLibrary::SphereOverlapActors(
    GetWorld(),
    GetActorLocation(),
    m_fExplosionRadius,
    m_oBombsCollisionSets,
    nullptr,
    aActorsToIgnore,
    aOutActors
  );

  for (auto it = aOutActors.begin(); it != aOutActors.end(); ++it)
  {
    if(AClean4Actor* pBomb = Cast<AClean4Actor>(*it))
    {
      if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
        if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
          pAchSubsystem->SetClean4ChainAchievement();
        }
      }

      pBomb->TimingExplosion();
    }
  }
}

void AClean4Actor::DamageInArea(TArray<TEnumAsByte<EObjectTypeQuery>> _oChannel, float _fDamage)
{
  TArray<AActor*> aActorsToIgnore;
  TArray<AActor*> aOutActors;

  if (m_bDebugMode)
  {
    DrawDebugSphere(GetWorld(), GetActorLocation(), m_fExplosionRadius, 32, FColor::Red, false, 1.f);
  }

  UKismetSystemLibrary::SphereOverlapActors(
    GetWorld(),
    GetActorLocation(),
    m_fExplosionRadius,
    _oChannel,
    nullptr,
    aActorsToIgnore,
    aOutActors
  );


  for (auto it = aOutActors.begin(); it != aOutActors.end(); ++it)
  {
    if (IDamageable* pEnemy = Cast<IDamageable>(*it))
    {
      pEnemy->Damage(_fDamage, EDirtType::Neutral);
    }

  }
}

void AClean4Actor::CheckNearbyEnemies()
{
  if (!IsValid(m_pDetectionCollider))
  {
    return;
  }

  if (m_bDebugMode)
  {
    DrawDebugSphere(GetWorld(), m_pDetectionCollider->GetComponentLocation(), m_pDetectionCollider->GetUnscaledSphereRadius(), 32, FColor::Blue, false, 1.f);
  }

  TArray<AActor*> outActors;
  TArray<AActor*> aActorsToIgnore;

  m_pDetectionCollider->GetOverlappingActors(outActors);
  
  for (auto it = outActors.begin(); it != outActors.end(); ++it)
  {
    if (Cast<IDamageable>(*it))
    {
      Explode();
      return;
    }
  }
}

void AClean4Actor::TimingExplosion()
{
  GetWorld()->GetTimerManager().SetTimer(m_oTimeHandle, this, &AClean4Actor::Explode, m_fOtherBombActivation);
}

void AClean4Actor::Explode()
{
  if (m_bHasDetone)
  {
    return;
  }
  if (IsValid(m_pDrawingComponent))
  {
    m_pDrawingComponent->Clean(m_pCleanTexture, m_vCleanSize, 0, FVector2D(0.5, 0.5));
  }

  if (m_oTimeHandle.IsValid())
  {
    GetWorld()->GetTimerManager().ClearTimer(m_oTimeHandle);
  }
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance());
  UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>();
  if (pGameInstance) {
    if (pAchSubsystem) {
      pAchSubsystem->m_bBombActive = true;
      pAchSubsystem->m_bClean4Active = true;
    }
  }


  m_bHasDetone = true;

  SpawnVFX(m_pExplosionVFXSystem, m_pExplosionVFXComponent);

  DamageInArea(m_oDamageCollisionSets, m_fExplosionDamage);
  DamageInArea(m_oDustCollisionSets, m_fCleanDamage);
  ActivateOtherBombs();

  // Play audio through FMOD System.
  if (pGameInstance) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlaySoundAtLocation(this, m_pExplosionEvent, GetActorLocation());
    }
  }

  if (pGameInstance) {
    if (pAchSubsystem) {
      pAchSubsystem->m_bClean4Active = false;
      pAchSubsystem->m_iClean4Deaths = 0;
    }
  }

  if (pGameInstance) {
    if (pAchSubsystem) {
      pAchSubsystem->m_bBombActive = false;
    }
  }

  Destroy();
}

void AClean4Actor::OnActivate()
{
  
  if (IsValid(m_pDetectionCollider))
  {
    m_pDetectionCollider->SetGenerateOverlapEvents(true);
    m_pDetectionCollider->OnComponentBeginOverlap.AddDynamic(this, &AClean4Actor::DetectNearbyEnemies);
  }

  CheckNearbyEnemies();

  // Play audio through FMOD System.
  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem) {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/CombatTools/Consumables/Clean4/SFX_CONS_C4_bombSetup", &pEventDesc);

    if (pEventDesc) pEventDesc->createInstance(&pEvent);
    if (pEvent) pEvent->start();
  }

  SpawnVFX(m_pAreaVFXSystem, m_pAreaVFXComponent);
}

void AClean4Actor::SpawnVFX(TObjectPtr<UNiagaraSystem> _pVFXSystem, TObjectPtr<UNiagaraComponent> _pVFXComponent)
{
  if (!IsValid(_pVFXSystem))
  { //@review UE_LOG
    return;
  }
  _pVFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), _pVFXSystem, GetActorLocation());
}
