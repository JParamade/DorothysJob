#include "InteractableCleaningCupboard.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
// VFX
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
// FMOD
#include "FMODBlueprintStatics.h"
#include <DorothysJob/Manager/BaseGameInstance.h>
#include "DorothysJob/Manager/Events/EventManager.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include <DorothysJob/Data/DataAssets/Tutorial/EventListDataAsset.h>
#include <DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h>

UInteractableCleaningCupboard::UInteractableCleaningCupboard() :
  m_fFirstPhaseTimer(2.0f),
  m_fSecondPhaseTimer(4.0f),
  m_iRegenerationPoints(40),
  m_fHealingRate(5.f),
  m_fHealingDelay(1.f),
  m_pVFXNiagaraSystem(nullptr),
  m_pVFXNiagaraComponent(nullptr),
  m_iMaxHealth(100),
  m_iCurrentHealth(0),
  m_iCurrentPhase(0)
{
}

void UInteractableCleaningCupboard::AddTime(float _TimeToAdd)
{
  m_fFirstPhaseTimer += _TimeToAdd;
  m_fSecondPhaseTimer += _TimeToAdd;
  m_iCurrentPhase = 0;

  if (UWorld* CachedWorld = GetWorld())
  {
    CachedWorld->GetTimerManager().ClearTimer(m_fthRechargeTimer);
    CachedWorld->GetTimerManager().SetTimer(m_fthRechargeTimer, this, &UInteractableCleaningCupboard::OnRegenerate, m_fFirstPhaseTimer, false);
  }
}

void UInteractableCleaningCupboard::BeginPlay()
{
  Super::BeginPlay();
  // this will start the regeneration, on zero
  GetWorld()->GetTimerManager().SetTimer(m_fthRechargeTimer, this, &UInteractableCleaningCupboard::OnRegenerate, m_fFirstPhaseTimer, false);
}

void UInteractableCleaningCupboard::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
  Super::EndPlay(EndPlayReason);
}

void UInteractableCleaningCupboard::Interact()
{
  if (bItsfirstTimeUsing)
  {
    bItsfirstTimeUsing = false;

    UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
    if (!pGameInstance)
    {
      UE_LOG(LogTemp, Warning, TEXT("GameInstance not found"));
      return;
    }

    FString SequenceNameText = FString::Printf(TEXT("FIRST_TIME_CleanStation"), pGameInstance->m_iCurrentLevel);
    FName SequenceName(*SequenceNameText);

    UEventManager* pEventManager = pGameInstance->GetEventManager();
    pEventManager->TriggerEvent(SequenceName);
  }

  if (!IsValid(m_pPlayer) || !m_bIsOverlapping || m_iCurrentHealth <= 0)
  {
    return;
  }

  if (m_pPlayer->GetCurrentHealth() == m_pPlayer->GetMaxHealth())
  {
    return;
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->AddProgress("SoapTroubles", 1);
    }
  }

  m_pPlayer->Heal(m_iCurrentHealth); // we heal the player

  ULevelManager* pLevelManager = nullptr;
  if (IsValid(GetWorld()))
  {
    pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  }

  m_iCurrentHealth = 0; // has been used so we set it to zero
  m_iCurrentPhase = 0;  // we reset the current phase as well

  GetWorld()->GetTimerManager().ClearTimer(m_fthRechargeTimer);
  GetWorld()->GetTimerManager().SetTimer(m_fthRechargeTimer, this, &UInteractableCleaningCupboard::OnRegenerate, m_fFirstPhaseTimer, false);

  // we spawn vfx
  if (IsValid(m_pVFXNiagaraSystem))
  {
    UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pVFXNiagaraSystem, GetOwner()->GetActorLocation(), FRotator::ZeroRotator);
  }

  // audio here
  FMOD::Studio::EventInstance* pEvent = nullptr;
  FMOD::Studio::System* pStudioSystem = IFMODStudioModule::Get().GetStudioSystem(EFMODSystemContext::Type::Runtime);
  if (pStudioSystem)
  {
    FMOD::Studio::EventDescription* pEventDesc = nullptr;
    pStudioSystem->getEvent("event:/SoundFX/CombatTools/CleanStation/SFX_CLS_cleanStationInteract", &pEventDesc);

    if (pEventDesc)
    {
      pEventDesc->createInstance(&pEvent);
    }
    if (pEvent)
    {
      pEvent->start();
    }
  }

  OnInteraction.Broadcast();

  SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
  SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void UInteractableCleaningCupboard::OnInteractableBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
  ABasePlayer* player = Cast<ABasePlayer>(OtherActor);
  if (player && (player->GetCurrentHealth() < player->GetMaxHealth()) && m_iCurrentPhase > 0)
  {
    Super::OnInteractableBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
  }
}

void UInteractableCleaningCupboard::OnRegenerate()
{
  m_iCurrentPhase = FMath::Clamp(++m_iCurrentPhase, 0, 2);

  switch (m_iCurrentPhase)
  {
  case 0:
    m_iCurrentHealth = 0;
    GetWorld()->GetTimerManager().ClearTimer(m_fthRechargeTimer);
    GetWorld()->GetTimerManager().SetTimer(m_fthRechargeTimer, this, &UInteractableCleaningCupboard::OnRegenerate, m_fFirstPhaseTimer, false);
    break;
  case 1:
    m_iCurrentHealth = m_iRegenerationPoints;
    SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    GetWorld()->GetTimerManager().ClearTimer(m_fthRechargeTimer);
    GetWorld()->GetTimerManager().SetTimer(m_fthRechargeTimer, this, &UInteractableCleaningCupboard::OnRegenerate, m_fSecondPhaseTimer, false);
    break;
  case 2:
    m_iCurrentHealth = m_iMaxHealth;
    GetWorld()->GetTimerManager().ClearTimer(m_fthRechargeTimer);
    break;
  }
  OnRegenerateHealth.Broadcast(m_iCurrentHealth);
}

