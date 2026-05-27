#include "PlayerAnimInstance.h"
#include <Kismet/KismetMathLibrary.h>
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/BaseWeapon.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Utils/GeneralUtils.h"
#include "DorothysJob/Animations/WeaponAnimInstance.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
  Super::NativeInitializeAnimation();

  APawn* pOwner = TryGetPawnOwner();
  if (IsValid(pOwner))
  {
    m_pPlayer = Cast<ABasePlayer>(pOwner);
    if (m_pPlayer.IsValid())
    {
      m_pPlayer->OnWeaponChange.AddUObject(this, &UPlayerAnimInstance::UpdateWeaponAnimations);
    }
  }
}

void UPlayerAnimInstance::NativeUpdateAnimation(float _fDeltaSeconds)
{
  Super::NativeUpdateAnimation(_fDeltaSeconds);

  if (!m_pPlayer.IsValid())
  {
    return;
  }

  USkeletalMeshComponent* pPlayerMesh = m_pPlayer->GetSkeletalMesh();
  FVector vVelocity = m_pPlayer->GetVelocity();
  FVector vForward = pPlayerMesh->GetForwardVector();
  m_fSpeed = FVector::DotProduct(vVelocity.GetSafeNormal(), vForward) * vVelocity.Length();
  FVector vRight = pPlayerMesh->GetRightVector();
  m_fLateralSpeed = FVector::DotProduct(vVelocity.GetSafeNormal(), vRight) * vVelocity.Length();
  m_fBlendWeigth = m_bMontagePlaying && GeneralUtils::IsDusty(m_pWeaponDataAsset) && (FName("Special") == Montage_GetCurrentSection(m_pActionMontage)) ? 0.f : 1.f;

  // Update weapon animation
  if (m_pCurrentWeaponAnimInstance->IsValidLowLevel())
  {
    m_pCurrentWeaponAnimInstance->SetSpeed(m_fSpeed, m_fLateralSpeed);
    if (GeneralUtils::IsDirtminator(m_pWeaponDataAsset))
    {
      FVector vLeftHandSocket = pPlayerMesh->GetSocketLocation(FName("WeaponSocketL"));
      FVector HandLocationLocal = m_pCurrentWeaponMesh->GetComponentTransform().InverseTransformPosition(vLeftHandSocket);
      m_pCurrentWeaponAnimInstance->SetLeftHandSocketPosition(HandLocationLocal);
    }
  }
}

bool UPlayerAnimInstance::PlayActionMontage(FName _sSectionName, float _fPlayRate)
{
  if (m_bMontagePlaying)
  {
    return false;
  }
  
  m_bMontagePlaying = Super::PlayActionMontage(_sSectionName, _fPlayRate);
  
  if (m_bMontagePlaying && m_pCurrentWeaponAnimInstance.IsValid())
  {
    m_pCurrentWeaponAnimInstance->PlayActionMontage(_sSectionName, _fPlayRate);
  }

  return m_bMontagePlaying;
}

void UPlayerAnimInstance::StopActionMontage()
{
  Super::StopActionMontage();

  if (m_pCurrentWeaponAnimInstance.IsValid())
  {
    m_pCurrentWeaponAnimInstance->StopActionMontage();
  }
}

bool UPlayerAnimInstance::JumpSectionActionMontage(FName _sSectionName)
{
  if ((!IsValid(m_pActionMontage)) || (!m_pActionMontage->IsValidSectionName(_sSectionName)))
  {
    return false;
  }
  
  if (m_bMontagePlaying && m_pActionMontage->IsValidSectionName(_sSectionName))
  {
    Montage_JumpToSection(_sSectionName, m_pActionMontage);
  }

  if (m_bMontagePlaying && m_pCurrentWeaponAnimInstance.IsValid())
  {
    m_pCurrentWeaponAnimInstance->PlayActionMontage(_sSectionName);
  }

  return m_bMontagePlaying;
}

void UPlayerAnimInstance::OnActionMontageEnded(UAnimMontage* _pMontage, bool _bInterrupted)
{
  Super::OnActionMontageEnded(_pMontage, _bInterrupted);
  m_bMontagePlaying = false;
}

FName UPlayerAnimInstance::JumptoNextSection()
{
  FName sNextSectionName = Super::JumptoNextSection();

  if (m_bMontagePlaying && m_pCurrentWeaponAnimInstance.IsValid())
  {
    m_pCurrentWeaponAnimInstance->PlayActionMontage(sNextSectionName);
  }

  return sNextSectionName;
}

void UPlayerAnimInstance::UpdateWeaponAnimations(EDirtType _eAttackType, EWeaponAttackType _eWeaponAttackType)
{
  StopActionMontage();

  if ((!m_pPlayer.IsValid()) || (!IsValid(m_pPlayer->GetCurrentWeapon())))
  {
    return;
  }

  m_pWeaponDataAsset = m_pPlayer->GetCurrentWeapon()->GetDataAsset();
  if (m_pWeaponDataAsset.IsValid())
  {
    m_pMovementBlendSpace = m_pWeaponDataAsset->m_pMovementBlendSpace;
    m_pActionMontage = m_pWeaponDataAsset->m_pAttackMontage;
  }

  m_pCurrentWeaponMesh = m_pPlayer->GetCurrentWeapon()->GetMeshComponent();
  if (m_pCurrentWeaponMesh.IsValid())
  {
    m_pCurrentWeaponAnimInstance = Cast<UWeaponAnimInstance>(m_pCurrentWeaponMesh->GetAnimInstance());
  }
}