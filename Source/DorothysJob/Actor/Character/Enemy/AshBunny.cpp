#include "AshBunny.h"
#include "Components/SphereComponent.h"
#include "DorothysJob/Actor/Projectile/BaseProjectile.h"
#include "DorothysJob/Data/DataAssets/Enemy/DustBunnyPropertiesDataAsset.h"

AAshBunny::AAshBunny() :
  ADustBunny()
{

}

#pragma region | Actions
void AAshBunny::PerformAttack(int32 _iCurrentCombo)
{
  ABaseEnemy::PerformAttack(_iCurrentCombo);

  UDustBunnyPropertiesDataAsset* pDustBunnyPropertiesDA = Cast<UDustBunnyPropertiesDataAsset>(m_pCharacterDataAsset);
  if (!IsValid(pDustBunnyPropertiesDA))
  {
    return;
  }

  if (IsValid(pDustBunnyPropertiesDA->m_oProjectileClass) && IsValid(m_pProjectileSpawnPoint) && IsValid(pDustBunnyPropertiesDA->m_oProjectileClass->GetDefaultObject<ABaseProjectile>()))
  {
    ABaseProjectile* pProjectile = pDustBunnyPropertiesDA->m_oProjectileClass->GetDefaultObject<ABaseProjectile>();

    USphereComponent* pSphere = IsValid(pProjectile) ? pProjectile->FindComponentByClass<USphereComponent>() : nullptr;
    float fRadius = IsValid(pSphere) ? pSphere->GetScaledSphereRadius() : 0.f;
  
    FRotator Rotation = GetActorRotation();
    FVector vLocation = m_pProjectileSpawnPoint->GetComponentLocation();;
    FVector vForwardDir = Rotation.Vector();
    FVector vRightDir = FVector::CrossProduct(vForwardDir, FVector::UpVector).GetSafeNormal();
    FRotator LeftRot = Rotation + FRotator(0, pDustBunnyPropertiesDA->m_fProjectileAngle, 0);
    FRotator RightRot = Rotation + FRotator(0, -pDustBunnyPropertiesDA->m_fProjectileAngle, 0);
    FVector vLeftLocation = vLocation - (vRightDir * fRadius * 2.5f);
    FVector vRightLocation = vLocation + (vRightDir * fRadius * 2.5f);

    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    GetWorld()->SpawnActor<ABaseProjectile>(pDustBunnyPropertiesDA->m_oProjectileClass, vLocation, Rotation, SpawnParams);
    GetWorld()->SpawnActor<ABaseProjectile>(pDustBunnyPropertiesDA->m_oProjectileClass, vLeftLocation, LeftRot, SpawnParams);
    GetWorld()->SpawnActor<ABaseProjectile>(pDustBunnyPropertiesDA->m_oProjectileClass, vRightLocation, RightRot, SpawnParams);
  }
}
#pragma endregion