#include "SpawnableConsumable.h"
#include "NavigationSystem.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Consumables/BaseConsumableActor.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

bool USpawnableConsumable::Use(ABasePlayer* _pPlayer)
{
  if (!m_bAchievementManager)
  {
    if (UBaseGameInstance* pGameInstance = Cast<UBaseGameInstance>(GetWorld()->GetGameInstance())) {
      if (UAchievementSubsystem* pAchSubsystem = pGameInstance->GetSubsystem<UAchievementSubsystem>()) {
        m_bAchievementManager = true;
        if (!pAchSubsystem->m_mGottaWashEmAll.FindRef(GetFName(), false))
        {
          pAchSubsystem->AddProgress("GottaWashEmAll", 1);
          pAchSubsystem->m_mGottaWashEmAll.Add(GetFName(), true);
        }
      }
    }
  }

  if ((m_uStackQuantity <= 0) || !IsValid(_pPlayer) || !IsValid(_pPlayer->GetWorld()))
  {
    return false;
  }

  // Search a location to spawn the consumable.
  FVector vLocationToSpawn = FVector::ZeroVector;
  bool bResult = GetRandomLocationToSpawn(_pPlayer, vLocationToSpawn);

  if (bResult)
  {
    _pPlayer->GetWorld()->SpawnActor<AActor>(m_oActorToSpawn, vLocationToSpawn, _pPlayer->GetSkeletalMesh()->GetRelativeRotation());
    --m_uStackQuantity;
  }

  return bResult;
}

bool USpawnableConsumable::GetRandomLocationToSpawn(const ABasePlayer* _pPlayer, FVector& vLocationToSapwn_)
{
  bool bReturn = false;
  if (!IsValid(_pPlayer) || !IsValid(_pPlayer->GetWorld()))
  {
    return bReturn;
  }

  vLocationToSapwn_ = FVector::ZeroVector;
  FVector vActorLocation = _pPlayer->GetActorLocation();
  UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(_pPlayer->GetWorld());
  FNavLocation tStartNavLocation;
  // It projects the player location in the navmesh with an extent, because the player could not be in the navmesh.
  if (!IsValid(pNavSys) || !pNavSys->ProjectPointToNavigation(vActorLocation, tStartNavLocation, FVector(100.f, 100.f, 100.f)))
  {
    return bReturn;
  }

  tStartNavLocation.Location.Z = vActorLocation.Z;
  const FNavAgentProperties tAgentProps = _pPlayer->GetNavAgentPropertiesRef();
  const ANavigationData* pNavData = pNavSys->GetNavDataForProps(tAgentProps);
  FNavLocation tNavLocation;
  for (int32 iTries = 0; iTries < 30; ++iTries)
  {
    float fRadius = FMath::FRandRange(200.f, 300.f);
    float fRandomAngle = FMath::FRandRange(-90.f, 90.f);
    FRotator oRandomRotator = _pPlayer->GetSkeletalMesh()->GetRelativeRotation() + FRotator(0., fRandomAngle, 0.);
    FVector vRandomDirection = oRandomRotator.Vector().GetSafeNormal() * fRadius;
    if (IsValid(pNavData) && pNavSys->ProjectPointToNavigation(tStartNavLocation.Location + vRandomDirection, tNavLocation))
    {
      FPathFindingQuery tPathQuery(_pPlayer, *pNavData, tStartNavLocation.Location, tNavLocation.Location);
      const FPathFindingResult tPathResult = pNavSys->FindPathSync(tAgentProps, tPathQuery);
      if (tPathResult.IsSuccessful() && !tPathResult.IsPartial())
      {
        tNavLocation.Location.Z = vActorLocation.Z;
        vLocationToSapwn_ = tNavLocation.Location;
        bReturn = true;
        break;
      }
    }
  }

  return bReturn;
}