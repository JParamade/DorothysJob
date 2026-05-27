#include "Dusty.h"
#include "DorothysJob/Component/Weapon/Dusty/DustyTradicionalHandle.h"
#include "DorothysJob/Component/Weapon/Dusty/DustyHummingbirdFeathers.h"
#include "DorothysJob/Component/Weapon/Dusty/SpecialAttackDustyComponent.h"
#include "DorothysJob/Data/DataAssets/Weapon/DustyWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Interface/Damageable.h"
#include "DorothysJob/Component/Weapon/Dusty/DustyWeaponAudioComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "FMODAudioComponent.h"


ADusty::ADusty()
{
  m_pHandle = CreateDefaultSubobject<UDustyTradicionalHandle>(TEXT("Hadle"));
  m_pFeathers = CreateDefaultSubobject<UDustyHummingbirdFeathers>(TEXT("Feathers"));
  m_pSpecialAttack = CreateDefaultSubobject<USpecialAttackDustyComponent>(TEXT("Special Attack Component"));
  m_pWeaponAudioComponent = CreateDefaultSubobject<UDustyWeaponAudioComponent>(TEXT("Weapon Audio Component"));
}

void ADusty::BeginPlay()
{
  Super::BeginPlay();

  pEffectComponentCleaningDust = nullptr;
  m_pOwnerPlayer = Cast<ABasePlayer>(GetOwner());
  m_pDataAsset = Cast<UDustyWeaponStatsDataAsset>(GetDataAsset().Get());
}

void ADusty::ComboStateChanged(bool _bStarts)
{
  Super::ComboStateChanged(_bStarts);

  if (_bStarts)
  {
    if (m_pOwnerPlayer.IsValid())
    {
      m_pOwnerPlayer->ChangeMovementVelocity(kDashComboVelocityScale);
    }
    auxDashCombo = true;
  }
  else
  {
    if (auxDashCombo)
    {
      if (m_pOwnerPlayer.IsValid())
      {
        m_pOwnerPlayer->ChangeMovementVelocity(kDashComboVelocityScaleInversed);
      }
      auxDashCombo = false;
    }
  }
}

void ADusty::MeleeAttackHitEnd()
{
  Super::MeleeAttackHitEnd();

  if (IsValid(m_pHandle))
  {
    m_pHandle->AttackFinish();
  }
}

void ADusty::Attack()
{
  if (IsValid(m_pHandle))
  {
    m_pHandle->Use();
  }
}

void ADusty::ReleaseAttack()
{
  if (IsValid(m_pHandle))
  {
    m_pHandle->Release();

    if (auxDashCombo && m_pOwnerPlayer.IsValid())
    {
      m_pOwnerPlayer->ChangeMovementVelocity(kDashComboVelocityScaleInversed);
      auxDashCombo = false;
    }
  }
}

void ADusty::SpecialAttack()
{
  if (IsValid(m_pSpecialAttack))
  {
    OnSpecialActive.Broadcast(true);
    m_pSpecialAttack->Use();
  }
}

void ADusty::Clean()
{
  if (IsValid(m_pDataAsset) && m_pOwnerPlayer.IsValid())
  {
    if (!isCleaningVelocity)
    {
      m_pOwnerPlayer->ChangeMovementVelocity(m_pDataAsset->m_iVelocityClean, m_pDataAsset->m_iVelocityRotationClean);
      isCleaningVelocity = true;
    }
  }

  if (IsValid(m_pFeathers))
  {
    m_pFeathers->Use();
  }

  ShowVFXCleanig();
}

void ADusty::ReleaseClean()
{
  if (IsValid(m_pFeathers) && m_pOwnerPlayer.IsValid())
  {
    if (IsValid(m_pDataAsset) && isCleaningVelocity)
    {
      m_pOwnerPlayer->ChangeMovementVelocity(1.0f / m_pDataAsset->m_iVelocityClean, 1.0f / m_pDataAsset->m_iVelocityRotationClean);
      isCleaningVelocity = false;
    }

    m_pFeathers->Release();

    if (IsValid(pEffectComponentCleaningDust))
    {
      pEffectComponentCleaningDust->Deactivate();
    }

    if (IsValid(pEffectComponentCleaningCircle))
    {
      pEffectComponentCleaningCircle->DeactivateImmediate();
    }
  }
}

void ADusty::ShowVFXCleanig()
{
  if (!IsValid(pEffectComponentCleaningDust))
  {
    if (IsValid(m_pDataAsset) && IsValid(m_pDataAsset->m_pCleanEffect))
    {
      pEffectComponentCleaningDust = UNiagaraFunctionLibrary::SpawnSystemAttached(
        m_pDataAsset->m_pCleanEffect,
        GetRootComponent(),
        NAME_None,
        FVector(0, 0, 100),
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        true);
    }
  }

  if (!IsValid(pEffectComponentCleaningCircle))
  {
    if (IsValid(m_pDataAsset) && IsValid(m_pDataAsset->m_pCleanEffectCircle) && m_pOwnerPlayer.IsValid() && IsValid(m_pOwnerPlayer->GetMuzzle()))
    {
      pEffectComponentCleaningCircle = UNiagaraFunctionLibrary::SpawnSystemAttached(
        m_pDataAsset->m_pCleanEffectCircle,
        m_pOwnerPlayer->GetMuzzle(),
        NAME_None,
        FVector(90, 0, -100),
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        true);
    }
  }

  if (IsValid(pEffectComponentCleaningDust) && !pEffectComponentCleaningDust->IsActive())
  {
    pEffectComponentCleaningDust->ReinitializeSystem();
  }

  if (IsValid(pEffectComponentCleaningCircle) && !pEffectComponentCleaningCircle->IsActive())
  {
    pEffectComponentCleaningCircle->ReinitializeSystem();
  }
}