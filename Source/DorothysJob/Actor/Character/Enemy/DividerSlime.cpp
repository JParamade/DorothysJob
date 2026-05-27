#include "DividerSlime.h"
#include "Components/CapsuleComponent.h"
#include "Components/MeshComponent.h"
#include "NavigationSystem.h"
#include "DorothysJob/Animations/Enemy/EnemyAnimInstance.h"
#include "DorothysJob/Data/DataAssets/Enemy/OozerPropertiesDataAsset.h"
#include "DorothysJob/Utils/GeneralUtils.h"
#include "DorothysJob/Actor/Room/BaseRoom.h"

ADividerSlime::ADividerSlime() : 
  ASlimeEnemy()
{
  // Capsule
  UpdateCapsule(100.f);
}

#pragma region | Game Over
void ADividerSlime::GameOver()
{
  if (!IsValid(m_pOozerPropertiesDA) || !IsValid(m_pOozerPropertiesDA->m_oChildSlimeClass))
  {
    Super::GameOver();
    return;
  }

  DivisionEnd();
}
#pragma endregion

void ADividerSlime::DivisionEnd()
{
  FVector vOriginalLocation = GetActorLocation();
  FRotator vOriginalRotation = GetActorRotation();
  const int32 iMaxTries = 30;
  UNavigationSystemV1* pNavSys = UNavigationSystemV1::GetCurrent(GetWorld());
  // Enemy division
  for (int32 iChild = 0; iChild < m_pOozerPropertiesDA->m_iChildrenNumber; ++iChild)
  {
    for (int32 iTries = 0; iTries < iMaxTries; ++iTries)
    {
      // The first one will be spawned in the father's location.
      float fRadius = (iChild > 0) ? FMath::FRandRange(m_fCapsuleRadius * 3.f, m_fCapsuleRadius * 4.f) : 0.f;
      float fAngle = FMath::FRandRange(0.f, 2.f * PI);
      FVector vRandomDirection = FVector(FMath::Cos(fAngle), FMath::Sin(fAngle), 0.f) * fRadius;

      FNavLocation tNavLocation;
      // If the selected point to spawn the child is reachable from the parent location, they are in the same navmesh.
      if (IsValid(pNavSys) && pNavSys->ProjectPointToNavigation(vOriginalLocation + vRandomDirection, tNavLocation)
          && GeneralUtils::IsReachable(this, tNavLocation.Location))
      {
        tNavLocation.Location.Z = vOriginalLocation.Z;
        FActorSpawnParameters spawnInfo;
        spawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        ABaseEnemy* pEnemy = GetWorld()->SpawnActor<ASlimeEnemy>(m_pOozerPropertiesDA->m_oChildSlimeClass, tNavLocation.Location, vOriginalRotation, spawnInfo);
        if (IsValid(m_pCurrentRoom) && IsValid(pEnemy))
        {
          pEnemy->SetMitigation(GetMitigation());
          m_pCurrentRoom->RegisterEnemy(pEnemy);
          pEnemy->SetCurrentRoom(m_pCurrentRoom);
        }
        break;
      }
    }
  }
  ABaseEnemy::GameOver();
}