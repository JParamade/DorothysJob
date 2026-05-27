#include "BasePlayer.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "EnhancedInputComponent.h"
#include "FMODEvent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Kismet/GameplayStatics.h"
#include "Components/WidgetComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "DorothysJob/Data/DataAssets/PlayerPropertiesDataAsset.h"
#include "DorothysJob/Component/Cleaning/Drawing.h"
#include "DorothysJob/Actor/Room/Tile/BaseTile.h"
#include "DorothysJob/Animations/Player/PlayerAnimInstance.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Camera/BaseCamera.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/WorldSubsystem/LevelManager.h"
#include "DorothysJob/Manager/WorldSubsystem/TimeManager.h"
#include "DorothysJob/Data/DataAssets/ConsumableDataAsset.h"
#include "DorothysJob/Consumables/Spawnable/SpawnableConsumable.h"
#include "DorothysJob/Actor/Consumables/BaseConsumableActor.h"
#include "DorothysJob/Consumables/Durable/DurableConsumable.h"
#include "DorothysJob/Status/Modify Stats/ModifyStatsEffect.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Utils/GeneralUtils.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

ABasePlayer::ABasePlayer() :
  ABaseCharacter()
{
  PrimaryActorTick.bCanEverTick = true;
  // Player parameters
  m_bStinky = false;
  m_bIsAttacking = false;
  m_bIsCleaning = false;
  m_bIsDashing = false;
  m_bCanDash = true;
  m_bCanShoot = true;
  m_bCanSpecialShoot = true;
  m_fDirtRotation = 0;
  m_bBlinkActive = true;
  // Don't rotate BasePlayer to camera direction
  bUseControllerRotationPitch = false;
  bUseControllerRotationRoll = false;
  bUseControllerRotationYaw = false;
  // Drawing component
  m_pDrawingComponent = CreateDefaultSubobject<UDrawing>(TEXT("DrawingComponent"));
}

void ABasePlayer::BeginPlay()
{
  Super::BeginPlay();

  m_bIsUsingLookAt = false;

  m_pPlayerPropertiesDA = Cast<UPlayerPropertiesDataAsset>(m_pCharacterDataAsset);
  if (IsValid(m_pPlayerPropertiesDA))
  {
    m_lDirtyMaterial = m_pPlayerPropertiesDA->m_lDirtyMaterial;
    m_fRotInterpSpeed = m_pPlayerPropertiesDA->m_fRotInterpSpeed;

    if (IsValid(m_pPlayerPropertiesDA->m_oCameraClass))
    {
      FActorSpawnParameters SpawnParams;
      SpawnParams.Owner = this;
      SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

      m_pCurrentCamera = GetWorld()->SpawnActor<ABaseCamera>(m_pPlayerPropertiesDA->m_oCameraClass, GetActorLocation(), GetActorRotation(), SpawnParams);
      if (IsValid(m_pCurrentCamera))
      {
        SetControllerCamera();
      }
    }

    if (IsValid(m_pMeshComponent) && IsValid(m_pPlayerPropertiesDA->m_pAimVFX))
    {
      UNiagaraFunctionLibrary::SpawnSystemAttached(
        m_pPlayerPropertiesDA->m_pAimVFX,
        m_pMeshComponent,
        NAME_None,
        FVector::ZeroVector,
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        true
      );
    }
  }

  // Get Managers.
  if (IsValid(GetWorld())) {
    m_pTimeManager = GetWorld()->GetSubsystem<UTimeManager>();
    m_pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  }

  SpawnSelectedWeapons();
  SetSelectedConsumables();
  UpdateConsumableInfo();
}

void ABasePlayer::Tick(float _fDeltaTime)
{
  Super::Tick(_fDeltaTime);

  if (IsValid(m_pMeshComponent))
  {
    FRotator currentRotation = FMath::RInterpTo(m_pMeshComponent->GetRelativeRotation(), m_rLookAtRotation, _fDeltaTime, m_fRotInterpSpeed);
    m_pMeshComponent->SetRelativeRotation(currentRotation);
  }

  if (m_bStartAudioInterp) {
    InterpolateAudio(
      "isDorothy_dirty",
      m_bStartAudioInterp,
      m_fCurrentAudioValue,
      m_fTargetAudioValue,
      m_fInterpAudioSpeed,
      _fDeltaTime
    );
  }
  if (IsValid(m_pTimeManager) && m_pTimeManager->m_bStartAudioInterp) {
    InterpolateAudio(
      "isTime_runningOut",
      m_pTimeManager->m_bStartAudioInterp,
      m_pTimeManager->m_fCurrentAudioValue,
      m_pTimeManager->m_fTargetAudioValue,
      m_pTimeManager->m_fInterpAudioSpeed,
      _fDeltaTime
    );
  }
  if (IsValid(m_pLevelManager) && m_pLevelManager->m_bStartAudioInterp) {
    InterpolateAudio(
      "isRoom_dirty",
      m_pLevelManager->m_bStartAudioInterp,
      m_pLevelManager->m_fCurrentAudioValue,
      m_pLevelManager->m_fTargetAudioValue,
      m_pLevelManager->m_fInterpAudioSpeed,
      _fDeltaTime
    );
  }
}

void ABasePlayer::EndPlay(const EEndPlayReason::Type _eEndPlayReason)
{
  GetWorldTimerManager().ClearAllTimersForObject(this);
  m_lSelectedConsumables.Empty();
  Super::EndPlay(_eEndPlayReason);
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* _pPlayerInputComponent)
{
  UEnhancedInputComponent* pEnhancedInputComponent = Cast<UEnhancedInputComponent>(_pPlayerInputComponent);
  m_pPlayerPropertiesDA = Cast<UPlayerPropertiesDataAsset>(m_pCharacterDataAsset);
  if (!IsValid(m_pPlayerPropertiesDA) || !IsValid(pEnhancedInputComponent))
  {
    return;
  }

  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pAttackAction, ETriggerEvent::Triggered, this, &ABasePlayer::Attack);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pAttackAction, ETriggerEvent::Completed, this, &ABasePlayer::ReleaseAttack);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pCleanAction, ETriggerEvent::Triggered, this, &ABasePlayer::Clean);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pCleanAction, ETriggerEvent::Completed, this, &ABasePlayer::ReleaseClean);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pWeaponChangeAction, ETriggerEvent::Triggered, this, &ABasePlayer::WeaponChange);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pMoveAction, ETriggerEvent::Triggered, this, &ABasePlayer::Move);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pLookAtAction, ETriggerEvent::Triggered, this, &ABasePlayer::LookAt);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pLookAtAction, ETriggerEvent::Completed, this, &ABasePlayer::LookAtComplete);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pDashAction, ETriggerEvent::Triggered, this, &ABasePlayer::Dash);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pInteractAction, ETriggerEvent::Triggered, this, &ABasePlayer::Interact);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pSpecialAttackAction, ETriggerEvent::Triggered, this, &ABasePlayer::SpecialAttack);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pSpecialAttackAction, ETriggerEvent::Ongoing, this, &ABasePlayer::SpecialAttack);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pSpecialAttackAction, ETriggerEvent::Completed, this, &ABasePlayer::ReleaseSpecialAttack);
  pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pPauseAction, ETriggerEvent::Completed, this, &ABasePlayer::Pause);

  FString sLevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
  if (sLevelName != FString("L_Hub"))
  {
    pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pUsePrimaryConsumableAction, ETriggerEvent::Triggered, this, &ABasePlayer::UsePrimaryConsumable);
    pEnhancedInputComponent->BindAction(m_pPlayerPropertiesDA->m_pUseSecondaryConsumableAction, ETriggerEvent::Triggered, this, &ABasePlayer::UseSecondaryConsumable);
  }
}

#pragma region | Stats
int32 ABasePlayer::Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _bIsCritical)
{
  int32 iDamage = 0;
  if ((!m_bStinky) && (!m_bIsInvulnerable))
  {
    iDamage = Super::Damage(_iModifyHealth, _eAttackType);

    SetIsInvulnerable(true);

    if (IsValid(m_pPlayerPropertiesDA))
    {
      m_pCurrentCamera->SetShakeComponent(m_pPlayerPropertiesDA->m_oDamageShakeBP);
      m_pCurrentCamera->PlayShakeComponent();

      if (APlayerController* PC = Cast<APlayerController>(GetController()))
      {
        m_oFeedbackHandle =  PC->PlayDynamicForceFeedback(
          m_pPlayerPropertiesDA->m_oDamageVibrationParams.m_fIntensity,
          m_pPlayerPropertiesDA->m_oDamageVibrationParams.m_fDuration,
          m_pPlayerPropertiesDA->m_oDamageVibrationParams.m_bAffectsLeftLarge,
          m_pPlayerPropertiesDA->m_oDamageVibrationParams.m_bAffectsLeftSmall,
          m_pPlayerPropertiesDA->m_oDamageVibrationParams.m_bAffectsRightLarge,
          m_pPlayerPropertiesDA->m_oDamageVibrationParams.m_bAffectsRightSmall,
          EDynamicForceFeedbackAction::Start);
      }

      TWeakObjectPtr<ABasePlayer> WeakThis(this);
      GetWorldTimerManager().SetTimer(m_tInvulnerableTimerHandle,
        [WeakThis]() {
          if (WeakThis.IsValid())
          {
            WeakThis->SetIsInvulnerable(false);
          }
        },
        m_pPlayerPropertiesDA->m_fInvulnerableTime, false);
    }
  }

  // setting the is hit on Dorothy
  if (IsValid(m_pLevelManager) && (iDamage != 0))
  {
    m_pLevelManager->HasDorothyBeenHit();
  }

  return iDamage;
}

void ABasePlayer::Heal(int32 _iModifyHealth)
{
  Super::Heal(_iModifyHealth);

  if (m_bStinky)
  {
    m_bStinky = false;

    // Start Audio Interpolation (IsDorothyDirty)
    m_bStartAudioInterp = true;
    m_fTargetAudioValue = 0.f;

    SetBaseMaterial();

    if (GetWorldTimerManager().IsTimerActive(m_tDirtCooldownTimerHandle))
    {
      GetWorldTimerManager().ClearTimer(m_tDirtCooldownTimerHandle);
    }
  }
}

void ABasePlayer::PristineTimeVFX() const
{
  if (m_pCollisionComponent && m_pPlayerPropertiesDA->m_pPristineTimeVFX)
  {
    UNiagaraComponent* PristineTimeVFXComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
      m_pPlayerPropertiesDA->m_pPristineTimeVFX,
      m_pCollisionComponent,
      NAME_None,
      FVector::ZeroVector,
      FRotator::ZeroRotator,
      EAttachLocation::KeepRelativeOffset,
      true
    );

    if (PristineTimeVFXComp)
    {
      FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, true);
      PristineTimeVFXComp->AttachToComponent(m_pCollisionComponent, Rules);
      PristineTimeVFXComp->bUseAttachParentBound = false;
      PristineTimeVFXComp->SetAbsolute(false, true, false);
    }
  }
}

float ABasePlayer::GetCurrentWeaponSpecialCharge()
{
  float value = 0.f;
  if (IsValid(m_pCurrentWeapon))
  {
    value = m_pCurrentWeapon->GetUltimatePercentage();
  }
  return value;
}

void ABasePlayer::SetCurrentWeaponSpecialCharge(float _fValue)
{
  if ((_fValue > 0.f) && IsValid(m_pCurrentWeapon))
  {
    m_pCurrentWeapon->SetUltimatePercentage(_fValue);
  }
}

void ABasePlayer::ModifyStats(const TMap<EModifiableStats, float> _mStats)
{
  for (auto pair : _mStats)
  {
    if (pair.Key == EModifiableStats::SPECIALCHARGE)
    {
      SetCurrentWeaponSpecialCharge(pair.Value);
    }
  }
}

void ABasePlayer::RevertStats(const TMap<EModifiableStats, float> _mStats)
{

}
#pragma endregion

#pragma region | Getters
ABaseWeapon* ABasePlayer::GetCurrentWeapon() const
{
  return m_pCurrentWeapon;
}

TArray<TObjectPtr<ABaseWeapon>>& ABasePlayer::GetCurrentWeapons()
{
  return m_lSpawnedWeapon;
}

FRotator ABasePlayer::GetLookAtRotation() const
{
  return m_rLookAtRotation;
}

ABaseCamera* ABasePlayer::GetCameraComponent() const
{
  return m_pCurrentCamera;
}

UBaseRoom* ABasePlayer::GetCurrentRoom()
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return nullptr;
  }

  return pLevelManager->GetCurrentRoom();
}

float ABasePlayer::GetAttackSpeedMultiplier() const
{
  return m_fAttackSpeedMultiplier;
}

bool ABasePlayer::GetIsPlayerStinky() const
{
  return m_bStinky;
}

bool ABasePlayer::GetIsPlayerWithB13() const
{
  return (m_fB13SpeedMultiplier < 0.99f);
}

int32 ABasePlayer::GetPrimaryConsumableAmount() const
{
  if (m_lSelectedConsumables.IsValidIndex(0))
  {
    return m_lSelectedConsumables[0]->m_uStackQuantity;
  }
  return 0;
}

int32 ABasePlayer::GetSecondaryConsumableAmount() const
{
  if (m_lSelectedConsumables.IsValidIndex(1))
  {
    return m_lSelectedConsumables[1]->m_uStackQuantity;
  }
  return 0;
}
#pragma endregion

#pragma region | Camera
void ABasePlayer::SetControllerCamera()
{
  APlayerController* pPlayerController = Cast<APlayerController>(GetController());
  if (IsValid(pPlayerController))
  {
    pPlayerController->SetViewTargetWithBlend(m_pCurrentCamera, 0.5f);
    m_pCurrentCamera->SetTargetActor(this);
  }
}
#pragma endregion

#pragma region | Weapon
void ABasePlayer::SpawnSelectedWeapons()
{
  // Old weapons in the list should be destroyed.
  for (ABaseWeapon* pWeapon : m_lSpawnedWeapon)
  {
    if (IsValid(pWeapon))
    {
      pWeapon->Destroy();
    }
  }
  m_lSpawnedWeapon.Empty();
  // Only the available weapons are spawned in the level.
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
  if (!IsValid(pGameInstance))
  {
    return;
  }
  // It will spawn the selected weapons attached in the correct socket.
  FName sHandSocket = "WeaponSocketL";
  FActorSpawnParameters tSpawnParams;
  tSpawnParams.Owner = this;
  tSpawnParams.Instigator = this;
  for (const UWeaponDataAsset* pWeaponAsset : pGameInstance->m_lSelectedWeapons)
  {
    if (IsValid(pWeaponAsset) && IsValid(pWeaponAsset->m_cWeaponClass))
    {
      ABaseWeapon* pWeapon = GetWorld()->SpawnActor<ABaseWeapon>(pWeaponAsset->m_cWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, tSpawnParams);
      if (IsValid(pWeapon))
      {
        sHandSocket = "WeaponSocketL";
        if (pWeaponAsset->m_eWeaponHandSocket == EWeaponHandSocket::Right)
        {
          sHandSocket = "WeaponSocketR";
        }
        USkeletalMeshComponent* pWeaponMeshComponent = pWeapon->GetMeshComponent();
        if (IsValid(pWeaponMeshComponent))
        {
          pWeaponMeshComponent->AttachToComponent(m_pMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, sHandSocket);
        }

        USkeletalMeshComponent* pSecondaryWeaponMesh = pWeapon->GetSecondaryMeshComponent();
        if ((pWeaponAsset->m_eWeaponHandSocket == EWeaponHandSocket::Both) && IsValid(pSecondaryWeaponMesh))
        {
          sHandSocket = "WeaponSocketR";
          pSecondaryWeaponMesh->AttachToComponent(m_pMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, sHandSocket);
        }

        pWeapon->SetActorHiddenInGame(true);
        pWeapon->SetActorEnableCollision(false);
        pWeapon->SetActorTickEnabled(false);
        m_lSpawnedWeapon.Add(pWeapon);
      }
    }
  }

  m_iWeaponsSize = m_lSpawnedWeapon.Num();
  SelectWeapon(0);
}
#pragma endregion

#pragma region | Consumables
void ABasePlayer::SetSelectedConsumables()
{
  // Old consumables in the list should be destroyed.
  for (TObjectPtr<UBaseConsumable> pConsumable : m_lSelectedConsumables)
  {
    if (IsValid(pConsumable))
    {
      pConsumable->ConditionalBeginDestroy();
    }
  }
  m_lSelectedConsumables.Empty();
  // Only available consumables will be created in the level.
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(UGameplayStatics::GetGameInstance(this));
  if (!IsValid(pGameInstance))
  {
    return;
  }
  // It will create the selected consumables.
  for (const UConsumableDataAsset* pConsumableAsset : pGameInstance->m_lSelectedConsumables)
  {
    if (!IsValid(pConsumableAsset))
    {
      continue;
    }

    UBaseConsumable* pConsumable = NewObject<UBaseConsumable>(this, pConsumableAsset->m_oConsumableClass);
    if (IsValid(pConsumable))
    {
      USpawnableConsumable* pSpawnable = Cast<USpawnableConsumable>(pConsumable);
      if (IsValid(pSpawnable) && IsValid(pConsumableAsset->m_oConsumableActor))
      {
        pSpawnable->m_oActorToSpawn = pConsumableAsset->m_oConsumableActor;
      }

      UDurableConsumable* pDurable = Cast<UDurableConsumable>(pConsumable);
      if (IsValid(pDurable))
      {
        pDurable->m_fEffectDuration = pConsumableAsset->m_iDuration;
      }

      pConsumable->m_uStackQuantity = pConsumableAsset->m_iStackQuantity;
      pConsumable->m_iRewardAddedAmount = pConsumableAsset->m_iAmountAddedWhenReward;

      m_lSelectedConsumables.Add(pConsumable);
    }
  }
  UpdateConsumableInfo();
}

void ABasePlayer::OnConsumableEffectStart(float _fMovementSpeedMultiplier, float _fAttackSpeedMultipliere)
{
  m_pMeshComponent->SetOverlayMaterial(m_pCharacterDataAsset->m_pBuffMaterialStart);

  if (UWorld* CachedWorld = GetWorld())
  {
    FTimerHandle TimerHandle;
    TWeakObjectPtr<ABasePlayer> WeakThis(this);
    CachedWorld->GetTimerManager().SetTimer(
      TimerHandle,
      [WeakThis, _fMovementSpeedMultiplier, _fAttackSpeedMultipliere]()
      {
        if (WeakThis.IsValid())
        {
          if (IsValid(WeakThis->m_pCharacterDataAsset) && IsValid(WeakThis->m_pCharacterDataAsset->m_pBuffMaterialApplied))
          {

          }
          WeakThis->m_pMeshComponent->SetOverlayMaterial(WeakThis->m_pCharacterDataAsset->m_pBuffMaterialApplied);

          WeakThis->m_fB13SpeedMultiplier = 1.0f / _fMovementSpeedMultiplier;
          WeakThis->m_fAttackSpeedMultiplier = _fAttackSpeedMultipliere;

          WeakThis->ChangeMovementVelocity(_fMovementSpeedMultiplier);

          for (ABaseWeapon* CurrentWeapon : WeakThis->m_lSpawnedWeapon)
          {
            if (!IsValid(CurrentWeapon))
            {
              continue;
            }

            CurrentWeapon->SetCooldownMultiplier(1.0f / _fAttackSpeedMultipliere);
          }
        }
      },
      1.0f,
      false
    );
  }
}

void ABasePlayer::OnConsumableEffectEnd()
{
  m_pMeshComponent->SetOverlayMaterial(nullptr);

  m_fAttackSpeedMultiplier = 1.0f;

  ChangeMovementVelocity(m_fB13SpeedMultiplier);

  m_fB13SpeedMultiplier = 1.f;

  for (ABaseWeapon* CurrentWeapon : m_lSpawnedWeapon)
  {
    if (!IsValid(CurrentWeapon))
    {
      continue;
    }

    CurrentWeapon->ResetCooldownMultiplier();
  }
}

void ABasePlayer::UpdateConsumableInfo()
{
  for (int32 iIndex = 0; iIndex < m_lSelectedConsumables.Num(); ++iIndex)
  {
    if (IsValid(m_lSelectedConsumables[iIndex]))
    {
      OnConsumableUpdated.Broadcast(m_lSelectedConsumables[iIndex]->m_uStackQuantity, iIndex);
    }
  }
}
void ABasePlayer::SetPrimaryConsumableAmount(uint32 _NewAmount)
{
  if (m_lSelectedConsumables.IsValidIndex(0))
  {
    m_lSelectedConsumables[0]->m_uStackQuantity = _NewAmount;
  }
}
void ABasePlayer::SetSecondaryConsumableAmount(uint32 _NewAmount)
{
  if (m_lSelectedConsumables.IsValidIndex(1))
  {
    m_lSelectedConsumables[1]->m_uStackQuantity = _NewAmount;
  }
}
#pragma endregion

#pragma region | Movement
void ABasePlayer::ChangeMovementVelocity(float _fVelocityMultiplicator, float _fRotationMultiplicator)
{
  if (!IsValid(m_pMovementComponent))
  {
    return;
  }

  m_fRotInterpSpeed *= _fRotationMultiplicator;
  m_pMovementComponent->MaxSpeed *= _fVelocityMultiplicator;
}

void ABasePlayer::PerformDash(const FVector& _vDirection, float _fDistance, float _fTime)
{
  if (!IsValid(m_pMovementComponent) || (_vDirection.IsNearlyZero()) || (_fDistance <= 0.f) || (_fTime <= 0.f))
  {
    return;
  }

  m_bIsDashing = true;
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
  }
  SetIsInvulnerable(true);
  m_pMovementComponent->Velocity = _vDirection.GetSafeNormal() * (_fDistance / _fTime);
  GetWorldTimerManager().SetTimer(m_tDashStopTimerHandle, this, &ABasePlayer::StopDash, _fTime);
  if (m_bStinky)
  {
    ScheduleToDirt();
  }
}
#pragma endregion

#pragma region | Game Over

bool ABasePlayer::GetCanPlayAudio() const {
  return m_bCanPlayUnavailableSound;
}

void ABasePlayer::SetCanPlayAudio(bool _bValue) {
  m_bCanPlayUnavailableSound = _bValue;
}

void ABasePlayer::GameOver()
{
  Super::GameOver();

  m_bStinky = true;

  // Set false the stinky or reload achievement
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->m_bNoStinkyReload = false;
    }
  }

  // Start Audio Interpolation (IsDorothyDirty)
  m_bStartAudioInterp = true;
  m_fTargetAudioValue = 1.f;

  SetBaseMaterial();
  ScheduleToDirt();

  if (APlayerController* PC = Cast<APlayerController>(GetController()))
  {
    m_oFeedbackHandle = PC->PlayDynamicForceFeedback(
      m_pPlayerPropertiesDA->m_oStinkyVibrationParams.m_fIntensity,
      m_pPlayerPropertiesDA->m_oStinkyVibrationParams.m_fDuration,
      m_pPlayerPropertiesDA->m_oStinkyVibrationParams.m_bAffectsLeftLarge,
      m_pPlayerPropertiesDA->m_oStinkyVibrationParams.m_bAffectsLeftSmall,
      m_pPlayerPropertiesDA->m_oStinkyVibrationParams.m_bAffectsRightLarge,
      m_pPlayerPropertiesDA->m_oStinkyVibrationParams.m_bAffectsRightSmall,
      EDynamicForceFeedbackAction::Start);
  }

  if (m_bIsCleaning && IsValid(m_pCurrentWeapon))
  {
    JumpSectionAnimMontage(FName("Clean_Release"));
    m_pCurrentWeapon->UseWeaponReleaseClean();
    m_bIsCleaning = false;
  }

  if (IsValid(m_pLevelManager))
  {
    m_pLevelManager->AddStinkyTimes();
  }
}
#pragma endregion

#pragma region | Skin
void ABasePlayer::SetBaseMaterial()
{
  if (!IsValid(m_pCharacterDataAsset) || !IsValid(m_pMeshComponent))
  {
    return;
  }

  // It selects Base or Dirty Skin
  TArray<TObjectPtr<UMaterialInterface>> lSelectedSkin = (!m_bStinky) ? m_pCharacterDataAsset->m_lBaseMaterial : m_lDirtyMaterial;
  for (int32 iMaterialIndex = 0; iMaterialIndex < lSelectedSkin.Num(); ++iMaterialIndex)
  {
    m_pMeshComponent->SetMaterial(iMaterialIndex, lSelectedSkin[iMaterialIndex]);
  }
}
#pragma endregion

#pragma region | Input Action Functions
void ABasePlayer::Move(const FInputActionValue& _oIAValue)
{
  FVector2D v2Movement = _oIAValue.Get<FVector2D>();
  if (IsValid(m_pCurrentCamera) && m_bCanDoActions && !m_bIsDashing)
  {
    const FRotator oYawCameraRotator(0.f, m_pCurrentCamera->GetActorRotation().Yaw, 0.f);
    const FVector vForward = oYawCameraRotator.RotateVector(GetActorForwardVector());
    const FVector vRight = UKismetMathLibrary::Cross_VectorVector(GetActorUpVector(), vForward);
    AddMovementInput(vForward, v2Movement.Y);
    AddMovementInput(vRight, v2Movement.X);

    UInputDeviceSubsystem* pInputDeviceSubsystem = GetGameInstance()->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>();
    bool bIsUsingGamepad = IsValid(pInputDeviceSubsystem) && (pInputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(GetPlatformUserId()).PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad);
    if (bIsUsingGamepad && !m_bIsUsingLookAt && !v2Movement.IsNearlyZero())
    {
      m_rLookAtRotation = oYawCameraRotator.RotateVector(FVector(v2Movement.Y, v2Movement.X, 0.)).Rotation();
    }
  }
}

void ABasePlayer::LookAt(const FInputActionValue& _oIAValue)
{
  FVector2D v2Looking = _oIAValue.Get<FVector2D>();
  FVector vActorLocation = GetActorLocation();
  FRotator desiredRotation = FRotator::ZeroRotator;
  m_bIsUsingLookAt = true;
  UInputDeviceSubsystem* pInputDeviceSubsystem = GetGameInstance()->GetEngine()->GetEngineSubsystem<UInputDeviceSubsystem>();
  if (IsValid(pInputDeviceSubsystem) && (pInputDeviceSubsystem->GetMostRecentlyUsedHardwareDevice(GetPlatformUserId()).PrimaryDeviceType == EHardwareDevicePrimaryType::Gamepad))
  {
    const FRotator oYawCameraRotator(0.f, m_pCurrentCamera->GetActorRotation().Yaw, 0.f);
    FVector vEndLocation = oYawCameraRotator.RotateVector(FVector(v2Looking, 0));
    desiredRotation = UKismetMathLibrary::FindLookAtRotation(vActorLocation, vActorLocation + vEndLocation);
  }
  else
  {
    APlayerController* pPlayerController = Cast<APlayerController>(Controller);
    if (IsValid(pPlayerController))
    {
      FVector2D MousePosition;
      FVector WorldOrigin, WorldDirection;
      if (pPlayerController->GetMousePosition(MousePosition.X, MousePosition.Y) &&
        pPlayerController->DeprojectScreenPositionToWorld(MousePosition.X, MousePosition.Y, WorldOrigin, WorldDirection))
      {
        float fIntersectionPlane = (vActorLocation.Z - WorldOrigin.Z) / WorldDirection.Z;
        FVector vMousePointer = WorldOrigin + WorldDirection * fIntersectionPlane;
        desiredRotation = UKismetMathLibrary::FindLookAtRotation(vActorLocation, vMousePointer);
      }
    }
  }
  m_rLookAtRotation = desiredRotation;
}

void ABasePlayer::LookAtComplete(const FInputActionValue& _oIAValue)
{
  m_bIsUsingLookAt = false;
}

void ABasePlayer::Attack(const FInputActionValue& _oIAValue)
{
  if (m_bIsCleaning || m_bIsSpecialActive || !m_bCanDoActions)
  {
    return;
  }

  if (IsValid(m_pCurrentWeapon))
  {
    TSoftObjectPtr<UBaseWeaponStatsDataAsset> pWeaponDataAsset = m_pCurrentWeapon->GetDataAsset();
    if (!pWeaponDataAsset.IsValid())
    {
      return;
    }

    bool bCanStartAnimation = false;
    if (pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Distance)
    {
      bCanStartAnimation = m_pCurrentWeapon->UseWeaponAttack();
    }

    bCanStartAnimation = bCanStartAnimation && GeneralUtils::IsWasheeFlashee(pWeaponDataAsset);
    bool bCanDirtminatorShoot = m_bCanShoot && GeneralUtils::IsDirtminator(pWeaponDataAsset);

    if ((pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Melee) || bCanStartAnimation || bCanDirtminatorShoot)
    {
      float fPlayRate = pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Melee ? m_fAttackSpeedMultiplier : 1.0f;
      PlayAnimMontage(FName("Attack"), fPlayRate);
      m_bIsAttacking = true;
    }
  }
}

void ABasePlayer::ReleaseAttack(const FInputActionValue& _oIAValue)
{
  if (!IsValid(m_pCurrentWeapon) || !m_bIsAttacking)
  {
    return;
  }

  TSoftObjectPtr<UBaseWeaponStatsDataAsset> pWeaponDataAsset = m_pCurrentWeapon->GetDataAsset();

  if (pWeaponDataAsset.IsValid())
  {
    if (m_bCanShoot)
    {
      JumpSectionAnimMontage(FName("Attack_Release"));
    }

    if (pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Distance)
    {
      m_pCurrentWeapon->UseWeaponReleaseAttack();
      m_bIsAttacking = false;
    }

    m_bCanShoot = true;
  }
}

void ABasePlayer::Clean(const FInputActionValue& _oIAValue)
{
  if (m_bIsAttacking || m_bIsSpecialActive || !m_bCanDoActions)
  {
    return;
  }

  if (m_bStinky) {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
      if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
        if (IsValid(m_pPlayerPropertiesDA) && m_bCanPlayUnavailableSound) {
          pAudioManager->PlaySound2D(GetWorld(), m_pPlayerPropertiesDA->m_pUnavailableEvent);
          m_bCanPlayUnavailableSound = false;
        }
      }
    }
    return;
  }

  m_bIsCleaning = true;
  if (IsValid(m_pCurrentWeapon))
  {
    PlayAnimMontage(FName("Clean"));
    m_pCurrentWeapon->UseWeaponClean();
  }
}

void ABasePlayer::ReleaseClean(const FInputActionValue& _oIAValue)
{
  m_bCanPlayUnavailableSound = true;

  if (!m_bIsCleaning || m_bStinky)
  {
    return;
  }

  if (IsValid(m_pCurrentWeapon))
  {
    JumpSectionAnimMontage(FName("Clean_Release"));
    m_pCurrentWeapon->UseWeaponReleaseClean();
  }
}

void ABasePlayer::Dash(const FInputActionValue& _oIAValue)
{
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
    if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
      pAchSubsystem->AddProgress("TheTestAchievement", 1);
    }
  }

  if (m_bCanDoActions && m_bCanDash)
  {
    FVector vDirection = GetVelocity().IsNearlyZero() ? m_pMeshComponent->GetForwardVector() : GetLastMovementInputVector().GetSafeNormal();
    PerformDash(vDirection, m_pPlayerPropertiesDA->m_fDashDistance, m_pPlayerPropertiesDA->m_fDashTime);
    m_bCanDash = !m_bIsDashing;
    OnDashModeChange.Broadcast(m_bIsDashing);

    if (m_bIsDashing)
    {
      PlayAnimMontage(FName("Dash"));
      if (m_pCollisionComponent && m_pPlayerPropertiesDA->m_pDashEffect)
      {
        UNiagaraFunctionLibrary::SpawnSystemAttached(
          m_pPlayerPropertiesDA->m_pDashEffect,
          m_pCollisionComponent,
          NAME_None,
          FVector::ZeroVector,
          FRotator::ZeroRotator,
          EAttachLocation::KeepRelativeOffset,
          true
        );
      }
      if (IsValid(m_pPlayerPropertiesDA))
      {
        PlayFMODEvent(m_pPlayerPropertiesDA->m_pDashEvent);
      }
    }
  }
}

void ABasePlayer::Interact(const FInputActionValue& _oIAValue)
{
  OnInteract.Broadcast();
}

void ABasePlayer::SpecialAttack(const FInputActionValue& _oIAValue)
{
  if (m_bIsAttacking || m_bIsCleaning || !m_bCanDoActions)
  {
    return;
  }

  if (IsValid(m_pCurrentWeapon) && m_bCanSpecialShoot)
  {
    if (m_pCurrentWeapon->IsAvailableSpecialAttack())
    {
      PlayAnimMontage(FName("Special"));
    }
    
    m_pCurrentWeapon->UseWeaponSpecial();
  }
}

void ABasePlayer::ReleaseSpecialAttack(const FInputActionValue& _oIAValue)
{
  TSoftObjectPtr<UBaseWeaponStatsDataAsset> pWeaponDataAsset = m_pCurrentWeapon->GetDataAsset();
  if (!pWeaponDataAsset.IsValid())
  {
    return;
  }

  if (IsValid(m_pCurrentWeapon) && GeneralUtils::IsMopressor(pWeaponDataAsset))
  {
    if (m_bCanSpecialShoot)
    {
      JumpSectionAnimMontage(FName("Special_Release"));
    }

    m_pCurrentWeapon->UseWeaponSpecialRelease();
  }

  m_bCanPlayUnavailableSound = true;
  m_bCanSpecialShoot = true;
}

void ABasePlayer::Pause(const FInputActionValue& _oIAValue)
{
  ULevelManager* pLevelManager = GetWorld()->GetSubsystem<ULevelManager>();
  if (!IsValid(pLevelManager))
  {
    return;
  }

  pLevelManager->OnGamePause(pLevelManager->GetCurrentGameState() == EBaseGameState::PLAYING);
}

void ABasePlayer::WeaponChange(const FInputActionValue& _oIAValue)
{
  if (m_bIsSpecialActive)
  {
    return;
  }

  if (IsValid(m_pCurrentWeapon))
  {
    
    m_pCurrentWeapon->UseWeaponReleaseClean();
    m_pCurrentWeapon->UseWeaponReleaseAttack();
    m_pCurrentWeapon->UseWeaponSpecialRelease();
  }

  if (m_iWeaponsSize > 1)
  {
    float fInc = _oIAValue.Get<float>();
    SelectWeapon((m_iCurrentWeaponIndex + static_cast<int32>(fInc) + m_iWeaponsSize) % m_iWeaponsSize);
    m_pCurrentWeapon->ChangeWeapon();
  }
}

void ABasePlayer::UsePrimaryConsumable(const FInputActionValue& _oIAValue)
{
  UseConsumable(0);
}

void ABasePlayer::UseSecondaryConsumable(const FInputActionValue& _oIAValue)
{
  UseConsumable(1);
}

void ABasePlayer::AddConsumableCharge(int32 _iConsumableID)
{
  if (!m_lSelectedConsumables.IsValidIndex(_iConsumableID))
  {
    return;
  }

  if (UBaseConsumable* Consumable = m_lSelectedConsumables[_iConsumableID])
  {
    Consumable->m_uStackQuantity += Consumable->m_iRewardAddedAmount;
    OnConsumableUpdated.Broadcast(Consumable->m_uStackQuantity, _iConsumableID);
  }
}
#pragma endregion

#pragma region | Private Functions
void ABasePlayer::SelectWeapon(int32 _iCurrentWeaponIndex)
{
  if (m_iWeaponsSize <= 0)
  {
    return;
  }

  if (IsValid(m_pCurrentWeapon))
  {
    m_pCurrentWeapon->SetActorHiddenInGame(true);
    m_pCurrentWeapon->SetActorEnableCollision(false);
    m_pCurrentWeapon->SetActorTickEnabled(false);
    m_pCurrentWeapon->HasAlreadyShot.RemoveAll(this);
    m_pCurrentWeapon->OnSpecialActive.RemoveAll(this);
    m_pCurrentWeapon->OnHasSpecialAlreadyShot.RemoveAll(this);

    if (IsValid(m_pMeshComponent))
    {
      UPlayerAnimInstance* pAnimInstance = Cast<UPlayerAnimInstance>(m_pMeshComponent->GetAnimInstance());
      if (IsValid(pAnimInstance))
      {
        pAnimInstance->OnActionStarts.Unbind();
        pAnimInstance->OnActionEnds.Unbind();
        pAnimInstance->OnComboStateChanges.Unbind();
        pAnimInstance->OnActionMontageEnds.Unbind();
        ResetActionState();
      }
    }
  }

  m_iCurrentWeaponIndex = _iCurrentWeaponIndex;
  m_pCurrentWeapon = m_lSpawnedWeapon[m_iCurrentWeaponIndex];
  if (IsValid(m_pCurrentWeapon))
  {
    m_pCurrentWeapon->SetActorHiddenInGame(false);
    m_pCurrentWeapon->SetActorEnableCollision(true);
    m_pCurrentWeapon->SetActorTickEnabled(true);
    m_pCurrentWeapon->HasAlreadyShot.AddDynamic(this, &ABasePlayer::WeaponAlreadyShot);
    m_pCurrentWeapon->OnSpecialActive.AddDynamic(this, &ABasePlayer::OnSpecialActive);
    m_pCurrentWeapon->OnHasSpecialAlreadyShot.AddDynamic(this, &ABasePlayer::WeaponSpecialAlreadyShot);

    if (IsValid(m_pMeshComponent))
    {
      UPlayerAnimInstance* pAnimInstance = Cast<UPlayerAnimInstance>(m_pMeshComponent->GetAnimInstance());
      if (IsValid(pAnimInstance))
      {
        pAnimInstance->OnActionStarts.BindUObject(m_pCurrentWeapon, &ABaseWeapon::MeleeAttackHitStart);
        pAnimInstance->OnActionEnds.BindUObject(m_pCurrentWeapon, &ABaseWeapon::MeleeAttackHitEnd);
        pAnimInstance->OnComboStateChanges.BindUObject(m_pCurrentWeapon, &ABaseWeapon::ComboStateChanged);
        pAnimInstance->OnActionMontageEnds.BindUObject(this, &ABasePlayer::ResetActionState);
      }
    }
    TSoftObjectPtr<UBaseWeaponStatsDataAsset> pWeaponDataAsset = m_pCurrentWeapon->GetDataAsset();
    if (pWeaponDataAsset.IsValid())
    {
      OnWeaponChange.Broadcast(pWeaponDataAsset->m_eWeaponDirtType, pWeaponDataAsset->m_eWeaponAttackType);
    }
  }
}

void ABasePlayer::StopDash()
{
  if (IsValid(m_pMovementComponent))
  {
    m_pMovementComponent->Velocity = FVector::ZeroVector;
  }
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
  }
  SetIsInvulnerable(false);
  m_bIsDashing = false;
  OnDashModeChange.Broadcast(m_bIsDashing);
  GetWorldTimerManager().SetTimer(m_tDashCooldownTimerHandle, this, &ABasePlayer::ResetDash, m_pPlayerPropertiesDA->m_fDashCooldown);
  if (m_bStinky)
  {
    ScheduleToDirt();
  }
}

void ABasePlayer::ResetDash()
{
  m_bCanDash = true;
}

void ABasePlayer::ScheduleToDirt()
{
  if (GetWorldTimerManager().IsTimerActive(m_tDirtCooldownTimerHandle))
  {
    GetWorldTimerManager().ClearTimer(m_tDirtCooldownTimerHandle);
  }
  float fTime = !m_bIsDashing ? m_pPlayerPropertiesDA->m_fDirtCooldown : m_pPlayerPropertiesDA->m_fDashingDirtCooldown;
  GetWorldTimerManager().SetTimer(m_tDirtCooldownTimerHandle, this, &ABasePlayer::ToDirt, fTime, true);
}

void ABasePlayer::ToDirt()
{
  if (m_bStinky && IsValid(m_pPlayerPropertiesDA) && IsValid(m_pDrawingComponent))
  {
    m_pDrawingComponent->Dirt(
      m_pPlayerPropertiesDA->m_pDirtTexture,
      m_pPlayerPropertiesDA->m_pCleanTexture,
      m_pPlayerPropertiesDA->m_vDirtSize,
      m_fDirtRotation,
      FVector2D(0.5f),
      EDrawColor::EBase,
      EDrawTier::Liquid
    );
    m_fDirtRotation += m_pPlayerPropertiesDA->m_fStepRotationAngle;
    m_fDirtRotation = FMath::Fmod(m_fDirtRotation, 360.f);
  }
}

void ABasePlayer::UseConsumable(int32 _iConsumableID)
{
  if (m_lSelectedConsumables.IsValidIndex(_iConsumableID) && IsValid(m_lSelectedConsumables[_iConsumableID]))
  {
    if (m_lSelectedConsumables[_iConsumableID]->Use(this))
    {
      OnConsumableUpdated.Broadcast(m_lSelectedConsumables[_iConsumableID]->m_uStackQuantity, _iConsumableID);
      if (IsValid(m_pLevelManager))
      {
        m_pLevelManager->AddConsumableUse();
      }
    }
    else
    {
      OnConsumableUnavailable.Broadcast(m_lSelectedConsumables[_iConsumableID]->m_uStackQuantity, _iConsumableID);

      if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance())) {
        if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>()) {
          if (IsValid(m_pPlayerPropertiesDA)) pAudioManager->PlaySound2D(GetWorld(), m_pPlayerPropertiesDA->m_pUnavailableEvent);
        }
      }
    }
  }
}

void ABasePlayer::PlayAnimMontage(const FName& _sSection, float _fPlayRate)
{
  if (IsValid(m_pMeshComponent))
  {
    UPlayerAnimInstance* pAnimInstance = Cast<UPlayerAnimInstance>(m_pMeshComponent->GetAnimInstance());
    if (IsValid(pAnimInstance))
    {
      TSoftObjectPtr<UBaseWeaponStatsDataAsset> pWeaponDataAsset = m_pCurrentWeapon->GetDataAsset();
      if (!pAnimInstance->IsActionMontagePlaying())
      {
        pAnimInstance->PlayActionMontage(FName(_sSection), _fPlayRate);
      }
      else if (m_bIsAttacking && GeneralUtils::IsWasheeFlashee(pWeaponDataAsset))
      {
        pAnimInstance->JumpSectionActionMontage(FName(_sSection));
      }

      pAnimInstance->SetComboActive(m_bIsAttacking);
    }
  }
}

void ABasePlayer::JumpSectionAnimMontage(const FName& _sSection)
{
  if (IsValid(m_pMeshComponent))
  {
    UPlayerAnimInstance* pAnimInstance = Cast<UPlayerAnimInstance>(m_pMeshComponent->GetAnimInstance());
    if (IsValid(pAnimInstance))
    {
      pAnimInstance->JumpSectionActionMontage(_sSection);
    }
  }
}

void ABasePlayer::WeaponAlreadyShot()
{
  m_bCanShoot = false;
  JumpSectionAnimMontage(FName("Attack_Release"));
}

void ABasePlayer::OnSpecialActive(bool _bIsSpecialActive)
{
  m_bIsSpecialActive = _bIsSpecialActive;

  TSoftObjectPtr<UBaseWeaponStatsDataAsset> pWeaponDataAsset = m_pCurrentWeapon->GetDataAsset();
  if (m_bIsSpecialActive || !pWeaponDataAsset.IsValid() || !IsValid(m_pMeshComponent))
  {
    return;
  }

  UPlayerAnimInstance* pAnimInstance = Cast<UPlayerAnimInstance>(m_pMeshComponent->GetAnimInstance());
  if (IsValid(pAnimInstance))
  {
    if (GeneralUtils::IsDusty(pWeaponDataAsset))
    {
      pAnimInstance->StopActionMontage();
    }
    else if (GeneralUtils::IsDirtminator(pWeaponDataAsset))
    {
      JumpSectionAnimMontage(FName("Special_Release"));
    }
  }
}

void ABasePlayer::WeaponSpecialAlreadyShot()
{
  m_bCanSpecialShoot = false;
  JumpSectionAnimMontage(FName("Special_Release"));
}

void ABasePlayer::ResetActionState()
{
  m_bIsCleaning = false;
  TSoftObjectPtr<UBaseWeaponStatsDataAsset> pWeaponDataAsset = m_pCurrentWeapon->GetDataAsset();
  if (!pWeaponDataAsset.IsValid())
  {
    return;
  }

  if (pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Melee)
  {
    m_bIsAttacking = false;
  }
  // Mopressor special attack release safeguard
  if (GeneralUtils::IsMopressor(pWeaponDataAsset))
  {
    m_bIsSpecialActive = false;
  }
}

void ABasePlayer::InterpolateAudio(FName _sParameterName, bool& _bStartInterpolating, float& _fCurrentValue, float _fTargetValue, float _fInterpolationSpeed, float _fDeltaTime)
{
  _fCurrentValue = FMath::FInterpTo(_fCurrentValue, _fTargetValue, _fDeltaTime, _fInterpolationSpeed);
  _bStartInterpolating = !FMath::IsNearlyEqual(_fCurrentValue, _fTargetValue);

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>())
    {
      pAudioManager->SetGlobalParameter({ {_sParameterName, _fCurrentValue} });
    }
  }
}
#pragma endregion