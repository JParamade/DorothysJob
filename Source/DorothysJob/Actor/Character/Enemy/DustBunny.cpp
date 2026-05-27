#include "DustBunny.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "DorothysJob/Data/DataAssets/Enemy/DustBunnyPropertiesDataAsset.h"
#include "DorothysJob/Actor/Projectile/BaseProjectile.h"
#include "DorothysJob/Actor/Dust/DustPoop.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Status/BaseStatus.h"

ADustBunny::ADustBunny() :
  ABaseEnemy()
{
  // Capsule
  UpdateCapsule(50.f);
  // Debug properties
  m_bAllwaysAttackDebug = false;
}

void ADustBunny::BeginPlay()
{
  Super::BeginPlay();
  // AI
  AAIController* pAIController = Cast<AAIController>(GetController());
  UDustBunnyPropertiesDataAsset* pDustBunnyPropertiesDA = Cast<UDustBunnyPropertiesDataAsset>(m_pCharacterDataAsset);
  if (IsValid(pAIController) && IsValid(pDustBunnyPropertiesDA))
  {
    if (UBlackboardComponent* pBlackboardComp = pAIController->GetBlackboardComponent())
    {
      // Dirt properties
      pBlackboardComp->SetValueAsFloat("DirtCooldown", pDustBunnyPropertiesDA->m_fDirtCooldown);
      // Flee properties
      pBlackboardComp->SetValueAsFloat("FleeDistance", pDustBunnyPropertiesDA->m_fFleeDistance);
      pBlackboardComp->SetValueAsFloat("FleeAngle", pDustBunnyPropertiesDA->m_fFleeAngle);
      pBlackboardComp->SetValueAsFloat("FleeBounceAngle", pDustBunnyPropertiesDA->m_fFleeBounceAngle);
      pBlackboardComp->SetValueAsFloat("FleeForbiddenBounceAngle", pDustBunnyPropertiesDA->m_fFleeForbiddenBounceAngle);
      pBlackboardComp->SetValueAsFloat("SpeedUpCooldown", pDustBunnyPropertiesDA->m_fSpeedUpCooldown);
      // Tower mode properties
      pBlackboardComp->SetValueAsBool("EnableTowerMode", false);
    }
  }
  // Debug
  if (m_bAllwaysAttackDebug)
  {
    StopBehaviorTree();
    TWeakObjectPtr<APawn> pPlayer = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    TWeakObjectPtr<ABaseEnemy> WeakThis(this);
    GetWorldTimerManager().SetTimer(
      m_tTimerAttack,
      [WeakThis, pPlayer]()
      {
        if (WeakThis.IsValid() && pPlayer.IsValid())
        {
          FRotator DesiredRotation = UKismetMathLibrary::FindLookAtRotation(WeakThis->GetActorLocation(), pPlayer->GetActorLocation());
          WeakThis->SetActorRotation(FRotator(0.f, DesiredRotation.Yaw, 0.f));
          WeakThis->Attack();
        }
      },
      1.f,
      true
    );
  }
}

bool ADustBunny::IsTowerModeAvailable()
{
  UDustBunnyPropertiesDataAsset* pDustBunnyPropertiesDA = Cast<UDustBunnyPropertiesDataAsset>(m_pCharacterDataAsset);

  return IsValid(pDustBunnyPropertiesDA) && pDustBunnyPropertiesDA->m_bEnableTowerMode;
}

#pragma region | Movement
bool ADustBunny::IsInMovement() const
{
  return !m_bIsHidden;
}
#pragma endregion

#pragma region | Hide
void ADustBunny::ToHide()
{
  UDustBunnyPropertiesDataAsset* pDustBunnyPropertiesDA = Cast<UDustBunnyPropertiesDataAsset>(m_pCharacterDataAsset);
  if ((!IsValid(pDustBunnyPropertiesDA)) || (!pDustBunnyPropertiesDA->m_bEnableTowerMode))
  {
    OnToHideComplete.Broadcast(this);
    return;
  }

  m_bIsHidden = true;
  m_bHasBeenHiddenAnyTime = true;
  SetIsInvulnerable(true);
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (IsValid(pAIController))
  {
    if (UBlackboardComponent* pBlackboardComp = pAIController->GetBlackboardComponent())
    {
      pBlackboardComp->SetValueAsBool("EnableTowerMode", true);
    }
  }

  Super::ToHide();
}

void ADustBunny::UnHide()
{
  UDustBunnyPropertiesDataAsset* pDustBunnyPropertiesDA = Cast<UDustBunnyPropertiesDataAsset>(m_pCharacterDataAsset);
  if ((!m_bIsHidden) && ((!IsValid(pDustBunnyPropertiesDA)) || (!pDustBunnyPropertiesDA->m_bEnableTowerMode)))
  {
    return;
  }

  m_bStunnedByDustPoop = true;
  m_bIsHidden = false;
  SetIsInvulnerable(false);
  AAIController* pAIController = Cast<AAIController>(GetController());
  if (IsValid(pAIController))
  {
    if (UBlackboardComponent* pBlackboardComp = pAIController->GetBlackboardComponent())
    {
      pBlackboardComp->SetValueAsBool("EnableTowerMode", false);
    }
  }
  // Apply stun effect
  if (IsValid(pDustBunnyPropertiesDA->m_oUnHideEffectClass))
  {
    ApplyStatus(pDustBunnyPropertiesDA->m_oUnHideEffectClass);
  }
}
#pragma endregion

#pragma region | Actions
void ADustBunny::PerformAttack(int32 _iCurrentCombo)
{
  ABaseEnemy::PerformAttack(_iCurrentCombo);

  UDustBunnyPropertiesDataAsset* pDustBunnyPropertiesDA = Cast<UDustBunnyPropertiesDataAsset>(m_pCharacterDataAsset);
  if (!IsValid(pDustBunnyPropertiesDA))
  {
    return;
  }

  if (IsValid(pDustBunnyPropertiesDA->m_oProjectileClass) && IsValid(m_pProjectileSpawnPoint))
  {
    FRotator Rotation = GetActorRotation();
    FVector vLocation = m_pProjectileSpawnPoint->GetComponentLocation();

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<ABaseProjectile>(pDustBunnyPropertiesDA->m_oProjectileClass, vLocation, Rotation, SpawnParams);
  }
}

void ADustBunny::PerformToDirt()
{
  ABaseEnemy::PerformToDirt();

  UDustBunnyPropertiesDataAsset* pDustBunnyPropertiesDA = Cast<UDustBunnyPropertiesDataAsset>(m_pCharacterDataAsset);
  if (!IsValid(pDustBunnyPropertiesDA))
  {
    return;
  }

  FRotator Rotation = GetActorRotation();
  FVector vLocation = GetActorLocation() - (GetActorForwardVector() * 100);
  vLocation.Z = 110.0f;

  if(IsValid(pDustBunnyPropertiesDA->m_oPoopClass))
  {
    ADustPoop* pDustPoop = GetWorld()->SpawnActor<ADustPoop>(pDustBunnyPropertiesDA->m_oPoopClass, vLocation, Rotation);
    if (IsValid(m_pCurrentRoom) && IsValid(pDustPoop))
    {
      m_pCurrentRoom->RegisterDustPoop(pDustPoop);
    }
  }
}
#pragma endregion

void ADustBunny::GameOver()
{
  // Obtener subsistema de logros una sola vez
  UAchievementSubsystem* AchSubsystem = nullptr;
  if (UBaseGameInstance* GameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    AchSubsystem = GameInstance->GetSubsystem<UAchievementSubsystem>();
  }

  if (!IsValid(AchSubsystem))
  {
    return;
  }

  AchSubsystem->AddProgress("LintCrusher", 1);

  if (m_bStunnedByDustPoop)
  {
    AchSubsystem->AddProgress("AshAboveSoBelow", 1);
  }

  if (UBaseRoom* pMyRoom = GetCurrentRoom())
  {
    pMyRoom->ReturnDustPooken(this);
  }

  Super::GameOver();
}
