#include "MrsCleanActor.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NavigationSystem.h"
#include "FMODAudioComponent.h"
#include "FMODEvent.h"
#include "DorothysJob/Animations/DecoyAnimInstance.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

AMrsCleanActor::AMrsCleanActor() :
  ABaseConsumableActor()
{
  m_pVFXEffectsComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Effects"));
  if (IsValid(m_pVFXEffectsComponent) && IsValid(m_pMeshComponent))
  {
    m_pVFXEffectsComponent->SetupAttachment(RootComponent);
    m_pVFXEffectsComponent->SetAutoActivate(true);
  }
  m_bSkellyModeEnabled = false;
}

void AMrsCleanActor::BeginPlay()
{
  Super::BeginPlay();

  m_iCurrentHealth = m_iMaxHealth;
  GetWorldTimerManager().SetTimer(m_tTimerHandler, this, &AMrsCleanActor::Death, m_fLifetime);

  if (IsValid(m_pMeshComponent))
  {
    UDecoyAnimInstance* pAnimInstance = Cast<UDecoyAnimInstance>(m_pMeshComponent->GetAnimInstance());
    if (IsValid(pAnimInstance))
    {
      pAnimInstance->SetActionMontage(m_pActionMontage);
      pAnimInstance->PlayInflateAnimation();
    }
  }

  if ((!m_bSkellyModeEnabled) && IsValid(m_pSpawnEvent))
  {
    PlayFMODEvent(m_pSpawnEvent);
  }

  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (IsValid(pLevelManager) && IsValid(pLevelManager->GetCurrentRoom()))
  {
    pLevelManager->GetCurrentRoom()->RegisterDecoy(this);
  }
}

#pragma region | Stats
int32 AMrsCleanActor::GetCurrentHealth() const
{
  return m_iCurrentHealth;
}

void AMrsCleanActor::SetCurrentHealth(int32 _iDamage)
{
  m_iCurrentHealth = _iDamage;
}

int32 AMrsCleanActor::GetMaxHealth() const
{
  return m_iMaxHealth;
}

int32 AMrsCleanActor::Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _bIsCritical)
{
  bool bShowFeedback = false;
  if ((m_iCurrentHealth > 0) && (_iModifyHealth > 0))
  {
    m_iCurrentHealth = FMath::Clamp(m_iCurrentHealth - _iModifyHealth, 0, m_iMaxHealth);
    bShowFeedback = (m_iCurrentHealth > 0);

    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        pAchSubsystem->AddProgress("StandingHereIRealize", 1);
      }
    }

    if (m_iCurrentHealth <= 0)
    {
      Death();
    }
  }

  if (bShowFeedback || m_bSkellyModeEnabled)
  {
    if (IsValid(m_pMeshComponent))
    {
      UDecoyAnimInstance* pAnimInstance = Cast<UDecoyAnimInstance>(m_pMeshComponent->GetAnimInstance());
      if (IsValid(pAnimInstance))
      {
        pAnimInstance->PlayHitAnimation();
      }
    }

    if (IsValid(m_pDamageEvent))
    {
      PlayFMODEvent(m_pDamageEvent);
    }
  }

  return _iModifyHealth;
}

void AMrsCleanActor::Heal(int32 _iModifyHealth)
{
  if (_iModifyHealth <= 0)
  {
    return;
  }

  m_iCurrentHealth = FMath::Clamp(m_iCurrentHealth + _iModifyHealth, 0, m_iMaxHealth);
}
#pragma endregion

#pragma region | Private Functions
void AMrsCleanActor::PlayFMODEvent(TObjectPtr<UFMODEvent> _pFMODEvent)
{
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
      pAudioManager->PlayEventOnComponent(m_pAudioComponent, _pFMODEvent);
    }
  }
}

void AMrsCleanActor::Death()
{
  if (m_iCurrentHealth>0)
  {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        pAchSubsystem->UnlockAchievement("ANewSwitcheroo");
      }
    }
  }

  m_iCurrentHealth = 0;
  if (GetWorldTimerManager().IsTimerActive(m_tTimerHandler))
  {
    GetWorldTimerManager().ClearTimer(m_tTimerHandler);
  }

  if (IsValid(m_pMeshComponent))
  {
    UDecoyAnimInstance* pAnimInstance = Cast<UDecoyAnimInstance>(m_pMeshComponent->GetAnimInstance());
    if (IsValid(pAnimInstance))
    {
      pAnimInstance->OnActionMontageEnds.BindUObject(this, &AMrsCleanActor::DestroyActor);
      pAnimInstance->PlayDeathAnimation();
    }
  }

  if (IsValid(m_pDeathEvent))
  {
    PlayFMODEvent(m_pDeathEvent);
  }

  OnDecoyDestroyed.Broadcast(this);
}

void AMrsCleanActor::DestroyActor()
{
  if (IsValid(m_pDeadEffect))
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pDeadEffect, GetActorLocation());
  }

  Destroy();
}
#pragma endregion