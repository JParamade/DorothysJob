#include "SpawnableConsumable.h"
#include "NavigationSystem.h"
#include "DorothysJob/Actor/Character/Player/BasePlayer.h"
#include "DorothysJob/Actor/Consumables/BaseConsumableActor.h"
#include "DorothysJob/Manager/BaseGameInstance.h"
#include "DorothysJob/Manager/GameInstanceSubsystem/AchievementSubsystem.h"

bool USpawnableConsumable::Use(ABasePlayer* _pPlayer) {
  // Achievement Logic.
  if (!m_bAchievementManager) {
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

  // Check if there are any consumables left in the stack and if the player and its world are valid.
  if ((m_uStackQuantity <= 0) || !IsValid(_pPlayer) || !IsValid(_pPlayer->GetWorld())) return false;

  // Try to get a location to spawn the consumable. If it can't find a location to spawn in the max tries, it will return false.
  FVector vLocationToSpawn = FVector::ZeroVector;
  bool bResult = GetRandomLocationToSpawn(_pPlayer, vLocationToSpawn);

  // Spawn the consumable actor in the world and remove one consumable from the stack.
  if (bResult) {
    _pPlayer->GetWorld()->SpawnActor<AActor>(m_oActorToSpawn, vLocationToSpawn, _pPlayer->GetSkeletalMesh()->GetRelativeRotation());
    --m_uStackQuantity;
  }

  // Return true if the consumable was used successfully, false otherwise.
  return bResult;
}

bool USpawnableConsumable::GetRandomLocationToSpawn(const ABasePlayer* _pPlayer, FVector& vLocationToSpawn) {
  bool bReturn = false;
  // Check if the player and its world are valid.
  if (!IsValid(_pPlayer) || !IsValid(_pPlayer->GetWorld())) {
    return bReturn;
  }

  // Initialize the output parameter to zero vector, and get the player's location and the navigation system.
  vLocationToSpawn = FVector::ZeroVector;
  FVector vActorLocation = _pPlayer->GetActorLocation();
  UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(_pPlayer->GetWorld());
  FNavLocation tStartNavLocation;

  // Try to project the player's location to the navigation mesh. If it fails, return false.
  if (!IsValid(pNavSys) || !pNavSys->ProjectPointToNavigation(vActorLocation, tStartNavLocation, FVector(100.f, 100.f, 100.f))) {
    return bReturn;
  }

  // Set the Z value of the start location to the player's Z value to avoid spawning the consumable in the air or underground.
  tStartNavLocation.Location.Z = vActorLocation.Z;
  const FNavAgentProperties tAgentProps = _pPlayer->GetNavAgentPropertiesRef();
  const ANavigationData* pNavData = pNavSys->GetNavDataForProps(tAgentProps);
  FNavLocation tNavLocation;

  // Try to find a random location to spawn the consumable in the max tries.
  for (int32 iTries = 0; iTries < 30; ++iTries) {
    // Get a random radius and angle to calculate a random direction from the player's location.
    float fRadius = FMath::FRandRange(200.f, 300.f);
    float fRandomAngle = FMath::FRandRange(-90.f, 90.f);

    // Calculate a random direction from the player's location using the random angle and radius, and project it to the navigation mesh.
    FRotator oRandomRotator = _pPlayer->GetSkeletalMesh()->GetRelativeRotation() + FRotator(0., fRandomAngle, 0.);
    FVector vRandomDirection = oRandomRotator.Vector().GetSafeNormal() * fRadius;
    if (IsValid(pNavData) && pNavSys->ProjectPointToNavigation(tStartNavLocation.Location + vRandomDirection, tNavLocation)) {
      FPathFindingQuery tPathQuery(_pPlayer, *pNavData, tStartNavLocation.Location, tNavLocation.Location);
      const FPathFindingResult tPathResult = pNavSys->FindPathSync(tAgentProps, tPathQuery);

      // If the pathfinding query is successful and not partial, set the Z value of the navigation location to the player's Z value and return true.
      if (tPathResult.IsSuccessful() && !tPathResult.IsPartial()) {
        tNavLocation.Location.Z = vActorLocation.Z;
        vLocationToSpawn = tNavLocation.Location;
        bReturn = true;
        break;
      }
    }
  }

  return bReturn;
}