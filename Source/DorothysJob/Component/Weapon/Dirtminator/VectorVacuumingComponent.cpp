#include "VectorVacuumingComponent.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Weapon/Dirtminator/Dirtminator.h"
#include "DorothysJob/Data/DataAssets/Weapon/DirtminatorWeaponStatsDataAsset.h"
#include "DorothysJob/Actor/Dust/DustPoop.h"
#include "DorothysJob/Actor/Camera/MainLegacyCameraShake.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"											
#include "Engine/OverlapResult.h"
#include "Kismet/KismetSystemLibrary.h"

UVectorVacuumingComponent::UVectorVacuumingComponent() :
  m_fCurrentLength(100.f),
  m_bShowDebug(true),
  m_bIsVacuuming(false)
{
}

void UVectorVacuumingComponent::BeginPlay()
{
  Super::BeginPlay();

  m_pWhirlwindComponent = nullptr;

  if (IsValid(m_pWhirlwindtMaterial))
  {
    m_pDynamicMaterial = UMaterialInstanceDynamic::Create(m_pWhirlwindtMaterial, this);
  }

  m_pOwnerWeapon = Cast<ABaseWeapon>(GetOwner());

  if (m_pOwnerWeapon.IsValid())
  {
    m_pOwnerPlayer = Cast<ABasePlayer>(m_pOwnerWeapon->GetOwner());

    if (m_pOwnerPlayer.IsValid())
    {
      m_pMuzzle = m_pOwnerPlayer->GetMuzzle();
      m_pPlayerSkeletalMesh = m_pOwnerPlayer->GetSkeletalMesh();

      m_lActorsToIgnore.Add(m_pOwnerPlayer.Get());
      m_lActorsToIgnore.Add(m_pOwnerWeapon.Get());
      m_lActorsToIgnore.RemoveAll([](AActor* Actor) { return !IsValid(Actor); }); // deleting nullptr just in case
    }

    if (m_pOwnerWeapon->GetDataAsset().IsValid())
    {
      m_pDataAsset = Cast<UDirtminatorWeaponStatsDataAsset>(m_pOwnerWeapon->GetDataAsset().Get());
    }
  }

  if (IsValid(m_pDataAsset))
  {
    m_fCurrentLength = m_pDataAsset->m_fMinCleanLength;
  }
}

void UVectorVacuumingComponent::Use()
{
  if (!m_pOwnerWeapon.IsValid() || !IsValid(m_pDataAsset))
  {
    return;
  }

  if (!m_bIsVacuuming)
  {
    SetPlayerVelocity(true);

    if (IsValid(m_pDataAsset->m_oCleanShakeBP))
    {
      m_pOwnerWeapon->SetCameraShake(m_pDataAsset->m_oCleanShakeBP);
    }

    FGamePadVibrationParams oVibrationParams = m_pDataAsset->m_oCleanVibrationParams;
    m_pOwnerWeapon->StartControllerRumble(
      oVibrationParams.m_fIntensity,
      oVibrationParams.m_fDuration,
      oVibrationParams.m_bAffectsLeftLarge,
      oVibrationParams.m_bAffectsLeftSmall,
      oVibrationParams.m_bAffectsRightLarge,
      oVibrationParams.m_bAffectsRightSmall);
    m_pOwnerWeapon->OnCleanStart.Broadcast();
  }

  m_bIsVacuuming = true;

  m_pOwnerWeapon->StartCameraShake();

  if (!m_bStopLength)
  {
    m_fCurrentLength = FMath::Clamp(m_fCurrentLength + m_pDataAsset->m_fStepLength, 0.0f, m_pDataAsset->m_fMaxCleanLength);
  }

  StartCleaningVFX();
  UpdateCleaningVFX();

  TArray<FHitResult> lHitDust;
  if (!DetectCleanCollision(lHitDust))
  {
    m_bStopLength = false;
    m_pWhirlwindComponent->SetVariableFloat(TEXT("User.DustCleaning"), 0.0f);
    return;
  }

  int32 iDamage = m_pDataAsset->m_iCleanDamage;
  for (const FHitResult& Result : lHitDust)
  {
    AActor* HitActor = Result.GetActor();
    m_pWhirlwindComponent->SetVariableFloat(TEXT("User.DustCleaning"), 10.0f);
    if (!IsValid(HitActor))
    {
      continue;
    }
    
    IDamageable* pDamageable = Cast<IDamageable>(HitActor);
    if (!pDamageable)
    {
      continue;
    }

    m_pOwnerWeapon->ApplyDustPoopDamage(pDamageable, iDamage, m_pDataAsset->m_eWeaponDirtType);
  }
}

void UVectorVacuumingComponent::Release()
{
  if (!m_pOwnerWeapon.IsValid())
  {
    return;
  }

  m_pOwnerWeapon->StopCameraShake();
  m_pOwnerWeapon->StopControllerRumble();
  m_pOwnerWeapon->OnCleanEnd.Broadcast();

  m_fCurrentLength = IsValid(m_pDataAsset) ? m_pDataAsset->m_fMinCleanLength : 0.0f;

  if (m_bIsVacuuming)
  {
    SetPlayerVelocity(false);
  }

  m_bIsVacuuming = false;

  StopCleaningVFX();
}


bool UVectorVacuumingComponent::DetectCleanCollision(TArray<FHitResult>& _lOutDust)
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset) || !IsValid(m_pMuzzle) || !IsValid(m_pPlayerSkeletalMesh))
  {
    return false;
  }
  
  _lOutDust.Empty();

  FRotator MeshRotation = m_pPlayerSkeletalMesh->GetComponentRotation();
  FVector Forward = m_pPlayerSkeletalMesh->GetForwardVector();

  FVector Right = m_pPlayerSkeletalMesh->GetRightVector();
  FVector Up = m_pPlayerSkeletalMesh->GetUpVector();

  FVector LocalOffset = m_pDataAsset->m_vCleaningOffset;

  FVector WorldOffset = Forward * LocalOffset.X
    + Right * LocalOffset.Y
    + Up * LocalOffset.Z;

  FVector Start = m_pMuzzle->GetComponentLocation() + WorldOffset;
  FVector End = Start + Forward * m_fCurrentLength;

  UWorld* CachedWorld = GetWorld();
  if (!IsValid(CachedWorld))
  {
    return false;
  }

  FHitResult oHit;
  if (CachedWorld->LineTraceSingleByObjectType(
    oHit,
    Start,
    End,
    m_lObjectQueryParamsRaycast
  ))
  {
    float fDistanceToWall = FVector::Distance(oHit.Location, Start);
    if (fDistanceToWall < m_fCurrentLength)
    {
      float offset = m_pDataAsset->m_fCleanCollisionOffset;
      m_fCurrentLength = fDistanceToWall + offset;
    }
  }

  FVector Range = Start + Forward * m_fCurrentLength;

  if (m_bShowDebug)
  {
    DrawDebugLine(CachedWorld, Start, End, FColor::Red, false, 0.1f);
  }

  FVector Size = FVector(m_pDataAsset->m_fCleanAttackWidth, m_pDataAsset->m_fCleanAttackWidth, m_pDataAsset->m_fCleanAttackWidth);

  bool bHit_ = UKismetSystemLibrary::BoxTraceMultiForObjects(
    CachedWorld,
    Start,
    Range,
    Size,
    MeshRotation,
    m_lObjectQueryParamsClean,
    false,
    m_lActorsToIgnore,
    m_bShowDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
    _lOutDust,
    true,
    FLinearColor::Red,
    FLinearColor::Green,
    0.1f
  );

  return bHit_;
}

void UVectorVacuumingComponent::StartCleaningVFX()
{
  if (!IsValid(m_pWhirlwindComponent) || !IsValid(m_pDataAsset))
  {
    if (m_pOwnerPlayer.IsValid() && IsValid(m_pWhirlwindEffect) && IsValid(m_pOwnerPlayer->GetMuzzle()))
    {
      m_pWhirlwindComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
        m_pWhirlwindEffect,
        m_pOwnerPlayer->GetMuzzle(),
        NAME_None,
        m_pDataAsset->m_vCleaningOffset,
        FRotator::ZeroRotator,
        EAttachLocation::KeepRelativeOffset,
        false,
        false);

      m_pWhirlwindComponent->SetVariableFloat(TEXT("User.DustCleaning"), 0.0f);
    }
  }

  if (IsValid(m_pWhirlwindComponent) && !m_pWhirlwindComponent->IsActive())
  {
    m_pWhirlwindComponent->ReinitializeSystem();
  }
}

void UVectorVacuumingComponent::UpdateCleaningVFX()
{
  if (!IsValid(m_pWhirlwindComponent) || !IsValid(m_pWhirlwindtMaterial) || !IsValid(m_pDataAsset))
  {
    return;
  }

  float fMinLenght = m_pDataAsset->m_fMinCleanLength;
  float fMaxLenght = m_pDataAsset->m_fMaxCleanLength;
  float fGrowthLerp = 0.0f;
  if (!FMath::IsNearlyEqual(fMaxLenght, fMinLenght))
  {
    fGrowthLerp = (m_fCurrentLength - fMinLenght) / (fMaxLenght - fMinLenght);
  }

  fGrowthLerp = FMath::Clamp(fGrowthLerp, 0.3f, 1.0f);

  float fScaledValue = fGrowthLerp * 1.6f;

  float fMinLifetime = 0.0f;
  float fMaxLifetime = 0.5f;
  float fLeadLifetime = FMath::Lerp(fMinLifetime, fMaxLifetime, fGrowthLerp);

  float fMinOffset = 0.0f;
  float fMaxOffset = 1000.0f;
  float fPositionOffset = FMath::Lerp(fMinOffset, fMaxOffset, fGrowthLerp);

  FVector vPositionOffset = FVector(fPositionOffset, 0.0f, 0.0f);

  if (IsValid(m_pDynamicMaterial) && IsValid(m_pWhirlwindComponent))
  {
    m_pDynamicMaterial->SetScalarParameterValue(FName("BlendProgres"), fScaledValue);
    m_pWhirlwindComponent->SetVariableMaterial(TEXT("ModulableMaterial"), m_pDynamicMaterial);
    m_pWhirlwindComponent->SetVariableFloat(TEXT("LeadLifetime"), fLeadLifetime);
    m_pWhirlwindComponent->SetVariableVec3(TEXT("SetPositionOffset"), vPositionOffset);
  }
}

void UVectorVacuumingComponent::StopCleaningVFX()
{
  if (IsValid(m_pWhirlwindComponent))
  {
    m_pWhirlwindComponent->DeactivateImmediate();
  }
}

void UVectorVacuumingComponent::SetPlayerVelocity(bool _bActivate)
{
  if (!m_pOwnerPlayer.IsValid() || !IsValid(m_pDataAsset))
  {
    return;
  }

  float MovementMultiplier = m_pDataAsset->m_fCleanMovementMultiplicator;
  float RotationMultiplier = m_pDataAsset->m_fCleanRotatorMultiplicator;

  if (FMath::IsNearlyZero(MovementMultiplier) || FMath::IsNearlyZero(RotationMultiplier))
  {
    return;
  }

  MovementMultiplier = _bActivate ? MovementMultiplier : 1.0f / MovementMultiplier;
  RotationMultiplier = _bActivate ? RotationMultiplier : 1.0f / RotationMultiplier;

  m_pOwnerPlayer->ChangeMovementVelocity(MovementMultiplier, RotationMultiplier);
}
