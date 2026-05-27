#include "BaseWeapon.h"
// DATA
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/PlayerPropertiesDataAsset.h"
// STATUS
#include "DorothysJob/Status/BaseStatus.h"
// ACTOR
#include "DorothysJob/Actor/Character/BaseCharacter.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Camera/BaseCamera.h"
#include "DorothysJob/Actor/Camera/MainLegacyCameraShake.h"
// MANAGER
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
// FMOD
#include "FMODAudioComponent.h"
#include "FMODEvent.h"
#include <Kismet/GameplayStatics.h>

ABaseWeapon::ABaseWeapon() :
  m_bReleasSpecialAttack(false),
  m_bCanAttack(true),
  m_bCanClean(true),
  m_fSpecialCharge(0.0f)
{
  // Mesh Component
  m_pWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
  if (IsValid(m_pWeaponMesh))
  {
    m_pWeaponMesh->SetupAttachment(RootComponent);
    RootComponent = m_pWeaponMesh;

    // Audio Component
    m_pCleanAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("CleanAudioComponent"));
    if (IsValid(m_pCleanAudioComponent)) {
      m_pCleanAudioComponent->SetupAttachment(m_pWeaponMesh);
    }
    m_pSpecialAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("SpecialAudioComponent"));
    if (IsValid(m_pSpecialAudioComponent)) {
      m_pSpecialAudioComponent->SetupAttachment(m_pWeaponMesh);
    }
  }

  m_bInCombo = false;
  m_iCurrentCombo = -1;
}

void ABaseWeapon::BeginPlay()
{
  Super::BeginPlay();

  m_fSpecialCharge = 0.0f;

  if (!IsValid(m_pWeaponDataAsset))
  {
    UE_LOG(LogTemp, Error, TEXT("The weapon stats data asset is not set"));
  }

  if (UWorld* pCachedWorld = GetWorld())
  {
    CachedWorld = pCachedWorld;
  }

  if (ABasePlayer* PlayerOwner = Cast<ABasePlayer>(GetOwner()))
  {
    m_pPlayerOwner = PlayerOwner;

    if (UPlayerPropertiesDataAsset* PlayerDataAsset = Cast<UPlayerPropertiesDataAsset>(m_pPlayerOwner->GetDataAsset()))
    {
      m_pPlayerDataAsset = PlayerDataAsset;
    }
  }

  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>())
    {
      m_pAudioManager = pAudioManager;
    }
  }
}

void ABaseWeapon::ChangeWeapon()
{
  OnWeaponSwicht.Broadcast();
}

bool ABaseWeapon::UseWeaponAttack()
{
  if (!m_bCanAttack)
  {
    return false;
  }

  Attack();

  m_bCanAttack = false;

  if (IsValid(CachedWorld) && IsValid(m_pWeaponDataAsset))
  {
    CachedWorld->GetTimerManager().SetTimer(m_oAttackRate, this, &ABaseWeapon::ResetAttack, m_pWeaponDataAsset->m_fAttackCooldown * m_fCooldownMultiplier, false);
    return true;
  }

  return false;
}

void ABaseWeapon::MeleeAttackHitStart(int32 _iCurrentCombo)
{
  if (m_bInCombo)
  {
    m_iCurrentCombo = _iCurrentCombo;
  }

  Attack();
}

void ABaseWeapon::MeleeAttackHitEnd()
{
}

void ABaseWeapon::UseWeaponReleaseAttack()
{
  ReleaseAttack();
}

void ABaseWeapon::ComboStateChanged(bool _bStarts)
{
  m_bInCombo = _bStarts;
  m_iCurrentCombo = -1;
}

bool ABaseWeapon::IsInCombo()
{
  return m_bInCombo;
}

int ABaseWeapon::CurrentComboHit()
{
  return m_iCurrentCombo;
}

void ABaseWeapon::UseWeaponClean()
{
  if (!m_bCanClean)
  {
    return;
  }

  Clean();

  m_bCanClean = false;

  if (IsValid(CachedWorld))
  {
    CachedWorld->GetTimerManager().SetTimer(m_oCleanRate, this, &ABaseWeapon::ResetClean, m_pWeaponDataAsset->m_fCleanCooldown * m_fCooldownMultiplier, false);
  }
}

void ABaseWeapon::UseWeaponReleaseClean()
{
  ReleaseClean();
}

void ABaseWeapon::UseWeaponSpecial()
{
  if (m_fSpecialCharge < m_pWeaponDataAsset->m_fMaxUltimateStamina)
  {
    if (m_pPlayerOwner.IsValid() && m_pPlayerDataAsset.IsValid() && m_pAudioManager.IsValid() && m_pPlayerOwner->GetCanPlayAudio()
      && IsValid(CachedWorld) && IsValid(m_pPlayerDataAsset->m_pUnavailableEvent)) 
    {
      if (UBaseGameInstance * pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
        if (pGameInstance->m_iCurrentLevel != 0)
          m_pAudioManager->PlaySound2D(CachedWorld, m_pPlayerDataAsset->m_pUnavailableEvent);
        m_pPlayerOwner->SetCanPlayAudio(false);
      }
    }
    return;
  }

  SpecialAttack();

  if (!m_bReleasSpecialAttack)
  {
    if (m_pPlayerOwner.IsValid())
    {
      m_pPlayerOwner->SetCanPlayAudio(false);
    }

    m_fSpecialCharge = 0.0f;
    m_bSpecialHasCharged = false;
  }
}

void ABaseWeapon::UseWeaponSpecialRelease()
{
  if (!m_bReleasSpecialAttack)
  {
    return;
  }

  ReleaseSpecialAttack();

  m_fSpecialCharge = 0.0f;
  m_bSpecialHasCharged = false;
  m_bReleasSpecialAttack = false;
}

void ABaseWeapon::UseWeaponSpecialHold()
{
  if (!IsValid(m_pWeaponDataAsset))
  {
    return;
  }

  if (m_fSpecialCharge < m_pWeaponDataAsset->m_fMaxUltimateStamina)
  {
    return;
  }
  HoldSpecialAttack();
  m_fSpecialCharge = 0.0f;
  m_bSpecialHasCharged = false;
}

void ABaseWeapon::SetCooldownMultiplier(float _fCooldownMultiplier)
{
  m_fCooldownMultiplier = _fCooldownMultiplier;
}

void ABaseWeapon::ResetCooldownMultiplier()
{
  m_fCooldownMultiplier = 1;
}

USkeletalMeshComponent* ABaseWeapon::GetMeshComponent() const
{
  return m_pWeaponMesh;
}

USkeletalMeshComponent* ABaseWeapon::GetSecondaryMeshComponent() const
{
  return nullptr;
}

void ABaseWeapon::ApplyFeedback(EMaterialFeedBack _feedBack, float _time)
{
  if (!IsValid(m_pWeaponMesh) || !IsValid(m_pAttackMaterial) || !IsValid(CachedWorld))
  {
    return;
  }

  FTimerDelegate DelegateTimer = FTimerDelegate::CreateUObject(this, &ABaseWeapon::DeactivateFeedback, _feedBack);

  switch (_feedBack)
  {
  case EMaterialFeedBack::ATTACKING:
    if (m_eMaterialFeedback == EMaterialFeedBack::KILLING)
    {
      return;
    }

    m_eMaterialFeedback = EMaterialFeedBack::ATTACKING;
    m_pWeaponMesh->SetMaterial(0, m_pAttackMaterial.Get());

    if (m_TimeHandle.IsValid())
    {
      CachedWorld->GetTimerManager().ClearTimer(m_TimeHandle);

      if (_time != 0)
      {
        CachedWorld->GetTimerManager().SetTimer(m_TimeHandle, DelegateTimer, _time, false);
      }
    }
    break;

  case EMaterialFeedBack::KILLING:

    if (m_TimeHandle.IsValid())
    {
      CachedWorld->GetTimerManager().ClearTimer(m_TimeHandle);
    }

    m_eMaterialFeedback = EMaterialFeedBack::KILLING;
    if (IsValid(m_pKillMaterial))
    {
      m_pWeaponMesh->SetMaterial(0, m_pKillMaterial.Get());
    }

    CachedWorld->GetTimerManager().SetTimer(m_TimeHandle, DelegateTimer, _time, false);
    break;

  case EMaterialFeedBack::NO_FEEDBACK:
  default:
    break;
  }
}

void ABaseWeapon::DeactivateFeedback(EMaterialFeedBack _feedback)
{
  if (!IsValid(m_pWeaponMesh) || !IsValid(m_pBaseMaterial))
  {
    return;
  }

  switch (m_eMaterialFeedback)
  {
  case EMaterialFeedBack::ATTACKING:
    if (_feedback != EMaterialFeedBack::ATTACKING)
    {
      return;
    }

    m_eMaterialFeedback = EMaterialFeedBack::NO_FEEDBACK;
    m_pWeaponMesh->SetMaterial(0, m_pBaseMaterial);
    break;

  case EMaterialFeedBack::KILLING:
    if (_feedback != EMaterialFeedBack::KILLING)
    {
      return;
    }
    m_eMaterialFeedback = EMaterialFeedBack::NO_FEEDBACK;
    m_pWeaponMesh->SetMaterial(0, m_pBaseMaterial);
    break;

  case EMaterialFeedBack::NO_FEEDBACK:
  default:
    break;
  }
}

void ABaseWeapon::ApplyDamage(IDamageable* _pEnemy, int32 _iDamage, EDirtType _eAttackType, bool _bCanChargeSpecial, const FVector& _vHitLocation, const FRotator& _vHitRotation)
{
  if (!_pEnemy || !IsValid(m_pWeaponDataAsset))
  {
    return;
  }

  if (ABasePlayer* player = Cast<ABasePlayer>(_pEnemy))
  { // if this cast is valid then it means we somehow got Dorothy's collider
    return; // placed here as a safety net
  }

  if (m_pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Melee && IsValid(m_pWeaponDataAsset->m_oDamageCameraShake))
  {
    SetCameraShake(m_pWeaponDataAsset->m_oDamageCameraShake);
    StartCameraShake();
    StartControllerRumble(
      m_pWeaponDataAsset->m_oDamageVibrationParams.m_fIntensity,
      m_pWeaponDataAsset->m_oDamageVibrationParams.m_fDuration,
      m_pWeaponDataAsset->m_oDamageVibrationParams.m_bAffectsLeftLarge,
      m_pWeaponDataAsset->m_oDamageVibrationParams.m_bAffectsLeftSmall,
      m_pWeaponDataAsset->m_oDamageVibrationParams.m_bAffectsRightLarge,
      m_pWeaponDataAsset->m_oDamageVibrationParams.m_bAffectsRightSmall
    );
  }

  int32 TotalDamage = _pEnemy->Damage(_iDamage, _eAttackType, _vHitLocation, _vHitRotation);
  ApplyFeedbackSound(_pEnemy);

  if (_bCanChargeSpecial)
  {
    NotifyDamage(TotalDamage);
  }

  if (ABaseCharacter* Character = Cast<ABaseCharacter>(_pEnemy))
  {
    ApplyAttackStatusEffecs(Character);
  }
}

void ABaseWeapon::ApplyDustPoopDamage(IDamageable* _pEnemy, int32 _iDamage, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _critical)
{
  if (!_pEnemy)
  {
    return;
  }

  if (ABasePlayer* player = Cast<ABasePlayer>(_pEnemy))
  { // if this cast is valid then it means we somehow got Dorothy's collider
    return; // placed here as a safety net
  }

  int32 life = _pEnemy->GetCurrentHealth();

  _pEnemy->Damage(_iDamage, _eAttackType, _vHitLocation, _vHitRotation, _critical);

  ApplyFeedbackSound(_pEnemy);

  if (_pEnemy->GetCurrentHealth() <= 0)
  {
    NotifyCleaningDust();
  }
}

EDirtType ABaseWeapon::GetDirtType() const
{
  return IsValid(m_pWeaponDataAsset) ? m_pWeaponDataAsset->m_eWeaponDirtType : EDirtType::Neutral;
}

TSoftObjectPtr<UBaseWeaponStatsDataAsset> ABaseWeapon::GetDataAsset() const
{
  return m_pWeaponDataAsset;
}

void ABaseWeapon::StartCameraShake()
{
  if (m_pPlayerOwner.IsValid() && IsValid(m_pPlayerOwner->GetCameraComponent()))
  {
    m_pPlayerOwner->GetCameraComponent()->PlayShakeComponent();
  }
}

void ABaseWeapon::StopCameraShake()
{
  if (m_pPlayerOwner.IsValid() && IsValid(m_pPlayerOwner->GetCameraComponent()))
  {
    m_pPlayerOwner->GetCameraComponent()->StopShakeComponent();
  }
}

void ABaseWeapon::SetCameraShake(TSubclassOf<UMainLegacyCameraShake> _ShakeClass)
{
  if (m_pPlayerOwner.IsValid() && IsValid(m_pPlayerOwner->GetCameraComponent()) && IsValid(_ShakeClass))
  {
    m_pPlayerOwner->GetCameraComponent()->SetShakeComponent(_ShakeClass);
  }
}

void ABaseWeapon::StartControllerRumble(float _fIntensity, float _fDuration, bool _bAffectsLeftLarge, bool _bAffectsLeftSmall, bool _bAffectsRightLarge, bool _bAffectsRightSmall)
{
  StopControllerRumble();

  if (!m_pPlayerOwner.IsValid())
  {
    return;
  }

  if (APlayerController* PC = Cast<APlayerController>(m_pPlayerOwner->GetController()))
  {
    FeedbackHandle = PC->PlayDynamicForceFeedback(
      _fIntensity,
      _fDuration,
      _bAffectsLeftLarge,
      _bAffectsLeftSmall,
      _bAffectsRightLarge,
      _bAffectsRightSmall,
      EDynamicForceFeedbackAction::Start);
  }
}

void ABaseWeapon::UpdateControllerRumble(float _fIntensity, float _fDuration, bool _bAffectsLeftLarge, bool _bAffectsLeftSmall, bool _bAffectsRightLarge, bool _bAffectsRightSmall)
{
  if (!m_pPlayerOwner.IsValid())
  {
    return;
  }

  if (APlayerController* PC = Cast<APlayerController>(m_pPlayerOwner->GetController()))
  {
    PC->PlayDynamicForceFeedback(
      _fIntensity,
      _fDuration,
      _bAffectsLeftLarge,
      _bAffectsLeftSmall,
      _bAffectsRightLarge,
      _bAffectsRightSmall,
      EDynamicForceFeedbackAction::Update,
      FeedbackHandle);
  }
}

void ABaseWeapon::StopControllerRumble()
{
  if (!m_pPlayerOwner.IsValid())
  {
    return;
  }

  if (APlayerController* PC = Cast<APlayerController>(m_pPlayerOwner->GetController()))
  {
    PC->PlayDynamicForceFeedback(
      0,
      0,
      false,
      false,
      false,
      false,
      EDynamicForceFeedbackAction::Stop,
      FeedbackHandle);
  }
}

UFMODAudioComponent* ABaseWeapon::GetCleanAudioComponent() {
  return m_pCleanAudioComponent;
}

UFMODAudioComponent* ABaseWeapon::GetSpecialAudioComponent()
{
  return m_pSpecialAudioComponent;
}

void ABaseWeapon::ResetAttack()
{
  m_bCanAttack = true;
}

void ABaseWeapon::ResetClean()
{
  m_bCanClean = true;
}

void ABaseWeapon::NotifyDamage(float _damage)
{
  if (!IsValid(m_pWeaponDataAsset))
  {
    return;
  }

  float charge = m_pWeaponDataAsset->m_fDamageUltimateCharge * _damage;
  charge = FMath::Abs(charge);
  ChargeSpecial(charge);
}

void ABaseWeapon::NotifyCleaningDust()
{
  if (!IsValid(m_pWeaponDataAsset))
  {
    return;
  }

  float charge = m_pWeaponDataAsset->m_fCleaningDustUltimateCharge;
  charge = FMath::Abs(charge);
  ChargeSpecial(charge);
}

void ABaseWeapon::NotifyCleaningLiquid(float _floorClean)
{
  if (!IsValid(m_pWeaponDataAsset))
  {
    return;
  }

  float charge = m_pWeaponDataAsset->m_fCleaningLiquidUltimateCharge * _floorClean * 100.0f;
  charge = FMath::Abs(charge);
  ChargeSpecial(charge);
}

float ABaseWeapon::GetUltimatePercentage()
{
  if (!IsValid(m_pWeaponDataAsset))
  {
    return 0.0f;
  }

  float value = m_fSpecialCharge / m_pWeaponDataAsset->m_fMaxUltimateStamina;
  value = FMath::Clamp(value, 0, 1);
  return value;
}

void ABaseWeapon::SetUltimatePercentage(float _fValue)
{
  if (_fValue <= 0)
  { // we accept only positive values here
    return;
  }

  m_fSpecialCharge = FMath::Clamp(m_fSpecialCharge - _fValue, 0.0f, 100.0f);
}

void ABaseWeapon::ApplyAttackStatusEffecs(ABaseCharacter* _character)
{
  if (!IsValid(_character))
  {
    return;
  }

  for (TSubclassOf<UBaseStatus> StatusEffect : m_pWeaponDataAsset->m_oAttackStatus)
  {
    if (!IsValid(StatusEffect))
    {
      continue;
    }

    _character->ApplyStatus(StatusEffect);
  }
}

void ABaseWeapon::ApplyCleanStatusEffects(ABaseCharacter* _character) const
{
  if (!IsValid(_character))
  {
    return;
  }

  for (TSubclassOf<UBaseStatus> StatusEffect : m_pWeaponDataAsset->m_oCleanStatus)
  {
    if (!IsValid(StatusEffect))
    {
      continue;
    }

    _character->ApplyStatus(StatusEffect);
  }
}

void ABaseWeapon::ApplySpecialStatusEffects(ABaseCharacter* _character)
{
  if (!IsValid(_character))
  {
    return;
  }

  for (TSubclassOf<UBaseStatus> StatusEffect : m_pWeaponDataAsset->m_oSpecialStatus)
  {
    if (!IsValid(StatusEffect))
    {
      continue;
    }

    _character->ApplyStatus(StatusEffect);
  }
}

float ABaseWeapon::GetAttackingCooldown() const
{
  return m_pWeaponDataAsset->m_fAttackCooldown;
}

void ABaseWeapon::ChargeSpecial(float _charge)
{
  if (!IsValid(m_pWeaponDataAsset))
  {
    return;
  }

  // THIS IS FOR HIDING THIS IN TUTORIAL (maybe we should do it in another place)
  UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance());
  if (!pGameInstance)
  {
    UE_LOG(LogTemp, Warning, TEXT("GameInstance not found"));
    return;
  }
  if (pGameInstance->m_iCurrentLevel == 0)
  {
    return;
  }
  /////////////////////////////////////////////

  float maxCharge = m_pWeaponDataAsset->m_fMaxUltimateStamina;
  m_fSpecialCharge += _charge;
  FMath::Clamp(m_fSpecialCharge, 0, maxCharge);
  if (m_fSpecialCharge >= maxCharge && !m_bSpecialHasCharged)
  {
    m_bSpecialHasCharged = true;
    OnSpecialCharge.Broadcast();
  }
}

void ABaseWeapon::SetReleaseSpecial(bool _release)
{
  m_bReleasSpecialAttack = _release;
}

bool ABaseWeapon::IsAvailableSpecialAttack()
{
  return (m_fSpecialCharge >= m_pWeaponDataAsset->m_fMaxUltimateStamina);
}
