#include "BaseCharacter.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "FMODAudioComponent.h"
#include "FMODEvent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "DorothysJob/Utils/CombatSystem/DamageCalculator.h"
#include "DorothysJob/Data/DataAssets/CharacterPropertiesDataAsset.h"
#include "DorothysJob/Animations/BaseAnimInstance.h"
#include "DorothysJob/Status/Modify Stats/ModifyStatsEffect.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AudioManager.h"
#include "DorothysJob/Status/BaseStatus.h"
#include "Enemy/DustBunny.h"

ABaseCharacter::ABaseCharacter()
{
  // Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = false;
  // Character parameters
  m_fCapsuleHalfHeigth = 105.f;
  m_fCapsuleRadius = 60.f;
  m_bCanDoActions = true;
  m_fBonusMultiplier = 1.f;
  m_fMitigation = 0.f;
  m_bIsInvulnerable = false;
  m_bBlinkActive = false;
  // Create collision component
  m_pCollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionComponent"));
  if (IsValid(m_pCollisionComponent))
  {
    m_pCollisionComponent->InitCapsuleSize(m_fCapsuleRadius, m_fCapsuleHalfHeigth);
    m_pCollisionComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
    m_pCollisionComponent->CanCharacterStepUpOn = ECB_No;
    m_pCollisionComponent->SetShouldUpdatePhysicsVolume(true);
    m_pCollisionComponent->SetCanEverAffectNavigation(false);
    m_pCollisionComponent->bDynamicObstacle = true;
    RootComponent = m_pCollisionComponent;
  }
  // Create mesh component
  m_pMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
  if (IsValid(m_pMeshComponent))
  {
    m_pMeshComponent->bOwnerNoSee = false;
    m_pMeshComponent->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPose;
    m_pMeshComponent->bCastDynamicShadow = true;
    m_pMeshComponent->bAffectDynamicIndirectLighting = true;
    m_pMeshComponent->PrimaryComponentTick.TickGroup = TG_PrePhysics;
    m_pMeshComponent->SetupAttachment(m_pCollisionComponent);
    m_pMeshComponent->SetCollisionProfileName(TEXT("CharacterMesh"));
    m_pMeshComponent->SetGenerateOverlapEvents(false);
    m_pMeshComponent->SetCanEverAffectNavigation(false);
    m_pMeshComponent->SetRelativeTransform(FTransform(FRotator::ZeroRotator, FVector(0, 0, -m_fCapsuleHalfHeigth)));
  }
  // Create movement component
  m_pMovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent0"));
  if (IsValid(m_pMovementComponent))
  {
    m_pMovementComponent->UpdatedComponent = m_pCollisionComponent;
  }
  // Create projectile spawn component
  m_pProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
  if (IsValid(m_pProjectileSpawnPoint))
  {
    m_pProjectileSpawnPoint->SetupAttachment(m_pMeshComponent);
  }
  // Audio component
  m_pAudioComponent = CreateOptionalDefaultSubobject<UFMODAudioComponent>(TEXT("AudioComponent"));
  if (m_pAudioComponent)
  {
    m_pAudioComponent->SetupAttachment(m_pCollisionComponent);
  }
  // Debug - Health component
  m_pHealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthWidgetComponent"));
  if (IsValid(m_pHealthWidgetComponent))
  {
    m_pHealthWidgetComponent->SetupAttachment(m_pCollisionComponent);
    m_pHealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    m_pHealthWidgetComponent->SetDrawSize(FVector2D(150.f, 50.f));
    m_pHealthWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
    m_pHealthWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    m_pHealthWidgetComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    m_pHealthWidgetComponent->SetCanEverAffectNavigation(false);
    m_pHealthWidgetComponent->SetGenerateOverlapEvents(false);
    m_pHealthWidgetComponent->CanCharacterStepUpOn = ECB_No;
  }
}

void ABaseCharacter::BeginPlay()
{
  Super::BeginPlay();
  // Skin
  SetBaseMaterial();
  // Animations
  if (IsValid(m_pCharacterDataAsset))
  {
    m_iCurrentHealth = m_pCharacterDataAsset->m_iMaxHealth;
    OnCharacterHealthChanged.ExecuteIfBound(m_iCurrentHealth, m_pCharacterDataAsset->m_iMaxHealth);
    if (IsValid(m_pCharacterDataAsset->m_oAnimBlueprintClass))
    {
      m_pMeshComponent->SetAnimInstanceClass(m_pCharacterDataAsset->m_oAnimBlueprintClass);
    }
  }
  // Health
  if (!m_bDamageDebug)
  {
    m_pHealthWidgetComponent->SetWidgetClass(nullptr);
  }
  else
  {
    m_iTotalDamage = 0;
    m_iDPSDamage = 0;
    GetWorldTimerManager().SetTimer(m_tTimerDPS, this, &ABaseCharacter::ResetDPS, 1.0f, true);
    m_pHealthWidgetComponent->SetWidgetClass(m_wDamageClass);
  }


}

#pragma region | Stats
int32 ABaseCharacter::GetCurrentHealth() const
{
  return m_iCurrentHealth;
}

void ABaseCharacter::SetCurrentHealth(int32 _iDamage)
{
  m_iCurrentHealth = FMath::Clamp(m_iCurrentHealth - _iDamage, 0, m_pCharacterDataAsset->m_iMaxHealth);
  OnCharacterHealthChanged.ExecuteIfBound(m_iCurrentHealth, m_pCharacterDataAsset->m_iMaxHealth);
}

int32 ABaseCharacter::Damage(int32 _iModifyHealth, EDirtType _eAttackType, const FVector& _vHitLocation, const FRotator& _vHitRotation, bool _bIsCritical)
{
  if ((m_iCurrentHealth <= 0) || m_bIsInvulnerable || (_iModifyHealth <= 0) || (m_fMitigation >= 1.f) || !IsValid(m_pCharacterDataAsset))
  {
    return 0;
  }

  int32 iTotalDamage = DamageCalculator::CalculateDamage(_iModifyHealth, m_fMitigation, _eAttackType, m_pCharacterDataAsset->m_eDirtType, m_fBonusMultiplier);
  float fEffectiveness = DamageCalculator::GetTypeDamageMultiplier(_eAttackType, m_pCharacterDataAsset->m_eDirtType);
  ShowHitImpact(_vHitLocation, _vHitRotation, fEffectiveness);
  if (m_bBlinkActive)
  {
    m_iBlinkCount = 0;
    if (IsValid(m_pCharacterDataAsset))
    {
      float fBlinkTime = m_pCharacterDataAsset->m_fInvulnerableTime / 6;
      GetWorldTimerManager().SetTimer(m_tTimerBlinkMaterial, this, &ABaseCharacter::InvulnerableBlink, fBlinkTime, true);
    }
  }
  else
  {
    SetDamagedMaterial(fEffectiveness);
  }

  if (!m_bDamageDebug)
  {
    m_iCurrentHealth = FMath::Clamp(m_iCurrentHealth - iTotalDamage, 0, m_pCharacterDataAsset->m_iMaxHealth);
    OnCharacterHealthChanged.ExecuteIfBound(m_iCurrentHealth, m_pCharacterDataAsset->m_iMaxHealth);

    if (m_iCurrentHealth <= 0)
    {
      GameOver();
    }
    else if (IsValid(m_pCharacterDataAsset))
    {
      PlayFMODEvent(m_pCharacterDataAsset->m_pDamageEvent);
    }
  }
  else
  {
    UpdateDamageWidget(iTotalDamage);
  }
  return iTotalDamage;
}

void ABaseCharacter::Heal(int32 _iModifyHealth)
{
  if ((_iModifyHealth <= 0) || !IsValid(m_pCharacterDataAsset))
  {
    return;
  }

  m_iCurrentHealth = FMath::Clamp(m_iCurrentHealth + _iModifyHealth, 0, m_pCharacterDataAsset->m_iMaxHealth);
  OnCharacterHealthChanged.ExecuteIfBound(m_iCurrentHealth, m_pCharacterDataAsset->m_iMaxHealth);
}

int32 ABaseCharacter::GetMaxHealth() const
{
  int32 iHealth = 0;

  if (IsValid(m_pCharacterDataAsset))
  {
    iHealth = m_pCharacterDataAsset->m_iMaxHealth;
  }

  return iHealth;
}

float ABaseCharacter::GetMitigation() const
{
  return m_fMitigation;
}

void ABaseCharacter::SetMitigation(float _fNewMitigation)
{
  m_fMitigation = _fNewMitigation;
}

float ABaseCharacter::GetBonusDamageMultiplier() const
{
  return m_fBonusMultiplier;
}

void ABaseCharacter::SetBonusDamageMultiplier(float _fBonusMultiplier)
{
  m_fBonusMultiplier = _fBonusMultiplier;
}
#pragma endregion

#pragma region | Status Effect
void ABaseCharacter::BePolluted()
{
  if (!m_bIsInvulnerable && IsValid(m_pCharacterDataAsset) && IsValid(m_pCharacterDataAsset->m_pPollutedStatus))
  {
    UModifyStatsEffect* pModifyStats = NewObject<UModifyStatsEffect>(this, m_pCharacterDataAsset->m_pPollutedStatus);
    if (IsValid(pModifyStats))
    {
      pModifyStats->ApplyStatus(this);
    }
  }
}

void ABaseCharacter::ApplyStatus(TSubclassOf<UBaseStatus> _oStatus)
{
  if (m_bIsInvulnerable || !_oStatus)
  {
    return;
  }

  if (UBaseStatus** Found = m_mActiveStatus.Find(_oStatus))
  {
    if ((*Found)->IsReactivable())
    {
      (*Found)->ApplyStatus(this);
    }
    return;
  }

  UBaseStatus* pNewStatus = NewObject<UBaseStatus>(this, _oStatus);
  if (IsValid(pNewStatus))
  {
    OnStateChanged.Broadcast(pNewStatus, true);
    pNewStatus->ApplyStatus(this);
    m_mActiveStatus.Add(_oStatus, pNewStatus);
  }
}

void ABaseCharacter::RemoveStatus(TSubclassOf<UBaseStatus> _oStatus)
{
  if (m_mActiveStatus.Contains(_oStatus))
  {
    if (UBaseStatus* pbaseStatus = *m_mActiveStatus.Find(_oStatus)) 
    {
      ADustBunny* db = Cast<ADustBunny>(this);
      if (db && pbaseStatus->GetStatusType() == EStatus::STUN)
      {
        db->m_bStunnedByDustPoop = false;
      }
      OnStateChanged.Broadcast(pbaseStatus, false);
    }
   
    m_mActiveStatus.Remove(_oStatus);
  }
}

void ABaseCharacter::SpeedUp()
{
  if (IsValid(m_pCharacterDataAsset) && IsValid(m_pCharacterDataAsset->m_oStatusEffectClass))
  {
    ApplyStatus(m_pCharacterDataAsset->m_oStatusEffectClass);
  }
}

bool ABaseCharacter::CanDoActions() const
{
  return m_bCanDoActions;
}

void ABaseCharacter::SetCanDoActions(bool _bCanDoActions)
{
  if (!m_bIsInvulnerable)
  {
    m_bCanDoActions = _bCanDoActions;
  }
}

bool ABaseCharacter::IsInvulnerable()
{
  return m_bIsInvulnerable;
}

void ABaseCharacter::SetIsInvulnerable(bool _bIsInvulnerable)
{
  m_bIsInvulnerable = _bIsInvulnerable;

  if (m_bBlinkActive && (!m_bIsInvulnerable) && GetWorldTimerManager().IsTimerActive(m_tTimerBlinkMaterial))
  {
    GetWorldTimerManager().ClearTimer(m_tTimerBlinkMaterial);
    SetBaseMaterial();
  }
}

void ABaseCharacter::ClearAllStatus()
{
  TArray<TSubclassOf<UBaseStatus>> Keys;
  m_mActiveStatus.GetKeys(Keys);

  for (auto& Key : Keys)
  {
    if (UBaseStatus* Status = m_mActiveStatus[Key])
    {
      Status->RemoveStatus();
    }
  }
}
#pragma endregion

#pragma region | Getters
UCharacterPropertiesDataAsset* ABaseCharacter::GetDataAsset() const
{
  return m_pCharacterDataAsset;
}

USkeletalMeshComponent* ABaseCharacter::GetSkeletalMesh() const
{
  return m_pMeshComponent;
}

UPawnMovementComponent* ABaseCharacter::GetMovementComponent() const
{
  return m_pMovementComponent;
}

USceneComponent* ABaseCharacter::GetMuzzle() const
{
  return m_pProjectileSpawnPoint;
}
#pragma endregion

#pragma region | Current Room
UBaseRoom* ABaseCharacter::GetCurrentRoom()
{
  return m_pCurrentRoom;
}

void ABaseCharacter::SetCurrentRoom(UBaseRoom* _pNewCurrentRoom)
{
  m_pCurrentRoom = _pNewCurrentRoom;
}
#pragma endregion


#pragma region | Game Over
void ABaseCharacter::GameOver()
{
  if (IsValid(m_pCharacterDataAsset))
  {
    if (IsValid(m_pCharacterDataAsset->m_pDeadEffect))
    {
      UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pCharacterDataAsset->m_pDeadEffect, GetActorLocation());
    }

    // Play Audio
    if (IsValid(m_pCharacterDataAsset->m_pDeathEvent))
    {
      PlayFMODEvent(m_pCharacterDataAsset->m_pDeathEvent);
    }
  }
}
#pragma endregion

#pragma region | Skin
void ABaseCharacter::SetBaseMaterial()
{
  if (!IsValid(m_pCharacterDataAsset) || !IsValid(m_pMeshComponent))
  {
    return;
  }

  for (int32 iMaterialIndex = 0; iMaterialIndex < m_pCharacterDataAsset->m_lBaseMaterial.Num(); ++iMaterialIndex)
  {
    m_pMeshComponent->SetMaterial(iMaterialIndex, m_pCharacterDataAsset->m_lBaseMaterial[iMaterialIndex]);
  }
}

void ABaseCharacter::SetDamagedMaterial(float _fEffectiveness)
{
  if (!IsValid(m_pCharacterDataAsset) || !IsValid(m_pMeshComponent))
  {
    return;
  }

  // It selects Damaged Skin according to effectiveness
  TArray<TObjectPtr<UMaterialInterface>> lSelectedSkin = (_fEffectiveness > 1.f) ? m_pCharacterDataAsset->m_lCriticalHitMaterial :
                                                         (_fEffectiveness < 1.f) ? m_pCharacterDataAsset->m_lReducedHitMaterial :
                                                          m_pCharacterDataAsset->m_lDamagedMaterial; // _fEffectiveness == 1.f

  for (int32 iMaterialIndex = 0; iMaterialIndex < lSelectedSkin.Num(); ++iMaterialIndex)
  {
    m_pMeshComponent->SetMaterial(iMaterialIndex, lSelectedSkin[iMaterialIndex]);
  }

  if (GetWorldTimerManager().IsTimerActive(m_tTimerResetMaterial))
  {
    GetWorldTimerManager().ClearTimer(m_tTimerResetMaterial);
  }

  GetWorldTimerManager().SetTimer(m_tTimerResetMaterial, this, &ABaseCharacter::SetBaseMaterial, 0.05f, false);
}
#pragma endregion

#pragma region | Audio
void ABaseCharacter::PlayFMODEvent(TObjectPtr<UFMODEvent> _pFMODEvent)
{
  if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetGameInstance()))
  {
    if (UAudioManager* pAudioManager = pGameInstance->GetSubsystem<UAudioManager>())
    {
      if (IsValid(m_pCharacterDataAsset))
      {
        // Play Event with Pitch parameters.
        pAudioManager->PlayEventWithParameters(m_pAudioComponent, _pFMODEvent, { { m_pCharacterDataAsset->m_sCustomPitchTag, m_pCharacterDataAsset->m_fCustomPitchValue } });
      }
    }
  }
}
#pragma endregion

#pragma region | Feedback
void ABaseCharacter::ShowHitImpact(const FVector& _vHitLocation, const FRotator& _vHitRotation, float _fEffectiveness)
{
  if (!IsValid(m_pCharacterDataAsset) || !IsValid(m_pCharacterDataAsset->m_pDamagedEffect))
  {
    return;
  }

  UNiagaraComponent* pEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), m_pCharacterDataAsset->m_pDamagedEffect, _vHitLocation, _vHitRotation);
  if (IsValid(pEffectComponent))
  {
    // Reduced Hit 0.f, Regular Hit 1.f
    pEffectComponent->SetVariableFloat(FName("Spawn Impact"), (_fEffectiveness < 1.f) ? 0.f : 20.f);
    // Critical Hit #FF2900, Regular Hit #FF4500
    FColor tColor = FColor::FromHex((_fEffectiveness > 1.f) ? TEXT("#FF2900") : TEXT("#FF4500"));
    FLinearColor tLinearColor = FLinearColor::FromSRGBColor(tColor);
    pEffectComponent->SetVariableLinearColor(FName("Impact Color"), tLinearColor);
  }
}

void ABaseCharacter::InvulnerableBlink()
{
  if (((m_iBlinkCount % 2) == 0) && IsValid(m_pCharacterDataAsset) && IsValid(m_pMeshComponent))
  {
    TArray<TObjectPtr<UMaterialInterface>> lSelectedSkin = m_pCharacterDataAsset->m_lDamagedMaterial;
    for (int32 iMaterialIndex = 0; iMaterialIndex < lSelectedSkin.Num(); ++iMaterialIndex)
    {
      m_pMeshComponent->SetMaterial(iMaterialIndex, lSelectedSkin[iMaterialIndex]);
    }
  }
  else
  {
    SetBaseMaterial();
  }
  ++m_iBlinkCount;
}
#pragma endregion

#pragma region | Debug Mode
void ABaseCharacter::UpdateDamageWidget(int32 _iDamageValue)
{
  if (IsValid(m_pHealthWidgetComponent))
  {
    UUserWidget* pDamageWidget = m_pHealthWidgetComponent->GetUserWidgetObject();
    if (IsValid(pDamageWidget))
    {
      UTextBlock* pLastHit = Cast<UTextBlock>(pDamageWidget->GetWidgetFromName(TEXT("sLastHitValue")));
      if (IsValid(pLastHit))
      {
        pLastHit->SetText(FText::FromString(FString::FromInt(_iDamageValue)));
      }
      UTextBlock* pDPS = Cast<UTextBlock>(pDamageWidget->GetWidgetFromName(TEXT("sDPSValue")));
      if (IsValid(pDPS))
      {
        m_iDPSDamage += _iDamageValue;
        pDPS->SetText(FText::FromString(FString::FromInt(m_iDPSDamage)));
      }
      UTextBlock* pTotal = Cast<UTextBlock>(pDamageWidget->GetWidgetFromName(TEXT("sTotalValue")));
      if (IsValid(pTotal))
      {
        m_iTotalDamage += _iDamageValue;
        pTotal->SetText(FText::FromString(FString::FromInt(m_iTotalDamage)));
      }
    }
  }
}

void ABaseCharacter::ResetDPS()
{
  m_iDPSDamage = 0;
}
#pragma endregion