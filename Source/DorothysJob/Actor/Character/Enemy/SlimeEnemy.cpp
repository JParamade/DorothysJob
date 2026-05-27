#include "SlimeEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DorothysJob/Data/DataAssets/Enemy/OozerPropertiesDataAsset.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Room/Tile/BaseTile.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Utils/GeneralUtils.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Interface/Damageable.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "FMODAudioComponent.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"
#include "DorothysJob/Manager/BaseGameInstance.h"

ASlimeEnemy::ASlimeEnemy()
  : ABaseEnemy()
{
  // Oozer parameters
  m_bCanDirt = true;
  m_bIsDashing = false;
  m_fDirtRotation = 0;
  // Capsule
  UpdateCapsule(90.f);
  // Create components
  m_pDrawingComponent = CreateDefaultSubobject<UDrawing>(TEXT("DrawingComponent"));
  m_pVFXNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraAttack"));
  if (IsValid(m_pVFXNiagaraComponent))
  {
    m_pVFXNiagaraComponent->SetupAttachment(RootComponent);
    m_pVFXNiagaraComponent->SetAutoActivate(false);
    m_pVFXNiagaraComponent->SetRelativeLocation(FVector(50., 0., -m_fCapsuleHalfHeigth));
  }
}

void ASlimeEnemy::BeginPlay()
{
  Super::BeginPlay();
  // AI
  AAIController* pAIController = Cast<AAIController>(GetController());
  m_pOozerPropertiesDA = Cast<UOozerPropertiesDataAsset>(m_pCharacterDataAsset);
  // Collision
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->BodyInstance.bUseCCD = true;
    m_pCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ASlimeEnemy::OnComponentBeginOverlap);
  }
  // VFX
  if (IsValid(m_pVFXNiagaraComponent) && IsValid(m_pOozerPropertiesDA) && IsValid(m_pOozerPropertiesDA->m_pAttackEffect))
  {
    m_pVFXNiagaraComponent->SetAsset(m_pOozerPropertiesDA->m_pAttackEffect);
  }
  // Audio
  if (IsValid(m_pOozerPropertiesDA))
  {
    PlayFMODEvent(m_pOozerPropertiesDA->m_pWalkEvent);
  }
  // Bind Pause Events.
  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      m_oPauseHandle = pLevelManager->OnGamePaused.AddUObject(this, &ASlimeEnemy::PauseFMODEvents);
      m_oUnpauseHandle = pLevelManager->OnGameUnpaused.AddUObject(this, &ASlimeEnemy::UnpauseFMODEvents);
    }
  }
}

#pragma region | Stats
int32 ASlimeEnemy::Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _bIsCritical)
{
  int32 iDamage = Super::Damage(_iModifyHealth, _eAttackType, _vHitLocation, _vHitRotation, _bIsCritical);

  if (GetWorldTimerManager().IsTimerActive(m_tBraveTimerHandle))
  {
    GetWorldTimerManager().ClearTimer(m_tBraveTimerHandle);
  }
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (IsValid(pAIController) && IsValid(m_pOozerPropertiesDA))
  {
    TWeakObjectPtr<UBlackboardComponent> pBlackboardComp = pAIController->GetBlackboardComponent();
    if (pBlackboardComp.IsValid())
    {
      pBlackboardComp->SetValueAsBool("BraveMode", true);
      GetWorldTimerManager().SetTimer(m_tBraveTimerHandle,
        [pBlackboardComp]() {
          if (pBlackboardComp.IsValid())
          {
            pBlackboardComp->SetValueAsBool("BraveMode", false);
          }
        },
        m_pOozerPropertiesDA->m_fBraveDuration, false);
    }
  }
  return iDamage;
}

void ASlimeEnemy::GameOver() 
{  
  UAchievementSubsystem* AchSubsystem = nullptr;
  if (UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    AchSubsystem = GameInstance->GetSubsystem<UAchievementSubsystem>();
  }

  if (AchSubsystem)
  {
    AchSubsystem->AddProgress("SludgeSlayer", 1);
  }

  // Remove Pause Event Bindings.
  if (UWorld* pWorld = GetWorld()) {
    if (ULevelManager* pLevelManager = pWorld->GetSubsystem<ULevelManager>()) {
      pLevelManager->OnGamePaused.Remove(m_oPauseHandle);
      pLevelManager->OnGameUnpaused.Remove(m_oUnpauseHandle);
    }
  }
  
  Super::GameOver();
}
#pragma endregion

#pragma region Attack
void ASlimeEnemy::Attack()
{
  PlayAnimMontage(FName("Attack"));
  FVector vDashPoint = GetActorLocation() + GetActorForwardVector() * m_fDetectRadius;
  FVector vReachablePoint;
  GeneralUtils::IsReachableLineTrace(this, vDashPoint, &vReachablePoint);
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (IsValid(pAIController) && IsValid(m_pOozerPropertiesDA))
  {
    m_bIsDashing = true;
    if (IsValid(m_pCollisionComponent))
    {
      m_pCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    }
    if (GetWorldTimerManager().IsTimerActive(m_tDirtCooldownTimerHandle))
    {
      GetWorldTimerManager().ClearTimer(m_tDirtCooldownTimerHandle);
      ResetToDirt();
    }
    if (IsValid(m_pMovementComponent))
    {
      ClearAllStatus();
      m_pMovementComponent->MaxSpeed = m_pOozerPropertiesDA->m_fDashSpeed;
    }
    if (IsValid(m_pVFXNiagaraComponent) && (!m_pVFXNiagaraComponent->IsActive()))
    {
      m_pVFXNiagaraComponent->ReinitializeSystem();
    }

    FAIMoveRequest tMoveReq(vReachablePoint);
    tMoveReq.SetUsePathfinding(true);
    tMoveReq.SetAllowPartialPath(true);
    tMoveReq.SetProjectGoalLocation(true);
    tMoveReq.SetNavigationFilter(pAIController->GetDefaultNavigationFilterClass());
    tMoveReq.SetAcceptanceRadius(m_pOozerPropertiesDA->m_fAcceptableRadius);
    tMoveReq.SetReachTestIncludesAgentRadius(false);
    tMoveReq.SetReachTestIncludesGoalRadius(false);
    tMoveReq.SetCanStrafe(true);

    EPathFollowingRequestResult::Type MoveResult = pAIController->MoveTo(tMoveReq);
    if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
    {
      pAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &ASlimeEnemy::FinishDash);
    }
    else
    {
      StopAnimMontage();
      FinishDash(FAIRequestID(), EPathFollowingResult::Success);
    }
  }
  else
  {
    OnAttackComplete.Broadcast(this);
  }
}
#pragma endregion

#pragma region Dirt
void ASlimeEnemy::ToDirt()
{
  if (m_bCanDirt && IsValid(m_pOozerPropertiesDA))
  {
    m_pDrawingComponent->Dirt(
      m_pOozerPropertiesDA->m_pCleanTexture,
      m_pOozerPropertiesDA->m_pDirtTexture,
      m_pOozerPropertiesDA->m_vDirtSize,
      m_fDirtRotation,
      FVector2D(0.5f),
      m_pOozerPropertiesDA->m_eDrawColor,
      EDrawTier::Liquid
    );

    m_fDirtRotation += m_pOozerPropertiesDA->m_fStepRotationAngle;
    m_fDirtRotation = FMath::Fmod(m_fDirtRotation, 360.f);
    m_bCanDirt = false;
    float fDirtCooldown = m_bIsDashing ? m_pOozerPropertiesDA->m_fDashingDirtCooldown : m_pOozerPropertiesDA->m_fDirtCooldown;
    GetWorldTimerManager().SetTimer(m_tDirtCooldownTimerHandle, this, &ASlimeEnemy::ResetToDirt, fDirtCooldown, false);
  }
  OnToDirtComplete.Broadcast(this);
}
void ASlimeEnemy::ResetToDirt()
{
  m_bCanDirt = true;
}
#pragma endregion

#pragma region | Dash
void ASlimeEnemy::FinishDash(FAIRequestID _tRequestID, const FPathFollowingResult& _tResult)
{
  m_bIsDashing = false;
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
  }
  if (IsValid(m_pMovementComponent) && IsValid(m_pOozerPropertiesDA))
  {
    m_pMovementComponent->MaxSpeed = m_pOozerPropertiesDA->m_fMaxWanderSpeed;
  }
  if (IsValid(m_pVFXNiagaraComponent))
  {
    m_pVFXNiagaraComponent->DeactivateImmediate();
  }

  OnAttackComplete.Broadcast(this);
}
#pragma endregion

#pragma region | Feedback
void ASlimeEnemy::ShowWalkFeedback()
{
  Super::ShowWalkFeedback();

  if (IsValid(m_pOozerPropertiesDA) && IsValid(m_pOozerPropertiesDA->m_pWanderEffect))
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pOozerPropertiesDA->m_pWanderEffect, GetActorLocation() - FVector(0., 0., m_fCapsuleHalfHeigth));
  }
}
#pragma endregion

void ASlimeEnemy::OnComponentBeginOverlap(UPrimitiveComponent* _pComponent, AActor* _pActor, UPrimitiveComponent* _pActorComp, int32 _iActorBodyIndex, bool _bFromSweep, const FHitResult& _tSweepResult)
{
  if (m_bIsDashing && IsValid(_pActor) && (_pActor != this))
  {
    IDamageable* pDamageable = Cast<IDamageable>(_pActor);
    if (pDamageable && IsValid(m_pOozerPropertiesDA))
    {
      pDamageable->Damage(m_pOozerPropertiesDA->m_iDamage, EDirtType::Liquid);
    }
  }
}

void ASlimeEnemy::PauseFMODEvents() {
  if (IsValid(m_pAudioComponent) && m_pAudioComponent->IsPlaying()) {
    m_pAudioComponent->SetPaused(true);
    m_bSpecialPaused = true;
  }
}

void ASlimeEnemy::UnpauseFMODEvents() {
  if (IsValid(m_pAudioComponent) && m_bSpecialPaused) {
    m_pAudioComponent->SetPaused(false);
    m_bSpecialPaused = false;
  }
}