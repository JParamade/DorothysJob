
#include "Dirtminator.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Component/Weapon/Dirtminator/RocketLauncherComponent.h"
#include "DorothysJob/Component/Weapon/Dirtminator/VectorVacuumingComponent.h"
#include "DorothysJob/Component/Weapon/Dirtminator/DirtminatorSAComponent.h"
#include "DorothysJob/Component/Weapon/Dirtminator/DirtminatorWeaponAudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

ADirtminator::ADirtminator()
{
  m_pRocketLauncherComponent = CreateDefaultSubobject<URocketLauncherComponent>(TEXT("EngineComponent"));
  m_pVacumCleanComponent = CreateDefaultSubobject<UVectorVacuumingComponent>(TEXT("HoseComponent"));
  m_pSpecialAttackComponent = CreateDefaultSubobject<UDirtminatorSAComponent>(TEXT("SpecialAttackComponent"));
  m_pWeaponAudioComponent = CreateDefaultSubobject<UDirtminatorWeaponAudioComponent>(TEXT("Weapon audio component"));

  m_pChargeVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ChargeVFX"));
  if (IsValid(m_pChargeVFX))
  {
    m_pChargeVFX->SetupAttachment(RootComponent);
    m_pChargeVFX->SetAutoActivate(false);
  }

  m_pAimVFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AimVFX"));
  if (IsValid(m_pAimVFX))
  {
    m_pAimVFX->SetupAttachment(RootComponent);
    m_pAimVFX->SetAutoActivate(false);
  }
}

void ADirtminator::BeginPlay()
{
  Super::BeginPlay();

  m_pOwnerPlayer = Cast<ABasePlayer>(GetOwner());
}

#pragma region Base Actions
void ADirtminator::Attack()
{
  if (IsValid(m_pRocketLauncherComponent) && !m_pSpecialAttackComponent->GetSpeciaAttackIsActive())
  {
    m_pRocketLauncherComponent->Use();
  }
}

void ADirtminator::ReleaseAttack()
{
  if (IsValid(m_pRocketLauncherComponent) && !m_pSpecialAttackComponent->GetSpeciaAttackIsActive())
  {
    m_pRocketLauncherComponent->Release();
  }
}

void ADirtminator::Clean()
{
  if (IsValid(m_pVacumCleanComponent) && !m_pSpecialAttackComponent->GetSpeciaAttackIsActive())
  {
    m_pVacumCleanComponent->Use();
  }
}

void ADirtminator::ReleaseClean()
{
  if (IsValid(m_pVacumCleanComponent) && !m_pSpecialAttackComponent->GetSpeciaAttackIsActive())
  {
    m_pVacumCleanComponent->Release();
  }
}

void ADirtminator::SpecialAttack()
{
  if (IsValid(m_pSpecialAttackComponent))
  {
    m_pSpecialAttackComponent->InitSpecialAttack();
  }
}
#pragma endregion

#pragma region Getters & Setters
void ADirtminator::SetChargeVFX(UNiagaraSystem* _pNewVFX)
{
  if (!IsValid(m_pChargeVFX))
  {
    return;
  }

  if (IsValid(_pNewVFX))
  {
    m_pChargeVFX->SetAsset(_pNewVFX);
    m_pChargeVFX->Activate();
    return;
  }

  m_pChargeVFX->SetAsset(_pNewVFX);
  m_pChargeVFX->Deactivate();
}

void ADirtminator::SetActiveAimVFX(bool _bIsActive, UNiagaraSystem* _NiagaraSystem)
{
  if (!m_pOwnerPlayer.IsValid())
  {
    return;
  }

  USkeletalMeshComponent* PlayerSkeletalMesh = m_pOwnerPlayer->GetSkeletalMesh();
  if (!IsValid(PlayerSkeletalMesh) || !IsValid(m_pAimVFX))
  {
    return;
  }

  if (IsValid(m_pAimVFX->GetAttachParent()) && m_pAimVFX->GetAttachParent() != PlayerSkeletalMesh)
  {
    m_pAimVFX->AttachToComponent(PlayerSkeletalMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
  }

  if (!IsValid(_NiagaraSystem))
  {
    return;
  }

  if (_bIsActive)
  {
    m_pAimVFX->SetAsset(_NiagaraSystem);
    m_pAimVFX->Activate();
    return;
  }

  m_pAimVFX->SetAsset(nullptr);
  m_pAimVFX->Deactivate();
}

void ADirtminator::SetCooldownMultiplier(float _fCooldownMultiplier)
{
  Super::SetCooldownMultiplier(_fCooldownMultiplier);
  if (IsValid(m_pRocketLauncherComponent))
  {
    m_pRocketLauncherComponent->SetCooldownMultiplier(_fCooldownMultiplier);
  }
}

void ADirtminator::ResetCooldownMultiplier()
{
  Super::ResetCooldownMultiplier();
  if (IsValid(m_pRocketLauncherComponent))
  {
    m_pRocketLauncherComponent->SetCooldownMultiplier(1.0f);
  }
}
#pragma endregion