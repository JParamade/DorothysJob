#include "GeneralUtils.h"
#include "GameFramework/Pawn.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "AIController.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "DorothysJob/Actor/Character/Enemy/BaseEnemy.h"
#include "DorothysJob/Data/DataAssets/WeaponDataAsset.h"
#include "DorothysJob/Data/DataAssets/Weapon/BaseWeaponStatsDataAsset.h"
#include "DorothysJob/Data/DataAssets/DirtTypesDataAsset.h"

bool GeneralUtils::GetRandomPointInCone(const APawn* _pPawn, float _fMinRadius, float _fMaxRadius, float _fAngle, int32 _iMaxTries, const FRotator& _oBaseRotation, FVector& vLocationToReach_, float _fForbiddenAngle)
{
  if (!IsValid(_pPawn))
  {
    return false;
  }

  vLocationToReach_ = FVector::ZeroVector;
  bool bReturn = false;
  FVector vActorLocation = _pPawn->GetActorLocation();

  for (int32 iTries = 0; iTries < _iMaxTries; ++iTries)
  {
    float fRadius = FMath::FRandRange(_fMinRadius, _fMaxRadius);
    float fRandomAngle = FMath::FRandRange(-_fAngle, _fAngle);
    if (_fForbiddenAngle > 0.f)
    {
      float fRange = _fAngle - _fForbiddenAngle;
      float fRandomSelector = FMath::FRandRange(0.f, fRange * 2);
      if (fRandomSelector < fRange)
      {
        fRandomAngle = FMath::FRandRange(-_fAngle, -_fForbiddenAngle);
      }
      else
      {
        fRandomAngle = FMath::FRandRange(_fForbiddenAngle, _fAngle);
      }
    }
    
    FRotator oRandomRotator = _oBaseRotation + FRotator(0., fRandomAngle, 0.);
    FVector vRandomDirection = oRandomRotator.Vector().GetSafeNormal() * fRadius;
    UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(_pPawn->GetWorld());
    FNavLocation tNavLocation;

    if (IsValid(pNavSys) && pNavSys->ProjectPointToNavigation(vActorLocation + vRandomDirection, tNavLocation)
      && IsReachable(_pPawn, tNavLocation.Location))
    {
      tNavLocation.Location.Z = vActorLocation.Z;
      vLocationToReach_ = tNavLocation.Location;
      bReturn = true;
      break;
    }
  }

  return bReturn;
}

bool GeneralUtils::GetStepPathPoint(APawn* _pPawn, float _fMinRadius, float _fMaxRadius, const FVector& _vLocationToReach, FVector& vStepLocation_)
{
  if (!IsValid(_pPawn))
  {
    return false;
  }

  // If the location to reach is too close, it won't find any step point
  vStepLocation_ = _vLocationToReach;

  UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(_pPawn->GetWorld());

  if (IsValid(pNavSys))
  {
    UNavigationPath* pNavPath = pNavSys->FindPathToLocationSynchronously(_pPawn->GetWorld(), _pPawn->GetActorLocation(), _vLocationToReach, _pPawn);

    if (IsValid(pNavPath) && pNavPath->IsValid() && (pNavPath->PathPoints.Num() > 1))
    {
      float fStepDistance = FMath::FRandRange(_fMinRadius, _fMaxRadius);
      FVector vStepPoint = _vLocationToReach;
      float fAccumulatedDistance = 0.f;

      for (int32 iIndex = 0; iIndex < pNavPath->PathPoints.Num() - 1; ++iIndex)
      {
        FVector vStartPoint = pNavPath->PathPoints[iIndex];
        FVector vEndPoint = pNavPath->PathPoints[iIndex + 1];
        float fSegmentDistance = FVector::Dist(vStartPoint, vEndPoint);

        if ((fAccumulatedDistance + fSegmentDistance) >= fStepDistance)
        {
          float fCoeficient = (fStepDistance - fAccumulatedDistance) / fSegmentDistance;
          vStepPoint = FMath::Lerp(vStartPoint, vEndPoint, fCoeficient);
          break;
        }
        else
        {
          fAccumulatedDistance += fSegmentDistance;
        }
      }

      if (IsReachable(_pPawn, vStepPoint))
      {
        vStepLocation_ = vStepPoint;
      }
    }
  }
  return true;
}

bool GeneralUtils::IsReachable(const APawn* _pPawn, const FVector& _vReachLocation)
{
  bool bIsReachable = false;
  AAIController* pAIController = Cast<AAIController>(_pPawn->GetController());
  UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(_pPawn->GetWorld());
  if (IsValid(pAIController) && IsValid(pNavSys))
  {
    const FNavAgentProperties tAgentProps = pAIController->GetNavAgentPropertiesRef();
    const ANavigationData* pNavData = pNavSys->GetNavDataForProps(tAgentProps);
    if (IsValid(pNavData))
    {
      FPathFindingQuery tPathQuery(pAIController, *pNavData, _pPawn->GetActorLocation(), _vReachLocation);
      const FPathFindingResult tPathResult = pNavSys->FindPathSync(tAgentProps, tPathQuery);
      bIsReachable = tPathResult.IsSuccessful() && !tPathResult.IsPartial();
    }
  }

  return bIsReachable;
}

bool GeneralUtils::IsReachableLineTrace(const APawn* _pPawn, const FVector& _vReachLocation, FVector* vReachablePoint_)
{
  bool bHit = true;
  FVector vLastReachable = FVector::ZeroVector;

  if (IsValid(_pPawn))
  {
    vLastReachable = _pPawn->GetActorLocation();
    UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(_pPawn->GetWorld());
    if (IsValid(pNavSys))
    {
      vLastReachable = _vReachLocation;
      FVector vNavHitLoc;
      bHit = UNavigationSystemV1::NavigationRaycast(_pPawn->GetWorld(), _pPawn->GetActorLocation(), _vReachLocation, vNavHitLoc);
      if (bHit)
      {
        vLastReachable = vNavHitLoc;
      }
    }
  }

  if (vReachablePoint_)
  {
    *vReachablePoint_ = vLastReachable;
  }

  return !bHit;
}

void GeneralUtils::DrawDebugCone(const APawn* _pPawn, const FRotator& _oBaseRotation, float _fMinRadius, float _fMaxRadius, float _fAngle, FColor _oMinColor, FColor _oMaxColor, float _fDuration, float _fForbiddenAngle)
{
  FVector vActorLocation = _pPawn->GetActorLocation();
  FRotator oYawRotator = _oBaseRotation + FRotator(0., -_fAngle, 0.);
  DrawDebugLine(_pPawn->GetWorld(), vActorLocation, vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), _oMinColor, false, _fDuration);
  DrawDebugLine(_pPawn->GetWorld(), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMaxRadius), _oMaxColor, false, _fDuration);
  oYawRotator = _oBaseRotation + FRotator(0., _fAngle, 0.);
  DrawDebugLine(_pPawn->GetWorld(), vActorLocation, vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), _oMinColor, false, _fDuration);
  DrawDebugLine(_pPawn->GetWorld(), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMaxRadius), _oMaxColor, false, _fDuration);
  
  float fMinAngle = -_fAngle;
  float fAngleSTep = (_fAngle * 2) / 10;
  FVector vPrevPoint;
  if (_fForbiddenAngle > 0.f)
  {
    fAngleSTep = (_fAngle - _fForbiddenAngle) * 2 / 10;
    for (float fCurrentAngle = fMinAngle; fCurrentAngle <= -_fForbiddenAngle; fCurrentAngle += fAngleSTep)
    {
      FRotator oAngleRot = _oBaseRotation + FRotator(0., fCurrentAngle, 0.);
      FVector vPointOnArc = oAngleRot.Vector();

      if (fCurrentAngle > fMinAngle)
      {
        DrawDebugLine(_pPawn->GetWorld(), vActorLocation + vPrevPoint * _fMinRadius, vActorLocation + vPointOnArc * _fMinRadius, _oMinColor, false, _fDuration);
        DrawDebugLine(_pPawn->GetWorld(), vActorLocation + vPrevPoint * _fMaxRadius, vActorLocation + vPointOnArc * _fMaxRadius, _oMaxColor, false, _fDuration);
      }

      vPrevPoint = vPointOnArc;
    }
    oYawRotator = _oBaseRotation + FRotator(0., -_fForbiddenAngle, 0.);
    DrawDebugLine(_pPawn->GetWorld(), vActorLocation, vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), _oMinColor, false, _fDuration);
    DrawDebugLine(_pPawn->GetWorld(), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMaxRadius), _oMaxColor, false, _fDuration);
    oYawRotator = _oBaseRotation + FRotator(0., _fForbiddenAngle, 0.);
    DrawDebugLine(_pPawn->GetWorld(), vActorLocation, vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), _oMinColor, false, _fDuration);
    DrawDebugLine(_pPawn->GetWorld(), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMinRadius), vActorLocation + (oYawRotator.Vector().GetSafeNormal() * _fMaxRadius), _oMaxColor, false, _fDuration);
    fMinAngle = _fForbiddenAngle;
  }
  
  for (float fCurrentAngle = fMinAngle; fCurrentAngle <= _fAngle; fCurrentAngle += fAngleSTep)
  {
    FRotator oAngleRot = _oBaseRotation + FRotator(0., fCurrentAngle, 0.);
    FVector vPointOnArc = oAngleRot.Vector();

    if (fCurrentAngle > fMinAngle)
    {
      DrawDebugLine(_pPawn->GetWorld(), vActorLocation + vPrevPoint * _fMinRadius, vActorLocation + vPointOnArc * _fMinRadius, _oMinColor, false, _fDuration);
      DrawDebugLine(_pPawn->GetWorld(), vActorLocation + vPrevPoint * _fMaxRadius, vActorLocation + vPointOnArc * _fMaxRadius, _oMaxColor, false, _fDuration);
    }

    vPrevPoint = vPointOnArc;
  }
}

void GeneralUtils::DrawDebugDoubleCircle(const APawn* _pPawn, float _fMinRadius, float _fMaxRadius, FColor _oMinColor, FColor _oMaxColor, float _fDuration)
{
  FVector vActorLocation = _pPawn->GetActorLocation();
  FVector vPrevPoint;
  for (int32 iCurrentAngle = 0; iCurrentAngle <= 360; iCurrentAngle += 10)
  {
    FRotator oAngleRot = FRotator(0., iCurrentAngle, 0.);
    FVector vPointOnArc = oAngleRot.Vector();

    if (iCurrentAngle > 0)
    {
      DrawDebugLine(_pPawn->GetWorld(), vActorLocation + vPrevPoint * _fMinRadius, vActorLocation + vPointOnArc * _fMinRadius, _oMinColor, false, _fDuration);
      DrawDebugLine(_pPawn->GetWorld(), vActorLocation + vPrevPoint * _fMaxRadius, vActorLocation + vPointOnArc * _fMaxRadius, _oMaxColor, false, _fDuration);
    }

    vPrevPoint = vPointOnArc;
  }
}

bool GeneralUtils::IsWasheeFlashee(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset)
{
    return _pWeaponDataAsset.IsValid() && (_pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Distance) && (_pWeaponDataAsset->m_eWeaponDirtType == EDirtType::Liquid);
}

bool GeneralUtils::IsMopressor(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset)
{
    return _pWeaponDataAsset.IsValid() && (_pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Melee) && (_pWeaponDataAsset->m_eWeaponDirtType == EDirtType::Liquid);
}

bool GeneralUtils::IsDirtminator(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset)
{
    return _pWeaponDataAsset.IsValid() && (_pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Distance) && (_pWeaponDataAsset->m_eWeaponDirtType == EDirtType::Dust);
}

bool GeneralUtils::IsDusty(TSoftObjectPtr<UBaseWeaponStatsDataAsset> _pWeaponDataAsset)
{
    return _pWeaponDataAsset.IsValid() && (_pWeaponDataAsset->m_eWeaponAttackType == EWeaponAttackType::Melee) && (_pWeaponDataAsset->m_eWeaponDirtType == EDirtType::Dust);
}
