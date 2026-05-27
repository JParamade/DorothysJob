#include "BaseEnemy.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "DorothysJob/Controller/BaseAIController.h"
#include "DorothysJob/Animations/Enemy/EnemyAnimInstance.h"
#include "DorothysJob/Data/DataAssets/Enemy/EnemyPropertiesDataAsset.h"
#include "DorothysJob/Status/Stun/StunEffect.h"
#include "DorothysJob/Status/Modify Stats/ModifyStatsEffect.h"
#include "DorothysJob/Utils/Random/RandomNumberGenerator.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Utils/GeneralUtils.h"
#include <DorothysJob/UI/World/WorldStatus.h>
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

ABaseEnemy::ABaseEnemy() :
  ABaseCharacter()
{
  // AI Controller
  AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
  AIControllerClass = ABaseAIController::StaticClass();
  // Enemy Parameters
  m_fDetectRadius = 0;
  m_fDangerRadius = 0;
  m_bIsHidden = false;
  m_bHasBeenHiddenAnyTime = false;
  // Collision
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->SetCollisionObjectType(ECC_GameTraceChannel1);
    UpdateCapsule(100.f);
  }
  // Movement
  if (IsValid(m_pMovementComponent))
  {
    m_pMovementComponent->bConstrainToPlane = true;
    m_pMovementComponent->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::Z);
  }
  // Status
  m_pVFXStatusComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraStatus"));
  if (IsValid(m_pVFXStatusComponent))
  {
    m_pVFXStatusComponent->SetupAttachment(RootComponent);
    m_pVFXStatusComponent->SetAutoActivate(false);
    m_pVFXStatusComponent->SetRelativeLocation(FVector(0., 0., -m_fCapsuleHalfHeigth));
  }
  m_pVFXPollutedComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraPolluted"));
  if (IsValid(m_pVFXPollutedComponent))
  {
    m_pVFXPollutedComponent->SetupAttachment(RootComponent);
    m_pVFXPollutedComponent->SetAutoActivate(false);
    m_pVFXPollutedComponent->SetRelativeLocation(FVector(0., 0., -m_fCapsuleHalfHeigth));
  }
  m_pVFXStunComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraStun"));
  if (IsValid(m_pVFXStunComponent))
  {
    m_pVFXStunComponent->SetupAttachment(RootComponent);
    m_pVFXStunComponent->SetAutoActivate(false);
  }
  m_pStatusWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("StatusWidgetComponent"));
  if (IsValid(m_pStatusWidgetComponent))
  {
    m_pStatusWidgetComponent->SetupAttachment(m_pCollisionComponent);
    m_pStatusWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    m_pStatusWidgetComponent->SetDrawSize(FVector2D(150.f, 50.f));
    m_pStatusWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 100.f));
  }
  // Debug properties
  m_bWanderDebug = false;
  m_bAttackDebug = false;
}

void ABaseEnemy::BeginPlay()
{
  Super::BeginPlay();

  // AI
  AAIController* pAIController = Cast<AAIController>(GetController());
  m_pEnemyPropertiesDA = Cast<UEnemyPropertiesDataAsset>(m_pCharacterDataAsset);
  if (IsValid(pAIController) && IsValid(m_pEnemyPropertiesDA))
  {
    if (UBlackboardComponent* pBlackboardComp = pAIController->GetBlackboardComponent())
    {
      // Movement properties
      pBlackboardComp->SetValueAsFloat("AcceptableRadius", m_pEnemyPropertiesDA->m_fAcceptableRadius);
      pBlackboardComp->SetValueAsFloat("ArrivalRadius", m_pEnemyPropertiesDA->m_fArrivalRadius);
      pBlackboardComp->SetValueAsFloat("MoveInterpSpeed", m_pEnemyPropertiesDA->m_fMoveInterpSpeed);
      pBlackboardComp->SetValueAsFloat("RotationInterpSpeed", m_pEnemyPropertiesDA->m_fRotationInterpSpeed);
      // Wander properties
      pBlackboardComp->SetValueAsFloat("WanderMinRadius", m_pEnemyPropertiesDA->m_fWanderMinRadius);
      pBlackboardComp->SetValueAsFloat("WanderMaxRadius", m_pEnemyPropertiesDA->m_fWanderMaxRadius);
      pBlackboardComp->SetValueAsFloat("WanderAngle", m_pEnemyPropertiesDA->m_fWanderAngle);
      pBlackboardComp->SetValueAsFloat("WanderBounceAngle", m_pEnemyPropertiesDA->m_fWanderBounceAngle);
      pBlackboardComp->SetValueAsBool("WanderDebug", m_bWanderDebug);
      // Brave properties
      pBlackboardComp->SetValueAsFloat("BraveRotInterpSpeed", m_pEnemyPropertiesDA->m_fBraveRotInterpSpeed);
      pBlackboardComp->SetValueAsFloat("BraveDurationTime", m_pEnemyPropertiesDA->m_fBraveDuration);
      pBlackboardComp->SetValueAsBool("BraveMode", false);
      // Attack properties
      pBlackboardComp->SetValueAsFloat("DetectRadius", m_pEnemyPropertiesDA->m_fDetectRadius);
      pBlackboardComp->SetValueAsFloat("DangerRadius", m_pEnemyPropertiesDA->m_fDangerRadius);
      pBlackboardComp->SetValueAsFloat("AttackCooldown", m_pEnemyPropertiesDA->m_fAttackCooldown);
      pBlackboardComp->SetValueAsBool("AttackDebug", m_bAttackDebug);
      // Polluted
      pBlackboardComp->SetValueAsBool("IsPolluted", false);
    }

    m_fDetectRadius = m_pEnemyPropertiesDA->m_fDetectRadius;
    m_fDangerRadius = m_pEnemyPropertiesDA->m_fDangerRadius;
    m_bIsHidden = false;
    m_bHasBeenHiddenAnyTime = false;

    if (IsValid(m_pMovementComponent))
    {
      m_pMovementComponent->MaxSpeed = m_pEnemyPropertiesDA->m_fMaxWanderSpeed;
    }
  }
  // Animations
  if (IsValid(m_pMeshComponent))
  {
    UEnemyAnimInstance* pAnimInstance = Cast<UEnemyAnimInstance>(m_pMeshComponent->GetAnimInstance());
    if (IsValid(pAnimInstance))
    {
      pAnimInstance->OnActionStarts.BindUObject(this, &ABaseEnemy::PerformAttack);
      pAnimInstance->OnToDirtAction.BindUObject(this, &ABaseEnemy::PerformToDirt);
    }
  }
  // Random number generator
  m_pRandom = MakeShared<RandomNumberGenerator>();
  // Status
  if (IsValid(m_pVFXStatusComponent) && IsValid(m_pEnemyPropertiesDA) && IsValid(m_pEnemyPropertiesDA->m_pBuffNiagara))
  {
    m_pVFXStatusComponent->SetAsset(m_pEnemyPropertiesDA->m_pBuffNiagara);
  }
  if (IsValid(m_pVFXPollutedComponent) && IsValid(m_pEnemyPropertiesDA) && IsValid(m_pEnemyPropertiesDA->m_pPollutedNiagara))
  {
    m_pVFXPollutedComponent->SetAsset(m_pEnemyPropertiesDA->m_pPollutedNiagara);
  }
  if (IsValid(m_pVFXStunComponent) && IsValid(m_pEnemyPropertiesDA) && IsValid(m_pEnemyPropertiesDA->m_pStunEffect))
  {
    m_pVFXStunComponent->SetAsset(m_pEnemyPropertiesDA->m_pStunEffect);
  }
  // Audio
  PlayIdleSound();
  // Debug
  if (m_bDamageDebug)
  {
    StopBehaviorTree();
  }
  if (IsValid(m_pStatusWidgetComponent))
  {
    if (UUserWidget* Widget = m_pStatusWidgetComponent->GetUserWidgetObject())
    {
      if (UWorldStatus* StatusWidget = Cast<UWorldStatus>(Widget))
      {
        StatusWidget->InitWidget(this);
        StatusWidget->Show();
      }
    }
  }
}

int32 ABaseEnemy::Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _bIsCritical)
{
  // Set last weapon attack receive by what weapon.
  ABasePlayer* Player = nullptr;
  if (APlayerController* PC = GetWorld() ? GetWorld()->GetFirstPlayerController() : nullptr)
  {
    Player = Cast<ABasePlayer>(PC->GetPawn());
  }

  UBaseWeaponStatsDataAsset* LiquidWeaponData = nullptr;
  UBaseWeaponStatsDataAsset* DustWeaponData = nullptr;

  if (Player)
  {
    const TArray<TObjectPtr<ABaseWeapon>>& CurrentWeapons = Player->GetCurrentWeapons();
    if (CurrentWeapons.IsValidIndex(0) && CurrentWeapons[0])
    {
      LiquidWeaponData = CurrentWeapons[0]->GetDataAsset().Get();
    }
    if (CurrentWeapons.IsValidIndex(1) && CurrentWeapons[1])
    {
      DustWeaponData = CurrentWeapons[1]->GetDataAsset().Get();
    }
  }

  const UBaseWeaponStatsDataAsset* WeaponData = nullptr;

  switch (_eAttackType)
  {
  case EDirtType::Liquid: WeaponData = LiquidWeaponData; break;
  case EDirtType::Dust:   WeaponData = DustWeaponData;   break;
  default: break;
  }
  if (UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    if (UAchievementSubsystem* AchSubsystem = GameInstance->GetSubsystem<UAchievementSubsystem>())
    {
      if (AchSubsystem->m_bBombActive)
      {
        m_eWeaponLastAttackReceive = EWeaponsTypes::BOMB;
      }
    }
  }
  if (WeaponData)
  {
    if (_eAttackType == EDirtType::Liquid)
    {
      m_eWeaponLastAttackReceive = (WeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance)
        ? EWeaponsTypes::WF
        : EWeaponsTypes::MOPPRESSOR;
    }
    else if (_eAttackType == EDirtType::Dust)
    {
      m_eWeaponLastAttackReceive = (WeaponData->m_eWeaponAttackType == EWeaponAttackType::Distance)
        ? EWeaponsTypes::DIRTMINATOR
        : EWeaponsTypes::DUSTY;
    }
  }
  else
  {
    m_eWeaponLastAttackReceive = EWeaponsTypes::NONE;
  }

  // Now continue with the damage.
  return Super::Damage(_iModifyHealth, _eAttackType, _vHitLocation, _vHitRotation, _bIsCritical);
}

void ABaseEnemy::EndPlay(const EEndPlayReason::Type _eEndPlayReason)
{
  GetWorldTimerManager().ClearAllTimersForObject(this);

  Super::EndPlay(_eEndPlayReason);

  if (IsValid(m_pStatusWidgetComponent))
  {
    if (UUserWidget* Widget = m_pStatusWidgetComponent->GetUserWidgetObject())
    {
      if (UWorldStatus* StatusWidget = Cast<UWorldStatus>(Widget))
      {
        StatusWidget->Hide();
      }
    }
  }
}

void ABaseEnemy::Tick(float _fDeltaTime)
{
  Super::Tick(_fDeltaTime);

  // Debug -> It draws attack and flee radius
  if (m_bAttackDebug)
  {
    GeneralUtils::DrawDebugDoubleCircle(this, m_fDangerRadius, m_fDetectRadius, FColor::Red, FColor::Blue);
  }
}

#pragma region | Stats
void ABaseEnemy::SetMitigation(float _fNewMitigation)
{
  Super::SetMitigation(_fNewMitigation);

  if (!IsValid(m_pVFXStatusComponent))
  {
    return;
  }

  if (_fNewMitigation > 0.f)
  {
    m_pVFXStatusComponent->ReinitializeSystem();
  }
  else
  {
    m_pVFXStatusComponent->DeactivateImmediate();
  }
}

void ABaseEnemy::SetMitigationVFXVisibility(bool _bVisible)
{
  if (IsValid(m_pVFXStatusComponent))
  {
    m_pVFXStatusComponent->SetVisibility(_bVisible, true);
  }
}

void ABaseEnemy::ModifyStats(const TMap<EModifiableStats, float> _mStats)
{
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (!IsValid(pAIController))
  {
    return;
  }

  if (UBlackboardComponent* pBlackboardComp = pAIController->GetBlackboardComponent())
  {
    pBlackboardComp->SetValueAsBool("IsPolluted", true);
  }
}

void ABaseEnemy::RevertStats(const TMap<EModifiableStats, float> _mStats)
{
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (!IsValid(pAIController))
  {
    return;
  }

  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetOverlayMaterial(nullptr);
  }

  if (IsValid(m_pVFXPollutedComponent))
  {
    m_pVFXPollutedComponent->DeactivateImmediate();
  }

  if (UBlackboardComponent* pBlackboardComp = pAIController->GetBlackboardComponent())
  {
    pBlackboardComp->SetValueAsBool("IsPolluted", false);
  }
}
#pragma endregion

#pragma region | Status Effect
void ABaseEnemy::BePolluted()
{
  Super::BePolluted();

  if (IsValid(m_pEnemyPropertiesDA) && IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->SetOverlayMaterial(IsValid(m_pEnemyPropertiesDA->m_pPollutedEffect) ? m_pEnemyPropertiesDA->m_pPollutedEffect : nullptr);
  }

  if (IsValid(m_pVFXPollutedComponent))
  {
    m_pVFXPollutedComponent->ReinitializeSystem();
  }
}
#pragma endregion

#pragma region | AI
UBehaviorTree* ABaseEnemy::GetBehaviorTree()
{
  UBehaviorTree* pBehaviorTree = nullptr;
  UEnemyPropertiesDataAsset* pEnemyPropertiesDA = Cast<UEnemyPropertiesDataAsset>(m_pCharacterDataAsset);
  if (IsValid(pEnemyPropertiesDA))
  {
    pBehaviorTree = pEnemyPropertiesDA->m_pBehaviorTree;
  }
  return pBehaviorTree;
}

void ABaseEnemy::RestartBehaviorTree()
{
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (IsValid(pAIController) && IsValid(pAIController->GetBrainComponent()))
  {
    pAIController->GetBrainComponent()->StartLogic();
  }
}

void ABaseEnemy::StopBehaviorTree(FString _sReason)
{
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (IsValid(pAIController) && IsValid(pAIController->GetBrainComponent()))
  {
    pAIController->GetBrainComponent()->StopLogic(_sReason);
  }
}
#pragma endregion

#pragma region | Movement
bool ABaseEnemy::IsInMovement() const
{
  return true;
}
#pragma endregion

#pragma region | Attack
void ABaseEnemy::PrepareAttack()
{
  PlayAnimMontage(FName("PrepareAttack"), &OnPrepareAttackComplete);
}

void ABaseEnemy::Attack()
{
  PlayAnimMontage(FName("Attack"), &OnAttackComplete);
}

void ABaseEnemy::RecoverAttack()
{
  PlayAnimMontage(FName("RecoverAttack"), &OnRecoverAttackComplete);
}
#pragma endregion

#pragma region | Dirt
void ABaseEnemy::PrepareDirt()
{
  PlayAnimMontage(FName("PrepareDirt"), &OnPrepareDirtComplete);
}

void ABaseEnemy::ToDirt()
{
  PlayAnimMontage(FName("ToDirt"), &OnToDirtComplete);
}

void ABaseEnemy::RecoverDirt()
{
  PlayAnimMontage(FName("RecoverDirt"), &OnRecoverDirtComplete);
}
#pragma endregion

#pragma region | Hide
void ABaseEnemy::ToHide()
{
  PlayAnimMontage(FName("ToHide"), &OnToHideComplete);
}

bool ABaseEnemy::HasEnemyBeenHiddenAnyTime() const
{
  return m_bHasBeenHiddenAnyTime;
}
#pragma endregion

#pragma region | Cancel Action
void ABaseEnemy::CancelAction()
{
  StopAnimMontage();
}
#pragma endregion

#pragma region | Status Effect
void ABaseEnemy::SetCanDoActions(bool _bCanDoActions)
{
  Super::SetCanDoActions(_bCanDoActions);

  if (_bCanDoActions)
  {
    StopAnimMontage();
    if (IsValid(m_pVFXStunComponent))
    {
      m_pVFXStunComponent->DeactivateImmediate();
    }
    RestartBehaviorTree();
  }
  else
  {
    StopBehaviorTree();
    PlayAnimMontage(FName("Stun"));
    if (IsValid(m_pVFXStunComponent))
    {
      m_pVFXStunComponent->ReinitializeSystem();
    }
  }
}

void ABaseEnemy::SpeedUp()
{
  Super::SpeedUp();
  OnSpeedUpComplete.Broadcast(this);
}
#pragma endregion

#pragma region | Enemy Manager
bool ABaseEnemy::AreAvailableTokens()
{
  if (UBaseRoom* pMyRoom = GetCurrentRoom())
  {
    return pMyRoom->AreAvailableTokens();
  }
  return false;
}

bool ABaseEnemy::RequestToken()
{
  if (UBaseRoom* pMyRoom = GetCurrentRoom())
  {
    return pMyRoom->RequestToken(this);
  }
  return false;
}

bool ABaseEnemy::ReturnToken()
{
  if (UBaseRoom* pMyRoom = GetCurrentRoom())
  {
    return pMyRoom->ReturnToken(this);
  }
  return false;
}
#pragma endregion

UEnemyPropertiesDataAsset* ABaseEnemy::GetEnemyDataAsset() const
{
  return m_pEnemyPropertiesDA;
}

#pragma region | Capsule
void ABaseEnemy::UpdateCapsule(float _fCapsuleRadius)
{
  if (IsValid(m_pCollisionComponent))
  {
    m_fCapsuleRadius = _fCapsuleRadius;
    m_pCollisionComponent->SetCapsuleSize(m_fCapsuleRadius, m_fCapsuleHalfHeigth);
    if (IsValid(m_pMeshComponent))
    {
      m_pMeshComponent->SetRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(0., 0., -m_fCapsuleHalfHeigth)));
    }
    if (IsValid(m_pMovementComponent))
    {
      m_pMovementComponent->UpdateNavAgent(*m_pCollisionComponent);
    }
  }
}
#pragma endregion

#pragma region | Game Over
void ABaseEnemy::GameOver()
{
  Super::GameOver();

  // Obtener subsistema de logros una sola vez
  UAchievementSubsystem* AchSubsystem = nullptr;
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    AchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>();
    if (pGameInstance->m_iCurrentEndlessLevel >= 0)
    {
      pGameInstance->EnemiesKilledInEndless++;
    }
  }

  if (AchSubsystem)
  {
    AchSubsystem->SetKillsAchievement(m_eWeaponLastAttackReceive);
    AchSubsystem->SetKillsWithB13();
    AchSubsystem->SetClean4DeathsAchievement();
    AchSubsystem->SetKillsWith1B13Achievement();
    if (m_fMitigation > 0.0f)
    {
      AchSubsystem->SetDirtMaxChargeAchievement();
    }
  }

  GetWorldTimerManager().ClearAllTimersForObject(this);
  OnEnemyEliminate.Broadcast(this);
  ReturnToken();
  Destroy();
}
#pragma endregion

#pragma region | Animations
void ABaseEnemy::PlayAnimMontage(const FName& _sSection, const FOnEnemyActionComplete* _pDelegate)
{
  if (!IsValid(m_pMeshComponent))
  {
    return;
  }
  UEnemyAnimInstance* pAnimInstance = Cast<UEnemyAnimInstance>(m_pMeshComponent->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->OnActionMontageEnds.Unbind();
    if (_pDelegate != nullptr)
    {
      pAnimInstance->OnActionMontageEnds.BindLambda([this, _pDelegate]() { _pDelegate->Broadcast(this); });
    }
    if (!pAnimInstance->PlayActionMontage(_sSection))
    {
      pAnimInstance->StopActionMontage();
    }
  }
}

void ABaseEnemy::StopAnimMontage()
{
  if (!IsValid(m_pMeshComponent))
  {
    return;
  }
  UEnemyAnimInstance* pAnimInstance = Cast<UEnemyAnimInstance>(m_pMeshComponent->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    pAnimInstance->OnActionMontageEnds.Unbind();
    pAnimInstance->StopActionMontage();
  }
}
#pragma endregion

#pragma region | Actions
void ABaseEnemy::PerformAttack(int32 _iCurrentCombo)
{
  if (IsValid(m_pEnemyPropertiesDA))
  {
    if (IsValid(m_pEnemyPropertiesDA->m_pAttackEffect))
    {
      UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pEnemyPropertiesDA->m_pAttackEffect, GetActorLocation());
    }

    PlayFMODEvent(m_pEnemyPropertiesDA->m_pAttackEvent);
  }
}

void ABaseEnemy::PerformToDirt()
{
  PlayFMODEvent(m_pEnemyPropertiesDA->m_pToDirtEvent);
}
#pragma endregion

#pragma region | Feedback
void ABaseEnemy::ShowWalkFeedback()
{

}
#pragma endregion

#pragma region | Audio
void ABaseEnemy::PlayIdleSound()
{
  if (!IsValid(m_pEnemyPropertiesDA))
  {
    return;
  }

  PlayFMODEvent(m_pEnemyPropertiesDA->m_pIdleEvent);
  ShowWalkFeedback();

  float fTime = m_pRandom->GetRandomFloatInRange(m_pEnemyPropertiesDA->m_fMinIdleLoopTime, m_pEnemyPropertiesDA->m_fMaxIdleLoopTime);

  GetWorldTimerManager().SetTimer(m_tIdleSoundTimer, this, &ABaseEnemy::PlayIdleSound, fTime, false);
}
#pragma endregion